// Globalconfig
#define JSONCONFIG_LEDCOLOR1              "ledcolor1"
#define JSONCONFIG_LEDCOLOR2              "ledcolor2"
#define JSONCONFIG_LEDCOLOR3              "ledcolor3"
#define JSONCONFIG_LEDCOLOR4              "ledcolor4"
#define JSONCONFIG_LEDCOLOR5              "ledcolor5" //neu
#define JSONCONFIG_LEDCOLOR6              "ledcolor6" //neu
#define JSONCONFIG_LEDCOLOR7              "ledcolor7" //neu
#define JSONCONFIG_LEDCOLOR8              "ledcolor8" //neu
#define JSONCONFIG_LEDCOLORMOON           "ledcolormoon" //neu
#define JSONCONFIG_TIME1                  "time1"
#define JSONCONFIG_TIME11                 "time11"
#define JSONCONFIG_TIME2                  "time2"
#define JSONCONFIG_TIME21                 "time21"
#define JSONCONFIG_TIME3                  "time3"
#define JSONCONFIG_TIME31                 "time31"
#define JSONCONFIG_TIME4                  "time4"
#define JSONCONFIG_TIME41                 "time41"
#define JSONCONFIG_TIME5                  "time5" //neu
#define JSONCONFIG_TIME51                 "time51"
#define JSONCONFIG_TIME6                  "time6" //neu
#define JSONCONFIG_TIME61                 "time61"
#define JSONCONFIG_TIME7                  "time7" //neu
#define JSONCONFIG_TIME71                 "time71"
#define JSONCONFIG_TIME8                  "time8" //neu
#define JSONCONFIG_TIME81                 "time81"
#define JSONCONFIG_HK1                    "hk1"
#define JSONCONFIG_HK2                    "hk2"
#define JSONCONFIG_HK3                    "hk3"
#define JSONCONFIG_HK4                    "hk4"
#define JSONCONFIG_HKM                    "hkm"
#define JSONCONFIG_ledanzges              "ledanzges"
#define JSONCONFIG_ledanzcol              "ledanzcol"
#define JSONCONFIG_ledanzmond             "ledanzmond"
#define JSONCONFIG_LATITUDE               "latitude" //neu
#define JSONCONFIG_LONGTITUDE             "longtitude" //neu
// netconfig
#define JSONCONFIG_IP                     "ip"
#define JSONCONFIG_NETMASK                "netmask"
#define JSONCONFIG_GW                     "gw"
#define JSONCONFIG_DNS1                   "DNS1"
#define JSONCONFIG_DNS2                   "DNS2"
#define JSONCONFIG_APIP                   "APip"
#define JSONCONFIG_APNETMASK              "APnetmask"
#define JSONCONFIG_APGW                   "APgw"
#define JSONCONFIG_CCUIP                  "ccuip"
#define JSONCONFIG_DEVICENAME             "devicename"
#define JSONCONFIG_NTPSERVER              "ntpserver" //neu
#define JSONCONFIG_TIMEZONE               "timezone" //neu
#define JSONCONFIG_ftp                    "ftp"
#define JSONCONFIG_swu                    "swu"
// last State
#define JSONCONFIG_CONFMode               "confmode"



