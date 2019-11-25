String _ssid;
String _psk;
String _apssid;

bool doWifiConnect() {
	_ssid = WiFi.SSID();
	_psk = WiFi.psk();
	_apssid = NetConfig.DeviceName;
	//	String _appsk = WiFi.psk();

	String _pskMask = "";
	for (int i = 0; i < _psk.length(); i++) {
		_pskMask += "*";
	}
	DEBUG("ssid = " + _ssid + ", psk = " + _pskMask);

	if (_ssid != "" || _ssid != "............" || _psk != "") {
		startWifiManager = true;
	}

	// config AP mode
	const char* APipStr = NetConfig.APip; byte APipBytes[4]; parseBytes(APipStr, '.', APipBytes, 4, 10);
	const char* APnetmaskStr = NetConfig.APnetmask;	byte APnetmaskBytes[4];	parseBytes(APnetmaskStr, '.', APnetmaskBytes, 4, 10);
	const char* APgwStr = NetConfig.APgw; byte APgwBytes[4]; parseBytes(APgwStr, '.', APgwBytes, 4, 10);

	// config STA mode
	const char* ipStr = NetConfig.ip; byte ipBytes[4]; parseBytes(ipStr, '.', ipBytes, 4, 10);
	const char* netmaskStr = NetConfig.netmask; byte netmaskBytes[4]; parseBytes(netmaskStr, '.', netmaskBytes, 4, 10);
	const char* gwStr = NetConfig.gw; byte gwBytes[4]; parseBytes(gwStr, '.', gwBytes, 4, 10);
	const char* dns1Str = NetConfig.DNS1; byte dns1Bytes[4]; parseBytes(dns1Str, '.', dns1Bytes, 4, 10);
	const char* dns2Str = NetConfig.DNS2; byte dns2Bytes[4]; parseBytes(dns2Str, '.', dns2Bytes, 4, 10);


	// auswahl AP oder STA Mode
	if (!startWifiManager || String(NetConfig.APip) != "0.0.0.0") {
		DEBUG(F("Connecting WLAN AP "));
		// start ap mode wenn bedingungen erfüllt sonst sta oder wifimanager
		// mDNS setzen des Device Namen
		//WiFiManager wifiManager;
		const char* ssid = NetConfig.DeviceName;
		const char* password = "";
		DEBUG(F("Connecting WLAN the AP..."));
		WiFi.softAP(ssid, password, 1, 0, 4);//WiFi.softAP(ssid, password, channel, hidden, max_connection)
		WiFi.mode(WIFI_AP);
		DEBUG("config WLAN STA");
		//WiFi.setSleepMode(WIFI_NONE_SLEEP);
		WiFi.softAPConfig(IPAddress(APipBytes[0], APipBytes[1], APipBytes[2], APipBytes[3]), IPAddress(APgwBytes[0], APgwBytes[1], APgwBytes[2], APgwBytes[3]), IPAddress(APnetmaskBytes[0], APnetmaskBytes[1], APnetmaskBytes[2], APnetmaskBytes[3]));//softAPConfig (local_ip, gateway, subnet)
		DEBUG("Start AP");
		startAPMode = true;
		Serial.print(WiFi.softAPIP());
		Serial.print(WiFi.hostname());
		Serial.println(WiFi.SSID());
		//wifiManager.autoConnect(NetConfig.DeviceName);
	}
	else {
		// start sta mode wenn bedingungen erfült sonst wifimanager 
			if (!startWifiManager || (_ssid != "" && _ssid != "............" && _psk != "")) {
			DEBUG(F("Connecting WLAN STA "));
			WiFi.mode(WIFI_STA);
			WiFi.hostname(NetConfig.DeviceName);
			WiFi.setSleepMode(WIFI_NONE_SLEEP);
			WiFi.setAutoReconnect(true);
			if (String(NetConfig.ip) != "0.0.0.0") {
				DEBUG("config STA");
				WiFi.config(IPAddress(ipBytes[0], ipBytes[1], ipBytes[2], ipBytes[3]), IPAddress(gwBytes[0], gwBytes[1], gwBytes[2], gwBytes[3]), IPAddress(netmaskBytes[0], netmaskBytes[1], netmaskBytes[2], netmaskBytes[3]), IPAddress(dns1Bytes[0], dns1Bytes[1], dns1Bytes[2], dns1Bytes[3]), IPAddress(dns2Bytes[0], dns2Bytes[1], dns2Bytes[2], dns2Bytes[3]));
				ETS_UART_INTR_DISABLE();
				wifi_station_disconnect();
				ETS_UART_INTR_ENABLE();
			}
			DEBUG("Start STA");
			WiFi.begin(_ssid.c_str(), _psk.c_str());
			int waitCounter = 0;
			Serial.println(WiFi.localIP());
			Serial.println(WiFi.hostname());

			while (WiFi.status() != WL_CONNECTED) {
				waitCounter++;
				Serial.print(".");
				if (waitCounter == 30) {
					return false;
				}
				delay(500);
			}

			DEBUG("Wifi STA Connected");
			WiFiConnected = true;

			return true;
		}
		else { // start config mode wifimanager ConfigPortal
				DEBUG("Start WIFI Manager");
				doWifiManager();
		}
		return true;
	}
}
void doWifiManager() {
	DEBUG("WIFI Manager gestartet");
	_ssid = WiFi.SSID();
	_psk = WiFi.psk();
	_apssid = NetConfig.DeviceName;

	// start config mode wifimanager ConfigPortal
	WiFiManager wifiManager;
	wifiManager.setDebugOutput(wifiManagerDebugOutput);
	wifiManager.setAPCallback(configModeCallback);
	wifiManager.setSaveConfigCallback(saveConfigCallback);
	// STA daten
	WiFiManagerParameter custom_text_STAmode("<br/><br><div>Stations Modus</div>");
	WiFiManagerParameter custom_STAName("STAName", "Ger&auml;tename", NetConfig.DeviceName, VARSIZE255, 0, "pattern='[A-Za-z0-9_ -]+'");
	WiFiManagerParameter custom_STAip("custom_STAip", "IP-Adresse", (String(NetConfig.ip) != "0.0.0.0") ? NetConfig.ip : "", VARSIZE16, 0, "pattern='((^|\\.)((25[0-5])|(2[0-4]\\d)|(1\\d\\d)|([1-9]?\\d))){4}$'");
	WiFiManagerParameter custom_STAnetmask("custom_STAnetmask", "Netzmaske", (String(NetConfig.netmask) != "0.0.0.0") ? NetConfig.netmask : "", VARSIZE16, 0, "pattern='((^|\\.)((25[0-5])|(2[0-4]\\d)|(1\\d\\d)|([1-9]?\\d))){4}$'");
	WiFiManagerParameter custom_STAgw("custom_STAgw", "Gateway", (String(NetConfig.gw) != "0.0.0.0") ? NetConfig.gw : "", VARSIZE16, 0, "pattern='((^|\\.)((25[0-5])|(2[0-4]\\d)|(1\\d\\d)|([1-9]?\\d))){4}$'");
	WiFiManagerParameter custom_DNS1ip("custom_DNS1ip", "IP-Adresse DNS1", (String(NetConfig.DNS1) != "0.0.0.0") ? NetConfig.DNS1 : "", VARSIZE16, 0, "pattern='((^|\\.)((25[0-5])|(2[0-4]\\d)|(1\\d\\d)|([1-9]?\\d))){4}$'");
	WiFiManagerParameter custom_DNS2ip("custom_DNS2ip", "IP-Adresse DNS2", (String(NetConfig.DNS2) != "0.0.0.0") ? NetConfig.DNS2 : "", VARSIZE16, 0, "pattern='((^|\\.)((25[0-5])|(2[0-4]\\d)|(1\\d\\d)|([1-9]?\\d))){4}$'");
	WiFiManagerParameter custom_STAtext("<br/><br><div>Statische IP (wenn leer, dann DHCP):</div>");
	wifiManager.addParameter(&custom_text_STAmode);
	wifiManager.addParameter(&custom_STAName);
	wifiManager.addParameter(&custom_STAtext);
	wifiManager.addParameter(&custom_STAip);
	wifiManager.addParameter(&custom_STAnetmask);
	wifiManager.addParameter(&custom_STAgw);
	wifiManager.addParameter(&custom_DNS1ip);
	wifiManager.addParameter(&custom_DNS2ip);
	// AP Daten
	WiFiManagerParameter custom_text_APmode("<br/><br><div>Access Point Modus</div>");
	WiFiManagerParameter custom_APip("custom_APip", "IP-Adresse", (String(NetConfig.APip) != "0.0.0.0") ? NetConfig.APip : "", VARSIZE16, 0, "pattern='((^|\\.)((25[0-5])|(2[0-4]\\d)|(1\\d\\d)|([1-9]?\\d))){4}$'");
	WiFiManagerParameter custom_APnetmask("custom_APnetmask", "Netzmaske", (String(NetConfig.APnetmask) != "0.0.0.0") ? NetConfig.APnetmask : "", VARSIZE16, 0, "pattern='((^|\\.)((25[0-5])|(2[0-4]\\d)|(1\\d\\d)|([1-9]?\\d))){4}$'");
	WiFiManagerParameter custom_APgw("custom_APgw", "Gateway", (String(NetConfig.APgw) != "0.0.0.0") ? NetConfig.APgw : "", VARSIZE16, 0, "pattern='((^|\\.)((25[0-5])|(2[0-4]\\d)|(1\\d\\d)|([1-9]?\\d))){4}$'");
	WiFiManagerParameter custom_APtext("<br/><br><div>AP IP (wenn leer, dann STAmode):</div>");
	wifiManager.addParameter(&custom_text_APmode);
	wifiManager.addParameter(&custom_APtext);
	wifiManager.addParameter(&custom_APip);
	wifiManager.addParameter(&custom_APnetmask);
	wifiManager.addParameter(&custom_APgw);

	wifiManager.setConfigPortalTimeout(ConfigPortalTimeout);
	if (startWifiManager == true) {
		DEBUG(_ssid);
		DEBUG(_psk);
		DEBUG(_apssid);
		wifiManager.resetSettings();
		wifiManager.startConfigPortal();
		DEBUG(F("WM: failed to connect and hit timeout"));
		Serial.println(wm_shouldSaveConfig);
		if (String(custom_STAip.getValue()).length() > 5) {
			DEBUG("Custom IP Address is set!");
			strcpy(NetConfig.ip, custom_STAip.getValue());
			strcpy(NetConfig.netmask, custom_STAnetmask.getValue());
			strcpy(NetConfig.gw, custom_STAgw.getValue());
			strcpy(NetConfig.DNS1, custom_DNS1ip.getValue());
			strcpy(NetConfig.DNS2, custom_DNS2ip.getValue());
		}
		else {
			DEBUG("Custom IP Address is not set!");
			strcpy(NetConfig.ip, "0.0.0.0");
			strcpy(NetConfig.netmask, "0.0.0.0");
			strcpy(NetConfig.gw, "0.0.0.0");
			strcpy(NetConfig.DNS1, "0.0.0.0");
			strcpy(NetConfig.DNS2, "0.0.0.0");
		}
		if (String(custom_APip.getValue()).length() > 5) {
			DEBUG("Custom APIP Address is set!");
			strcpy(NetConfig.APip, custom_APip.getValue());
			strcpy(NetConfig.APnetmask, custom_APnetmask.getValue());
			strcpy(NetConfig.APgw, custom_APgw.getValue());
		}
		else {
			DEBUG("Custom IP Address is not set!");
			strcpy(NetConfig.APip, "0.0.0.0");
			strcpy(NetConfig.APnetmask, "0.0.0.0");
			strcpy(NetConfig.APgw, "0.0.0.0");
		}
		// ÃƒÆ’Ã‚Â¼bergibtv werte an NetConfig
		// Projekt Daten
		strcpy(NetConfig.DeviceName, custom_STAName.getValue());

		//Speichert in GlobalConfig
		DEBUG("NetConfig Speichen");
		saveNetConfig();
		DEBUG("resetBootConfigMode");
		printWifiStatus();
		resetBootConfigMode(); // rücksetzen Boot Config Mode und reset
	}
}

void configModeCallback(WiFiManager* myWiFiManager) {
	DEBUG("AP-Modus ist aktiv!");
}

void saveConfigCallback() {
	DEBUG("Should save config");
	wm_shouldSaveConfig = true;
}

void parseBytes(const char* str, char sep, byte* bytes, int maxBytes, int base) {
	for (int i = 0; i < maxBytes; i++) {
		bytes[i] = strtoul(str, NULL, base);
		str = strchr(str, sep);
		if (str == NULL || *str == '\0') {
			break;
		}
		str++;
	}
}

void printWifiStatus() {
	DEBUG("SSID: " + WiFi.SSID());
	DEBUG("IP Address: " + IpAddress2String(WiFi.localIP()));
	DEBUG("Gateway Address: " + IpAddress2String(WiFi.gatewayIP()));
	DEBUG("signal strength (RSSI):" + String(WiFi.RSSI()) + " dBm");
}
