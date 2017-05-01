
hsv rgb2hsv(rgb in) {
  hsv         out;
  double      min, max, delta;

  min = in.r < in.g ? in.r : in.g;
  min = min  < in.b ? min  : in.b;

  max = in.r > in.g ? in.r : in.g;
  max = max  > in.b ? max  : in.b;

  out.v = max;                                // v
  delta = max - min;
  if (delta < 0.00001)
  {
    out.s = 0;
    return out;
  }
  if( max > 0.0 ) { // NOTE: if Max is == 0, this divide would cause a crash
    out.s = (delta / max);                  // s
  } else {
    // if max is 0, then r = g = b = 0              
    // s = 0, v is undefined
    out.s = 0.0;
    out.h = NAN;                            // its now undefined
    return out;
  }
  if( in.r >= max )                           // > is bogus, just keeps compilor happy
    out.h = ( in.g - in.b ) / delta;        // between yellow & magenta
  else
  if( in.g >= max )
    out.h = 2.0 + ( in.b - in.r ) / delta;  // between cyan & yellow
  else
    out.h = 4.0 + ( in.r - in.g ) / delta;  // between magenta & cyan

  out.h *= 60.0;                              // degrees

  if( out.h < 0.0 )
    out.h += 360.0;

  return out;
}

rgb hsv2rgb(hsv in) {
  // TODO: This function is super slow for some reason. Figure out how to make it faster, or
  //       just avoid using it so often
  double      hh, p, q, t, ff;
  long        i;
  rgb         out;

  if(in.s <= 0.0) {       // < is bogus, just shuts up warnings
    out.r = in.v;
    out.g = in.v;
    out.b = in.v;
    return out;
  }
  hh = in.h;
  if(hh >= 360.0) hh = 0.0;
  hh /= 60.0;
  i = (long)hh;
  ff = hh - i;
  p = in.v * (1.0 - in.s);
  q = in.v * (1.0 - (in.s * ff));
  t = in.v * (1.0 - (in.s * (1.0 - ff)));
  switch(i) {
  case 0:
    out.r = in.v;
    out.g = t;
    out.b = p;
    break;
  case 1:
    out.r = q;
    out.g = in.v;
    out.b = p;
    break;
  case 2:
    out.r = p;
    out.g = in.v;
    out.b = t;
    break;

  case 3:
    out.r = p;
    out.g = q;
    out.b = in.v;
    break;
  case 4:
    out.r = t;
    out.g = p;
    out.b = in.v;
    break;
  case 5:
  default:
    out.r = in.v;
    out.g = p;
    out.b = q;
    break;
  }
  return out;     
}

// Dims an rgb colour by a factor of [0, 1]
rgb dim(rgb c, float amount) {
  return {
    c.r - (c.r * amount),
    c.g - (c.g * amount),
    c.b - (c.b * amount)
  };
}

rgb multColour(rgb c, float amount) {
  return {
    c.r * amount,
    c.g * amount,
    c.b * amount
  };
}

// Adds two RGB colours
rgb addColours(rgb c1, rgb c2) {
  return {
    clamp(c1.r + c2.r, 1),
    clamp(c1.g + c2.g, 1),
    clamp(c1.b + c2.b, 1)
  };
}

