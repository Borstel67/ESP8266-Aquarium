/*
  ESP8266 Bibliotek 2.4.1
  ArduinoJson.h version 5
  Generic ESP8285 Module
  Flash Mode: DOUT
  Flash Frequency: 40 MHz
  CPU Frequency: 80 MHz
  Flash Size: 1M (64k SPIFFS)
*/

#define VARSIZE4				4
#define VARSIZE6				6
#define VARSIZE8				8
#define VARSIZE10				10
#define VARSIZE16				16
#define VARSIZE32	    		32
#define VARSIZE255	    		255


#include <Arduino.h>
#include <FS.h>
#include <ArduinoJson.h>				// v.5.xx
#include <ESP8266WiFi.h>
#include <ESP8266FtpServer.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266mDNS.h>
#include <sunMoon.h>
#include <Time.h>						// für NTP
#include <Timezone.h>					// sommer winterzeit
#include <TimeLib.h>					// für NTP und SunMoon
#include <ArduinoOTA.h>
#include <WiFiUdp.h>
#include <FastLED.h>
#include <OneWire.h>				// für Temperatur DS18B20
#include <DallasTemperature.h>		// für Temperatur DS18B20
#include "WM.h"
#include "css_global.h"
#include "js_global.h"
#include "js_webled.h"
#include "js_fwupd.h"

// definire Programmteile
// def Debug Serial ein und ausschalten
#define	LEDStrip // definierung wenn ein LEDStrip angeschlossen
//#define UDPDEBUG
#define SERIALDEBUG
#define TELNET

// definire Parameter 
#define MillisKeyBounce			100		//Millisekunden zwischen 2xtasten
#define ConfigPortalTimeout		180		//Timeout (Sekunden) des AccessPoint-Modus
#define HTTPTimeOut				1500	//Timeout (Millisekunden) für http requests
#define UDPPORT					6676	// UDP Port
#define KEYPRESSLONGMILLIS		1500	//Millisekunden für langen Tastendruck bei Sonoff Touch als Sender
#define MAX_TELNET_CLIENTS		2		// Telnet Server

const String FIRMWARE_VERSION = "1.3.5";


//FW über GITHUB
const char GITHUB_REPO_URL[] PROGMEM = "https://api.github.com/repos/jp112sdl/SonoffHMLOX/releases/latest";

//Debug Syslog über UDP
#ifdef UDPDEBUG
const char* SYSLOGIP = "192.168.178.100";
#define SYSLOGPORT          514
#endif
enum _SyslogSeverity {
	_slEmergency,
	_slAlert,
	_slCritical,
	_slError,
	_slWarning,
	_slNotice,
	_slInformational,
	_slDebug
};
// end Debug

//Auswahl Homatic Loxon
enum BackendTypes_e {
	BackendType_HomeMatic,
};

enum TransmitStates_e {
	NO_TRANSMITSTATE,
	TRANSMITSTATE
};

