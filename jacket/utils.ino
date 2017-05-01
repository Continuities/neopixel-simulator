// Transforms a cartesian coordinate into a pixel index
static int index(double x, double y) {
  return index((int)round(x), (int)round(y));
}

// Transforms a cartesian coordinate into a pixel index
static int index(int x, int y) {
  if (x < 0 || x >= COLS || y < 0 || y >= BACK_ROWS) {
    return -1;
  }
  return (x * 3) + (x % 2 > 0 ? 2 - y : y);
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