// laden der Daten aus config.json
bool loadSystemConfig() {
	DEBUG(F("loadSystemConfig mounting FS..."), "loadSystemConfig()", _slInformational);
	if (SPIFFS.begin()) {
		DEBUG(F("loadSystemConfig mounted file system"), "loadSystemConfig()", _slInformational);
		if (SPIFFS.exists("/" + configJsonFile)) {
			DEBUG(F("loadSystemConfig reading config file"), "loadSystemConfig()", _slInformational);
			File _File = SPIFFS.open("/" + configJsonFile, "r");
			if (_File) {
				DEBUG(F("loadSystemConfig opened config file"), "loadSystemConfig()", _slInformational);
				size_t size = _File.size();
				std::unique_ptr<char[]> buf(new char[size]);
				_File.readBytes(buf.get(), size);
				DynamicJsonBuffer jsonBuffer;
				JsonObject& json = jsonBuffer.parseObject(buf.get());
				DEBUG("Content of JSON Config-File: /" + configJsonFile, "loadSystemConfig()", _slInformational);
				//#ifdef SERIALDEBUG
				json.printTo(Serial);
				Serial.println();
				//#endif
				if (json.success()) {
					Serial.println("\nJSON OK loadSystemConfig()");
					DEBUG("\nJSON OK", "loadSystemConfig()", _slInformational);
					((json[JSONCONFIG_LEDCOLOR1]).as<String>()).toCharArray(GlobalConfig.ledcolor1, VARSIZE8);
					((json[JSONCONFIG_LEDCOLOR2]).as<String>()).toCharArray(GlobalConfig.ledcolor2, VARSIZE8);
					((json[JSONCONFIG_LEDCOLOR3]).as<String>()).toCharArray(GlobalConfig.ledcolor3, VARSIZE8);
					((json[JSONCONFIG_LEDCOLOR4]).as<String>()).toCharArray(GlobalConfig.ledcolor4, VARSIZE8);
					((json[JSONCONFIG_LEDCOLOR5]).as<String>()).toCharArray(GlobalConfig.ledcolor5, VARSIZE8);//neu
					((json[JSONCONFIG_LEDCOLOR6]).as<String>()).toCharArray(GlobalConfig.ledcolor6, VARSIZE8);//neu
					((json[JSONCONFIG_LEDCOLOR7]).as<String>()).toCharArray(GlobalConfig.ledcolor7, VARSIZE8);//neu
					((json[JSONCONFIG_LEDCOLOR8]).as<String>()).toCharArray(GlobalConfig.ledcolor8, VARSIZE8);//neu
					((json[JSONCONFIG_TIME1]).as<String>()).toCharArray(GlobalConfig.time1, VARSIZE6);
					((json[JSONCONFIG_TIME11]).as<String>()).toCharArray(GlobalConfig.time11, VARSIZE6);
					((json[JSONCONFIG_TIME2]).as<String>()).toCharArray(GlobalConfig.time2, VARSIZE6);
					((json[JSONCONFIG_TIME21]).as<String>()).toCharArray(GlobalConfig.time21, VARSIZE6);
					((json[JSONCONFIG_TIME3]).as<String>()).toCharArray(GlobalConfig.time3, VARSIZE6);
					((json[JSONCONFIG_TIME31]).as<String>()).toCharArray(GlobalConfig.time31, VARSIZE6);
					((json[JSONCONFIG_TIME4]).as<String>()).toCharArray(GlobalConfig.time4, VARSIZE6);
					((json[JSONCONFIG_TIME41]).as<String>()).toCharArray(GlobalConfig.time41, VARSIZE6);
					((json[JSONCONFIG_TIME5]).as<String>()).toCharArray(GlobalConfig.time5, VARSIZE6);//neu
					((json[JSONCONFIG_TIME51]).as<String>()).toCharArray(GlobalConfig.time51, VARSIZE6);//neu
					((json[JSONCONFIG_TIME6]).as<String>()).toCharArray(GlobalConfig.time6, VARSIZE6);//neu
					((json[JSONCONFIG_TIME61]).as<String>()).toCharArray(GlobalConfig.time61, VARSIZE6);//neu
					((json[JSONCONFIG_TIME7]).as<String>()).toCharArray(GlobalConfig.time7, VARSIZE6);//neu
					((json[JSONCONFIG_TIME71]).as<String>()).toCharArray(GlobalConfig.time71, VARSIZE6);//neu
					((json[JSONCONFIG_TIME8]).as<String>()).toCharArray(GlobalConfig.time8, VARSIZE6);//neu
					((json[JSONCONFIG_TIME81]).as<String>()).toCharArray(GlobalConfig.time81, VARSIZE6);//neu
					((json[JSONCONFIG_HK1]).as<String>()).toCharArray(GlobalConfig.hk1, VARSIZE4);//neu
					((json[JSONCONFIG_HK2]).as<String>()).toCharArray(GlobalConfig.hk2, VARSIZE4);//neu
					((json[JSONCONFIG_HK3]).as<String>()).toCharArray(GlobalConfig.hk3, VARSIZE4);//neu
					((json[JSONCONFIG_HK4]).as<String>()).toCharArray(GlobalConfig.hk4, VARSIZE4);//neu
					((json[JSONCONFIG_LEDCOLORMOON]).as<String>()).toCharArray(GlobalConfig.ledcolormoon, VARSIZE8);//neu
					((json[JSONCONFIG_HKM]).as<String>()).toCharArray(GlobalConfig.hkm, VARSIZE4);//neu
					((json[JSONCONFIG_ledanzges]).as<String>()).toCharArray(GlobalConfig.ledanzges, VARSIZE4);//neu
					((json[JSONCONFIG_ledanzcol]).as<String>()).toCharArray(GlobalConfig.ledanzcol, VARSIZE4);//neu
					((json[JSONCONFIG_ledanzmond]).as<String>()).toCharArray(GlobalConfig.ledanzmond, VARSIZE255);//neu
					((json[JSONCONFIG_LATITUDE]).as<String>()).toCharArray(GlobalConfig.latitude, VARSIZE10);//neu
					((json[JSONCONFIG_LONGTITUDE]).as<String>()).toCharArray(GlobalConfig.longtitude, VARSIZE10);//neu
				}
				else {
					DEBUG(F("\nloadSystemConfig ERROR loading config"), "loadSystemConfig()", _slInformational);
				}
			}
			return true;
		}
		else {
			DEBUG("/" + configJsonFile + " not found.", "loadSystemConfig()", _slInformational);
			saveSystemConfig();
			return false;
		}
		SPIFFS.end();
	}
	else {
		DEBUG(F("loadSystemConfig failed to mount FS"), "loadSystemConfig()", _slCritical);
		return false;
	}
}
// schreiben der Daten in config.json
bool saveSystemConfig() {
	SPIFFS.begin();
	DEBUG(F("saving config"), "saveSystemConfig()", _slInformational);
	DynamicJsonBuffer jsonBuffer;
	JsonObject& json = jsonBuffer.createObject();
	json[JSONCONFIG_LEDCOLOR1] = GlobalConfig.ledcolor1;
	json[JSONCONFIG_LEDCOLOR2] = GlobalConfig.ledcolor2;
	json[JSONCONFIG_LEDCOLOR3] = GlobalConfig.ledcolor3;
	json[JSONCONFIG_LEDCOLOR4] = GlobalConfig.ledcolor4;
	json[JSONCONFIG_LEDCOLOR5] = GlobalConfig.ledcolor5;//neu
	json[JSONCONFIG_LEDCOLOR6] = GlobalConfig.ledcolor6;//neu
	json[JSONCONFIG_LEDCOLOR7] = GlobalConfig.ledcolor7;//neu
	json[JSONCONFIG_LEDCOLOR8] = GlobalConfig.ledcolor8;//neu
	json[JSONCONFIG_TIME1] = GlobalConfig.time1;
	json[JSONCONFIG_TIME11] = GlobalConfig.time11;
	json[JSONCONFIG_TIME2] = GlobalConfig.time2;
	json[JSONCONFIG_TIME21] = GlobalConfig.time21;
	json[JSONCONFIG_TIME3] = GlobalConfig.time3;
	json[JSONCONFIG_TIME31] = GlobalConfig.time31;
	json[JSONCONFIG_TIME4] = GlobalConfig.time4;
	json[JSONCONFIG_TIME41] = GlobalConfig.time41;
	json[JSONCONFIG_TIME5] = GlobalConfig.time5;//neu
	json[JSONCONFIG_TIME51] = GlobalConfig.time51;//neu
	json[JSONCONFIG_TIME6] = GlobalConfig.time6;//neu
	json[JSONCONFIG_TIME61] = GlobalConfig.time61;//neu
	json[JSONCONFIG_TIME7] = GlobalConfig.time7;//neu
	json[JSONCONFIG_TIME71] = GlobalConfig.time71;//neu
	json[JSONCONFIG_TIME8] = GlobalConfig.time8;//neu
	json[JSONCONFIG_TIME81] = GlobalConfig.time81;//neu
	json[JSONCONFIG_HK1] = GlobalConfig.hk1;//neu
	json[JSONCONFIG_HK2] = GlobalConfig.hk2;//neu
	json[JSONCONFIG_HK3] = GlobalConfig.hk3;//neu
	json[JSONCONFIG_HK4] = GlobalConfig.hk4;//neu
	json[JSONCONFIG_LEDCOLORMOON] = GlobalConfig.ledcolormoon;//neu
	json[JSONCONFIG_HKM] = GlobalConfig.hkm;//neu
	json[JSONCONFIG_ledanzges] = GlobalConfig.ledanzges;//neu
	json[JSONCONFIG_ledanzcol] = GlobalConfig.ledanzcol;//neu
	json[JSONCONFIG_ledanzmond] = GlobalConfig.ledanzmond;//neu
	json[JSONCONFIG_LATITUDE] = GlobalConfig.latitude;//neu
	json[JSONCONFIG_LONGTITUDE] = GlobalConfig.longtitude;//neu
	SPIFFS.remove("/" + configJsonFile);
	File _File = SPIFFS.open("/" + configJsonFile, "w");
	if (!_File) {
		DEBUG(F("failed to open config file for writing"), "saveSystemConfig()", _slCritical);
		return false;
	}
	//#ifdef SERIALDEBUG
	json.printTo(Serial);
	Serial.println();
	//#endif
	json.printTo(_File);
	_File.close();
	SPIFFS.end();
	loadSystemConfig();
	saveNetConfig();
	return true;
}

