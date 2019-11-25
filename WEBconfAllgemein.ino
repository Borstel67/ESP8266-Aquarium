//#include "WEBconfAllgemein.h"
/*
	Name:       WEBconfAllgemein.ino
	Created:	12.05.2019
	Author:     borstel
	Inhalt:		Webseiten für allgemeine Configuration
*/

void confAllgHtml() {
	bool sc = false;
	bool saveSuccess = false;
	bool showHMDevError = false;
	char buf[3];
	strcpy(NetConfig.DateMan, "1970-01-01");
	strcpy(NetConfig.TimeMan, "07:00");
	Serial.println(WebServer.args());
	if (WebServer.args() > 0) {
		for (int i = 0; i < WebServer.args(); i++) {
			if (WebServer.argName(i) == "btnSave")
				sc = (WebServer.arg(i).toInt() == 1);
			if (WebServer.argName(i) == "datemanu")
				strcpy(NetConfig.DateMan, WebServer.arg(i).c_str());
			if (WebServer.argName(i) == "timemanu")
				strcpy(NetConfig.TimeMan, WebServer.arg(i).c_str());
			if (WebServer.argName(i) == "ntps")
				strcpy(NetConfig.NTPSERVER, WebServer.arg(i).c_str());
			if (WebServer.argName(i) == "TZlist")
				strcpy(NetConfig.TIMEZONE, WebServer.arg(i).c_str());
			if (WebServer.argName(i) == "swu")
				strcpy(NetConfig.swu, WebServer.arg(i).c_str());
		}
	}
	String page = FPSTR(HTTP_HEAD);
	page += F("<script>");
	page += FPSTR(HTTP_GLOBALSCRIPT);
	page += FPSTR(HTTP_WEBLEDSCRIPT);
	page += F("</script>");
	page += FPSTR(HTTP_ALL_STYLE);
	page += FPSTR(HTTP_HM_STYLE);
	page += FPSTR(HTTP_HEAD_END);
	page += F("<div class='fbg'>");
	page += FPSTR(HTTP_TITLE_LABEL);
	page += FPSTR(HTTP_TITLE_LABEL_1);
	page.replace("{vs}", "Zeitserver Config");
	page += F("<form method='get' action='getTimeJSON'>");// Anzeige aktuelle Zeit
	page += FPSTR(HTTP_ANZEIGE_TXT);
	page.replace("{id}", "aDatumZeit");
	page.replace("{bez1}", "");
	page.replace("{bez2}", "");
	page.replace("{bez3}", "");
	page += F("</form><br>");
	page += F("<form method='post' action='configAllg'>");
	page += F("<div>");
	page += FPSTR(HTTP_CONF_DATE);// Manuelle Einstellung des Datums
	page.replace("{bez}", "Datum ");
	page.replace("{id}", "datemanu");
	page.replace("{pla}", "1970-01-01");
	page.replace("{name}", "datemanu");
	page.replace("{val}", NetConfig.DateMan);
	page += F("<br>");
	page += FPSTR(HTTP_CONF_TIME);// Manuelle Einstellung der Zeit
	page.replace("{bez}", "Zeit &nbsp &nbsp &nbsp");
	page.replace("{id}", "timemanu");
	page.replace("{name}", "timemanu");
	page.replace("{pla}", "07:00");
	page.replace("{val}", NetConfig.TimeMan);
	page += F("</div><br>");
	page += FPSTR(HTTP_CONF_TXT);// Einstellung des NTP Servers
	page.replace("{bez}", "NTP Server ");
	page.replace("{id}", "ntps");
	page.replace("{name}", "ntps");
	page.replace("{pat}", "[A-Za-z0-9._+-]+");
	page.replace("{pla}", "Zeitserver");
	page.replace("{val}", ntpServerName);
	page += FPSTR(HTTP_LABEL_STATUS);
	page.replace("{sl}", NTPStaus);
	page += FPSTR(HTTP_CONF_TZLIST);// Einstellung der Zeitzone 
	page.replace("{bez}", "Zeitzone ");
	page.replace("{id}", "TZlist");
	page.replace("{list}", "TZlist");
	page.replace("{name}", "TZlist");
	page.replace("{val}", itoa(timeZone, buf, 10));
	//page += FPSTR(HTTP_CONF_TZ);// Einstellung der Zeitzone 
	//page.replace("{tz}", itoa(timeZone, buf, 10));
	page += F("<div>");
	page += FPSTR(HTTP_CONF_CHKBox);// Aktivierung der Sommer / Winter Zeit Umschaaltung 
	page.replace("{bez}", "Sommerumschaltung ");
	page.replace("{id}", "swu");
	page.replace("{name}", "swu");
	page.replace("{val}", "1");
	if (strcmp(NetConfig.swu, "1") == 0) {
		page.replace("{ch}", "checked");
	}
	else {
		page.replace("{ch}", "");
	}
	page += F("</div>");
	// Status Save
	page += FPSTR(HTTP_LABEL_STATUS);
	if (sc) {
		saveSuccess = saveSystemConfig();
		NTP_Set();
		TimeHand(NetConfig.DateMan, NetConfig.TimeMan);
		delay(100);
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
	page += FPSTR(HTTP_BUTTON_HOME);
	page += FPSTR(HTTP_LABEL_FW);
	page.replace("{fw}", FIRMWARE_VERSION);

	page += F("</form></div>");
	page += F("</div></body></html>");
	page.replace("{v}", NetConfig.DeviceName);

	WebServer.send(200, "text/html", page);
}
void replyTimeJSON() { // senden der Daten an die webseit / anzeiige in WEBLED confLEDHtml 
	DEBUG("ReplyTime");
	String DatumZeit = "aktuelle Zeit: " + strTimeDay(day()) + "-" + strTimeDay(month()) + "-" + strTimeDay(year()) + " " + strTimeDay(akt_hour) + ":" + strTimeDay(minute()) + ":" + strTimeDay(second());
	WebServer.send(200, "application/json", "{\"joDatumZeit\":\"" + DatumZeit + "\"}");
}
//void replyTimeJSON() { // senden der Daten an die webseit / anzeiige in WEBLED confLEDHtml 
//	DEBUG("ReplyTime");
//	String DatumZeit = strTimeDay(day()) + "-" + strTimeDay(month()) + "-" + strTimeDay(year()) + " " + strTimeDay(akt_hour) + ":" + strTimeDay(minute()) + ":" + strTimeDay(second());
//	String Temp = String(Temperatur[0], 2) + " &degC";
//	WebServer.send(200, "application/json", "{\"joDatumZeit\":\"" + DatumZeit + "\",\"joTemp\":\"" + Temp + "\"}");
//}

