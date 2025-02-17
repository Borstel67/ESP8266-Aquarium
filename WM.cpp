/**************************************************************
   WiFiManager is a library for the ESP8266/Arduino platform
   (https://github.com/esp8266/Arduino) to enable easy
   configuration and reconfiguration of WiFi credentials using a Captive Portal
   inspired by:
   http://www.esp8266.com/viewtopic.php?f=29&t=2520
   https://github.com/chriscook8/esp-arduino-apboot
   https://github.com/esp8266/Arduino/tree/esp8266/hardware/esp8266com/esp8266/libraries/DNSServer/examples/CaptivePortalAdvanced
   Built by AlexT https://github.com/tzapu
   Licensed under MIT license
 **************************************************************/

#include "WM.h"
#include <FS.h>
String NETZ_NAME_AP = "ESP8266";

WiFiManagerParameter::WiFiManagerParameter(const char* custom) {
	_id = NULL;
	_placeholder = NULL;
	_length = 0;
	_value = NULL;
	_type = 0;
	_customHTML = custom;
}

WiFiManagerParameter::WiFiManagerParameter(const char* id, const char* placeholder, const char* defaultValue, int length) {
	init(id, placeholder, defaultValue, length, 0, "");
}

WiFiManagerParameter::WiFiManagerParameter(const char* id, const char* placeholder, const char* defaultValue, int length, byte type) {
	init(id, placeholder, defaultValue, length, type, "");
}

WiFiManagerParameter::WiFiManagerParameter(const char* id, const char* placeholder, const char* defaultValue, int length, byte type, const char* custom) {
	init(id, placeholder, defaultValue, length, type, custom);
}

void WiFiManagerParameter::init(const char* id, const char* placeholder, const char* defaultValue, int length, byte type, const char* custom) {
	_id = id;
	_placeholder = placeholder;
	_length = length;
	_type = type;
	_value = new char[length + 1];
	for (int i = 0; i < length; i++) {
		_value[i] = 0;
	}
	if (defaultValue != NULL) {
		strncpy(_value, defaultValue, length);
	}
	_customHTML = custom;
}

const char* WiFiManagerParameter::getValue() {
	return _value;
}
const char* WiFiManagerParameter::getID() {
	return _id;
}
const char* WiFiManagerParameter::getPlaceholder() {
	return _placeholder;
}
int WiFiManagerParameter::getValueLength() {
	return _length;
}
const char* WiFiManagerParameter::getCustomHTML() {
	return _customHTML;
}
byte WiFiManagerParameter::getType() {
	return _type;
}

WiFiManager::WiFiManager() {
}

void WiFiManager::addParameter(WiFiManagerParameter* p) {
	if (_paramsCount + 1 > WIFI_MANAGER_MAX_PARAMS)
	{
		//Max parameters exceeded!
		DEBUG_WM("WIFI_MANAGER_MAX_PARAMS exceeded, increase number (in WiFiManager.h) before adding more parameters!");
		DEBUG_WM("Skipping parameter with ID:");
		DEBUG_WM(p->getID());
		return;
	}
	_params[_paramsCount] = p;
	_paramsCount++;
	DEBUG_WM("Adding parameter");
	DEBUG_WM(p->getID());
}