//lade Net config
bool loadNetConfig() {
	DEBUG(F("loadNetConfig mounting FS..."), "loadNetConfig()", _slInformational);
	if (SPIFFS.begin()) {
		DEBUG(F("loadNetConfig mounted file system"), "loadNetConfig()", _slInformational);
		if (SPIFFS.exists("/" + NetconfigJsonFile)) {
			DEBUG(F("loadNetConfig reading config file"), "loadNetConfig()", _slInformational);
			File _File = SPIFFS.open("/" + NetconfigJsonFile, "r");
			if (_File) {
				DEBUG(F("loadNetConfig opened config file"), "loadNetConfig()", _slInformational);
				size_t size = _File.size();
				std::unique_ptr<char[]> buf(new char[size]);
				_File.readBytes(buf.get(), size);
				DynamicJsonBuffer jsonBuffer;
				JsonObject& json = jsonBuffer.parseObject(buf.get());
				DEBUG("Content of JSON Config-File: /" + NetconfigJsonFile, "loadNetConfig()", _slInformational);
				//#ifdef SERIALDEBUG
				json.printTo(Serial);
				Serial.println();
				//#endif
				if (json.success()) {
					Serial.println("\nJSON OK loadNetConfig()");
					DEBUG("\nJSON OK", "loadNetConfig()", _slInformational);
					((json[JSONCONFIG_IP]).as<String>()).toCharArray(NetConfig.ip, VARSIZE16);
					((json[JSONCONFIG_NETMASK]).as<String>()).toCharArray(NetConfig.netmask, VARSIZE16);
					((json[JSONCONFIG_GW]).as<String>()).toCharArray(NetConfig.gw, VARSIZE16);
					((json[JSONCONFIG_CCUIP]).as<String>()).toCharArray(NetConfig.ccuIP, VARSIZE16);
					((json[JSONCONFIG_DEVICENAME]).as<String>()).toCharArray(NetConfig.DeviceName, VARSIZE255);
					((json[JSONCONFIG_NTPSERVER]).as<String>()).toCharArray(NetConfig.NTPSERVER, VARSIZE255);//neu
					((json[JSONCONFIG_TIMEZONE]).as<String>()).toCharArray(NetConfig.TIMEZONE, VARSIZE4);//neu
					((json[JSONCONFIG_ftp]).as<String>()).toCharArray(NetConfig.ftp, VARSIZE4);//neu
					((json[JSONCONFIG_swu]).as<String>()).toCharArray(NetConfig.swu, VARSIZE4);//neu
					((json[JSONCONFIG_APIP]).as<String>()).toCharArray(NetConfig.APip, VARSIZE16);
					((json[JSONCONFIG_APNETMASK]).as<String>()).toCharArray(NetConfig.APnetmask, VARSIZE16);
					((json[JSONCONFIG_APGW]).as<String>()).toCharArray(NetConfig.APgw, VARSIZE16);
					((json[JSONCONFIG_DNS1]).as<String>()).toCharArray(NetConfig.DNS1, VARSIZE16);
					((json[JSONCONFIG_DNS2]).as<String>()).toCharArray(NetConfig.DNS2, VARSIZE16);

					GlobalConfig.Hostname = String(NetConfig.DeviceName);
					DEBUG(GlobalConfig.Hostname);
				}
				else {
					DEBUG(F("\nloadNetConfig ERROR loading config"), "loadNetConfig()", _slInformational);
				}
			}
			return true;
		}
		else {
			DEBUG("/" + NetconfigJsonFile + " not found.", "loadNetConfig()", _slInformational);
			saveNetConfig();
			return false;
		}
		SPIFFS.end();
	}
	else {
		DEBUG(F("loadNetConfig failed to mount FS"), "loadNetConfig()", _slCritical);
		return false;
	}
}
bool saveNetConfig() {
	SPIFFS.begin();
	DEBUG(F("saving Netconfig"), "saveNetConfig()", _slInformational);
	DynamicJsonBuffer jsonBuffer;
	JsonObject& json = jsonBuffer.createObject();
	json[JSONCONFIG_IP] = NetConfig.ip;
	json[JSONCONFIG_NETMASK] = NetConfig.netmask;
	json[JSONCONFIG_GW] = NetConfig.gw;
	json[JSONCONFIG_CCUIP] = NetConfig.ccuIP;
	json[JSONCONFIG_DEVICENAME] = NetConfig.DeviceName;
	json[JSONCONFIG_NTPSERVER] = NetConfig.NTPSERVER;//neu
	json[JSONCONFIG_TIMEZONE] = NetConfig.TIMEZONE;//neu
	json[JSONCONFIG_ftp] = NetConfig.ftp;//neu
	json[JSONCONFIG_swu] = NetConfig.swu;//neu
	json[JSONCONFIG_APIP] = NetConfig.APip;//neu
	json[JSONCONFIG_APNETMASK] = NetConfig.APnetmask;//neu
	json[JSONCONFIG_APGW] = NetConfig.APgw;//neu
	json[JSONCONFIG_DNS1] = NetConfig.DNS1;//neu
	json[JSONCONFIG_DNS2] = NetConfig.DNS2;//neu
	SPIFFS.remove("/" + NetconfigJsonFile);
	File _File = SPIFFS.open("/" + NetconfigJsonFile, "w");
	if (!_File) {
		DEBUG(F("failed to open config file for writing"), "saveNetConfig()", _slCritical);
		return false;
	}
	//#ifdef SERIALDEBUG
	json.printTo(Serial);
	Serial.println();
	//#endif
	json.printTo(_File);
	_File.close();
	SPIFFS.end();
	loadNetConfig();
	return true;
}

