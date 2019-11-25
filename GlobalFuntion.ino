String IpAddress2String(const IPAddress& ipAddress) {
	return String(ipAddress[0]) + String(".") + \
		String(ipAddress[1]) + String(".") + \
		String(ipAddress[2]) + String(".") + \
		String(ipAddress[3]);
}
// Funktion Aufteiling String
/*_string -> Zeichenfolge	zur Teilung / _delimiter -> Trennzeichen / _strret -> dyn Variabke rückgabe
Rückgabe werte werden als string[1] und string[2] string[0] ist die Anzahl der Teilungen
splitString(timeE, _delimiter, _zeit);*/
char* splitString(char* _string, char* _delimiter, char _strret[VARSIZE4][VARSIZE8]) {
	//	
	char* _ptr;	// Wert der Trennug
	int i = 1;	// Zähler 
	char buff[3]; //Buffer für wmdlung
	_ptr = strtok(_string, _delimiter);
	do {
		strcpy(_strret[i], _ptr);
		_ptr = strtok(NULL, _delimiter);
		i++;
	} while (_ptr != NULL);
	strcpy(_strret[0], itoa(i - 1, buff, 10));
}
// Funktion Zeit zu einer Zahl zusammenfügen
unsigned long catHMS(unsigned long LStunden, unsigned long LMinuten, unsigned long LSekunden) {
	unsigned long Zeitstempel;
	Zeitstempel = LStunden * 10000;  // aus 2h werden 20000
	Zeitstempel = Zeitstempel + LMinuten * 100; // 20000 + 3300 = 23300
	Zeitstempel = Zeitstempel + LSekunden;         // 23300 + 15 = 23315

	return Zeitstempel;
}
//NTP code
const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

//Setzen der Zeit nach Zeitzone und Sommer Winterzeit
void NTP_Set() {
	setSyncProvider(getNtpTime); //konfig timezone
	setSyncInterval(86400); //konfig timezone einmal am Tag sync
	//Prüfen ob leer
	if (strncmp(NetConfig.NTPSERVER, "", 2) == 0) {
		strcpy(ntpServerName, "us.pool.ntp.org");
	}
	else {
		strcpy(ntpServerName, NetConfig.NTPSERVER);
	}
	//Prüfen ob leer
	if (strncmp(NetConfig.TIMEZONE, "", 2) == 0) {
		timeZone = 0;
	}
	else {
		timeZone = atoi(NetConfig.TIMEZONE);
	}
	time_t getNtpTime();// setzen der zeit 
	//Prüfen ob Sommer Winterzeit
	time_t tT = now();
	time_t tTlocal = CE.toLocal(tT);
	time_t s_date = CE.toLocal(tT);
	CE.utcIsDST(tT);
	CE.locIsDST(tTlocal);
	if (strncmp(NetConfig.swu, "0", 2) == 0) { // Sommerzeitumschalzung aktiviert=1
		akt_hour = hour(tT);
		akt_minute = minute(tT);
		akt_second = second(tT);
		offsetCEST = 60;
	}
	else {
		if (CE.locIsDST(tTlocal) == false) { // prüfeb ob Sommerzeit = true
			akt_hour = hour(tT);
			akt_minute = minute(tT);
			akt_second = second(tT);
			offsetCEST = 60;
		}
		else {
			akt_hour = hour(tTlocal);
			akt_minute = minute(tTlocal);
			akt_second = second(tTlocal);
			offsetCEST = 120;
		}
	}
}


void digitalClockDisplay() {
	// digital clock display of the time
	TM(String(day()) + "." + String(month()) + "." + String(year()) + " " + String(hour()) + ":" + String(minute()) + ":" + String(second()));
}

void printDigits(int digits) {
	// utility for digital clock display: prints preceding colon and leading 0
	TM(":");
	if (digits < 10)
		TM("0");
	TM(String(digits));
}

