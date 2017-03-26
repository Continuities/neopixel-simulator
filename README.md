# neopixel-simulator
Simulates a neopixel strip in javascript for quick prototyping.

## Installation
1. Clone repository
2. `npm install`
3. `npm start`
4. Visit `http://localhost:8080` to view the test pattern

## Write your own patterns!
Modify the code in `index.js` to quickly prototype your Neopixel patterns
without compiling and uploading to an Arduino board. It's pretty close to
real Arduino C syntax, so it's easily portable to the real hardware.

## Custom pixel layout
By default, the Neopixel simulator will generate a straight strip of pixels.
If you want to simulate a more complex layout, you can create the elements yourself
in `src/index.html`. The only requirement is that each pixel have the `pixel` class, and
a `p-0` class (where 0 is the index of the pixel, incrementing).

Instead of `new Neopixel(NUM_PIXELS)` instantiate your strip with `Neopixel.fromElement(STRIP_ELEMENT)`.

## Arduino runtime simulator
The Arduino constructor takes two functions: `begin` and `loop`. 
`begin` is called once when the Arduino runtimes starts. `loop` is called
continuously. Unlike javascript, Arduino is synchronous and you call `delay`
to sleep between loop executions. In javascript, you need to use the `await` keyword
and the `delay` function on the Arduino runtime object.