bool loadlastState() {
	DEBUG(F("loadlastState mounting FS..."), "loadlastState()", _slInformational);
	if (SPIFFS.begin()) {
		DEBUG(F("loadlastState mounted file system"), "loadlastState()", _slInformational);
		if (SPIFFS.exists("/" + NetconfigJsonFile)) {
			DEBUG(F("loadlastState reading config file"), "loadlastState()", _slInformational);
			File _File = SPIFFS.open("/" + lastStateFile, "r");
			if (_File) {
				DEBUG(F("loadlastState opened config file"), "loadlastState()", _slInformational);
				size_t size = _File.size();
				std::unique_ptr<char[]> buf(new char[size]);
				_File.readBytes(buf.get(), size);
				DynamicJsonBuffer jsonBuffer;
				JsonObject& json = jsonBuffer.parseObject(buf.get());
				DEBUG("Content of JSON Config-File: /" + lastStateFile, "loadlastState()", _slInformational);
				//#ifdef SERIALDEBUG
				json.printTo(Serial);
				Serial.println();
				//#endif
				if (json.success()) {
					Serial.println("\nJSON OK loadlastState()");
					DEBUG("\nJSON OK", "loadlastState()", _slInformational);
					((json[JSONCONFIG_CONFMode]).as<String>()).toCharArray(lastState.confMode, VARSIZE4);
				}
				else {
					DEBUG(F("\nloadNetConfig ERROR loading State"), "loadlastState()", _slInformational);
				}
			}
			return true;
		}
		else {
			DEBUG("/" + lastStateFile + " not found.", "loadlastState()", _slInformational);
			saveNetConfig();
			return false;
		}
		SPIFFS.end();
	}
	else {
		DEBUG(F("loadNetConfig failed to mount FS"), "loadlastState()", _slCritical);
		return false;
	}
}
bool savelastState() {
	SPIFFS.begin();
	DEBUG(F("saving last State"), "savelastState()", _slInformational);
	DynamicJsonBuffer jsonBuffer;
	JsonObject& json = jsonBuffer.createObject();
	json[JSONCONFIG_CONFMode] = lastState.confMode;
	SPIFFS.remove("/" + lastStateFile);
	File _File = SPIFFS.open("/" + lastStateFile, "w");
	if (!_File) {
		DEBUG(F("failed to open config file for writing"), "savelastState()", _slCritical);
		return false;
	}
	//#ifdef SERIALDEBUG
	json.printTo(Serial);
	Serial.println();
	//#endif
	json.printTo(_File);
	_File.close();
	SPIFFS.end();
	loadlastState();
	return true;
}