void WiFiManager::setupConfigPortal() {
	DEBUG_WM(F("Starting Web Portal"));
	// setup dns and web servers
	dnsServer.reset(new DNSServer());
	server.reset(new ESP8266WebServer(80));
	_configPortalStart = millis();
	dnsServer->setErrorReplyCode(DNSReplyCode::NoError);
	DEBUG_WM("dns server started with ip: ");
	DEBUG_WM(WiFi.softAPIP());
	dnsServer->start(DNS_PORT, F("*"), WiFi.softAPIP());

	// Setup web pages: root, wifi config pages, SO captive portal detectors and not found. */
	server->on("/", std::bind(&WiFiManager::handleRoot, this));
	server->on("/wifi", std::bind(&WiFiManager::handleWifi, this, true));
	server->on("/0wifi", std::bind(&WiFiManager::handleWifi, this, false));
	server->on("/wifisave", std::bind(&WiFiManager::handleWifiSave, this));
	server->on("/i", std::bind(&WiFiManager::handleInfo, this)); //Chip Info
	server->on("/r", std::bind(&WiFiManager::handleReset, this)); // Einstellung Reset 
	server->on("/c", std::bind(&WiFiManager::handleClearAllData, this)); // Alles zurÃƒÆ’Ã‚Â¼cksetzen
	//server->on("/LED", std::bind(&WiFiManager::handleLED, this)); // Alles zurÃƒÆ’Ã‚Â¼cksetzen
	server->on("/generate_204", std::bind(&WiFiManager::handle204, this));  //Android/Chrome OS captive portal check.
	server->on("/fwlink", std::bind(&WiFiManager::handleRoot, this));  //Microsoft captive portal. Maybe not needed. Might be handled by notFound handler.
	server->onNotFound(std::bind(&WiFiManager::handleNotFound, this));
	server->begin(); // Web server start
	DEBUG_WM(F("HTTP server started"));
}

boolean WiFiManager::autoConnect() {
	String ssid = NETZ_NAME_AP + "_" + String(ESP.getChipId());
	return autoConnect(ssid.c_str(), NULL);
}

boolean WiFiManager::autoConnect(char const* apName, char const* apPassword) {
	DEBUG_WM(F(""));
	DEBUG_WM(F("AutoConnect"));


	// attempt to connect; should it fail, fall back to AP
	WiFi.mode(WIFI_STA);

	if (connectWifi("", "") == WL_CONNECTED) {
		DEBUG_WM(F("IP Address:"));
		DEBUG_WM(WiFi.localIP());
		//connected
		return true;
	}

	return startConfigPortal(apName);
}

boolean WiFiManager::configPortalHasTimeout() {
	if (_configPortalTimeout == 0 || wifi_softap_get_station_num() > 0) {
		_configPortalStart = millis(); // kludge, bump configportal start time to skew timeouts
		return false;
	}
	return (millis() > _configPortalStart + _configPortalTimeout);
}

boolean WiFiManager::startConfigPortal() {
	String ssid = NETZ_NAME_AP + "_" + String(ESP.getChipId());
	return startConfigPortal(ssid.c_str());
}

boolean  WiFiManager::startConfigPortal(char const* apName) {
	//setup AP
	DEBUG_WM(F("Configuring access point... "));
	DEBUG_WM(_apName);
	Serial.println("Configuring access point... ");
	String ssid = NETZ_NAME_AP + "_" + String(WiFi.macAddress());
	delay(100);
	_apName = ssid.c_str();
	WiFi.softAP(ssid.c_str());
	WiFi.mode(WIFI_AP);
	DEBUG_WM("SET AP");
	Serial.println("SET AP");


	//notify we entered AP mode
	if (_apcallback != NULL) {
		_apcallback(this);
	}

	connect = false;
	setupConfigPortal();
	int lcount = 0;
	while (1) {

		// check if timeout
		if (configPortalHasTimeout()) break;

		//DNS
		dnsServer->processNextRequest();
		//HTTP
		server->handleClient();

		if (connect) {
			connect = false;
			if (_cuapIP != "") {
				break;
			}
			delay(2000);
			DEBUG_WM(F("Connecting to new AP"));
			Serial.println("Connecting to new AP");
			// Verwenden der vom Benutzer angegebenen _ssid, _pass anstelle der vom System gespeicherten ssid und pass
			if (connectWifi(_ssid, _pass) != WL_CONNECTED) {
				DEBUG_WM(F("Failed to connect."));
				Serial.println("Failed to connect.");
			}
			else {
				//connected
				WiFi.mode(WIFI_STA);
				//Benachrichtigen Sie, dass sich die Konfiguration geändert hat und alle optionalen Parameter gespeichert werden sollten
				if (_savecallback != NULL) {
					//todo: Überprüfen Sie, ob benutzerdefinierte Parameter vorhanden sind, und überprüfen Sie, ob sie sich tatsächlich geändert haben
					_savecallback();
				}
				break;
			}

			if (_shouldBreakAfterConfig) {
				Serial.println("_shouldBreakAfterConfig");
				//flag set to exit after config after trying to connect
				//notify that configuration has changed and any optional parameters should be saved
				if (_savecallback != NULL) {
					//todo: check if any custom parameters actually exist, and check if they really changed maybe
					_savecallback();
				}
				break;
			}
		}
		yield();
		//Serial.println("end while");
		//break;
	}

	//server.reset();
	//dnsServer.reset();
	Serial.println("confPortend");

	return  WiFi.status() == WL_CONNECTED;
}


