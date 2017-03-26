/**
 * Mimics an Arduino runtime
 **/

'use strict';

const BEGIN = Symbol('begin');
const LOOP = Symbol('loop');
const DELAY = Symbol('delay');

export default class Arduino {
  constructor(begin, loop) {
    this[BEGIN] = begin;
    this[LOOP] = loop;
  }

  async start() {
    this[BEGIN]();
    const self = this;
    (async function loop() {
      await self[LOOP]();
      setTimeout(loop, 0);
    })();
  }

  delay(millis) {
    return new Promise(resolve => setTimeout(resolve, millis));
  }
};