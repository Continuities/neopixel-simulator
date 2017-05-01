// Transforms a cartesian coordinate into a pixel index on the back strip
static int backIndex(double x, double y) {
  return backIndex((int)round(x), (int)round(y));
}

// Transforms a cartesian coordinate into a pixel index on the back strip
static int backIndex(int x, int y) {
  if (x < 0 || x >= COLS || y < 0 || y >= BACK_ROWS) {
    return -1;
  }
  return (x * 3) + (x % 2 > 0 ? 2 - y : y);
}

// Transforms a cartesian coordinate into a pixel index on the back strip
static int collarIndex(double x, double y) {
  return collarIndex((int)round(x), (int)round(y));
}

static int collarIndex(int x, int y) {
  if (x < 0 || x >= COLS || y < 0 || y >= COLLAR_ROWS) {
    return -1;
  }
  return (x * 2) + (x % 2 == 0 ? 1 - y : y);
}

static int rows() {
  return BACK_ROWS + COLLAR_ROWS;
}

// Clamps a float betweeen min and max
static float clamp(float num, float mini, float maxi) {
  return max(mini, min(num, maxi));
}

// Clamps a float between 0 and maximum
static float clamp(float num, float maximum) {
  return clamp(num, 0, maximum);
}

// Get a random double between min and max
static double randomDouble(double minf, double maxf) {
  return minf + random(1UL << 31) * (maxf - minf) / (1UL << 31);  // use 1ULL<<63 for max double values)
}

static void printRGB(rgb colour) {
  Serial.print("{ ");
  Serial.print(colour.r * 255);
  Serial.print(", ");
  Serial.print(colour.g * 255);
  Serial.print(", ");
  Serial.print(colour.b * 255);
  Serial.print(" }");
}

static double asymp(double x, int a) {
  // Higher powers are slow, so this is actually faster
  double b = x + 1;
  for (int i = 0; i < a; i++) {
    b *= (x + 1);
  }
  return 1 / b;
}

void addToBuffer(int x, int y, rgb colour) {

  if (y < COLLAR_ROWS) {
    // Write to the collar strip
    int i = collarIndex(x, y);
    if (i < 0 || i >= collar_strip.numPixels()) return; 
    collarBuffer[i] = addColours(collarBuffer[i], colour);
  }
  else {
    // Write to the back strip
    int i = backIndex(x, y - COLLAR_ROWS);
    if (i < 0 || i >= back_strip.numPixels()) return; 
    backBuffer[i] = addColours(backBuffer[i], colour);
  }
}