int WiFiManager::connectWifi(String ssid, String pass) {
	DEBUG_WM(F("Connecting as wifi client..."));

	// check if we've got static_ip settings, if we do, use those.
	if (_sta_static_ip) {
		DEBUG_WM(F("Custom STA IP/GW/Subnet"));
		WiFi.config(_sta_static_ip, _sta_static_gw, _sta_static_sn);
		DEBUG_WM(WiFi.localIP());
	}
	//fix for auto connect racing issue
	if (WiFi.status() == WL_CONNECTED) {
		DEBUG_WM("Already connected. Bailing out.");
		return WL_CONNECTED;
	}
	//check if we have ssid and pass and force those, if not, try with last saved values
	if (ssid != "") {
		WiFi.begin(ssid.c_str(), pass.c_str());
	}
	else {
		if (WiFi.SSID()) {
			DEBUG_WM("Using last saved values, should be faster");
			//trying to fix connection in progress hanging
			ETS_UART_INTR_DISABLE();
			wifi_station_disconnect();
			ETS_UART_INTR_ENABLE();

			WiFi.begin();
		}
		else {
			DEBUG_WM("No saved credentials");
		}
	}

	int connRes = waitForConnectResult();
	DEBUG_WM("Connection result: ");
	DEBUG_WM(connRes);
	//not connected, WPS enabled, no pass - first attempt
	if (_tryWPS && connRes != WL_CONNECTED && pass == "") {
		startWPS();
		//should be connected at the end of WPS
		connRes = waitForConnectResult();
	}
	return connRes;
}

uint8_t WiFiManager::waitForConnectResult() {
	if (_connectTimeout == 0) {
		return WiFi.waitForConnectResult();
	}
	else {
		DEBUG_WM(F("Waiting for connection result with time out"));
		unsigned long start = millis();
		boolean keepConnecting = true;
		uint8_t status;
		while (keepConnecting) {
			status = WiFi.status();
			if (millis() > start + _connectTimeout) {
				keepConnecting = false;
				DEBUG_WM(F("Connection timed out"));
			}
			if (status == WL_CONNECTED || status == WL_CONNECT_FAILED) {
				keepConnecting = false;
			}
			delay(100);
		}
		return status;
	}
}

void WiFiManager::startWPS() {
	DEBUG_WM("START WPS");
	WiFi.beginWPSConfig();
	DEBUG_WM("END WPS");
}

String WiFiManager::getConfigPortalSSID() {
	return _apName;
}

void WiFiManager::resetSettings() {
	DEBUG_WM(F("settings invalidated"));
	DEBUG_WM(F("THIS MAY CAUSE AP NOT TO START UP PROPERLY. YOU NEED TO COMMENT IT OUT AFTER ERASING THE DATA."));
	WiFi.disconnect(true);
	//delay(200);
}
void WiFiManager::setTimeout(unsigned long seconds) {
	setConfigPortalTimeout(seconds);
}

void WiFiManager::setConfigPortalTimeout(unsigned long seconds) {
	_configPortalTimeout = seconds * 1000;
}

void WiFiManager::setConnectTimeout(unsigned long seconds) {
	_connectTimeout = seconds * 1000;
}

void WiFiManager::setDebugOutput(boolean debug) {
	_debug = debug;
}

void WiFiManager::setSTAStaticIPConfig(IPAddress ip, IPAddress gw, IPAddress sn) {
	_sta_static_ip = ip;
	_sta_static_gw = gw;
	_sta_static_sn = sn;
}

void WiFiManager::setMinimumSignalQuality(int quality) {
	_minimumQuality = quality;
}

