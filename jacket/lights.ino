#define DENSITY 10

static void roundLight(lightsource *light) {
  rgb lightRgb = hsv2rgb(light->colour);
  for (int x = 0; x < COLS; x++) {
    for (int y = 0; y < rows(); y++) {
      double d = abs(x - light->x) + abs(y - light->y);
      addToBuffer(x, y, multColour(lightRgb, asymp(d, light->fade)));
    }
  }
}

static void verticalLight(lightsource *light) {
  rgb lightRgb = hsv2rgb(light->colour);
  for (int x = 0; x < COLS; x++) {
    for (int y = 0; y < rows(); y++) {
      double d = abs(x - light->x);
      addToBuffer(x, y, multColour(lightRgb, asymp(d, light->fade)));
    }
  }
}

static void horizontalLight(lightsource *light) {
  rgb lightRgb = hsv2rgb(light->colour);
  for (int x = 0; x < COLS; x++) {
    for (int y = 0; y < rows(); y++) {
      double d = abs(y - light->y);
      addToBuffer(x, y, multColour(lightRgb, asymp(d, light->fade)));
    }
  }
}
