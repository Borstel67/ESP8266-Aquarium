//#ifdef LEDStrip

String _color;
int _icolor;
int _anzled;
int _anzcolor;
int _k;

void FastLED_Set() {

	const int anzled = atoi(GlobalConfig.ledanzges);// anzahl der led der kette
	FastLED.addLeds<LED_TYPE, LED_STRIP_PIN, COLOR_ORDER>(leds, anzled).setCorrection(TypicalLEDStrip);
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
	_k = 256.0;
	_color = GlobalConfig.ledcolor1;
	Serial.println(GlobalConfig.ledcolor1);
	_icolor = 1;
	_anzled = atoi(GlobalConfig.ledanzges);
	_anzcolor = atoi(GlobalConfig.ledanzcol);
	setAll(_k, _anzled, _anzcolor, _icolor, _color);
}
void LEDsetColosEinK2() {
	// Color 2
	_k = 256.0;
	_color = GlobalConfig.ledcolor2;
	Serial.println(GlobalConfig.ledcolor2);
	_icolor = 2;
	_anzled = atoi(GlobalConfig.ledanzges);
	_anzcolor = atoi(GlobalConfig.ledanzcol);
	setAll(_k, _anzled, _anzcolor, _icolor, _color);
}
void LEDsetColosEinK3() {
	// Color 3
	_k = 256.0;
	_color = GlobalConfig.ledcolor3;
	Serial.println(GlobalConfig.ledcolor3);
	_icolor = 3;
	_anzled = atoi(GlobalConfig.ledanzges);
	_anzcolor = atoi(GlobalConfig.ledanzcol);
	setAll(_k, _anzled, _anzcolor, _icolor, _color);
}
void LEDsetColosEinK4() {
	// Color 4
	_k = 256.0;
	_color = GlobalConfig.ledcolor4;
	Serial.println(GlobalConfig.ledcolor4);
	_icolor = 4;
	_anzled = atoi(GlobalConfig.ledanzges);
	_anzcolor = atoi(GlobalConfig.ledanzcol);
	setAll(_k, _anzled, _anzcolor, _icolor, _color);
}
void LEDsetColosAusK1() {
	// Color 1
	_k = 256.0;
	_color = GlobalConfig.ledcolor5;
	Serial.println(GlobalConfig.ledcolor5);
	_icolor = 1;
	_anzled = atoi(GlobalConfig.ledanzges);
	_anzcolor = atoi(GlobalConfig.ledanzcol);
	setAll(_k, _anzled, _anzcolor, _icolor, _color);
}
void LEDsetColosAusK2() {
	// Color 2
	_k = 256.0;
	_color = GlobalConfig.ledcolor6;
	Serial.println(GlobalConfig.ledcolor6);
	_icolor = 2;
	_anzled = atoi(GlobalConfig.ledanzges);
	_anzcolor = atoi(GlobalConfig.ledanzcol);
	setAll(_k, _anzled, _anzcolor, _icolor, _color);
}
void LEDsetColosAusK3() {
	// Color 3
	_k = 256.0;
	_color = GlobalConfig.ledcolor7;
	Serial.println(GlobalConfig.ledcolor7);
	_icolor = 3;
	_anzled = atoi(GlobalConfig.ledanzges);
	_anzcolor = atoi(GlobalConfig.ledanzcol);
	setAll(_k, _anzled, _anzcolor, _icolor, _color);
}
void LEDsetColosAusK4() {
	// Color 4
	_k = 256.0;
	_color = GlobalConfig.ledcolor8;
	_icolor = 4;
	_anzled = atoi(GlobalConfig.ledanzges);
	_anzcolor = atoi(GlobalConfig.ledanzcol);
	setAll(_k,_anzled, _anzcolor, _icolor, _color);
}