void WiFiManager::setBreakAfterConfig(boolean shouldBreak) {
	_shouldBreakAfterConfig = shouldBreak;
}

// Handle root or redirect to captive portal
void WiFiManager::handleRoot() {
	DEBUG_WM(F("Handle root"));
	if (captivePortal()) { // If caprive portal redirect instead of displaying the page.
		return;
	}

	String page = FPSTR(HTTP_HEAD);
	page.replace("{v}", "Options");
	page += FPSTR(HTTP_SCRIPT);
	page += FPSTR(HTTP_STYLE);
	page += _customHeadElement;
	page += FPSTR(HTTP_HEAD_END);
	page += "<center><h3>";
	page += _apName;
	page += "</h3></center>";
	//page += F("<center><h3>Konfigurationsportal LED</h3></center>");
	//page += FPSTR(HTTP_BUTTON_LED_AP);
	page += F("<center><h3>Konfigurationsportal WIFI</h3></center>");
	page += FPSTR(HTTP_BUTTON_CONFIG_WIFI_AP);
	page += FPSTR(HTTP_BUTTON_CHIPINFO);
	page += FPSTR(HTTP_BUTTON_RESTART);
	page += FPSTR(HTTP_BUTTON_FACTORYRESET);
	page += FPSTR(HTTP_END);

	server->sendHeader("Content-Length", String(page.length()));
	server->send(200, "text/html", page);

}

