static void roundLight(lightsource *light) {
  rgb lightRgb = hsv2rgb(light->colour);
  for (int x = 0; x < COLS; x++) {
    for (int y = 0; y < ROWS; y++) {
      int i = index(x, y);
      if (i < 0 || i >= strip.numPixels()) continue;
  
      int d = abs((x * DENSITY) - light->x) + abs((y * DENSITY) - light->y);
      double fade = (double)expo(DENSITY, light->fade) / expo(d + DENSITY, light->fade);
      if (d > (7 - light->fade) * DENSITY) {
        fade = 0;
      }
      if (fade > 1) {
        // double overflow. Oops.
        fade = 0;
      }
//      if (i == index(2, 1)) { Serial.print(d); Serial.print(" :: "); Serial.print(expo(d + DENSITY, light->fade)); Serial.print("\n"); }
      colourBuffer[i] = addColours(colourBuffer[i], multColour(lightRgb, fade));

    }
  }
}

//static void verticalLight(lightsource *light) {
//  rgb lightRgb = hsv2rgb(light->colour);
//  for (int x = 0; x < COLS; x++) {
//    for (int y = 0; y < ROWS; y++) {
//      int i = index(x, y);
//      if (i < 0 || i >= strip.numPixels()) continue;
//      double d = abs(x - light->x);
//      colourBuffer[i] = addColours(colourBuffer[i], multColour(lightRgb, asymp(d, light->fade)));
//    }
//  }
//}
//
//static void horizontalLight(lightsource *light) {
//  rgb lightRgb = hsv2rgb(light->colour);
//  for (int x = 0; x < COLS; x++) {
//    for (int y = 0; y < ROWS; y++) {
//      int i = index(x, y);
//      if (i < 0 || i >= strip.numPixels()) continue;
//      double d = abs(y - light->y);
//      colourBuffer[i] = addColours(colourBuffer[i], multColour(lightRgb, asymp(d, light->fade)));
//    }
//  }
//}
