#include <Adafruit_NeoPixel.h>
#include <Math.h>
#define BACK_PIN 10
#define COLLAR_PIN 9
#define BUTTON_PIN 6
#define BACK_ROWS 3
#define COLLAR_ROWS 2
#define COLS 11
#define FRAME_DELAY 33
#define MAX_LIGHTS 20
#define NUM_BLOBS 4
#define NUM_FIREWORKS 3
#define TWINKLOCITY 0.02
#define WISPINESS 0.2
#define VOLATILITY 0.02
#define SHOW_FPS true
#define DEBUG_INPUT false
#define LONG_PRESS 700
#define NUM_THEMES 3
#define NUM_PATTERNS 7
#define AUTO_ROTATE 45000
#define DOUBLECLICK_WINDOW 200

typedef struct {
  double r;       // a fraction between 0 and 1
  double g;       // a fraction between 0 and 1
  double b;       // a fraction between 0 and 1
} rgb;

typedef struct {
  double h;       // angle in degrees
  double s;       // a fraction between 0 and 1
  double v;       // a fraction between 0 and 1
} hsv;

typedef struct lightsource lightsource;
struct lightsource {
  double x;
  double y;
  double dx;
  double dy;
  double db;
  hsv colour;
  double fade;
  void (*lightFunc)(lightsource *);
  void (*behaviourFunc)(lightsource *);
};

typedef int (*Pattern)(lightsource *lights);
typedef hsv (*Theme)(bool wantRandom);

static hsv   rgb2hsv(rgb in);
static rgb   hsv2rgb(hsv in);
static hsv   addColours(hsv c1, hsv c2);
static rgb   dim(rgb c, float amount);

Adafruit_NeoPixel back_strip = Adafruit_NeoPixel(BACK_ROWS * COLS, BACK_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel collar_strip = Adafruit_NeoPixel(COLLAR_ROWS * COLS, COLLAR_PIN, NEO_GRB + NEO_KHZ800);
unsigned long lastFrame;
rgb backBuffer[BACK_ROWS * COLS] = { {0.0, 0.0, 0.0} };
rgb collarBuffer[COLLAR_ROWS * COLS] = { {0.0, 0.0, 0.0} };
lightsource lights[MAX_LIGHTS];
int numLights = 0;
unsigned long buttonDown = 0;
unsigned long lastPress = 0;
unsigned long lastRotate = 0;
bool switchingTheme = false;
int brightness = 255;

float acc = 0;

void initScene() {
  numLights = getCurrentPattern()(lights);
}

void setup() {
  randomSeed(analogRead(9));

  pinMode(BUTTON_PIN, INPUT);
  digitalWrite(BUTTON_PIN, HIGH);

  initScene();

  lastFrame = millis();
  back_strip.begin();
  collar_strip.begin();
  back_strip.show();
  collar_strip.show();
}

void handleShort() {
  
}

void loop() {

  // Accept input
  if (digitalRead(BUTTON_PIN) == LOW) {
    if (DEBUG_INPUT) {
      Serial.print("DOWN :: "); Serial.print(buttonDown); Serial.print(" :: ");Serial.print(millis()); Serial.print("\n");
    }
    if (buttonDown == 0) {
      buttonDown = millis();
      if (DEBUG_INPUT) {
        Serial.print("====== SET ======\n");
      }
    } else if(millis() - buttonDown >= LONG_PRESS) {
      if (DEBUG_INPUT) {
        Serial.print("======= LONG =====\n");
      }
      changeTheme();
      buttonDown = 0;
      lastPress = 0;
      switchingTheme = true;
    }
  }
  else if (buttonDown > 0) {
    if (DEBUG_INPUT) {
      Serial.print("======= SHORT =====\n");
    }
    
    if (!switchingTheme && lastPress == 0) {
      // First press. Wait to see if we get a double-click.
      lastPress = millis();  
    }
    else if(!switchingTheme) {
      // Second click. Change brightness.
      brightness = brightness == 255 ? 127 : 255;
    }
    
    switchingTheme = false;
    buttonDown = 0;
  } 
  else if (lastPress > 0 && millis() - lastPress > DOUBLECLICK_WINDOW) {
    // Just a single click
    lastPress = 0;
    if (!switchingTheme) {
      changePattern(false);
      lastRotate = millis();
    }
  }

  // Auto-rotate patterns
  if (millis() - lastRotate > AUTO_ROTATE) {
    changePattern(true);
    lastRotate = millis();
  }
  
  if (millis() - lastFrame < FRAME_DELAY) {
    return;
  }

  if (SHOW_FPS) {
    Serial.print(1000 / (millis() - lastFrame));
    Serial.print("\n");
  }

  lastFrame = millis();

  // Calculate the lighting for the next frame
  for (int i = 0; i < BACK_ROWS*COLS; i++) {
    backBuffer[i] = {0, 0, 0}; 
  }
  for (int i = 0; i < COLLAR_ROWS*COLS; i++) {
    collarBuffer[i] = {0, 0, 0};
  }
  for (int l = 0; l < numLights; l++) {
    lightsource light = lights[l];
    light.colour.v = clamp(light.colour.v + light.db, 1);
    light.x += light.dx;
    light.y += light.dy;
    light.behaviourFunc(&light);
    light.lightFunc(&light);
    lights[l] = light; // Aaaaah structs are value types!
  }

  // Light the pixels
  for (int i = 0; i < BACK_ROWS * COLS; i++) {
    rgb colour = backBuffer[i];
    back_strip.setPixelColor(i, back_strip.Color(colour.r * brightness, colour.g * brightness, colour.b * brightness));
  }
  for (int i = 0; i < COLLAR_ROWS * COLS; i++) {
    rgb colour = collarBuffer[i];
    collar_strip.setPixelColor(i, collar_strip.Color(colour.r * brightness, colour.g * brightness, colour.b * brightness));
  }
  back_strip.show();
  collar_strip.show();
}