/** Wifi config page handler */
void WiFiManager::handleWifi(boolean scan) {

	String page = FPSTR(HTTP_HEAD);
	page.replace("{v}", "Config ESP");
	page += FPSTR(HTTP_SCRIPT);
	page += FPSTR(HTTP_STYLE);
	page += _customHeadElement;
	page += FPSTR(HTTP_HEAD_END);

	if (scan) {
		int n = WiFi.scanNetworks();
		DEBUG_WM(F("Scan done"));
		if (n == 0) {
			DEBUG_WM(F("No networks found"));
			page += F("Kein WLAN gefunden.");
		}
		else {

			//sort networks
			int indices[n];
			for (int i = 0; i < n; i++) {
				indices[i] = i;
			}

			// RSSI SORT

			// old sort
			for (int i = 0; i < n; i++) {
				for (int j = i + 1; j < n; j++) {
					if (WiFi.RSSI(indices[j]) > WiFi.RSSI(indices[i])) {
						std::swap(indices[i], indices[j]);
					}
				}
			}

			/*std::sort(indices, indices + n, [](const int & a, const int & b) -> bool
			  {
			  return WiFi.RSSI(a) > WiFi.RSSI(b);
			  });*/

			  // remove duplicates ( must be RSSI sorted )
			if (_removeDuplicateAPs) {
				String cssid;
				for (int i = 0; i < n; i++) {
					if (indices[i] == -1) continue;
					cssid = WiFi.SSID(indices[i]);
					for (int j = i + 1; j < n; j++) {
						if (cssid == WiFi.SSID(indices[j])) {
							DEBUG_WM("DUP AP: " + WiFi.SSID(indices[j]));
							indices[j] = -1; // set dup aps to index -1
						}
					}
				}
			}

			//display networks in page
			for (int i = 0; i < n; i++) {
				if (indices[i] == -1) continue; // skip dups
				DEBUG_WM(WiFi.SSID(indices[i]));
				DEBUG_WM(WiFi.RSSI(indices[i]));
				int quality = getRSSIasQuality(WiFi.RSSI(indices[i]));

				if (_minimumQuality == -1 || _minimumQuality < quality) {
					String item = FPSTR(HTTP_ITEM);
					String rssiQ;
					rssiQ += quality;
					item.replace("{v}", WiFi.SSID(indices[i]));
					item.replace("{r}", rssiQ);
					if (WiFi.encryptionType(indices[i]) != ENC_TYPE_NONE) {
						item.replace("{i}", "l");
					}
					else {
						item.replace("{i}", "");
					}
					//DEBUG_WM(item);
					page += item;
					delay(0);
				}
				else {
					DEBUG_WM(F("Skipping due to quality"));
				}

			}
			page += "<br/>";
		}
	}

	page += FPSTR(HTTP_FORM_START);
	char parLength[2];
	// add the extra parameters to the form
	for (int i = 0; i < _paramsCount; i++) {
		if (_params[i] == NULL) {
			break;
		}

		if (_params[i]->getType() == 0) {
			String pitem = FPSTR(HTTP_FORM_PARAM_INPUT);
			if (_params[i]->getID() != NULL) {
				pitem.replace("{i}", _params[i]->getID());
				pitem.replace("{n}", _params[i]->getID());
				pitem.replace("{p}", _params[i]->getPlaceholder());
				snprintf(parLength, 2, "%d", _params[i]->getValueLength());
				pitem.replace("{l}", parLength);
				pitem.replace("{v}", _params[i]->getValue());
				pitem.replace("{c}", _params[i]->getCustomHTML());
			}
			else {
				pitem = _params[i]->getCustomHTML();
			}
			page += pitem;
		}
		if (_params[i]->getType() == 1) {
			String pitem = FPSTR(HTTP_FORM_PARAM_CKB);
			pitem.replace("{i}", _params[i]->getID());
			pitem.replace("{n}", _params[i]->getID());
			pitem.replace("{p}", _params[i]->getPlaceholder());
			snprintf(parLength, 2, "%d", _params[i]->getValueLength());
			pitem.replace("{l}", parLength);
			if (atoi(_params[i]->getValue()) == 1) {
				pitem.replace("{v}", "checked");
			}
			else {
				pitem.replace("{v}", "");
			}
			pitem.replace("{c}", _params[i]->getCustomHTML());
			page += pitem;
		}
		if (_params[i]->getType() == 2) {
			String pitem = FPSTR(HTTP_FORM_PARAM_COB);
			pitem.replace("{i}", _params[i]->getID());
			pitem.replace("{n}", _params[i]->getID());
			pitem.replace("{p}", _params[i]->getPlaceholder());
			snprintf(parLength, 2, "%d", _params[i]->getValueLength());
			pitem.replace("{l}", parLength);
			pitem.replace("{v}", _params[i]->getValue());
			pitem.replace("{c}", _params[i]->getCustomHTML());
			page += pitem;
		}
		if (_params[i]->getType() == 4) {
			String pitem = FPSTR(HTTP_FORM_PARAM_PWD);
			pitem.replace("{i}", _params[i]->getID());
			pitem.replace("{n}", _params[i]->getID());
			pitem.replace("{p}", _params[i]->getPlaceholder());
			snprintf(parLength, 2, "%d", _params[i]->getValueLength());
			pitem.replace("{l}", parLength);
			pitem.replace("{v}", _params[i]->getValue());
			pitem.replace("{c}", _params[i]->getCustomHTML());
			page += pitem;
		}
		if (_params[i]->getType() == 5) {
			String pitem = FPSTR(HTTP_FORM_PARAM_PWD);
			pitem.replace("{i}", _params[i]->getID());
			pitem.replace("{n}", _params[i]->getID());
			pitem.replace("{p}", _params[i]->getPlaceholder());
			snprintf(parLength, 2, "%d", _params[i]->getValueLength());
			pitem.replace("{l}", parLength);
			pitem.replace("{v}", _params[i]->getValue());
			pitem.replace("{c}", _params[i]->getCustomHTML());
			page += pitem;
		}
		if (_params[i]->getType() == 6) {
			String pitem = FPSTR(HTTP_FORM_PARAM_PWD);
			pitem.replace("{i}", _params[i]->getID());
			pitem.replace("{n}", _params[i]->getID());
			pitem.replace("{p}", _params[i]->getPlaceholder());
			snprintf(parLength, 2, "%d", _params[i]->getValueLength());
			pitem.replace("{l}", parLength);
			pitem.replace("{v}", _params[i]->getValue());
			pitem.replace("{c}", _params[i]->getCustomHTML());
			page += pitem;
		}
		if (_params[i]->getType() == 7) {
			String pitem = FPSTR(HTTP_FORM_PARAM_PWD);
			pitem.replace("{i}", _params[i]->getID());
			pitem.replace("{n}", _params[i]->getID());
			pitem.replace("{p}", _params[i]->getPlaceholder());
			snprintf(parLength, 2, "%d", _params[i]->getValueLength());
			pitem.replace("{l}", parLength);
			pitem.replace("{v}", _params[i]->getValue());
			pitem.replace("{c}", _params[i]->getCustomHTML());
			page += pitem;
		}
		if (_params[i]->getType() == 8) {
			String pitem = FPSTR(HTTP_FORM_PARAM_PWD);
			pitem.replace("{i}", _params[i]->getID());
			pitem.replace("{n}", _params[i]->getID());
			pitem.replace("{p}", _params[i]->getPlaceholder());
			snprintf(parLength, 2, "%d", _params[i]->getValueLength());
			pitem.replace("{l}", parLength);
			pitem.replace("{v}", _params[i]->getValue());
			pitem.replace("{c}", _params[i]->getCustomHTML());
			page += pitem;
		}
		if (_params[i]->getType() == 9) {
			String pitem = FPSTR(HTTP_FORM_PARAM_PWD);
			pitem.replace("{i}", _params[i]->getID());
			pitem.replace("{n}", _params[i]->getID());
			pitem.replace("{p}", _params[i]->getPlaceholder());
			snprintf(parLength, 2, "%d", _params[i]->getValueLength());
			pitem.replace("{l}", parLength);
			pitem.replace("{v}", _params[i]->getValue());
			pitem.replace("{c}", _params[i]->getCustomHTML());
			page += pitem;
		}
		if (_params[i]->getType() == 10) {
			String pitem = FPSTR(HTTP_FORM_PARAM_PWD);
			pitem.replace("{i}", _params[i]->getID());
			pitem.replace("{n}", _params[i]->getID());
			pitem.replace("{p}", _params[i]->getPlaceholder());
			snprintf(parLength, 2, "%d", _params[i]->getValueLength());
			pitem.replace("{l}", parLength);
			pitem.replace("{v}", _params[i]->getValue());
			pitem.replace("{c}", _params[i]->getCustomHTML());
			page += pitem;
		}
	}

	if (_params[0] != NULL) {
		page += "<br/>";
	}

	page += FPSTR(HTTP_FORM_END);
	page += FPSTR(HTTP_SCAN_LINK);

	page += FPSTR(HTTP_END);

	server->sendHeader("Content-Length", String(page.length()));
	server->send(200, "text/html", page);


	DEBUG_WM(F("Sent config page"));
}

