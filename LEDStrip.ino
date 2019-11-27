//#ifdef LEDStrip

String _color;
int _icolor;
int _anzled;
int _anzcolor;

void FastLED_Set() {
	
	const int anzled = atoi(GlobalConfig.ledanzges);// anzahl der led der kette
	FastLED.addLeds<WS2812, LED_STRIP_PIN, GRB>(leds, anzled).setCorrection(TypicalLEDStrip);
//	LEDSwitchK1();
//	LEDSwitchK2();
//	LEDSwitchK3();
//	LEDSwitchK4();
}

// setze alle led
void LEDsetColorsAll() {
  LEDsetColosEinK1();
  LEDsetColosEinK2();
  LEDsetColosEinK3();
  LEDsetColosEinK4();
  LEDsetColosAusK1();
  LEDsetColosAusK2();
  LEDsetColosAusK3();
  LEDsetColosAusK4();
}
void LEDsetColosEinK1() {
  // Color 1
  _color = GlobalConfig.ledcolor1;
  Serial.println(GlobalConfig.ledcolor1);
  _icolor = 1;
  _anzled = atoi(GlobalConfig.ledanzges);
  _anzcolor = atoi(GlobalConfig.ledanzcol);
  setAll(_anzled, _anzcolor, _icolor, _color);
}
void LEDsetColosEinK2() {
  // Color 2
	_color = GlobalConfig.ledcolor2;
	Serial.println(GlobalConfig.ledcolor2);
	_icolor = 2;
	_anzled = atoi(GlobalConfig.ledanzges);
	_anzcolor = atoi(GlobalConfig.ledanzcol);
	setAll(_anzled, _anzcolor, _icolor, _color);
}
void LEDsetColosEinK3() {
  // Color 3
	_color = GlobalConfig.ledcolor3;
	Serial.println(GlobalConfig.ledcolor3);
	_icolor = 3;
	_anzled = atoi(GlobalConfig.ledanzges);
	_anzcolor = atoi(GlobalConfig.ledanzcol);
	setAll(_anzled, _anzcolor, _icolor, _color);
}
void LEDsetColosEinK4() {
  // Color 4
	_color = GlobalConfig.ledcolor4;
	Serial.println(GlobalConfig.ledcolor4);
	_icolor = 4;
	_anzled = atoi(GlobalConfig.ledanzges);
	_anzcolor = atoi(GlobalConfig.ledanzcol);
	setAll(_anzled, _anzcolor, _icolor, _color);
}
void LEDsetColosAusK1() {
  // Color 1
	_color = GlobalConfig.ledcolor5;
	Serial.println(GlobalConfig.ledcolor5);
	_icolor = 1;
	_anzled = atoi(GlobalConfig.ledanzges);
	_anzcolor = atoi(GlobalConfig.ledanzcol);
	setAll(_anzled, _anzcolor, _icolor, _color);
}
void LEDsetColosAusK2() {
  // Color 2
	_color = GlobalConfig.ledcolor6;
	Serial.println(GlobalConfig.ledcolor6);
	_icolor = 2;
	_anzled = atoi(GlobalConfig.ledanzges);
	_anzcolor = atoi(GlobalConfig.ledanzcol);
	setAll(_anzled, _anzcolor, _icolor, _color);
}
void LEDsetColosAusK3() {
  // Color 3
	_color = GlobalConfig.ledcolor7;
	Serial.println(GlobalConfig.ledcolor7);
	_icolor = 3;
	_anzled = atoi(GlobalConfig.ledanzges);
	_anzcolor = atoi(GlobalConfig.ledanzcol);
	setAll(_anzled, _anzcolor, _icolor, _color);
}
void LEDsetColosAusK4() {
  // Color 4
	_color = GlobalConfig.ledcolor8;
	_icolor = 4;
	_anzled = atoi(GlobalConfig.ledanzges);
	_anzcolor = atoi(GlobalConfig.ledanzcol);
	setAll(_anzled, _anzcolor, _icolor, _color);
}

byte setAll(int al, int ac, int ic, String c) {
  // color nach r/g/b
  // Get rid of '#' and convert it to integer
  int number = (int) strtol( &c[1], NULL, 16);
  // Split them up into r, g, b values
  int red = number >> 16;
  int green = number >> 8 & 0xFF;
  int blue = number & 0xFF;
  delay(20);
  //    Serial.print("nach yield() Farbe");
  //delay(10);
  // set LED
  int ipix_end = al;
  //- (ac - ic);
  for (int ipix = ic - 1; ipix < ipix_end; ipix = ipix + ac) {
    //        //Serial.println(ipix);
    setPixel(ipix, red, green, blue);
  }
  FastLED.show();
}

int setPixel(int Pixel, byte red, byte green, byte blue) {
  // FastLED
  leds[Pixel].r = red;
  leds[Pixel].g = green;
  leds[Pixel].b = blue;
}
//#endif // LEDStrip