time_t getNtpTime() {
	IPAddress ntpServerIP; // NTP server's ip address

	while (UDPClient.UDP.parsePacket() > 0); // discard any previously received packets
	//Serial.println("Transmit NTP Request");
	// get a random server from the pool
	WiFi.hostByName(ntpServerName, ntpServerIP);
	//Serial.print(ntpServerName);
	//Serial.print(": ");
	//Serial.println(ntpServerIP);
	sendNTPpacket(ntpServerIP);
	uint32_t beginWait = millis();
	while (millis() - beginWait < 1500) {
		int size = UDPClient.UDP.parsePacket();
		if (size >= NTP_PACKET_SIZE) {
			//Serial.println("Receive NTP Response");
			UDPClient.UDP.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
			unsigned long secsSince1900;
			// convert four bytes starting at location 40 to a long integer
			secsSince1900 = (unsigned long)packetBuffer[40] << 24;
			secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
			secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
			secsSince1900 |= (unsigned long)packetBuffer[43];
			return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
		}
	}
	//Serial.println("No NTP Response :-(");
	//strcpy(noNTP, "No NTP Response");
	return 0; // return 0 if unable to get the time
}
void NTPStatus() {
	if (timeStatus() == timeNotSet) {
		strcpy(NTPStaus, "Die Zeit wurde nie eingestellt, die Uhr lief mit 1. januar 1970 an");
	}
	if (timeStatus() == timeNeedsSync) {
		strcpy(NTPStaus, "Die Zeit wurde eingestellt, aber ein Synchronisierungsversuch schlug fehl");
	}
	if (timeStatus() == timeSet) {
		strcpy(NTPStaus, "Die Uhrzeit ist eingestellt und wird synchronisiert");
	}

}
// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress & address) {
	// set all bytes in the buffer to 0
	memset(packetBuffer, 0, NTP_PACKET_SIZE);
	// Initialize values needed to form NTP request
	// (see URL above for details on the packets)
	packetBuffer[0] = 0b11100011;   // LI, Version, Mode
	packetBuffer[1] = 0;     // Stratum, or type of clock
	packetBuffer[2] = 6;     // Polling Interval
	packetBuffer[3] = 0xEC;  // Peer Clock Precision
	// 8 bytes of zero for Root Delay & Root Dispersion
	packetBuffer[12] = 49;
	packetBuffer[13] = 0x4E;
	packetBuffer[14] = 49;
	packetBuffer[15] = 52;
	// all NTP fields have been given values, now
	// you can send a packet requesting a timestamp:
	UDPClient.UDP.beginPacket(address, 123); //NTP requests are to port 123
	UDPClient.UDP.write(packetBuffer, NTP_PACKET_SIZE);
	UDPClient.UDP.endPacket();
}
// Setze Zeit Manuell
int TimeHand(char* _Date, char* _Time) {
	int Day = 20;
	int Month = 05;
	int Year = 2019;
	int Hour = 12;
	int Minute = 35;
	int Second = 0;
	char _zeitDatum[VARSIZE4][VARSIZE8];// return des Zeitsting
	// Teilung Zeit
	char _delimiter_z[] = ":";
	splitString(_Time, _delimiter_z, _zeitDatum);
	Hour = atoi(_zeitDatum[1]) - timeZone;
	Minute = atoi(_zeitDatum[2]);
	Serial.println(Hour);
	Serial.println(Minute);
	Serial.println(_Date);
	char _delimiter_d[] = "-";
	splitString(_Date, _delimiter_d, _zeitDatum);
	Day = atoi(_zeitDatum[3]);
	Month = atoi(_zeitDatum[2]);
	Year = atoi(_zeitDatum[1]);
	Serial.println(Day);
	Serial.println(Month);
	Serial.println(Year);
	setTime(Hour, Minute, Second, Day, Month, Year);
	//adjustTime(timeZone * SECS_PER_HOUR);
}

//Setzen der Sonne Mond berechnungen
void SunMmoon_Set() {
	sm_latitude = atoi(GlobalConfig.latitude); // Jena cordinates
	sm_longtitude = atoi(GlobalConfig.longtitude);
	sm_timezone = offsetCEST * timeZone; // localtime with UTC difference in minutes + sommer winterzeit
	sm.init(sm_timezone, sm_latitude, sm_longtitude);
}

void Sunrise() {
	//	time_t s_date = getNtpTime();
	TM("Specific date was ");
	printDate(s_date);
	TM("");
	jDay = sm.julianDay(s_date);
	mDay = sm.moonDay(s_date);  // Moon Tag
	sRise = sm.sunRise(s_date); // Sun Aufgang
	sSet = sm.sunSet(s_date);  // Sun Untergang

	TM("Julian day of specific date was ");
	TM("Today is " + String(jDay) + " Julian day");
	TM("Moon age is " + String(mDay) + " day(s)");
	TM("Today sunrise and sunset: ");
	TM(printDate(sRise));
	TM(printDate(sSet));
}

String strTimeDay(int strTD) {
	String str_TD;
	if (strTD < 10) {
		str_TD = "0" + String(strTD);
		DEBUG(str_TD);
	}
	else
	{
		str_TD = String(strTD);
	}
	return str_TD;
}