/** Handle the WLAN save form and redirect to WLAN config page again */
void WiFiManager::handleWifiSave() {
	DEBUG_WM(F("WiFi save"));
	Serial.println("Wifi Save");
	//SAVE/connect here
	_ssid = server->arg("s").c_str();
	_pass = server->arg("p").c_str();
	_cuapIP = server->arg("custom_APip").c_str();

	//for (int i = 0; i < server->args(); i++) {
	//    DEBUG_WM(server->argName(i)); //Get the name of the parameter
	//    DEBUG_WM(server->arg(i));
	//}
	//parameters
	for (int i = 0; i < _paramsCount; i++) {
		if (_params[i] == NULL) {
			break;
		}
		//read parameterF
		String value = server->arg(_params[i]->getID()).c_str();
		value.trim();

		//store it in array
		value.toCharArray(_params[i]->_value, _params[i]->_length);
		DEBUG_WM(F("Parameter"));
		DEBUG_WM(_params[i]->getID());
		DEBUG_WM(value);
	}
	Serial.println("Wifi Save if staip");

	if (server->arg("custom_STAip") != "") {
		DEBUG_WM(F("static ip"));
		DEBUG_WM(server->arg("custom_STAip"));
		_sta_static_ip.fromString(server->arg("custom_STAip"));
		String ip = server->arg("custom_STAip");
		optionalIPFromString(&_sta_static_ip, ip.c_str());
	}
	Serial.println("Wifi Save if stagw");

	if (server->arg("custom_STAgw") != "") {
		DEBUG_WM(F("static gateway"));
		DEBUG_WM(server->arg("custom_STAgw"));
		String gw = server->arg("custom_STAgw");
		optionalIPFromString(&_sta_static_gw, gw.c_str());
	}
	Serial.println("Wifi Save stanet");

	if (server->arg("custom_STAnetmask") != "") {
		DEBUG_WM(F("static netmask"));
		DEBUG_WM(server->arg("custom_STAnetmask"));
		String sn = server->arg("custom_STAnetmask");
		optionalIPFromString(&_sta_static_sn, sn.c_str());
	}
	String page = FPSTR(HTTP_HEAD);
	page.replace("{v}", "Credentials Saved");
	page += FPSTR(HTTP_SCRIPT);
	page += FPSTR(HTTP_STYLE);
	page += _customHeadElement;
	page += FPSTR(HTTP_HEAD_END);
	page += FPSTR(HTTP_SAVED);
	page += FPSTR(HTTP_END);

	server->send(200, "text/html", page);

	DEBUG_WM(F("Sent wifi save page"));

	connect = true; //signal ready to connect/reset
	Serial.println("Wifi Save end");

}

