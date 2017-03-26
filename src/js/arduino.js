/**
 * Mimics an Arduino runtime
 * for use with the Neopixel strip simulator
 **/

'use strict';

const BEGIN = Symbol('begin');
const LOOP = Symbol('loop');

export default class Arduino {

  /**
   * Construct an Arduino runtime
   * @param begin Function run when the Arduino "boots"
   * @param loop Function run repeatedly
   */
  constructor(begin, loop) {
    this[BEGIN] = begin;
    this[LOOP] = loop;
  }

  /**
   * Boots the arduino, causing the begin function to run
   * and starts the repeated loop calls.
   */
  async start() {
    this[BEGIN]();
    const self = this;
    (async function loop() {
      await self[LOOP]();
      setTimeout(loop, 0);
    })();
  }

  /**
   * await this function to sleep. Simulates the arduino
   * synchronous delay.
   * @param millis Milliseconds to sleep
   * @returns {Promise}
   */
  delay(millis) {
    return new Promise(resolve => setTimeout(resolve, millis));
  }
};