// Golbal Config schreiben in Datei jonson.conf
struct globalconfig_t {
	char ledcolor1[VARSIZE8] = "#ffffff";				// Color Kanal 1 Ein
	char ledcolor2[VARSIZE8] = "#ffff00";				// Color Kanal 2 Ein
	char ledcolor3[VARSIZE8] = "#ccff00";				// Color Kanal 3 Ein
	char ledcolor4[VARSIZE8] = "#ffa200";				// Color Kanal 4 Ein
	char ledcolor5[VARSIZE8] = "#000000";				// Color Kanal 1 Aus
	char ledcolor6[VARSIZE8] = "#000000";				// Color Kanal 2 Aus
	char ledcolor7[VARSIZE8] = "#000000";				// Color Kanal 3 Aus
	char ledcolor8[VARSIZE8] = "#000000";				// Color Kanal 4 Aus
	char ledcolormoon[VARSIZE8] = "#000000";			// Color bei Mond
	char time1[VARSIZE6] = "06:30";						// Zeit Kanal 1 Ein
	char time11[VARSIZE6] = "00:00";					// Zeit Kanal 1 Ein
	char time2[VARSIZE6] = "06:45";						// Zeit Kanal 2 Ein
	char time21[VARSIZE6] = "00:00";					// Zeit Kanal 2 Ein
	char time3[VARSIZE6] = "06:15";						// Zeit Kanal 3 Ein
	char time31[VARSIZE6] = "00:00";					// Zeit Kanal 3 Ein
	char time4[VARSIZE6] = "06:30";						// Zeit Kanal 4 Ein
	char time41[VARSIZE6] = "00:00";					// Zeit Kanal 4 Ein
	char time5[VARSIZE6] = "21:00";						// Zeit Kanal 1 Aus
	char time51[VARSIZE6] = "00:00";					// Zeit Kanal 1 Aus
	char time6[VARSIZE6] = "21:00";						// Zeit Kanal 2 Aus
	char time61[VARSIZE6] = "00:00";					// Zeit Kanal 2 Aus
	char time7[VARSIZE6] = "21:30";						// Zeit Kanal 3 Aus
	char time71[VARSIZE6] = "00:00";					// Zeit Kanal 3 Aus
	char time8[VARSIZE6] = "21:45";						// Zeit Kanal 4 Aus
	char time81[VARSIZE6] = "00:00";					// Zeit Kanal 4 Aus
	char hk1[VARSIZE4] = "2";							// Hand Kanal 1 (0=Aus/1=Ein/2=Uhr)
	char hk2[VARSIZE4] = "2";							// Hand Kanal 2 (0=Aus/1=Ein/2=Uhr)
	char hk3[VARSIZE4] = "2";							// Hand Kanal 3 (0=Aus/1=Ein/2=Uhr)
	char hk4[VARSIZE4] = "2";							// Hand Kanal 4 (0=Aus/1=Ein/2=Uhr)
	char hkm[VARSIZE4] = "2";							// Hand Kanal M (0=Auto/1=Ein/2=Uhr)
	char ledanzges[VARSIZE4] = "76";					// Anzahl LED in Strip
	char ledanzcol[VARSIZE4] = "4";						// Anzahl der Collorkanäle
	char ledanzmond[VARSIZE255] = "54-56";// Anzahl der mond led
	char latitude[VARSIZE10] = "50.92878";				// Jena cordinates
	char longtitude[VARSIZE10] = "11.5899";
	//char DevicAPName[VARSIZE32] = "AP_TEST";			// Name des Access Point
	byte BackendType = BackendType_HomeMatic;			// definierung des Aussehens der HTML Seiten
	String Hostname = "ESP56";
} GlobalConfig;

// Net Config
struct NetConfig_t {
	char ip[VARSIZE16] = "192.168.178.251";				// IP STA
	char netmask[VARSIZE16] = "255.255.255.0";			// Netzmaske STA
	char gw[VARSIZE16] = "192.168.178.1";				// Gateway STA
	char DNS1[VARSIZE16] = "192.168.178.1";				// DNS Server 1 STA
	char DNS2[VARSIZE16] = "0.0.0.0";					// DNS Server 2 STA
	char APip[VARSIZE16] = "0.0.0.0";					// IP des AP
	char APnetmask[VARSIZE16] = "0.0.0.0";				// Netzmaske AP
	char APgw[VARSIZE16] = "0.0.0.0";					// Gateway AP
	char ccuIP[VARSIZE16] = "";							// IP der CCU CuxD
	char DeviceName[VARSIZE32] = "TEST";				// Netzname des ESP (Hostname)
	char NTPSERVER[VARSIZE255] = "192.168.178.1";		// NTP Server
	char TIMEZONE[VARSIZE4] = "1";						// Zeitzone
	char swu[VARSIZE4] = "1";							// Sommer/Winter Umschaltung
	char ftp[VARSIZE4] = "0";							// FTP ein
	char SYSLOGIP[VARSIZE4] = "0";						// SYSLOGIP ein
	char Telnet[VARSIZE4] = "0";						// Telnet ein
	char DateMan[VARSIZE16] = "0";						// Datum Manuelle 
	char TimeMan[VARSIZE6] = "0";						// Zeit Manuelle 
} NetConfig;

