/*
	Name:       WEB.ino
	Created:	28.04.2019 18:56:26
	Author:     borstel
	Inhalt:		Webseiten Allgemein
*/

void initWebServerHandler() {
	//LED
	WebServer.on("/LED", LEDHtml);
	WebServer.on("/configLED", confLEDHtml);
	// config Allgemein
	WebServer.on("/configAllg", confAllgHtml);
	WebServer.on("/version", versionHtml);
	WebServer.on("/firmware", versionHtml);
	WebServer.on("/configCUxD", confCUxDHtml);
	WebServer.on("/bootConfigMode", setBootConfigModeHtml);
	WebServer.on("/reloadCUxD", []() {
		String ret = reloadCUxDAddress(TRANSMITSTATE);
		WebServer.send(200, "text/plain", ret);
		});
	WebServer.on("/reboot", []() {
		WebServer.send(200, "text/plain", "rebooting");
		delay(100);
		ESP.restart();
		});
	WebServer.on("/restart", []() {
		WebServer.send(200, "text/plain", "rebooting");
		delay(100);
		ESP.restart();
		});
	WebServer.on("/getTimeJSON", replyTimeJSON);
	WebServer.on("/getTempJSON", replyTempJSON);
	WebServer.onNotFound(defaultHtml);
}

void defaultHtml() {
	DEBUG("Starte defaultHtml");
	if (WebServer.args() > 0) {
		for (int i = 0; i < WebServer.args(); i++) {
			if (WebServer.argName(i) == "btnAction") {
				//        switchRelay(WebServer.arg(i).toInt(), TRANSMITSTATE);
			}
			if (WebServer.argName(i) == "timer") {
				TimerSeconds = WebServer.arg(i).toInt();
				if (TimerSeconds > 0) {
					TimerStartMillis = millis();
				}
			}
		}
	}

	String page = FPSTR(HTTP_HEAD);
	page += FPSTR(HTTP_SCRIPT);
	page += FPSTR(HTTP_ALL_STYLE);
	page += FPSTR(HTTP_HM_STYLE);
	page += FPSTR(HTTP_HEAD_END);
	page += F("<div class='fbg'>");
	page += F("<form method='post' action='control'>");
	page += FPSTR(HTTP_TITLE_LABEL);
	//  page += FPSTR(HTTP_CURRENT_STATE_LABEL);
	page.replace("{v}", NetConfig.DeviceName);

	page += FPSTR(HTTP_BUTTON_LED);
	page.replace("{led}", "LED");
	page += FPSTR(HTTP_BUTTON_confAll);
	page += FPSTR(HTTP_BUTTON_CONFIG_CUxD);
	page += FPSTR(HTTP_BUTTON_CONFIG_WIFI);
	String restZeit = "";
	if (TimerSeconds > 0) restZeit = String(TimerSeconds - (millis() - TimerStartMillis) / 1000);
	page.replace("{ts}", restZeit);
	page += FPSTR(HTTP_LABEL_FW);
	page.replace("{fw}", FIRMWARE_VERSION);
	page += F("</form></div>");
	page += F("</div></body></html>");
	page.replace("{v}", NetConfig.DeviceName);

	//  page += F("</div><script>");
	//  page += FPSTR(HTTP_CUSTOMSCRIPT);
	//  page += FPSTR(HTTP_CUSTOMUPDATESCRIPT);
	//  page.replace("{fw}", FIRMWARE_VERSION);
	//  page += F("</script></div></body></html>");
	WebServer.sendHeader("Content-Length", String(page.length()));
	WebServer.send(200, "text/html", page);
	//  DEBUG (page);
}

void confCUxDHtml() {
	bool sc = false;
	bool saveSuccess = false;
	bool showHMDevError = false;
	if (WebServer.args() > 0) {
		for (int i = 0; i < WebServer.args(); i++) {
			if (WebServer.argName(i) == "btnSave")
				sc = (WebServer.arg(i).toInt() == 1);
			if (WebServer.argName(i) == "ccuip")
				strcpy(NetConfig.ccuIP, WebServer.arg(i).c_str());
			//if (WebServer.argName(i) == "devicename")
			//	strcpy(NetConfig.DeviceName, WebServer.arg(i).c_str());
		}
	}
	String page = FPSTR(HTTP_HEAD);
	page += FPSTR(HTTP_SCRIPT);
	page += FPSTR(HTTP_ALL_STYLE);
	// if (GlobalConfig.BackendType == BackendType_HomeMatic)
	page += FPSTR(HTTP_HM_STYLE);
	page += FPSTR(HTTP_HEAD_END);
	page += F("<div class='fbg'>");
	page += F("<form method='post' action='configCUxD'>");
	page += FPSTR(HTTP_TITLE_LABEL);
	page += FPSTR(HTTP_CONF_CCUIP);
	page.replace("{st}", "CCU IP");
	page.replace("{ccuip}", NetConfig.ccuIP);
	//page += FPSTR(HTTP_CONF_CCU_DEVNAME);
	//page.replace("{dn}", NetConfig.DeviceName);
	// Status Save
	page += FPSTR(HTTP_LABEL_STATUS);
	if (sc) {
		saveSuccess = saveNetConfig();
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

void setBootConfigModeHtml() {
	String page = FPSTR(HTTP_HEAD);
	page += FPSTR(HTTP_SCRIPT);
	page += FPSTR(HTTP_ALL_STYLE);
	page += FPSTR(HTTP_HM_STYLE);
	page += FPSTR(HTTP_HEAD_END);
	page += F("<div class='fbg'>");
	page += F("<form method='post' action='bootConfigMode'>");
	page += FPSTR(HTTP_ANZEIGE_TXT);
	page.replace("{id}", "");
	page.replace("{bez1}", "Wechsel in den AP Mode! <br>");
	page.replace("{bez2}", "Die WLAN - Netzwerkverbindung <br>wird abgebrochen...<br>");
	page.replace("{bez3}", "Bitte mit dem AP verbinden!<br><br> -Rebooting-");
	page += FPSTR(HTTP_LABEL_FW);
	page.replace("{fw}", FIRMWARE_VERSION);
	page += F("</form></div>");
	page += F("</div></body></html>");
	WebServer.send(200, "text/html", page);
	// starte Boot Config Mode
	setBootConfigMode();
}

void versionHtml() {
	WebServer.send(200, "text/plain", "<fw>" + FIRMWARE_VERSION + "</fw>");
}