void setBootConfigMode() {
	if (SPIFFS.begin()) {
		DEBUG(F("setBootConfigMode mounted file system"), "setBootConfigMode()", _slInformational);
		//		if (!SPIFFS.exists("/" + bootConfigModeFilename)) {
		SPIFFS.remove("/" + bootConfigModeFilename);
		File _File = SPIFFS.open("/" + bootConfigModeFilename, "w");
		_File.print("1");
		_File.close();
		SPIFFS.end();
		DEBUG(F("Boot to ConfigMode requested. Restarting..."), "setBootConfigMode()", _slInformational);
		yield;
		ESP.reset();
		//ESP.restart(); bei ESP32
	}
}

void resetBootConfigMode() {
	if (SPIFFS.begin()) {
		DEBUG(F("resetBootConfigMode mounted file system"), "resetBootConfigMode()", _slInformational);
		//		if (!SPIFFS.exists("/" + bootConfigModeFilename)) {
		SPIFFS.remove("/" + bootConfigModeFilename);
		File _File = SPIFFS.open("/" + bootConfigModeFilename, "w");
		_File.print("0");
		_File.close();
		SPIFFS.end();
		DEBUG(F("Boot to ConfigMode requested. Restarting..."), "setBootConfigMode()", _slInformational);
		yield;
		ESP.reset();
		//ESP.restart(); bei ESP32
	}
}



