static int testlight(lightsource *lights) {
  lights[0] = {
    5, 1,
    0, 0, 0,
    rgb2hsv({1, 1, 1}),
    5,
    roundLight,
    stationary
  };
  return 1;
}

static int blobs(lightsource *lights) {
  for (int n = 0; n < NUM_BLOBS; n++) {
    lights[n] = {
      random(0, COLS),
      random(0, rows()),
      0,
      0,
      0,
      getCurrentTheme()(true),
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
      getCurrentTheme()(true),
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
      random(0, COLS), random(-2, rows() + 1),
      0, 0, 0,
      getCurrentTheme()(false),
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
      random(0, COLS), random(-2, rows() + 1),
      0, 0, 0,
      getCurrentTheme()(true),
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
    getCurrentTheme()(false),
    3,
    verticalLight,
    scan
  };
  lights[1] = {
    5, 0,
    -0.2, 0, 0,
    getCurrentTheme()(false),
    3,
    verticalLight,
    scan
  };
  return 2;
}

static int fireworks(lightsource *lights) {
  for (int n = 0; n < NUM_FIREWORKS; n++) {
    lights[n] = {
      random(0, COLS - 1), random(0, rows() - 1),
      0, 0, 0,
      getCurrentTheme()(true),
      6,
      roundLight,
      explode
    };
    lights[n].colour.v = 0;
  }
  return NUM_FIREWORKS;
}

static int beacon(lightsource *lights) {
  lights[0] = {
    5, 1,
    0, 0, 0,
    getCurrentTheme()(false),
    6,
    horizontalLight,
    pulse
  };
  lights[0].colour.v = 0;
  return 1;
}

static Pattern PATTERNS[NUM_PATTERNS] = { blobs, bands, matrix, fire, cylon, fireworks, beacon };
int currentPattern = random(0, NUM_PATTERNS);
Pattern getCurrentPattern() {
  return PATTERNS[currentPattern];
}
static void changePattern(bool wantRandom) {
  if (wantRandom) {
    currentPattern = random(0, NUM_PATTERNS);
  }
  else if (++currentPattern >= NUM_PATTERNS) {
    currentPattern = 0;
  }
  initScene();
}

