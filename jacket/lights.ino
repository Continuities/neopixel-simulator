#define DENSITY 10

static void roundLight(lightsource *light) {
  rgb lightRgb = hsv2rgb(light->colour);
  for (int x = 0; x < COLS; x++) {
    for (int y = 0; y < ROWS; y++) {
      int i = index(x, y);
      if (i < 0 || i >= strip.numPixels()) continue;
  
      double d = abs(x - light->x) + abs(y - light->y);
      colourBuffer[i] = addColours(colourBuffer[i], multColour(lightRgb, asymp(d, light->fade)));
    }
  }
}

static void verticalLight(lightsource *light) {
  rgb lightRgb = hsv2rgb(light->colour);
  for (int x = 0; x < COLS; x++) {
    for (int y = 0; y < ROWS; y++) {
      int i = index(x, y);
      if (i < 0 || i >= strip.numPixels()) continue;
      double d = abs(x - light->x);
      colourBuffer[i] = addColours(colourBuffer[i], multColour(lightRgb, asymp(d, light->fade)));
    }
  }
}

static void horizontalLight(lightsource *light) {
  rgb lightRgb = hsv2rgb(light->colour);
  for (int x = 0; x < COLS; x++) {
    for (int y = 0; y < ROWS; y++) {
      int i = index(x, y);
      if (i < 0 || i >= strip.numPixels()) continue;
      double d = abs(y - light->y);
      colourBuffer[i] = addColours(colourBuffer[i], multColour(lightRgb, asymp(d, light->fade)));
    }
  }
}