struct lastState_t {
	char confMode[VARSIZE4] = "0";						// configmode aktivieren
} lastState;

// homeMatic Config
struct hmconfig_t {
	String ChannelName = "";
	String ChannelNameSender = "";
	char PowerVariableName[VARSIZE255] = "";
	char EnergyCounterVariableName[VARSIZE16] = "";
} HomeMaticConfig;

// Config Allg
const String bootConfigModeFilename = "bootcfg.mod";
const String lastStateFile = "laststat.json";
const String configJsonFile = "config.json";
const String NetconfigJsonFile = "Netconfig.json";
bool RelayState = LOW;
bool KeyPress = false;
bool startAPMode = false;
bool WiFiConnected = false;
unsigned long time_m = 0;
unsigned long time_mn = 0;
unsigned long LastMillisKeyPress = 0;
unsigned long TimerStartMillis = 0;
unsigned long KeyPressDownMillis = 0;
unsigned long TimerSeconds = 0;
unsigned long LastWiFiReconnectMillis = 0;
unsigned long previousMillis = 0; // speichert wie viele Sekunden seit derletzten Änderung vergangen sind
unsigned long interval = 1000;    // Interval zwischen zwei Änderungen
unsigned long previousMillis_1 = 0; // speichert wie viele Sekunden seit derletzten Änderung vergangen sind
unsigned long interval_1 = 1000;    // Interval zwischen zwei Änderungen
bool OTAStart = false;
bool UDPReady = false;
bool startWifiManager = false;
bool wm_shouldSaveConfig = false;
bool PRESS_LONGsent = false;
#define wifiManagerDebugOutput   true

//FastLED
#define LED_STRIP_PIN			0		// pin WS2812
//#define LED_STRIP_PIN			3		// pin WS2812 NodeMCU 1.0 (ESP-12)
#define NUM_LEDS                300
#define LED_TYPE                WS2812
#define COLOR_ORDER             GRB
#define FRAMES_PER_SECOND       60
CRGB leds[NUM_LEDS];                    //Anzahl von LED des Strip
//#define NUM_LEDS 221//192+139+86


// Time NTP
char ntpServerName[VARSIZE255] = "us.pool.ntp.org";
//char noNTP[VARSIZE16] = "";
char NTPStaus[VARSIZE32] = "";
int timeZone = 1;     // Central European Time
int intAktZeit, akt_hour, akt_minute, akt_second, offsetCEST;
time_t prevDisplay = 0; // when the digital clock was displayed
TimeChangeRule CEST = { "CEST", Last, Sun, Mar, 2, 60 };     //Central European Summer Time
TimeChangeRule CET = { "CET ", Last, Sun, Oct, 3, 0 };       //Central European Standard Time
Timezone CE(CEST, CET);
// end Time NTP

// SunMoon
sunMoon  sm;
int sm_latitude;
int sm_longtitude;
int sm_timezone;
uint32_t jDay;
byte mDay;
time_t sRise;
time_t sSet;
time_t s_date;
//end SunMoon

// WEB Server
ESP8266WebServer WebServer(80);
ESP8266HTTPUpdateServer httpUpdater;
// end WEB Server

// Telnet Server
WiFiServer TelnetServer(23);
WiFiClient TelnetClient[MAX_TELNET_CLIENTS];
// end TelNet Server

// FTP Server
FtpServer ftpSrv;   //set #define FTP_DEBUG in ESP8266FtpServer.h to see ftp verbose on serial
// end FTP Server

