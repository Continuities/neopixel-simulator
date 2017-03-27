'use strict';

import Arduino from './arduino';
import Neopixel from './neopixel';

const PIXELS = 10;
const strip = Neopixel.fromElement(document.querySelector('.custom-strip'));

const arduino = new Arduino(

    // This function is run once when the arduino starts
    function begin() {
      strip.begin();
      strip.show();
    },

    // This function is run continuously
    // Use the delay function to prevent it from burning cycles
    // like crazy
    async function loop() {
      await chase(strip.Color(255, 0, 0)); // Red
      await chase(strip.Color(0, 255, 0)); // Green
      await chase(strip.Color(0, 0, 255)); // Blue
    }
);

// Neopixel test pattern
async function chase(c) {
  for (var i = 0; i < strip.numPixels() + 4; i++) {
    strip.setPixelColor(i, c); // Draw new pixel
    strip.setPixelColor(i - 4, strip.Color(0, 0, 0)); // Erase pixel a few steps back
    strip.show();
    await arduino.delay(100);
  }
}

arduino.start();