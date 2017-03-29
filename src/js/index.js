'use strict';

import Arduino from './arduino';
import Neopixel from './neopixel';
import *  as Convert from './colour-conversion';

const ROWS = 3;
const COLS = 11;
const FADE_FRAMES = 30;
const FRAME_DELAY = 30;

const strip = Neopixel.fromElement(document.querySelector('.custom-strip'));
const lights = [];

var currentColourBuffer;

class HSL {
  constructor(hue, saturation, luminosity) {
    this.hue = hue;
    this.saturation = saturation;
    this.luminosity = luminosity;
  }

  toRGB() {
    const rgb = Convert.hslToRgb(this.hue, this.saturation, this.luminosity);
    return new RGB(rgb[0], rgb[1], rgb[2]);
  }
}

class RGB {
  constructor(red, green, blue) {
    this.red = red;
    this.green = green;
    this.blue = blue;
  }

  toHSL() {
    const hsl = Convert.rgbToHsl(this.red, this.green, this.blue);
    return new HSL(hsl[0], hsl[1], hsl[2]);
  }
}

const arduino = new Arduino(

    // This function is run once when the arduino starts
    function begin() {

      for (var n = 0; n < 4; n++) {
        lights.push(randomLight());
      }

      currentColourBuffer = ColourBuffer(strip.numPixels());

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
        lights[l].lightFunc(lights[l], colours);
      }

      // Crossfade between the current and next frames
      currentColourBuffer = await fade(currentColourBuffer, colours);

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

// Fades the pixels between two buffers
// Returns the final buffer when complete
async function fade(from, to) {
  var c;
  for (var frame = 0; frame < FADE_FRAMES; frame++) {
    for (var i = 0; i < from.length; i++) {
      c = transRGBColour(from[i].toRGB(), to[i].toRGB(), frame)
      strip.setPixelColor(i, strip.Color(c.red, c.blue, c.green));
    }
    strip.show();
    await arduino.delay(FRAME_DELAY);
  }
  return to;
}

function transRGBColour(from, to, frame) {
  return new RGB(
      from.red + Math.round(((to.red - from.red) / FADE_FRAMES) * frame),
      from.green + Math.round(((to.green - from.green) / FADE_FRAMES) * frame),
      from.blue + Math.round(((to.blue - from.blue) / FADE_FRAMES) * frame)
  );
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
  return new RGB(
      clamp(rgb1.red + rgb2.red, 255),
      clamp(rgb1.green + rgb2.green, 255),
      clamp(rgb1.blue + rgb2.blue, 255)
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

function randomLight() {

  const light = LightSource(
      rand(0, COLS),
      rand(0, ROWS),
      new HSL(Math.random(), 1, Math.random()),
      pointLight
  );
  light.dx = rand(-0.7, 0.7);
  light.dy = rand(-0.7, 0.7);
  light.db = rand(-0.2, 0.2);

  return light;
}

function moveLight(light) {
  light.colour.luminosity = clamp(light.colour.luminosity + light.db, 1);
  if (light.colour.luminosity <= 0 || light.colour.luminosity >= 1) {
    light.db *= -1;
  }
  light.x = clamp(light.x + light.dx, COLS);
  if (light.x <= 0 || light.x >= COLS) {
    light.dx *= -1;
  }
  light.y = clamp(light.y + light.dy, ROWS);
  if (light.y <= 0 || light.y >= ROWS) {
    light.dy *= -1;
  }
}

function LightSource(x, y, colour, lightFunc) {
  return { x, y, colour, lightFunc, dx: 0, dy: 0, db: 0 };
}

function pointLight(lightsource, buffer) {
  const radius = 3;
  for (var x = -radius; x <= radius; x++) {
    for (var y = -radius; y <= radius; y++) {
      const i = index(lightsource.x + x, lightsource.y + y);
      if (i < 0 || i >= strip.numPixels()) {
        continue;
      }
      buffer[i] = addRgb(buffer[i].toRGB(), dim(lightsource.colour, Math.pow(0.50, Math.abs(x) + Math.abs(y))).toRGB()).toHSL();
    }
  }
}

arduino.start();