// UTP Protokoll
struct udp_t {
	WiFiUDP UDP;
	char incomingPacket[255];
} UDPClient;

// Temperatur DS18B20
// Data wire is plugged into pin D5 on the ESP8266 12-E - GPIO 14 (D5)
#define ONE_WIRE_BUS_PIN		14		// pin Temperatur DS18B20
#define DS18B20_Aufloesung		8		// auflösung Temperatur DS18B20
#define Anzahl_Sensoren_DS18B20	1		// Anzahl der angeschlossenen Sensoren - Mögliche Werte: '0','1','2'
DeviceAddress DS18B20_Adressen;
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS_PIN);// on pin (a 4.7K resistor is necessary)
DallasTemperature myDS18B20(&oneWire);
// end Temperatur DS18B20

void setup() {
	Serial.begin(115200);
	TM("\nESP " + String(WiFi.macAddress()) + " startet... (FW: " + String(FIRMWARE_VERSION) + ")");

	DEBUG(F("Config-Modus aktivieren? "));
	if (SPIFFS.begin()) {
		DEBUG(F("-> bootConfigModeFilename mounted file system"));
		if (SPIFFS.exists("/" + bootConfigModeFilename)) {
			File _File = SPIFFS.open("/" + bootConfigModeFilename, "r+");
			DEBUG(_File + " existiert", "setup()", _slInformational);
			startWifiManager = false;
			if (_File && _File.size()) {
				String content = String(char(_File.read()));
				DEBUG("bootConfigMode FileContent = " + content, "setup()", _slInformational);
				if (content == "1") {
					startWifiManager = (content == "1");
					DEBUG("Starte doWifiManager");
					doWifiManager();
				}
			}
			_File.print("0");
			DEBUG("-> " + bootConfigModeFilename + " existiert, starte Programm");
			_File.close();
			SPIFFS.end();
		}
		else {
			DEBUG("-> " + bootConfigModeFilename + " existiert NICHT setBootConfigMode");
			DEBUG("Starte setBootConfigMode");
			setBootConfigMode();
		}
	}
	else {
		Serial.println(F("-> Nein, SPIFFS mount fail!"));
	}
	//lade Projetinstellungen
	loadNetConfig();
	loadSystemConfig();
	loadlastState();
	DEBUG(F("Projetinstellungen geladen"));
	//
//Ab hier ist die Config geladen und alle Variablen sind mit deren Werten belegt!
	//
	//GlobalConfig.Hostname = String(NetConfig.DeviceName);

	//Starte Wifi 
	doWifiConnect();

	// beginn Dienste
	// WEB Server
	initWebServerHandler();
	httpUpdater.setup(&WebServer);
	WebServer.begin();
	// end WEB

	// OTA
	startOTAhandling();
	DEBUG("Starte UDP-Handler an Port " + String(UDPPORT) + "...");
	UDPClient.UDP.begin(UDPPORT);
	UDPReady = true;
	// end OTA

	// Start Telnet Server
	//Serial.println("Starting Telnet server");
	TelnetServer.begin();
	TelnetServer.setNoDelay(true);
	// end Telnet Server

	// Start FTP Server
	ftpSrv.begin("esp8266", "esp8266");   //username, password for ftp.  set ports in ESP8266FtpServer.h  (default 21, 50009 for PASV)
	// end FTP Server

	DEBUG(WiFi.hostname() + " - Boot abgeschlossen, SSID = " + WiFi.SSID() + ", IP = " + String(IpAddress2String(WiFi.localIP())) + ", RSSI = " + WiFi.RSSI() + ", MAC = " + WiFi.macAddress(), "Setup", _slInformational);
	// ende stat dienste

	// Time NTP
	void digitalClockDisplay();
	void printDigits(int digits);
	void sendNTPpacket(IPAddress & address);
	NTP_Set();
	// end NTP

	// config HomeMatic CCU CuxD 
	CUxD_Set();
	// end config HomeMatic CCU CuxD 

	// Temperatr DS18B20
	TempDS18B20_Set();
	// end Temperatur DS18B20

	// SunMoon
	SunMmoon_Set();
	// end SunMoon

	//LED Strip
	FastLED_Set();	
	/*const int anzled = atoi(GlobalConfig.ledanzges);
	anzahl der led der kette FastLED.addLeds<WS2812, LED_STRIP_PIN, GRB>(leds, anzled).setCorrection(TypicalLEDStrip); */
	// end LED Strip

	//if (timeStatus() == timeNotSet) {
	//	TimeHand();
	//}
}

