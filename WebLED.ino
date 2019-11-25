/*
	Name:       WEBLED.ino
	Created:	28.04.2019 18:56:26
	Author:     borstel
	Inhalt:		Webseiten der LED
*/

// Anzeige LED status
//#ifdef LEDStrip
void LEDHtml() {
	String page = FPSTR(HTTP_HEAD);
	page += F("<script>");
	page += FPSTR(HTTP_GLOBALSCRIPT);
	page += FPSTR(HTTP_WEBLEDSCRIPT);
	page += F("</script>");
	//page += FPSTR(HTTP_SCRIPT);
	page += FPSTR(HTTP_ALL_STYLE);
	page += FPSTR(HTTP_HM_STYLE);
	page += FPSTR(HTTP_HEAD_END);
	page += F("<div class='fbg'>");
	page += F("<form method='post' action='LED'>");
	page += FPSTR(HTTP_TITLE_LABEL);
	// Anzeige LED Kanäle
	page += F("<div><pre>");
	page += FPSTR(HTTP_ANZEIGE_RADIO);
	page.replace("{bez}", "LED 1");
	page.replace("{id}", "sz1");
	page.replace("{name}", "sz1");
	page.replace("{val}", "1");
	if (SZ11 || SZ12 || HKE1) {
		page.replace("{ch}", "checked");
	}
	else {
		page.replace("{ch}", "");
	}
	page += FPSTR(HTTP_ANZEIGE_RADIO);
	page.replace("{bez}", "LED 2");
	page.replace("{id}", "sz2");
	page.replace("{name}", "sz2");
	page.replace("{val}", "1");
	DEBUG(String(SZ21) + String(SZ22) + String(HKE2));
	if (SZ21 || SZ22 || HKE2) {
		page.replace("{ch}", "checked");
	}
	else {
		page.replace("{ch}", "");
	}
	page += FPSTR(HTTP_ANZEIGE_RADIO);
	page.replace("{bez}", "LED 3");
	page.replace("{id}", "sz3");
	page.replace("{name}", "sz3");
	page.replace("{val}", "1");
	if (SZ31 || SZ32 || HKE3) {
		page.replace("{ch}", "checked");
	}
	else {
		page.replace("{ch}", "");
	}
	page += FPSTR(HTTP_ANZEIGE_RADIO);
	page.replace("{bez}", "LED 4");
	page.replace("{id}", "sz4");
	page.replace("{name}", "sz4");
	page.replace("{val}", "1");
	if (SZ41 || SZ42 || HKE4) {
		page.replace("{ch}", "checked");
	}
	else {
		page.replace("{ch}", "");
	}
	page += F("</pre></div>");
	page += F("<div>");
	page += FPSTR(HTTP_ANZEIGE_RADIO);
	page.replace("{bez}", "Mond ");
	page.replace("{id}", "szm");
	page.replace("{name}", "szm");
	page.replace("{val}", "1");
	if (SZM) {
		page.replace("{ch}", "checked");
	}
	else {
		page.replace("{ch}", "");
	}
	page += F("</div>");
	page += F("<br>");
	// Temperatur
	page += F("<pre>");
	page += F("<form method='get' action='getTempJSON'>");
	//page += FPSTR(HTTP_ANZEIGE_TXT);
	//page.replace("{id}", "");
	//page.replace("{bez1}", "Temperatur ");
	//page.replace("{bez2}", "");
	//page.replace("{bez3}", "");
	page += FPSTR(HTTP_ANZEIGE_TXT);
	page.replace("{id}", "aTemp");
	page.replace("{bez1}", "");
	page.replace("{bez2}", "");
	page.replace("{bez3}", "");
	page += F("</form><br>");
	// Zeit des ESP
	page += F("<form method='get' action='getTimeJSON'>");
	//page += FPSTR(HTTP_ANZEIGE_TXT);
	//page.replace("{id}", "");
	//page.replace("{bez1}", "aktuelle Zeit ");
	//page.replace("{bez2}", "");
	//page.replace("{bez3}", "");
	page += FPSTR(HTTP_ANZEIGE_TXT);
	page.replace("{id}", "aDatumZeit");
	page.replace("{bez1}", "");
	page.replace("{bez2}", "");
	page.replace("{bez3}", "");
	page += F("</form><br>");
	// Mondsteuerung Sonnenaufgang untergang
	page += F("<form method='post' action='LED'>");
	page += FPSTR(HTTP_ANZEIGE_TXT);
	page.replace("{id}", "");
	page.replace("{bez1}", "Sonnenaufgang ");
	page.replace("{bez2}", "");
	page.replace("{bez3}", "");
	page += FPSTR(HTTP_ANZEIGE_TXT);
	page.replace("{id}", "sRise");
	page.replace("{bez1}", printDate(sRise));
	page.replace("{bez2}", "");
	page.replace("{bez3}", "");
	page += FPSTR(HTTP_ANZEIGE_TXT);
	page.replace("{id}", "");
	page.replace("{bez1}", "Sonnenuntergang ");
	page.replace("{bez2}", "");
	page.replace("{bez3}", "");
	page += FPSTR(HTTP_ANZEIGE_TXT);
	page.replace("{id}", "sSet");
	page.replace("{bez1}", printDate(sSet));
	page.replace("{bez2}", "");
	page.replace("{bez3}", "");
	page += F("</pre>");
	//butten config und home
	page += FPSTR(HTTP_BUTTON_confLED);
	page.replace("{text}", "Konfiguration");
	page += FPSTR(HTTP_BUTTON_HOME);
	page += FPSTR(HTTP_LABEL_FW);
	page.replace("{fw}", FIRMWARE_VERSION);
	page += F("</form>");
	page += F("</div></body></html>");
	page.replace("{v}", NetConfig.DeviceName);

	WebServer.send(200, "text/html", page);
}
// LED Configuration
void confLEDHtml() {
	bool sc = false;
	bool saveSuccess = false;
	bool showHMDevError = false;
	// wenn werte geändert dann schreiben in global.conf
	if (WebServer.args() > 0) {
		for (int i = 0; i < WebServer.args(); i++) {
			if (WebServer.argName(i) == "btnSave")
				sc = (WebServer.arg(i).toInt() == 1);
			if (WebServer.argName(i) == "ledcolor1")
				strcpy(GlobalConfig.ledcolor1, WebServer.arg(i).c_str());
			if (WebServer.argName(i) == "ledcolor2")
				strcpy(GlobalConfig.ledcolor2, WebServer.arg(i).c_str());
			if (WebServer.argName(i) == "ledcolor3")
				strcpy(GlobalConfig.ledcolor3, WebServer.arg(i).c_str());
			if (WebServer.argName(i) == "ledcolor4")
				strcpy(GlobalConfig.ledcolor4, WebServer.arg(i).c_str());
			if (WebServer.argName(i) == "ledcolor5")
				strcpy(GlobalConfig.ledcolor5, WebServer.arg(i).c_str());
			if (WebServer.argName(i) == "ledcolor6")
				strcpy(GlobalConfig.ledcolor6, WebServer.arg(i).c_str());
			if (WebServer.argName(i) == "ledcolor7")
				strcpy(GlobalConfig.ledcolor7, WebServer.arg(i).c_str());
			if (WebServer.argName(i) == "ledcolor8")
				strcpy(GlobalConfig.ledcolor8, WebServer.arg(i).c_str());
			if (WebServer.argName(i) == "ledcolormoon")
				strcpy(GlobalConfig.ledcolormoon, WebServer.arg(i).c_str());
			if (WebServer.argName(i) == "time1")
				strcpy(GlobalConfig.time1, WebServer.arg(i).c_str());
			if (WebServer.argName(i) == "time2")
				strcpy(GlobalConfig.time2, WebServer.arg(i).c_str());
			if (WebServer.argName(i) == "time3")
				strcpy(GlobalConfig.time3, WebServer.arg(i).c_str());
			if (WebServer.argName(i) == "time4")
				strcpy(GlobalConfig.time4, WebServer.arg(i).c_str());
			if (WebServer.argName(i) == "time5")
				strcpy(GlobalConfig.time5, WebServer.arg(i).c_str());
			if (WebServer.argName(i) == "time6")
				strcpy(GlobalConfig.time6, WebServer.arg(i).c_str());
			if (WebServer.argName(i) == "time7")
				strcpy(GlobalConfig.time7, WebServer.arg(i).c_str());
			if (WebServer.argName(i) == "time8")
				strcpy(GlobalConfig.time8, WebServer.arg(i).c_str());
			if (WebServer.argName(i) == "time11")
				strcpy(GlobalConfig.time11, WebServer.arg(i).c_str());
			if (WebServer.argName(i) == "time21")
				strcpy(GlobalConfig.time21, WebServer.arg(i).c_str());
			if (WebServer.argName(i) == "time31")
				strcpy(GlobalConfig.time31, WebServer.arg(i).c_str());
			if (WebServer.argName(i) == "time41")
				strcpy(GlobalConfig.time41, WebServer.arg(i).c_str());
			if (WebServer.argName(i) == "time51")
				strcpy(GlobalConfig.time51, WebServer.arg(i).c_str());
			if (WebServer.argName(i) == "time61")
				strcpy(GlobalConfig.time61, WebServer.arg(i).c_str());
			if (WebServer.argName(i) == "time71")
				strcpy(GlobalConfig.time71, WebServer.arg(i).c_str());
			if (WebServer.argName(i) == "time81")
				strcpy(GlobalConfig.time81, WebServer.arg(i).c_str());
			if (WebServer.argName(i) == "hk1")
				strcpy(GlobalConfig.hk1, WebServer.arg(i).c_str());
			if (WebServer.argName(i) == "hk2")
				strcpy(GlobalConfig.hk2, WebServer.arg(i).c_str());
			if (WebServer.argName(i) == "hk3")
				strcpy(GlobalConfig.hk3, WebServer.arg(i).c_str());
			if (WebServer.argName(i) == "hk4")
				strcpy(GlobalConfig.hk4, WebServer.arg(i).c_str());
			if (WebServer.argName(i) == "hkm")
				strcpy(GlobalConfig.hkm, WebServer.arg(i).c_str());
			if (WebServer.argName(i) == "latitude")
				strcpy(GlobalConfig.latitude, WebServer.arg(i).c_str());
			if (WebServer.argName(i) == "longtitude")
				strcpy(GlobalConfig.longtitude, WebServer.arg(i).c_str());
			if (WebServer.argName(i) == "anzmoon")
				strcpy(GlobalConfig.ledanzmond, WebServer.arg(i).c_str());
			if (WebServer.argName(i) == "anzled")
				strcpy(GlobalConfig.ledanzges, WebServer.arg(i).c_str());
		}
	}
	// anzeige der werte aus global.conf
	String page = FPSTR(HTTP_HEAD);
	page += FPSTR(HTTP_ALL_STYLE);
	page += FPSTR(HTTP_HM_STYLE);
	page += FPSTR(HTTP_HEAD_END);
	page += F("<div class='fbg'>");
	page += F("<form method='post' action='configLED'>");
	page += FPSTR(HTTP_TITLE_LABEL);

	// LED Kanal 1
	page += FPSTR(HTTP_TITLE_LABEL_1);
	page.replace("{vs}", "LED Config Kanal 1");
	page += F("<div><pre>");
	page += FPSTR(HTTP_CONF_RADIO);
	page.replace("{bez}", "UHR ");
	page.replace("{id}", "hk1");
	page.replace("{name}", "hk1");
	page.replace("{val}", "2");
	switch (atoi(GlobalConfig.hk1)) {
	case 0: // Hand Aus //do something when var equals 1
		page.replace("{ch}", "");
		break;
	case 1: // Hand Ein //do something when var equals 2
		page.replace("{ch}", "");
		break;
	case 2: // Uhr //do something when var equals 2
		page.replace("{ch}", "checked");
		break;
	default:// if nothing else matches, do the default // default is optional
		page.replace("{ch}", "");
		break;
	}
	page += FPSTR(HTTP_CONF_RADIO);
	page.replace("{bez}", "EIN ");
	page.replace("{id}", "hk1");
	page.replace("{name}", "hk1");
	page.replace("{val}", "1");
	switch (atoi(GlobalConfig.hk1)) {
	case 0: // Hand Aus //do something when var equals 1
		page.replace("{ch}", "");
		break;
	case 1: // Hand Ein //do something when var equals 2
		page.replace("{ch}", "checked");
		break;
	case 2: // Uhr //do something when var equals 2
		page.replace("{ch}", "");
		break;
	default:// if nothing else matches, do the default // default is optional
		page.replace("{ch}", "");
		break;
	}
	page += FPSTR(HTTP_CONF_RADIO);
	page.replace("{bez}", "AUS ");
	page.replace("{id}", "hk1");
	page.replace("{name}", "hk1");
	page.replace("{val}", "0");
	//Serial.print("begin HK1");
	switch (atoi(GlobalConfig.hk1)) {
	case 0: // Hand Aus //do something when var equals 1
		page.replace("{ch}", "checked");
		break;
	case 1: // Hand Ein //do something when var equals 2
		page.replace("{ch}", "");
		break;
	case 2: // Uhr //do something when var equals 2
		page.replace("{ch}", "");
		break;
	default:// if nothing else matches, do the default // default is optional
		page.replace("{ch}", "");
		break;
	}
	page += F("</pre></div>");
	// Collor
	page += F("<div><pre>");
	page += FPSTR(HTTP_CONF_LEDCOLOR);
	page.replace("{bez}", "Lichtfarbe EIN ");
	page.replace("{id}", "ledcolor1");
	page.replace("{name}", "ledcolor1");
	page.replace("{val}", GlobalConfig.ledcolor1);
	page += FPSTR(HTTP_CONF_LEDCOLOR);
	page.replace("{bez}", " AUS ");
	page.replace("{id}", "ledcolor5");
	page.replace("{name}", "ledcolor5");
	page.replace("{val}", GlobalConfig.ledcolor5);
	page += F("</pre></div>");
	// Time
	page += F("<div><pre>");
	page += FPSTR(HTTP_CONF_TIME);
	page.replace("{bez}", "EIN ");
	page.replace("{id}", "time1");
	page.replace("{name}", "time1");
	page.replace("{pla}", "00:00");
	page.replace("{val}", GlobalConfig.time1);
	page += FPSTR(HTTP_CONF_TIME);
	page.replace("{bez}", " AUS ");
	page.replace("{id}", "time5");
	page.replace("{name}", "time5");
	page.replace("{pla}", "00:00");
	page.replace("{val}", GlobalConfig.time5);
	page += F("</pre></div>");
	page += F("<div><pre>");
	page += FPSTR(HTTP_CONF_TIME);
	page.replace("{bez}", "EIN ");
	page.replace("{id}", "time11");
	page.replace("{name}", "time11");
	page.replace("{pla}", "00:00");
	page.replace("{val}", GlobalConfig.time11);
	page += FPSTR(HTTP_CONF_TIME);
	page.replace("{bez}", " AUS ");
	page.replace("{id}", "time51");
	page.replace("{name}", "time51");
	page.replace("{pla}", "00:00");
	page.replace("{val}", GlobalConfig.time51);
	page += F("</pre></div>");
	// LED Kanal 2
	page += FPSTR(HTTP_TITLE_LABEL_1);
	page.replace("{vs}", "LED Config Kanal 2");
	page += F("<div><pre>");
	page += FPSTR(HTTP_CONF_RADIO);
	page.replace("{bez}", "UHR ");
	page.replace("{id}", "hk2");
	page.replace("{name}", "hk2");
	page.replace("{val}", "2");
	switch (atoi(GlobalConfig.hk2)) {
	case 0: // Hand Aus //do something when var equals 1
		page.replace("{ch}", "");
		break;
	case 1: // Hand Ein //do something when var equals 2
		page.replace("{ch}", "");
		break;
	case 2: // Uhr //do something when var equals 2
		page.replace("{ch}", "checked");
		break;
	default:// if nothing else matches, do the default // default is optional
		page.replace("{ch}", "");
		break;
	}
	page += FPSTR(HTTP_CONF_RADIO);
	page.replace("{bez}", "EIN ");
	page.replace("{id}", "hk2");
	page.replace("{name}", "hk2");
	page.replace("{val}", "1");
	switch (atoi(GlobalConfig.hk2)) {
	case 0: // Hand Aus //do something when var equals 1
		page.replace("{ch}", "");
		break;
	case 1: // Hand Ein //do something when var equals 2
		page.replace("{ch}", "checked");
		break;
	case 2: // Uhr //do something when var equals 2
		page.replace("{ch}", "");
		break;
	default:// if nothing else matches, do the default // default is optional
		page.replace("{ch}", "");
		break;
	}
	page += FPSTR(HTTP_CONF_RADIO);
	page.replace("{bez}", "AUS ");
	page.replace("{id}", "hk2");
	page.replace("{name}", "hk2");
	page.replace("{val}", "0");
	//Serial.print("begin HK1");
	switch (atoi(GlobalConfig.hk2)) {
	case 0: // Hand Aus //do something when var equals 1
		page.replace("{ch}", "checked");
		break;
	case 1: // Hand Ein //do something when var equals 2
		page.replace("{ch}", "");
		break;
	case 2: // Uhr //do something when var equals 2
		page.replace("{ch}", "");
		break;
	default:// if nothing else matches, do the default // default is optional
		page.replace("{ch}", "");
		break;
	}
	page += F("</pre></div>");
	// Collor
	page += F("<div><pre>");
	page += FPSTR(HTTP_CONF_LEDCOLOR);
	page.replace("{bez}", "Lichtfarbe EIN ");
	page.replace("{id}", "ledcolor2");
	page.replace("{name}", "ledcolor2");
	page.replace("{val}", GlobalConfig.ledcolor2);
	page += FPSTR(HTTP_CONF_LEDCOLOR);
	page.replace("{bez}", " AUS ");
	page.replace("{id}", "ledcolor6");
	page.replace("{name}", "ledcolor6");
	page.replace("{val}", GlobalConfig.ledcolor6);
	page += F("</pre></div>");
	//Time
	page += F("<div><pre>");
	page += FPSTR(HTTP_CONF_TIME);
	page.replace("{bez}", "EIN ");
	page.replace("{id}", "time2");
	page.replace("{name}", "time2");
	page.replace("{pla}", "00:00");
	page.replace("{val}", GlobalConfig.time2);
	page += FPSTR(HTTP_CONF_TIME);
	page.replace("{bez}", " AUS ");
	page.replace("{id}", "time6");
	page.replace("{name}", "time6");
	page.replace("{pla}", "00:00");
	page.replace("{val}", GlobalConfig.time6);
	page += F("</pre></div>");
	page += F("<div><pre>");
	page += FPSTR(HTTP_CONF_TIME);
	page.replace("{bez}", "EIN ");
	page.replace("{id}", "time21");
	page.replace("{name}", "time21");
	page.replace("{pla}", "00:00");
	page.replace("{val}", GlobalConfig.time21);
	page += FPSTR(HTTP_CONF_TIME);
	page.replace("{bez}", " AUS ");
	page.replace("{id}", "time61");
	page.replace("{name}", "time61");
	page.replace("{pla}", "00:00");
	page.replace("{val}", GlobalConfig.time61);
	page += F("</pre></div>");
	// LED Kanal 3
	page += FPSTR(HTTP_TITLE_LABEL_1);
	page.replace("{vs}", "LED Config Kanal 3");
	page += F("<div><pre>");
	page += FPSTR(HTTP_CONF_RADIO);
	page.replace("{bez}", "UHR ");
	page.replace("{id}", "hk3");
	page.replace("{name}", "hk3");
	page.replace("{val}", "2");
	switch (atoi(GlobalConfig.hk3)) {
	case 0: // Hand Aus //do something when var equals 1
		page.replace("{ch}", "");
		break;
	case 1: // Hand Ein //do something when var equals 2
		page.replace("{ch}", "");
		break;
	case 2: // Uhr //do something when var equals 2
		page.replace("{ch}", "checked");
		break;
	default:// if nothing else matches, do the default // default is optional
		page.replace("{ch}", "");
		break;
	}
	page += FPSTR(HTTP_CONF_RADIO);
	page.replace("{bez}", "EIN ");
	page.replace("{id}", "hk3");
	page.replace("{name}", "hk3");
	page.replace("{val}", "1");
	switch (atoi(GlobalConfig.hk3)) {
	case 0: // Hand Aus //do something when var equals 1
		page.replace("{ch}", "");
		break;
	case 1: // Hand Ein //do something when var equals 2
		page.replace("{ch}", "checked");
		break;
	case 2: // Uhr //do something when var equals 2
		page.replace("{ch}", "");
		break;
	default:// if nothing else matches, do the default // default is optional
		page.replace("{ch}", "");
		break;
	}
	page += FPSTR(HTTP_CONF_RADIO);
	page.replace("{bez}", "AUS ");
	page.replace("{id}", "hk3");
	page.replace("{name}", "hk3");
	page.replace("{val}", "0");
	//Serial.print("begin HK1");
	switch (atoi(GlobalConfig.hk3)) {
	case 0: // Hand Aus //do something when var equals 1
		page.replace("{ch}", "checked");
		break;
	case 1: // Hand Ein //do something when var equals 2
		page.replace("{ch}", "");
		break;
	case 2: // Uhr //do something when var equals 2
		page.replace("{ch}", "");
		break;
	default:// if nothing else matches, do the default // default is optional
		page.replace("{ch}", "");
		break;
	}
	page += F("</pre></div>");
	// Collor
	page += F("<div><pre>");
	page += FPSTR(HTTP_CONF_LEDCOLOR);
	page.replace("{bez}", "Lichtfarbe EIN ");
	page.replace("{id}", "ledcolor3");
	page.replace("{name}", "ledcolor3");
	page.replace("{val}", GlobalConfig.ledcolor3);
	page += FPSTR(HTTP_CONF_LEDCOLOR);
	page.replace("{bez}", " AUS ");
	page.replace("{id}", "ledcolor7");
	page.replace("{name}", "ledcolor7");
	page.replace("{val}", GlobalConfig.ledcolor7);
	page += F("</pre></div>");
	//Time
	page += F("<div><pre>");
	page += FPSTR(HTTP_CONF_TIME);
	page.replace("{bez}", "EIN ");
	page.replace("{id}", "time3");
	page.replace("{name}", "time3");
	page.replace("{pla}", "00:00");
	page.replace("{val}", GlobalConfig.time3);
	page += FPSTR(HTTP_CONF_TIME);
	page.replace("{bez}", " AUS ");
	page.replace("{id}", "time7");
	page.replace("{name}", "time7");
	page.replace("{pla}", "00:00");
	page.replace("{val}", GlobalConfig.time7);
	page += F("</pre></div>");
	page += F("<div><pre>");
	page += FPSTR(HTTP_CONF_TIME);
	page.replace("{bez}", "EIN ");
	page.replace("{id}", "time31");
	page.replace("{name}", "time31");
	page.replace("{pla}", "00:00");
	page.replace("{val}", GlobalConfig.time31);
	page += FPSTR(HTTP_CONF_TIME);
	page.replace("{bez}", " AUS ");
	page.replace("{id}", "time71");
	page.replace("{name}", "time71");
	page.replace("{pla}", "00:00");
	page.replace("{val}", GlobalConfig.time71);
	page += F("</pre></div>");
	// LED Kanal 4
	page += FPSTR(HTTP_TITLE_LABEL_1);
	page.replace("{vs}", "LED Config Kanal 4");
	page += F("<div><pre>");
	page += FPSTR(HTTP_CONF_RADIO);
	page.replace("{bez}", "UHR ");
	page.replace("{id}", "hk4");
	page.replace("{name}", "hk4");
	page.replace("{val}", "2");
	switch (atoi(GlobalConfig.hk4)) {
	case 0: // Hand Aus //do something when var equals 1
		page.replace("{ch}", "");
		break;
	case 1: // Hand Ein //do something when var equals 2
		page.replace("{ch}", "");
		break;
	case 2: // Uhr //do something when var equals 2
		page.replace("{ch}", "checked");
		break;
	default:// if nothing else matches, do the default // default is optional
		page.replace("{ch}", "");
		break;
	}
	page += FPSTR(HTTP_CONF_RADIO);
	page.replace("{bez}", "EIN ");
	page.replace("{id}", "hk4");
	page.replace("{name}", "hk4");
	page.replace("{val}", "1");
	switch (atoi(GlobalConfig.hk4)) {
	case 0: // Hand Aus //do something when var equals 1
		page.replace("{ch}", "");
		break;
	case 1: // Hand Ein //do something when var equals 2
		page.replace("{ch}", "checked");
		break;
	case 2: // Uhr //do something when var equals 2
		page.replace("{ch}", "");
		break;
	default:// if nothing else matches, do the default // default is optional
		page.replace("{ch}", "");
		break;
	}
	page += FPSTR(HTTP_CONF_RADIO);
	page.replace("{bez}", "AUS ");
	page.replace("{id}", "hk4");
	page.replace("{name}", "hk4");
	page.replace("{val}", "0");
	//Serial.print("begin HK1");
	switch (atoi(GlobalConfig.hk4)) {
	case 0: // Hand Aus //do something when var equals 1
		page.replace("{ch}", "checked");
		break;
	case 1: // Hand Ein //do something when var equals 2
		page.replace("{ch}", "");
		break;
	case 2: // Uhr //do something when var equals 2
		page.replace("{ch}", "");
		break;
	default:// if nothing else matches, do the default // default is optional
		page.replace("{ch}", "");
		break;
	}
	page += F("</pre></div>");
	// Collor
	page += F("<div><pre>");
	page += FPSTR(HTTP_CONF_LEDCOLOR);
	page.replace("{bez}", "Lichtfarbe EIN ");
	page.replace("{id}", "ledcolor4");
	page.replace("{name}", "ledcolor4");
	page.replace("{val}", GlobalConfig.ledcolor4);
	page += FPSTR(HTTP_CONF_LEDCOLOR);
	page.replace("{bez}", " AUS ");
	page.replace("{id}", "ledcolor8");
	page.replace("{name}", "ledcolor8");
	page.replace("{val}", GlobalConfig.ledcolor8);
	page += F("</pre></div>");
	//Time
	page += F("<div><pre>");
	page += FPSTR(HTTP_CONF_TIME);
	page.replace("{bez}", "EIN ");
	page.replace("{id}", "time4");
	page.replace("{name}", "time4");
	page.replace("{pla}", "00:00");
	page.replace("{val}", GlobalConfig.time4);
	page += FPSTR(HTTP_CONF_TIME);
	page.replace("{bez}", " AUS ");
	page.replace("{id}", "time8");
	page.replace("{name}", "time8");
	page.replace("{pla}", "00:00");
	page.replace("{val}", GlobalConfig.time8);
	page += F("</pre></div>");
	page += F("<div><pre>");
	page += FPSTR(HTTP_CONF_TIME);
	page.replace("{bez}", "EIN ");
	page.replace("{id}", "time41");
	page.replace("{name}", "time41");
	page.replace("{pla}", "00:00");
	page.replace("{val}", GlobalConfig.time41);
	page += FPSTR(HTTP_CONF_TIME);
	page.replace("{bez}", " AUS ");
	page.replace("{id}", "time81");
	page.replace("{name}", "time81");
	page.replace("{val}", GlobalConfig.time81);
	page += F("</pre></div>");
	// LED Moon
	page += FPSTR(HTTP_TITLE_LABEL_1);
	page.replace("{vs}", "LED Config Mond");
	//Auto Hand Aus
	page += F("<div><pre>");
	page += FPSTR(HTTP_CONF_RADIO);
	page.replace("{bez}", "AUTO ");
	page.replace("{id}", "hkm");
	page.replace("{name}", "hkm");
	page.replace("{val}", "2");
	switch (atoi(GlobalConfig.hkm)) {
	case 0: // Hand Aus //do something when var equals 1
		page.replace("{ch}", "");
		break;
	case 1: // Hand Ein //do something when var equals 2
		page.replace("{ch}", "");
		break;
	case 2: // Uhr //do something when var equals 2
		page.replace("{ch}", "checked");
		break;
	default:// if nothing else matches, do the default // default is optional
		page.replace("{ch}", "");
		break;
	}
	page += FPSTR(HTTP_CONF_RADIO);
	page.replace("{bez}", "EIN ");
	page.replace("{id}", "hkm");
	page.replace("{name}", "hkm");
	page.replace("{val}", "1");
	switch (atoi(GlobalConfig.hkm)) {
	case 0: // Hand Aus //do something when var equals 1
		page.replace("{ch}", "");
		break;
	case 1: // Hand Ein //do something when var equals 2
		page.replace("{ch}", "checked");
		break;
	case 2: // Uhr //do something when var equals 2
		page.replace("{ch}", "");
		break;
	default:// if nothing else matches, do the default // default is optional
		page.replace("{ch}", "");
		break;
	}
	page += FPSTR(HTTP_CONF_RADIO);
	page.replace("{bez}", "AUS ");
	page.replace("{id}", "hkm");
	page.replace("{name}", "hkm");
	page.replace("{val}", "0");
	//Serial.print("begin HK1");
	switch (atoi(GlobalConfig.hkm)) {
	case 0: // Hand Aus //do something when var equals 1
		page.replace("{ch}", "checked");
		break;
	case 1: // Hand Ein //do something when var equals 2
		page.replace("{ch}", "");
		break;
	case 2: // Uhr //do something when var equals 2
		page.replace("{ch}", "");
		break;
	default:// if nothing else matches, do the default // default is optional
		page.replace("{ch}", "");
		break;
	}
	page += F("</pre></div>");
	// Collor
	page += F("<div><pre>");
	page += FPSTR(HTTP_CONF_LEDCOLOR);
	page.replace("{bez}", "Lichtfarbe Mond ");
	page.replace("{id}", "ledcolormoon");
	page.replace("{name}", "ledcolormoon");
	page.replace("{val}", GlobalConfig.ledcolormoon);
	page += F("</pre></div>");
	// sonne mond
	page += F("<pre>");
	page += FPSTR(HTTP_TITLE_LABEL_1);
	page.replace("{vs}", "Sonne und Mond");
	// Latitude
	page += FPSTR(HTTP_CONF_TXT);
	page.replace("{bez}", "Latitude &nbsp  &nbsp");
	page.replace("{id}", "latitude");
	page.replace("{name}", "latitude");
	page.replace("{pat}", "[0-9\\.]+");
	page.replace("{pla}", "Latitude des Ortes");
	page.replace("{val}", GlobalConfig.latitude);
	// Longtitude
	page += FPSTR(HTTP_CONF_TXT);
	page.replace("{bez}", "Longtitude ");
	page.replace("{id}", "longtitude");
	page.replace("{name}", "longtitude");
	page.replace("{pat}", "[0-9\\.]+");
	page.replace("{pla}", "Longtitude des Ortes");
	page.replace("{val}", GlobalConfig.longtitude);

	// moon led
	page += FPSTR(HTTP_TITLE_LABEL_1);
	page.replace("{vs}", "Mond LED ");
	page += FPSTR(HTTP_CONF_TXT);
	page.replace("{bez}", "LEDs ");
	page.replace("{id}", "anzmoon");
	page.replace("{name}", "anzmoon");
	page.replace("{pat}", "[0-9-;]+");
	page.replace("{pla}", "Anzahl der Mond LED");
	page.replace("{val}", GlobalConfig.ledanzmond);

	// Allgemein Anzahl LED
	page += FPSTR(HTTP_TITLE_LABEL_1);
	page.replace("{vs}", "Anzahl LED ");
	page += FPSTR(HTTP_CONF_TXT);
	page.replace("{bez}", "LEDs ");
	page.replace("{id}", "anzled");
	page.replace("{name}", "anzled");
	page.replace("{pat}", "[0-9]+");
	page.replace("{pla}", "Anzahl der LED des Strip");
	page.replace("{val}", GlobalConfig.ledanzges);
	page += F("</pre>");

	// Status Save
	page += FPSTR(HTTP_LABEL_STATUS);
	if (sc) {
		saveSuccess = saveSystemConfig();
		Serial.print("sc=1");
		LEDSwitchSave();
		LEDMoonSave();
		SunMmoon_Set();
		FastLED_Set();
		LEDsetColorsAll();
	}
	if (sc && !showHMDevError) {
		if (saveSuccess) {
			page.replace("{sl}", F("<label class='green'>Speichern erfolgreich.</label>"));
		}
		else {
			page.replace("{sl}", F("<label class='red'>Speichern fehlgeschlagen.</label>"));
		}
	}
	if (showHMDevError)
		page.replace("{sl}", F("<label class='red'>Ger&auml;tenamen in CUxD pr&uuml;fen!</label>"));
	if (!sc && !showHMDevError)
		page.replace("{sl}", "");

	page += FPSTR(HTTP_BUTTON_SAVE);
	page += FPSTR(HTTP_BUTTON_LED_LED);
	page.replace("{led}", "Zur&uuml;ck");
	page += FPSTR(HTTP_LABEL_FW);
	page.replace("{fw}", FIRMWARE_VERSION);

	page += F("</form></div>");
	page += F("</div></body></html>");
	page.replace("{v}", NetConfig.DeviceName);

	WebServer.send(200, "text/html", page);
}
void replyTempJSON() { // senden der Daten an die webseit / anzeiige in WEBLED confLEDHtml 
	DEBUG("ReplyTempü");
	String Temp = "Temperatur: " + String(Temperatur[0], 2) + " &degC";
	WebServer.send(200, "application/json", "{\"joTemp\":\"" + Temp + "\"}");
}

