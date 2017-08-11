static hsv bsod(bool wantRandom) {
  if (wantRandom) {
    hsv c = rgb2hsv({ randomDouble(0, 0.4), randomDouble(0, 0.6), 0.8 });
    c.v = randomDouble(0, 1);
    return c;
  }
  else {
    return rgb2hsv({ 0, 0, 1 });
  }
}

static hsv empire(bool wantRandom) {
  if (wantRandom) {
    hsv c = rgb2hsv({ 1, randomDouble(0, 0.8), 0 });
    c.v = randomDouble(0, 1);
    return c;
  }
  else {
    return rgb2hsv({ 1, 0, 0 });
  }
}

static hsv rainbow(bool wantRandom) {
  return { random(0, 360), 1, randomDouble(0, 1) };
}

static Theme THEMES[NUM_THEMES] = { bsod, empire, rainbow };
static int currentTheme = random(0, NUM_THEMES);
Theme getCurrentTheme() {
  return THEMES[currentTheme];
}
void changeTheme() {
  if (++currentTheme >= NUM_THEMES) {
    currentTheme = 0;
  }
  initScene();
}