void loop() {
	//Überlauf der millis() abfangen
	if (LastMillisKeyPress > millis())
		LastMillisKeyPress = millis();
	if (TimerStartMillis > millis())
		TimerStartMillis = millis();
	if (LastWiFiReconnectMillis > millis())
		LastWiFiReconnectMillis = millis();

	//Reconnect WiFi wenn nicht verbunden (alle 30 Sekunden)
	if (!startAPMode) {
		if (WiFi.status() != WL_CONNECTED) {
			WiFiConnected = false;
			if (millis() - LastWiFiReconnectMillis > 30000) {
				LastWiFiReconnectMillis = millis();
				DEBUG("WiFi Connection lost! Reconnecting...");
				WiFi.reconnect();
			}
			// Setup MDNS responder
			if (!MDNS.begin(NetConfig.DeviceName)) {
				Serial.println("Error setting up MDNS responder!");
			}
			else {
				Serial.println("mDNS responder started");
				// Add service to MDNS-SD
				MDNS.addService("http", "tcp", 80);
			}
		}
		else {
			if (!WiFiConnected) {
				DEBUG("WiFi reconnected!");
				WiFiConnected = true;
			}
		}
	}

	//auf OTA Anforderung reagieren
	ArduinoOTA.handle();
	if (!OTAStart) {
		//eingehende UDP Kommandos abarbeiten
		String udpMessage = handleUDP();
		if (udpMessage == "bootConfigMode")
			setBootConfigMode();
		if (udpMessage == "reboot")
			ESP.restart();

		// telnet
		Telnet();

		// FTP Server
		//ftpSrv.handleFTP();        //make sure in loop you call handleFTP()!!

		//eingehende HTTP Anfragen abarbeiten
		WebServer.handleClient();
		delay(10);
	}

	// Time NTP
	if (timeStatus() != timeNotSet) {
		if (now() != prevDisplay) { //update the display only if time has changed
			prevDisplay = now();
			digitalClockDisplay();
		}
	}

	// aufruf nach Zeit
	interval = 10000;
	if (millis() - previousMillis > interval) {
		previousMillis = millis();   // aktuelle Zeit abspeichern
		// funktion ausführen
		Sunrise();
		NTP_Set();
		NTPStatus();

	}
	interval_1 = 1000;
	if (millis() - previousMillis_1 > interval) {
		previousMillis_1 = millis();   // aktuelle Zeit abspeichern
		// funktion ausführen
		LEDSwitchK1();
		LEDSwitchK2();
		LEDSwitchK3();
		LEDSwitchK4();
		Serial.println("LED Swich end");
		LEDMoon();
		Serial.println("LED Moon end");
		TempDS18B20();
		Serial.println("LED Temp end");
	}
	// FTP Server
	ftpSrv.handleFTP();        //make sure in loop you call handleFTP()!!
}



//Timer
//    if (TimerSeconds > 0 && millis() - TimerStartMillis > TimerSeconds * 1000) {
//      DEBUG(F("Timer abgelaufen. Schalte Relais aus."), "loop()", _slInformational);
//      switchRelay(RELAYSTATE_OFF, TRANSMITSTATE);
//    }
//needed for UDP packet parser
