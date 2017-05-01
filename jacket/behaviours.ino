static void stationary(lightsource *light) {
  light->dx = 0;
  light->dy = 0;
  light->db = 0;
}

static void bounce(lightsource *light) {
  if (light->dx == 0) light->dx = randomDouble(0.01, 0.1) * (random(100) > 50 ? -1 : 1);
  if (light->dy == 0) light->dy = randomDouble(0.005, 0.05) * (random(100) > 50 ? -1 : 1);
  if (light->db == 0) light->db = randomDouble(0.001, 0.01) * (random(100) > 50 ? -1 : 1);
  if (light->colour.v <= 0.1 || light->colour.v >= 1) {
    light->colour.v = clamp(light->colour.v, 0.1, 1);
    light->db *= -1;
  }
  if (light->x <= 0 || light->x >= COLS - 1) {
    light->x = clamp(light->x, COLS - 1);
    light->dx *= -1;
  }
  if (light->y <= 0 || light->y >= rows() - 1) {
    light->y = clamp(light->y, rows() - 1);
    light->dy *= -1;
  }
}

static void scan(lightsource *light) {
  if (light->dx == 0) light->dx = randomDouble(0.01, 0.1) * (random(100) > 50 ? -1 : 1);
  if (light->x <= 0 || light->x >= COLS - 1) {
    light->dx *= -1;
  }
}

static void twinkle(lightsource *light) {
  if (light->colour.v <= 0) {
    light->colour = { randomDouble(0, 1), 1, 0.4 };
    light->db = 0;
  }
  else if (light->db == 0) {
    if (randomDouble(0, 1) <= TWINKLOCITY) {
      light->db = -0.03;
    }
  }
  else if (light->db > 0 && light->colour.v > 0.5) {
    light->db = 0;
  }
}

static void rain(lightsource *light) {
  if (light->dy == 0) light->dy = randomDouble(0.05, 0.2);
  if (light->y > rows() + 2) {
    light->y = -2;
    light->x = randomDouble(0, COLS -1);
    light->dy = randomDouble(0.05, 0.2);
  }
}

static void smoke(lightsource *light) {
  if (light->dx == 0) light->dx = randomDouble(-0.1, 0.1);
  if (light->dy == 0) light->dy = randomDouble(-0.1, -0.2);
  if (light->y < -1) {
    light->y = rows() + 1;
    light->x = randomDouble(0, COLS - 1);
    light->dy = randomDouble(-0.1, -0.2);
  }
  if (randomDouble(0, 1) < WISPINESS) {
    light->dx *= -1;
  }
}

static void explode(lightsource *light) {
  light->db = -0.02;
  light->fade = max(2, light->fade - 1);
  if (light->colour.v <= 0 && randomDouble(0, 1) < VOLATILITY) {
    light->fade = 6;
    light->colour = getCurrentTheme()(true);
    light->colour.v = 1;
    light->x = random(0, COLS - 1);
    light->y = random(0, rows() - 1);
  }
}

static void pulse(lightsource *light) {
  light->db = -0.01;
  light->fade = max(2, light->fade - 0.25);
  if (light->colour.v <= 0) {
    light-> fade = 6;
    light->colour.v = 1;
  }
}


