//#ifdef LEDStrip

bool SZ11 = false, SZ12 = false, KE1 = false, KA1 = false, HKE1 = false, HKA1 = false;
bool SZ21 = false, SZ22 = false, KE2 = false, KA2 = false, HKE2 = false, HKA2 = false;
bool SZ31 = false, SZ32 = false, KE3 = false, KA3 = false, HKE3 = false, HKA3 = false;
bool SZ41 = false, SZ42 = false, KE4 = false, KA4 = false, HKE4 = false, HKA4 = false;

//String colmoon;
bool SZM = false, HKEM = false, HKAM = false;

// rücksetzen nach speicherung zeiten
void LEDSwitchSave() {
	bool SZ11 = false, SZ12 = false, KE1 = false, KA1 = false, HKE1 = false, HKA1 = false;
	bool SZ21 = false, SZ22 = false, KE2 = false, KA2 = false, HKE2 = false, HKA2 = false;
	bool SZ31 = false, SZ32 = false, KE3 = false, KA3 = false, HKE3 = false, HKA3 = false;
	bool SZ41 = false, SZ42 = false, KE4 = false, KA4 = false, HKE4 = false, HKA4 = false;
	LEDSwitch();
}

// led schalten
void LEDSwitch() {
	char _buffe[VARSIZE6];
	char _buffa[VARSIZE6];
	//Kanal 1
	switch (atoi(GlobalConfig.hk1)) {
	case 0: // Hand Aus //do something when var equals 1
		if (!HKA1) {
			LEDsetColosAusK1();
			HKE1 = false;
			HKA1 = true;
		}
		break;
	case 1: // Hand Ein //do something when var equals 2
		if (!HKE1) {
			LEDsetColosEinK1();
			HKE1 = true;
			HKA1 = false;
		}
		break;
	case 2: // Uhr //do something when var equals 2
		strcpy(_buffe, GlobalConfig.time1);
		strcpy(_buffa, GlobalConfig.time5);
		SZ11 = Schalten(_buffe, _buffa);
		strcpy(_buffe, GlobalConfig.time11);
		strcpy(_buffa, GlobalConfig.time51);
		SZ12 = Schalten(_buffe, _buffa);

		if ((SZ11 || SZ12) && !KE1) {
			DEBUG("K1 Ein");
			LEDsetColosEinK1();
			KE1 = true;
			KA1 = false;
		}
		if (!SZ11 && !SZ12 && !KA1) {
			DEBUG("K1 Aus");
			LEDsetColosAusK1();
			KE1 = false;
			KA1 = true;
		}
		//      }
		break;
	default:
		// if nothing else matches, do the default // default is optional
		strcpy(_buffe, GlobalConfig.time1);
		strcpy(_buffa, GlobalConfig.time5);
		SZ11 = Schalten(_buffe, _buffa);
		strcpy(_buffe, GlobalConfig.time11);
		strcpy(_buffa, GlobalConfig.time51);
		SZ12 = Schalten(_buffe, _buffa);

		if ((SZ11 || SZ12) && !KE1) {
			LEDsetColosEinK1();
			KE1 = true;
			KA1 = false;
		}
		if ((!SZ11 || !SZ12) && !KA1) {
			LEDsetColosAusK1();
			KA1 = true;
			KE1 = false;
		}
		break;
	}

	//Kanal 2
	switch (atoi(GlobalConfig.hk2)) {
	case 0: // Hand Aus //do something when var equals 1
		if (!HKA2) {
			LEDsetColosAusK2();
			HKE2 = false;
			HKA2 = true;
		}
		break;
	case 1: // Hand Ein //do something when var equals 2
		if (!HKE2) {
			LEDsetColosEinK2();
			HKE2 = true;
			HKA2 = false;
		}
		break;
	case 2: // Uhr //do something when var equals 2
		strcpy(_buffe, GlobalConfig.time2);
		strcpy(_buffa, GlobalConfig.time6);
		SZ21 = Schalten(_buffe, _buffa);
		strcpy(_buffe, GlobalConfig.time21);
		strcpy(_buffa, GlobalConfig.time61);
		SZ22 = Schalten(_buffe, _buffa);
		if ((SZ21 || SZ22) && !KE2) {
			LEDsetColosEinK2();
			KE2 = true;
			KA2 = false;
		}
		if (!SZ21 && !SZ22 && !KA2) {
			LEDsetColosAusK2();
			KA2 = true;
			KE2 = false;
		}
		//      }
		break;
	default:
		// if nothing else matches, do the default // default is optional
		strcpy(_buffe, GlobalConfig.time2);
		strcpy(_buffa, GlobalConfig.time6);
		SZ21 = Schalten(_buffe, _buffa);
		strcpy(_buffe, GlobalConfig.time21);
		strcpy(_buffa, GlobalConfig.time61);
		SZ22 = Schalten(_buffe, _buffa);
		if ((SZ21 || SZ22) && !KE2) {
			LEDsetColosEinK2();
			KE2 = true;
			KA2 = false;
		}
		if (!SZ21 && !SZ22 && !KA2) {
			LEDsetColosAusK2();
			KA2 = true;
			KE2 = false;
		}
		break;
	}
	//Kanal3
	switch (atoi(GlobalConfig.hk3)) {
	case 0: // Hand Aus //do something when var equals 1
		if (!HKA3) {
			LEDsetColosAusK3();
			HKE3 = false;
			HKA3 = true;
		}
		break;
	case 1: // Hand Ein //do something when var equals 2
		if (!HKE3) {
			LEDsetColosEinK3();
			HKE3 = true;
			HKA3 = false;
		}
		break;
	case 2: // Uhr //do something when var equals 2
		strcpy(_buffe, GlobalConfig.time3);
		strcpy(_buffa, GlobalConfig.time7);
		SZ31 = Schalten(_buffe, _buffa);
		strcpy(_buffe, GlobalConfig.time31);
		strcpy(_buffa, GlobalConfig.time71);
		SZ32 = Schalten(_buffe, _buffa);

		if ((SZ31 || SZ32) && !KE3) {
			LEDsetColosEinK3();
			KE3 = true;
			KA3 = false;
		}
		if (!SZ31 && !SZ32 && !KA3) {
			LEDsetColosAusK3();
			KA3 = true;
			KE3 = false;
		}
		break;
	default:// if nothing else matches, do the default // default is optional
		strcpy(_buffe, GlobalConfig.time3);
		strcpy(_buffa, GlobalConfig.time7);
		SZ31 = Schalten(_buffe, _buffa);
		strcpy(_buffe, GlobalConfig.time31);
		strcpy(_buffa, GlobalConfig.time71);
		SZ32 = Schalten(_buffe, _buffa);

		if ((SZ31 || SZ32) && !KE3) {
			LEDsetColosEinK3();
			KE3 = true;
			KA3 = false;
		}
		if (!SZ31 && !SZ32 && !KA3) {
			LEDsetColosAusK3();
			KA3 = true;
			KE3 = false;
		}
		break;
	}
	//Kanal4
	switch (atoi(GlobalConfig.hk4)) {
	case 0: // Hand Aus //do something when var equals 1
		if (!HKA4) {
			LEDsetColosAusK4();
			HKE4 = false;
			HKA4 = true;
		}
		break;
	case 1: // Hand Ein //do something when var equals 2
		if (!HKE4) {
			LEDsetColosEinK4();
			HKE4 = true;
			HKA4 = false;
		}
		break;
	case 2: // Uhr //do something when var equals 2
		strcpy(_buffe, GlobalConfig.time4);
		strcpy(_buffa, GlobalConfig.time8);
		SZ41 = Schalten(_buffe, _buffa);
		strcpy(_buffe, GlobalConfig.time41);
		strcpy(_buffa, GlobalConfig.time81);
		SZ42 = Schalten(_buffe, _buffa);

		if ((SZ41 || SZ42) && !KE4) {
			LEDsetColosEinK4();
			KE4 = true;
			KA4 = false;
		}
		if (!SZ41 && !SZ42 && !KA4) {
			LEDsetColosAusK4();
			KA4 = true;
			KE4 = false;
		}
		break;
	default: // if nothing else matches, do the default // default is optional
		strcpy(_buffe, GlobalConfig.time4);
		strcpy(_buffa, GlobalConfig.time8);
		SZ41 = Schalten(_buffe, _buffa);
		strcpy(_buffe, GlobalConfig.time41);
		strcpy(_buffa, GlobalConfig.time81);
		SZ42 = Schalten(_buffe, _buffa);

		if ((SZ41 || SZ42) && !KE4) {
			LEDsetColosEinK4();
			KE4 = true;
			KA4 = false;
		}
		if (!SZ41 && !SZ42 && !KA4) {
			LEDsetColosAusK4();
			KA4 = true;
			KE4 = false;
		}
		break;
	}
}
// rücksetzen nach Speicherung moon
void LEDMoonSave() {
	bool SZM = false, HKEM = false, HKAM = false;
	LEDMoon();
}
// setzen der moon led
void LEDMoon() {
	//Moon
	char _ledanzmond[255];
	int _fmDay = 2; // facktor mDay
	int _kmDay = 0;
	String _colmoon = GlobalConfig.ledcolormoon;
	strcpy(_ledanzmond, GlobalConfig.ledanzmond);

	switch (atoi(GlobalConfig.hkm)) {
	case 0: // Hand Aus //do something when var equals 1
		if (!HKEM) {
			HKEM = true;
			HKAM = false;
			LEDSwitchSave();
		}
		break;
	case 1: // Hand Ein //do something when var equals 2
		if (!HKAM) {
			HKEM = false;
			HKAM = true;
			if (mDay > 15) {
				_kmDay = _fmDay * abs(mDay - 31);
				splitMoon(_kmDay, _ledanzmond, _colmoon);
			}
			else {
				_kmDay = _fmDay * abs(mDay);
				splitMoon(_kmDay, _ledanzmond, _colmoon);
			}
		}
		break;
	case 2: // Uhr //do something when var equals 2
		if (!SZ11 && !SZ12 && !SZ21 && !SZ22 && !SZ31 && !SZ32 && !SZ41 && !SZ42 && !SZM) {
			SZM = true;
			TM("Mond Ein");
			//        SZAM = false;
			if (mDay > 15) {
				_kmDay = _fmDay * abs(mDay - 31);
				splitMoon(_kmDay, _ledanzmond, _colmoon);
			}
			else {
				_kmDay = _fmDay * abs(mDay);
				splitMoon(_kmDay, _ledanzmond, _colmoon);
			}
		}
		if ((SZ11 || SZ12 || SZ21 || SZ22 || SZ31 || SZ32 || SZ41 || SZ42) && SZM) {
			// LEDSwitchSave();
			SZM = false;
			TM("Mond Aus");
		}
		break;
	default:// if nothing else matches, do the default // default is optional
		if (!SZ11 && !SZ12 && !SZ21 && !SZ22 && !SZ31 && !SZ32 && !SZ41 && !SZ42 && !SZM) {
			SZM = true;
			TM("Mond Ein");
			//        SZAM = false;
			if (mDay > 15) {
				_kmDay = _fmDay * abs(mDay - 31);
				splitMoon(_kmDay, _ledanzmond, _colmoon);
			}
			else {
				_kmDay = _fmDay * abs(mDay);
				splitMoon(_kmDay, _ledanzmond, _colmoon);
			}
		}
		if ((SZ11 || SZ12 || SZ21 || SZ22 || SZ31 || SZ32 || SZ41 || SZ42) && SZM) {
			// LEDSwitchSave();
			SZM = false;
			TM("Mond Aus");
		}
		break;
	}
	TM(String(_kmDay));
}
/*funktion spliten des eingabe stringes der für moon benötigten LED
_mDay = tag der Moonphase _anzled = zeichenkette der LED _colmoon = farbe der led
*/
byte splitMoon(int _mDay, char* _anzled, String _colmoon) {
	char ledsmond1[4][8]; // array ertse teilung = { {"4"},{"10-20"},{"10"},{"999-999"} };
	char ledsmond2[4][8]; // arrey für zweite teilung = "10-20" [0]=anzahl [1]= anfang [2]= ende
	char* ychr;
	Serial.print(_anzled);
	int i = 1;
	Serial.println(GlobalConfig.ledanzmond);
	splitString(_anzled, ";", ledsmond1);  // Splite String aus Globelconfig
	Serial.println(ledsmond1[0]);
	int y = atoi(ledsmond1[0]);
	Serial.println(y);
	for (i = 1; i < y + 1; i++) {
		if (strchr(ledsmond1[i], '-')) { // sucht nach (-)
			Serial.println(ledsmond1[i]);
			splitString(ledsmond1[i], "-", ledsmond2);  // [0]=Zähler [1]=Anfang [2]=Ende
			setMoon(_mDay, atoi(ledsmond2[1]), atoi(ledsmond2[2]), _colmoon);// setzen der LED
			Serial.println(ledsmond2[1]);
			Serial.println(ledsmond2[2]);
		}
		else {
			setMoon(_mDay, atoi(ledsmond1[i]), atoi(ledsmond1[i]), _colmoon);
			Serial.println(ledsmond1[i]);
		}
		Serial.println(i);
		yield();
		//delay(100);
	}
	Serial.println("set moon for end");
}

