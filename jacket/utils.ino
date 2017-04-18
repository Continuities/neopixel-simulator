// Transforms a cartesian coordinate into a pixel index
static int index(double x, double y) {
  return index((int)round(x), (int)round(y));
}

// Transforms a cartesian coordinate into a pixel index
static int index(int x, int y) {
  if (x < 0 || x >= COLS || y < 0 || y >= ROWS) {
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

static double asymp(int x, int a) {
  return 1 / expo(x, a);
}

unsigned long expo(int a, int b) {
  if (b==1)
    return a;
  if (b==2)
    return square(a);

  if (b%2==0){
    return expo(expo(a,b/2),2);
  }
  else{
    return a*expo(expo(a,(b-1)/2),2);
  }
}

void printDouble( double val, byte precision){
 // prints val with number of decimal places determine by precision
 // precision is a number from 0 to 6 indicating the desired decimial places
 // example: printDouble( 3.1415, 2); // prints 3.14 (two decimal places)

 Serial.print (int(val));  //prints the int part
 if( precision > 0) {
   Serial.print("."); // print the decimal point
   unsigned long frac;
   unsigned long mult = 1;
   byte padding = precision -1;
   while(precision--)
      mult *=10;
      
   if(val >= 0)
     frac = (val - int(val)) * mult;
   else
     frac = (int(val)- val ) * mult;
   unsigned long frac1 = frac;
   while( frac1 /= 10 )
     padding--;
   while(  padding--)
     Serial.print("0");
   Serial.print(frac,DEC) ;
 }
}



