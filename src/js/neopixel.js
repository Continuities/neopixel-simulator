/**
 * Mimics the Neopixel library from Adafruit
 * https://github.com/adafruit/Adafruit_NeoPixel
 */

'use strict';

import css from '../css/neopixel.css';

const PIXELS = Symbol();
export default class Neopixel {

  /**
   * Constructs a Neopixel Strip
   * @param numPixels The number of pixels in the strip
   */
  constructor(numPixels) {
    const pixels = [];
    this[PIXELS] = pixels;
    for (var i = 0; i < numPixels; i++) {
      pixels.push(new Pixel(i, new Color(0, 0, 0)));
    }
  }

  /**
   * Renders the pixels to the page
   */
  begin() {
    const strip = document.createElement('ul');

    strip.className = 'pixel-strip';
    this[PIXELS].forEach((pixel) => {
      strip.appendChild(pixel.element);
    });

    document.body.appendChild(strip);
  }

  /**
   * Sets the colour of an in-memory pixel object
   * @param index The index of the pixel to set
   * @param colour The colour to set the pixel
   */
  setPixelColor(index, colour) {
    const pixel = this[PIXELS][index];

    if (pixel) {
      pixel.colour = colour;
    }
  }

  /**
   * Updates the display colour of all pixsels in the strip
   */
  show() {
    this[PIXELS].forEach(p => p.updateColour());
  }

  /**
   *
   * @returns {*}
   */
  numPixels() {
    return this[PIXELS].length;
  }

  /**
   * Creates a Color object
   * @param r Red byte (0-255)
   * @param g Green byte (0-255)
   * @param b Blue byte (0-255)
   * @returns {Color} Color object
   * @constructor
   */
  Color(r, g, b) {
    return new Color(r, g, b);
  }
}

class Color {
  constructor(r, g, b) {
    this.red = r;
    this.green = g;
    this.blue = b;
  }
}

class Pixel {
  constructor(index, colour) {
    this.index = index;
    this.colour = colour;
    this.element = document.createElement('li');
    this.element.className = `pixel p-${index}`;
    this.updateColour();
  }

  updateColour() {
    this.element.style.background = `rgb(${this.colour.red}, ${this.colour.green}, ${this.colour.blue})`;
  }

}