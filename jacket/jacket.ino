#include <Adafruit_NeoPixel.h>
#include <Math.h>
#define PIN 10
#define BUTTON_PIN 6
#define ROWS 3
#define COLS 11
#define FRAME_DELAY 33
#define MAX_LIGHTS 20
#define NUM_BLOBS 4
#define NUM_FIREWORKS 3
#define TWINKLOCITY 0.02
#define WISPINESS 0.2
#define VOLATILITY 0.02
#define DENSITY 20
#define SHOW_FPS true

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
  float x;
  float y;
  float dx;
  float dy;
  double db;
  hsv colour;
  double fade;
  void (*lightFunc)(lightsource *);
  void (*behaviourFunc)(lightsource *);
};

static hsv   rgb2hsv(rgb in);
static rgb   hsv2rgb(hsv in);
static hsv   addColours(hsv c1, hsv c2);
static rgb   dim(rgb c, float amount);

//Adafruit_NeoPixel onboard = Adafruit_NeoPixel(1, 8, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(ROWS * COLS, PIN, NEO_GRB + NEO_KHZ800);
unsigned long lastFrame;
rgb colourBuffer[ROWS * COLS] = { {0.0, 0.0, 0.0} };
lightsource lights[MAX_LIGHTS];
int numLights = 0;

int acc = 0;

void setup() {
  randomSeed(analogRead(9));

  //setup input
  pinMode(BUTTON_PIN, INPUT);
  digitalWrite(BUTTON_PIN, HIGH);

  // Choose a pattern
//    numLights = testlight(lights);
  numLights = blobs(lights);
//  numLights = matrix(lights);
//  numLights = fire(lights);
//  numLights = cylon(lights);
//  numLights = bands(lights);
//  numLights = fireworks(lights);
//  numLights = beacon(lights);

  lastFrame = millis();
//  onboard.begin();
  strip.begin();
//  onboard.show();
  strip.show();
}

void loop() {

  // Test the input button
//  if (digitalRead(BUTTON_PIN) == LOW) {
//    onboard.setPixelColor(0, onboard.Color(0, 0, 255));
//  }
//  else {
//    onboard.setPixelColor(0, onboard.Color(0, 0, 0));
//  }
//  onboard.show();
  
  if (millis() - lastFrame < FRAME_DELAY) {
    return;
  }

  if (SHOW_FPS) {
    Serial.print(1000 / (millis() - lastFrame));
    Serial.print("\n");
  }

  lastFrame = millis();

  // Calculate the lighting for the next frame
  for (int i = 0; i < ROWS*COLS; i++) {
    colourBuffer[i] = {0, 0, 0}; 
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
  for (int i = 0; i < ROWS * COLS; i++) {
    rgb colour = colourBuffer[i];
    strip.setPixelColor(i, strip.Color(colour.r * 255, colour.g * 255, colour.b * 255));
   
  }
  strip.show();
}