/** Handle the info page */
void WiFiManager::handleInfo() {
	DEBUG_WM(F("Info"));

	String page = FPSTR(HTTP_HEAD);
	page.replace("{v}", "Info");
	page += FPSTR(HTTP_SCRIPT);
	page += FPSTR(HTTP_STYLE);
	page += _customHeadElement;
	page += FPSTR(HTTP_HEAD_END);
	page += F("<dl>");
	page += F("<dt>Chip ID</dt><dd>");
	page += ESP.getChipId();
	page += F("</dd>");
	page += F("<dt>Flash Chip ID</dt><dd>");
	page += ESP.getFlashChipId();
	page += F("</dd>");
	page += F("<dt>IDE Flash Size</dt><dd>");
	page += ESP.getFlashChipSize();
	page += F(" bytes</dd>");
	page += F("<dt>Real Flash Size</dt><dd>");
	page += ESP.getFlashChipRealSize();
	page += F(" bytes</dd>");
	page += F("<dt>Soft AP IP</dt><dd>");
	page += WiFi.softAPIP().toString();
	page += F("</dd>");
	page += F("<dt>Soft AP MAC</dt><dd>");
	page += WiFi.softAPmacAddress();
	page += F("</dd>");
	page += F("<dt>Station MAC</dt><dd>");
	page += WiFi.macAddress();
	page += F("</dd>");
	page += F("</dl>");
	page += FPSTR(HTTP_END);

	server->send(200, "text/html", page);

	DEBUG_WM(F("Sent info page"));
}

/** Handle the reset page */
void WiFiManager::handleReset() {
	DEBUG_WM(F("Restart"));

	String page = FPSTR(HTTP_HEAD);
	page.replace("{v}", "Info");
	page += FPSTR(HTTP_SCRIPT);
	page += FPSTR(HTTP_STYLE);
	page += _customHeadElement;
	page += FPSTR(HTTP_HEAD_END);
	page += F("ESP startet in wenigen Sekunden neu...");
	page += FPSTR(HTTP_END);
	server->sendHeader("Content-Length", String(page.length()));
	server->send(200, "text/html", page);

	DEBUG_WM(F("Sent reset page"));
	delay(5000);
	ESP.restart();
	delay(2000);
}

void WiFiManager::handleClearAllData() {
	DEBUG_WM(F("handleClearAllData"));

	String page = FPSTR(HTTP_HEAD);
	page.replace("{v}", "Clear All Data");
	page += FPSTR(HTTP_SCRIPT);
	page += FPSTR(HTTP_STYLE);
	page += _customHeadElement;
	page += FPSTR(HTTP_HEAD_END);
	page += "<center><h1>Clearing all data...</h1><br>This can take a minute</center>";
	page += FPSTR(HTTP_END);
	server->send(200, "text/html", page);


	DEBUG_WM(F("handleClearAllData::WiFi.disconnect(true)"));

	//don't know, which one is the right one
	//WiFi.disconnect();
	WiFi.disconnect(true);

	DEBUG_WM(F("handleClearAllData::SPIFFS.format()"));
	SPIFFS.format();

	DEBUG_WM(F("handleClearAllData::ESP.eraseConfig()"));
	ESP.eraseConfig();

	DEBUG_WM(F("handleClearAllData::Doing reset..."));
	// Fire Watchdog Reset
	while (1);
}

