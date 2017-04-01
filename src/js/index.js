'use strict';

import Arduino from './arduino';
import Neopixel from './neopixel';
import *  as Convert from './colour-conversion';

const ROWS = 3;
const COLS = 11;
const FRAME_DELAY = 30;

const strip = Neopixel.fromElement(document.querySelector('.custom-strip'));
var lights = [];

class HSL {
  constructor(hue, saturation, luminosity) {
    this.hue = hue;
    this.saturation = saturation;
    this.luminosity = luminosity;
  }

  static fromRGB(r, g, b) {
    const hsl = Convert.rgbToHsl(r, g, b);
    return new HSL(hsl[0], hsl[1], hsl[2]);
  }

  toRGB() {
    return Convert.hslToRgb(this.hue, this.saturation, this.luminosity);
  }
}

const Pattern = {
  /*
   * Blobby morphy colours
   */
  BLOBS: function() {
    var lights = [];
    for (var n = 0; n < 4; n++) {
      lights.push(new LightSource(
          rand(0, COLS), rand(0, ROWS),
          new HSL(rand(0, 1), 1, rand(0, 1)),
          0.6,
          roundLight,
          bounce
      ));
    }
    return lights;
  },

  /*
   * Scanning vertical bands of colour
   */
  BANDS: function() {
    var lights = [];
    for (var n = 0; n < 4; n++) {
      lights.push(new LightSource(
          rand(0, COLS), rand(0, ROWS),
          new HSL(rand(0, 1), 1, 0.5), //rand(0, 1)),
          0.3,
          verticalLight,
          scan
      ));
    }
    return lights;
  },

  /*
   * Lots of twinkly colours
   */
  TWINKLES: function() {
    var lights = [];
    for (var x = 0; x < COLS; x++) {
      for (var y = 0; y < ROWS; y++) {
        if ((x + y) % 2 == 0) {
          lights.push(new LightSource(
              x, y,
              new HSL(rand(0, 1), 1, 0.4),
              0.3,
              roundLight,
              twinkle
          ));
        }
      }
    }
    return lights;
  },

  /*
   * Kill all humans.
   */
  CYLON: function() {
    const speed = 0.2;
    var lights = [], l;
    l = new LightSource(
        0, 0,
        HSL.fromRGB(255, 100, 100),
        0.4,
        verticalLight,
        scan
    );
    l.dx = speed;
    lights.push(l);
    l = new LightSource(
        COLS - 1, 0,
        HSL.fromRGB(255, 100, 100),
        0.4,
        verticalLight,
        scan
    );
    l.dx = -speed;
    lights.push(l);
    return lights;
  },

  /*
   * Whoa.
   */
  MATRIX: function() {
    var lights = [];
    for (var i = 0; i < 15; i++) {
      lights.push(new LightSource(
          rand(0, COLS - 1), rand(-2, ROWS + 1),
          HSL.fromRGB(100, 255, 100),
          0.2,
          roundLight,
          rain
      ));
    }
    return lights;
  },

  /*
   * Fwoosh!
   */
  FIRE: function() {
    var lights = [];
    for (var i = 0; i < 10; i++) {
      lights.push(new LightSource(
          rand(0, COLS - 1), rand(-2, ROWS + 1),
          HSL.fromRGB(255, rand(0, 200), 0),
          0.35,
          roundLight,
          smoke
      ));
    }
    return lights;
  },

  FIREWORKS: function() {
    return [new LightSource(
      5, 1,
      new HSL(rand(0, 1), 1, 0.5),
      0,
      roundLight,
      explode
    )];
  }

};

const arduino = new Arduino(

    // This function is run once when the arduino starts
    function begin() {

      lights = Pattern.BLOBS();
      //lights = Pattern.BANDS();
      //lights = Pattern.TWINKLES();
      //lights = Pattern.CYLON();
      //lights = Pattern.MATRIX();
      //lights = Pattern.FIRE();
      //lights = Pattern.FIREWORKS();

      strip.begin();
      strip.show();
    },

    // This function is run continuously
    // Use the delay function to prevent it from burning cycles
    // like crazy
    async function loop() {

      // Calculate the lighting for the next frame
      const colours = ColourBuffer(strip.numPixels());
      for (var l = 0; l < lights.length; l++) {
        moveLight(lights[l]);
        lights[l].behaveFunc(lights[l]);
        lights[l].lightFunc(lights[l], colours);
      }

      // Light the pixels
      var c;
      for (var i = 0; i < colours.length; i++) {
        c = colours[i].toRGB();
        strip.setPixelColor(i, strip.Color(c[0], c[1], c[2]));
      }
      strip.show();

      // Pause before the next frame
      await arduino.delay(FRAME_DELAY);
    }
);

function ColourBuffer(x) {
  const buffer = [];
  for (var i = 0; i < x; i++) {
    buffer.push(new HSL(0, 0, 0));
  }
  return buffer;
}

