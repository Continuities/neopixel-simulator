static int testlight(lightsource *lights) {
  lights[0] = {
    5, 1,
    0, 0, 0,
    rgb2hsv({1, 0, 0}),
    5,
    roundLight,
    bounce
  };
  return 1;
}

static int blobs(lightsource *lights) {
  for (int n = 0; n < NUM_BLOBS; n++) {
    lights[n] = {
      random(0, COLS),
      random(0, ROWS),
      0,
      0,
      0,
      {
        randomDouble(0, 360),
        1,
        randomDouble(0, 1)
      },
      2,
      roundLight,
      bounce
    };
  }
  return NUM_BLOBS;
}

static int bands(lightsource *lights) {
    for (int n = 0; n < NUM_BLOBS; n++) {
    lights[n] = {
      random(0, COLS),
      1,
      0,
      0,
      0,
      {
        randomDouble(0, 360),
        1,
        0.5
      },
      3,
      verticalLight,
      bounce
    };
  }
  return NUM_BLOBS;
}

static int matrix(lightsource *lights) {
  for (int n = 0; n < 5; n++) {
    lights[n] = {
      random(0, COLS), random(-2, ROWS + 1),
      0, 0, 0,
      rgb2hsv({ 100/255, 1, 100/255 }),
      3,
      roundLight,
      rain
    };
  }
  return 5;
}

static int fire(lightsource *lights) {
    for (int n = 0; n < 5; n++) {
    lights[n] = {
      random(0, COLS), random(-2, ROWS + 1),
      0, 0, 0,
      rgb2hsv({ 1, randomDouble(0, 0.8), 0 }),
      3,
      roundLight,
      smoke
    };
  }
  return 5;
}

static int cylon(lightsource *lights) {
  lights[0] = {
    5, 0,
    0.2, 0, 0,
    rgb2hsv({1, 100/255, 100/255}),
    3,
    verticalLight,
    scan
  };
  lights[1] = {
    5, 0,
    -0.2, 0, 0,
    rgb2hsv({1, 180/255, 180/255}),
    3,
    verticalLight,
    scan
  };
  return 2;
}

static int fireworks(lightsource *lights) {
  for (int n = 0; n < NUM_FIREWORKS; n++) {
    lights[n] = {
      random(0, COLS - 1), random(0, ROWS - 1),
      0, 0, 0,
      { random(0, 360), 1, 0 },
      6,
      roundLight,
      explode
    };
  }
  return NUM_FIREWORKS;
}

static int beacon(lightsource *lights) {
  lights[0] = {
    5, 1,
    0, 0, 0,
    { random(0, 360), 1, 0 },
    6,
    horizontalLight,
    pulse
  };
  return 1;
}