void LEDsetMoon(int _mDay) {
	_anzled = atoi(GlobalConfig.ledanzges);
	_color = GlobalConfig.ledcolormoon;
	char _ledanzmond[255];
	strcpy(_ledanzmond, GlobalConfig.ledanzmond);
	char ledsmond1[4][8]; // array ertse teilung = { {"4"},{"10-20"},{"10"},{"999-999"} };
	char ledsmond2[4][8]; // arrey für zweite teilung = "10-20" [0]=anzahl [1]= anfang [2]= ende
	char* ychr;
	Serial.print(_anzled);
	int i = 1;
	Serial.println(GlobalConfig.ledanzmond);
	splitString(_ledanzmond, ";", ledsmond1);  // Splite String aus Globelconfig
	Serial.println(ledsmond1[0]);
	int y = atoi(ledsmond1[0]);
	Serial.println(y);
	for (i = 1; i < y + 1; i++) {
		if (strchr(ledsmond1[i], '-')) { // sucht nach (-)
			Serial.println(ledsmond1[i]);
			splitString(ledsmond1[i], "-", ledsmond2);  // [0]=Zähler [1]=Anfang [2]=Ende
			setAll(_mDay, atoi(ledsmond2[2]),1,atoi(ledsmond2[1]), _color);// setzen der LED
			Serial.println(ledsmond2[1]);
			Serial.println(ledsmond2[2]);
		}
		else {
			setAll(_mDay, atoi(ledsmond1[i]),1, atoi(ledsmond1[i]), _color);
			Serial.println(ledsmond1[i]);
		}
		Serial.println(i);
		yield();
		//delay(100);
	}
	Serial.println("set moon for end");
}
byte setAll(int k, int al, int ac, int ic, String c) {
	//k = Helligkeit ; al = ende ; ac = Schriweite ; ic = anfang; c = Farbe
	// color nach r/g/b
	  // Get rid of '#' and convert it to integer
	int number = (int)strtol(&c[1], NULL, 16);
	// Split them up into r, g, b values
	int red = number >> 16;
	int green = number >> 8 & 0xFF;
	int blue = number & 0xFF;
	int kred = (k / 256.0) * red;
	int kgreen = (k / 256.0) * green;
	int kblue = (k / 256.0) * blue;
	int ipix_end = al;
	//- (ac - ic);
	for (int ipix = ic - 1; ipix < ipix_end; ipix = ipix + ac) {
		//Serial.println(ipix);
		leds[ipix].setRGB(kred, kgreen, kblue);
	}
	FastLED.show();
	//delay(30);
	FastLED.delay(1000 / FRAMES_PER_SECOND);
}
/* funktion setzen der Mondhelligkeit
	k= Mondtagstufe(1-15), an=LED anfang, en= LED ende, c=Farbe rgb
	color nach r/g/b
	Get rid of '#' and convert it to integer
*/
//byte setMoon(int k, int an, int en, String c) {
//	int number = (int)strtol(&c[1], NULL, 16);
//	// Split them up into r, g, b values
//	int red = number >> 16;
//	int green = number >> 8 & 0xFF;
//	int blue = number & 0xFF;
//	// end Split
//	int kred = (k / 256.0) * red;
//	int kgreen = (k / 256.0) * green;
//	int kblue = (k / 256.0) * blue;
//	// set LED
//	for (int ipix = an; ipix < en; ipix = ipix + 1) {
//		leds[ipix].setRGB(kred, kgreen, kblue);
//		//		setPixel(ipix, kred, kgreen, kblue);
//	}
//
//	FastLED.show();
//	delay(30);
//}
//byte setAll(int al, int ac, int ic, String c) {
//  // color nach r/g/b
//  // Get rid of '#' and convert it to integer
//  int number = (int) strtol( &c[1], NULL, 16);
//  // Split them up into r, g, b values
//  int red = number >> 16;
//  int green = number >> 8 & 0xFF;
//  int blue = number & 0xFF;
//  delay(100);
//  //    Serial.print("nach yield() Farbe");
//  //delay(10);
//  // set LED
//  int ipix_end = al;
//  //- (ac - ic);
//  for (int ipix = ic - 1; ipix < ipix_end; ipix = ipix + ac) {
//    //        //Serial.println(ipix);
//    setPixel(ipix, red, green, blue);
//  }
//  FastLED.show();
//}

//int setPixel(int Pixel, byte red, byte green, byte blue) {
//	// FastLED
//	leds[Pixel].r = red;
//	leds[Pixel].g = green;
//	leds[Pixel].b = blue;
//}
//#endif // LEDStrip