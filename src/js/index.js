'use strict';

import Arduino from './arduino';
import Neopixel from './neopixel';

const PIXELS = 20;
const strip = new Neopixel(PIXELS);

const arduino = new Arduino(
    function begin() {
      strip.begin();
      strip.show();
    },

    async function loop() {
      await chase(strip.Color(255, 0, 0)); // Red
      await chase(strip.Color(0, 255, 0)); // Green
      await chase(strip.Color(0, 0, 255)); // Blue
    }
);

async function chase(c) {
  for (var i = 0; i < strip.numPixels() + 4; i++) {
    strip.setPixelColor(i, c); // Draw new pixel
    strip.setPixelColor(i - 4, strip.Color(0, 0, 0)); // Erase pixel a few steps back
    strip.show();
    await arduino.delay(100);
  }
}

arduino.start();