// Transforms a cartesian coordinate into a pixel index
function index(x, y) {
  x = Math.round(x);
  y = Math.round(y);
  if (x < 0 || x >= COLS || y < 0 || y >= ROWS) {
    return -1;
  }
  return (x * 3) + (x % 2 > 0 ? 2 - y : y);
}

// Adds two RGB colours
function addRgb(rgb1, rgb2) {
  return HSL.fromRGB(
      clamp(rgb1[0] + rgb2[0], 255),
      clamp(rgb1[1] + rgb2[1], 255),
      clamp(rgb1[2] + rgb2[2], 255)
  );
}

// dims a colour
function dim(c, amount) {
  return new HSL(
      c.hue,
      c.saturation,
      clamp(c.luminosity * amount, 1)
  );
}

// Clamps a number between 0 and max
function clamp(num, max) {
  return Math.max(0, Math.min(num, max));
}

// random number between and max
function rand(min, max) {
  return Math.random() * (max - min) + min;
}

function moveLight(light) {
  light.colour.luminosity = clamp(light.colour.luminosity + light.db, 1);
  //light.x = clamp(light.x + light.dx, COLS - 1);
  //light.y = clamp(light.y + light.dy, ROWS - 1);
  light.x += light.dx;
  light.y += light.dy;
}

function LightSource(x, y, colour, fade, lightFunc, behaveFunc) {
  return { x, y, colour, fade, lightFunc, behaveFunc, dx: 0, dy: 0, db: 0 };
}

function roundLight(lightsource, buffer) {
  for (var x = 0; x < COLS; x++) {
    for (var y = 0; y < ROWS; y++) {
      const i = index(x, y);
      if (i < 0 || i >= strip.numPixels()) {
        continue;
      }
      buffer[i] = addRgb(buffer[i].toRGB(), dim(lightsource.colour, Math.pow(lightsource.fade, Math.abs(x - lightsource.x) + Math.abs(y - lightsource.y))).toRGB());
    }
  }
}

function verticalLight(lightsource, buffer) {
  for (var x = 0; x < COLS; x++) {
    for (var y = 0; y < ROWS; y++) {
      const i = index(x, y);
      if (i < 0 || i >= strip.numPixels()) {
        continue;
      }
      buffer[i] = addRgb(buffer[i].toRGB(), dim(lightsource.colour, Math.pow(lightsource.fade, Math.abs(x - lightsource.x))).toRGB());
    }
  }
}

function bounce(lightsource) {
  lightsource.dx = lightsource.dx || rand(0.005, 0.05) * (Math.random() > 0.5 ? -1 : 1);
  lightsource.dy = lightsource.dy || rand(0.005, 0.05) * (Math.random() > 0.5 ? -1 : 1);
  lightsource.db = lightsource.db || rand(0.001, 0.01) * (Math.random() > 0.5 ? -1 : 1);

  if (lightsource.colour.luminosity <= 0.3 || lightsource.colour.luminosity >= 1) {
    lightsource.db *= -1;
  }
  if (lightsource.x <= 0 || lightsource.x >= COLS - 1) {
    lightsource.dx *= -1;
  }
  if (lightsource.y <= 0 || lightsource.y >= ROWS - 1) {
    lightsource.dy *= -1;
  }
}

function scan(lightsource) {
  lightsource.dx = lightsource.dx || rand(0.01, 0.1) * (Math.random() > 0.5 ? -1 : 1);
  if (lightsource.x <= 0 || lightsource.x >= COLS - 1) {
    lightsource.dx *= -1;
  }
}

function twinkle(lightsource) {
  const TWINKLOCITY = 0.02;
  if (lightsource.colour.luminosity <= 0) {
    lightsource.colour = new HSL(rand(0, 1), 1, 0.4);
    lightsource.db = 0;
  } else if (lightsource.db == 0){
    if (Math.random() <= TWINKLOCITY) {
      lightsource.db = -0.03;
    }
  } else if (lightsource.db > 0 && lightsource.colour.luminosity > 0.5) {
    lightsource.db = 0;
  }
}

function rain(l) {
  l.dy = l.dy || rand(0.05, 0.2);
  l.dx = 0;
  if (l.y > ROWS + 2) {
    l.y = -2;
    l.x = rand(0, COLS - 1);
    l.dy = rand(0.05, 0.2);
  }
}

function smoke(l) {
  const WIGGLE = 0.2;
  l.dy = l.dy || rand(-0.1, -0.2);
  l.dx = l.dx || rand(-0.1, 0.1);
  if (l.y < -1) {
    l.y = ROWS + 1;
    l.x = rand(0, COLS - 1);
    l.dy = rand(-0.1, -0.2);
  }
  if (Math.random() < WIGGLE) {
    l.dx *= -1;
  }
}

function explode(l) {
  // TODO: Keep working on this. Why does the diffuse light get BRIGHTER as the core fades?
  l.fade += 0.02;
  if (l.fade >= 1 && l.db == 0) {
    l.fade = 0.9;
    l.db = -0.01;
  }
  if (l.colour.luminosity <= 0) {
    l.fade = 0;
    l.db = 0;
    l.colour.luminosity = 0.5;
  }
}

arduino.start();