//void setLastRelayState(bool state) {
//  GlobalConfig.lastRelayState = state;
//  if (GlobalConfig.restoreOldRelayState == RelayStateOnBoot_LAST) {
//    if (SPIFFS.begin()) {
//      DEBUG(F("setLastState mounted file system"), "setLastState()", _slInformational);
//      //SPIFFS.remove("/" + lastStateFile);
//      File setLastStateFile = SPIFFS.open("/" + lastStateFile, "w");
//      setLastStateFile.print(state);
//      setLastStateFile.close();
//      SPIFFS.end();
//      DEBUG("setLastState (" + String(state) + ") saved.", "setLastState()", _slInformational);
//    } else {
//      DEBUG(F("setLastState SPIFFS mount fail!"), "setLastState()", _slCritical);
//    }
//  }
//}

//bool getLastRelayState() {
//  if (GlobalConfig.restoreOldRelayState == RelayStateOnBoot_LAST) {
//    if (SPIFFS.begin()) {
//      DEBUG(F("getLastState mounted file system"), "getLastState()", _slInformational);
//      if (SPIFFS.exists("/" + lastStateFile)) {
//        DEBUG(lastStateFile + " existiert", "getLastState()", _slInformational);
//        File lastStateFile = SPIFFS.open("/" + lastStateFile, "r");
//        bool bLastState = false;
//        if (lastStateFile && lastStateFile.size()) {
//          String content = String(char(lastStateFile.read()));
//          DEBUG("getLastState FileContent = " + content, "getLastState()", _slInformational);
//          bLastState = (content == "1");
//        }
//        SPIFFS.end();
//        return bLastState;
//      } else {
//        DEBUG(lastStateFile + " existiert nicht", "getLastState()", _slInformational);
//      }
//    } else {
//      DEBUG(F("getLastState SPIFFS mount fail!"), "getLastState()", _slCritical);
//      false;
//    }
//  } else {
//    return false;
//  }
//}
