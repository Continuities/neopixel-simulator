/**
 * Mimics the Neopixel library from Adafruit
 * https://github.com/adafruit/Adafruit_NeoPixel
 */

'use strict';

import css from '../css/neopixel.css';

const IndexPattern = /p-(\d+)/;

const PIXELS = Symbol();
const STRIP = Symbol();
const AMMETER = Symbol();
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
   * Creates a Neopixel strip from a pre-created DOM element
   * @param stripElement The element containing the pixels
   * @returns {Neopixel}
   */
  static fromElement(stripElement) {
    const strip = new Neopixel(0);
    strip[STRIP] = stripElement;
    strip[PIXELS] = Array.prototype.map.call(stripElement.querySelectorAll('.pixel'), e => Pixel.fromElement(e))
      .sort((a, b) => {
        const ai = IndexPattern.exec(a.element.className)[1];
        const bi = IndexPattern.exec(b.element.className)[1];
        return ai - bi;
      });
    return strip;
  }

  /**
   * Renders the pixels to the page
   */
  begin() {
    if (this[STRIP] == null) {
      const strip = document.createElement('ul');
      this[STRIP] = strip;
      strip.className = 'pixel-strip';
      this[PIXELS].forEach((pixel) => {
        strip.appendChild(pixel.element);
      });
      document.body.appendChild(strip);
    }

    const ammeter = this[AMMETER] = document.createElement('div');
    ammeter.className = 'ammeter';
    this[STRIP].parentNode.insertBefore(ammeter, this[STRIP]);
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
    this[AMMETER].innerText = Math.round(this[PIXELS]
        .reduce((current, pixel) => current + pixel.updateColour(), 0));
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
    this.colour = colour;
    this.element = document.createElement('li');
    this.element.className = `pixel p-${index}`;
    this.updateColour();
  }

  static fromElement(element) {
    const pixel = new Pixel(0, new Color(0, 0, 0));
    pixel.element = element;
    pixel.updateColour();
    return pixel;
  }

  updateColour() {
    this.element.style.background = `rgb(${this.colour.red}, ${this.colour.green}, ${this.colour.blue})`;

    // Estimate the current drawn by this pixel
    return ((this.colour.red + this.colour.blue + this.colour.green) / (255 * 3)) * 60;
  }

}