/* funktion setzen der Mondhelligkeit
	k= Mondtagstufe(1-15), an=LED anfang, en= LED ende, c=Farbe rgb
	color nach r/g/b
	Get rid of '#' and convert it to integer
*/
byte setMoon(int k, int an, int en, String c) {
	int number = (int)strtol(&c[1], NULL, 16);
	// Split them up into r, g, b values
	int red = number >> 16;
	int green = number >> 8 & 0xFF;
	int blue = number & 0xFF;
	// end Split
	int kred = (k / 256.0) * red;
	int kgreen = (k / 256.0) * green;
	int kblue = (k / 256.0) * blue;
	// set LED
	for (int ipix = an; ipix < en; ipix = ipix + 1) {
		setPixel(ipix, kred, kgreen, kblue);
	}
	FastLED.show();
}

/* funktion schalten  */
bool Schalten(char* timeE, char* timeA) {
	int hmE;
	int hmA;
	bool schaltzu;
	char _zeit[VARSIZE4][VARSIZE8];
	char _delimiter[] = ":";
	//Ein
	splitString(timeE, _delimiter, _zeit);
	hmE = catHMS(atoi(_zeit[1]), atoi(_zeit[2]), 0);
	splitString(timeA, _delimiter, _zeit);
	hmA = catHMS(atoi(_zeit[1]), atoi(_zeit[2]), 0);
	intAktZeit = catHMS(akt_hour, akt_minute, akt_second);
	if (hmE == hmA) {
		schaltzu = false;
		return schaltzu;
	}
	else {
		if ((intAktZeit >= hmE) && (intAktZeit <= hmA)) {
			schaltzu = true;
			return schaltzu;
		}
		else {
			schaltzu = false;
			return schaltzu;
		}
	}
}
/* funktion zusammensetzen Datum */
String printDate(time_t date) {
	char buff[20];
	sprintf(buff, "%2d-%02d-%4d %02d:%02d:%02d", day(date), month(date), year(date), hour(date), minute(date), second(date));
	return buff;
}
//#endif // LEDStrip
