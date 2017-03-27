'use strict';

import Arduino from './arduino';
import Neopixel from './neopixel';

const ROWS = 3;
const COLS = 11;

const strip = Neopixel.fromElement(document.querySelector('.custom-strip'));
const lights = [];

const arduino = new Arduino(

    // This function is run once when the arduino starts
    function begin() {

      for (var n = 0; n < 10; n++) {
        lights.push(randomLight());
      }

      strip.begin();
      strip.show();
    },

    // This function is run continuously
    // Use the delay function to prevent it from burning cycles
    // like crazy
    async function loop() {
      const colours = ColourBuffer(strip.numPixels());
      for (var l = 0; l < lights.length; l++) {
        moveLight(lights[l]);
        lights[l].lightFunc(lights[l], colours);
      }

      for (var i = 0; i < colours.length; i++) {
        strip.setPixelColor(i, colours[i]);
      }
      strip.show();
      await arduino.delay(100);
    }
);

function ColourBuffer(x) {
  const buffer = [];
  for (var i = 0; i < x; i++) {
    buffer.push(strip.Color(0, 0, 0));
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

// Adds two colours
function add(c1, c2) {
  return strip.Color(
      clamp(c1.red + c2.red, 255),
      clamp(c1.green + c2.green, 255),
      clamp(c1.blue + c2.blue, 255)
  );
}

// Multiplies a colour
function mult(c, num) {
  return strip.Color(
      clamp(Math.round(c.red * num), 255),
      clamp(Math.round(c.green * num), 255),
      clamp(Math.round(c.blue * num), 255)
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
      strip.Color(rand(0, 255), rand(0, 255), rand(0, 255)),
      rand(0, 1),
      pointLight
  );
  light.dx = rand(-0.2, 0.2);
  light.dy = rand(-0.2, 0.2);
  light.db = rand(-0.2, 0.2);

  return light;
}

function moveLight(light) {
  light.brightness = clamp(light.brightness + light.db, 1);
  if (light.brightness <= 0 || light.brightness >= 1) {
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

function LightSource(x, y, colour, brightness, lightFunc) {
  return { x, y, colour, brightness, lightFunc, dx: 0, dy: 0, db: 0 };
}

function pointLight(lightsource, buffer) {
  const radius = 3;
  for (var x = -radius; x <= radius; x++) {
    for (var y = -radius; y <= radius; y++) {
      const i = index(lightsource.x + x, lightsource.y + y);
      if (i < 0 || i >= strip.numPixels()) {
        continue;
      }
      buffer[i] = add(buffer[i], mult(mult(lightsource.colour, lightsource.brightness), Math.pow(0.50, Math.abs(x) + Math.abs(y))));
    }
  }
}

arduino.start();