void WiFiManager::handleLED() {
	DEBUG_WM(F("handleLED"));
	//String page = FPSTR(HTTP_HEAD);
	//page.replace("{v}", "handleLED");
	//page += FPSTR(HTTP_SCRIPT);
	//page += FPSTR(HTTP_STYLE);
	//page += _customHeadElement;
	//page += FPSTR(HTTP_HEAD_END);
	//page += "<center><h1>Config LED</h1><br>This can take a minute</center>";
	//page += FPSTR(HTTP_END);
	//server->send(200, "text/html", page);

}
//removed as mentioned here https://github.com/tzapu/WiFiManager/issues/114
void WiFiManager::handle204() {
	DEBUG_WM(F("204 No Response"));
	server->sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
	server->sendHeader("Pragma", "no-cache");
	server->sendHeader("Expires", "-1");
	server->send(204, "text/plain", "");
}

void WiFiManager::handleNotFound() {
	if (captivePortal()) { // If captive portal redirect instead of displaying the error page.
		return;
	}
	String message = "File Not Found\n\n";
	message += "URI: ";
	message += server->uri();
	message += "\nMethod: ";
	message += (server->method() == HTTP_GET) ? "GET" : "POST";
	message += "\nArguments: ";
	message += server->args();
	message += "\n";

	for (uint8_t i = 0; i < server->args(); i++) {
		message += " " + server->argName(i) + ": " + server->arg(i) + "\n";
	}
	server->sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
	server->sendHeader("Pragma", "no-cache");
	server->sendHeader("Expires", "-1");
	server->sendHeader("Content-Length", String(message.length()));
	server->send(404, "text/plain", message);
}


/** Redirect to captive portal if we got a request for another domain. Return true in that case so the page handler do not try to handle the request again. */
boolean WiFiManager::captivePortal() {
	if (!isIp(server->hostHeader())) {
		DEBUG_WM(F("Request redirected to captive portal"));
		server->sendHeader("Location", String("http://") + toStringIp(server->client().localIP()), true);
		server->send(302, "text/plain", ""); // Empty content inhibits Content-length header so we have to close the socket ourselves.
		server->client().stop(); // Stop is needed because we sent no content length
		return true;
	}
	return false;
}

//start up config portal callback
void WiFiManager::setAPCallback(void (*func)(WiFiManager * myWiFiManager)) {
	_apcallback = func;
}

//start up save config callback
void WiFiManager::setSaveConfigCallback(void (*func)(void)) {
	_savecallback = func;
}

//sets a custom element to add to head, like a new style tag
void WiFiManager::setCustomHeadElement(const char* element) {
	_customHeadElement = element;
}

//if this is true, remove duplicated Access Points - defaut true
void WiFiManager::setRemoveDuplicateAPs(boolean removeDuplicates) {
	_removeDuplicateAPs = removeDuplicates;
}



template <typename Generic>
void WiFiManager::DEBUG_WM(Generic text) {
	if (_debug) {
		//Serial.print("*WM: ");
		//Serial.println(text);
	}
}

int WiFiManager::getRSSIasQuality(int RSSI) {
	int quality = 0;

	if (RSSI <= -100) {
		quality = 0;
	}
	else if (RSSI >= -50) {
		quality = 100;
	}
	else {
		quality = 2 * (RSSI + 100);
	}
	return quality;
}

/** Is this an IP? */
boolean WiFiManager::isIp(String str) {
	for (int i = 0; i < str.length(); i++) {
		int c = str.charAt(i);
		if (c != '.' && (c < '0' || c > '9')) {
			return false;
		}
	}
	return true;
}

/** IP to String? */
String WiFiManager::toStringIp(IPAddress ip) {
	String res = "";
	for (int i = 0; i < 3; i++) {
		res += String((ip >> (8 * i)) & 0xFF) + ".";
	}
	res += String(((ip >> 8 * 3)) & 0xFF);
	return res;
}
