//Anbindung HomeMatic
void CUxD_Set() {
	// config HomeMatic CCU CuxD 
	if (String(NetConfig.ccuIP) != "0.0.0.0" && String(NetConfig.ccuIP) != "") {
		if (GlobalConfig.BackendType == BackendType_HomeMatic) {
			reloadCUxDAddress(NO_TRANSMITSTATE);
			byte tryCount = 0;
			byte tryCountMax = 5;
			while (HomeMaticConfig.ChannelName == "CUxD.") {
				tryCount++;
				DEBUG("Failed getting CUxD Device from HomeMaticConfig.ChannelName. Retry " + String(tryCount) + " / " + String(tryCountMax));
				delay(1000);
				reloadCUxDAddress(NO_TRANSMITSTATE);
				if (tryCount == tryCountMax) break;
			}
		}
	}
	// end config HomeMatic CCU CuxD 
}


bool setStateCUxD(String id, String value) {
	if (id.indexOf(".null.") == -1 && String(NetConfig.ccuIP) != "0.0.0.0") {
		if (WiFi.status() == WL_CONNECTED) {
			HTTPClient http;
			http.setTimeout(HTTPTimeOut);
			id.replace(" ", "%20");
			String url = "http://" + String(NetConfig.ccuIP) + ":8181/cuxd.exe?ret=dom.GetObject(%22" + id + "%22).State(" + value + ")";
			DEBUG("setStateCUxD url: " + url, "setStateCUxD()", _slInformational);
			http.begin(url);
			int httpCode = http.GET();
			String payload = "";

			if (httpCode > 0) {
				DEBUG("HTTP " + id + " success", "setStateCUxD()", _slInformational);
				payload = http.getString();
			}
			if (httpCode != 200) {
				//        blinkLED(3);
				DEBUG("HTTP " + id + " failed with HTTP Error Code " + String(httpCode), "setStateCUxD()", _slError);
			}
			http.end();

			payload = payload.substring(payload.indexOf("<ret>"));
			payload = payload.substring(5, payload.indexOf("</ret>"));

			DEBUG("result: " + payload, "setStateCUxD()", (payload != "null") ? _slInformational : _slError);

			return (payload != "null");

		}
		else {
			DEBUG("setStateCUxD: WiFi.status() != WL_CONNECTED, trying to reconnect", "setStateCUxD()", _slError);
			/*if (!doWifiConnect()) {
			  DEBUG("setStateCUxD: doWifiConnect() failed.", "setStateCUxD()", _slError);
			  //ESP.restart();
			  }*/
		}
	}
	else return true;
}

String getStateCUxD(String id, String type) {
	if (id != "" && id.indexOf(".null.") == -1 && String(NetConfig.ccuIP) != "0.0.0.0" && String(NetConfig.ccuIP) != "") {
		if (WiFi.status() == WL_CONNECTED) {
			HTTPClient http;
			http.setTimeout(HTTPTimeOut);
			id.replace(" ", "%20");
			String url = "http://" + String(NetConfig.ccuIP) + ":8181/cuxd.exe?ret=dom.GetObject(%22" + id + "%22)." + type + "()";
			DEBUG("getStateFromCUxD url: " + url, "getStateCUxD()", _slInformational);
			http.begin(url);
			int httpCode = http.GET();
			String payload = "error";
			if (httpCode > 0) {
				payload = http.getString();
			}
			if (httpCode != 200) {
				//        blinkLED(3);
				DEBUG("HTTP " + id + " fail", "getStateCUxD()", _slError);
			}
			http.end();

			payload = payload.substring(payload.indexOf("<ret>"));
			payload = payload.substring(5, payload.indexOf("</ret>"));
			DEBUG("result: " + payload, "getStateCUxD()", _slInformational);

			return payload;
		}
		else {
			DEBUG("getStateCUxD: WiFi.status() != WL_CONNECTED, trying to reconnect", "getStateCUxD()", _slError);
			/*if (!doWifiConnect()) {
			  DEBUG("getStateCUxD: doWifiConnect() failed.", "getStateCUxD()", _slError);
			  //ESP.restart();
			  }*/
		}
	}
	else return "null";
}

String reloadCUxDAddress(bool transmitState) {
	String ret = "";
	HomeMaticConfig.ChannelName = "CUxD." + getStateCUxD(String(NetConfig.DeviceName), "Address");
	ret += "CUxD Address = " + HomeMaticConfig.ChannelName;
	DEBUG("HomeMaticConfig.ChannelName = " + HomeMaticConfig.ChannelName);


	//  if (GlobalConfig.SonoffModel == SonoffModel_TouchAsSender || (GlobalConfig.GPIO14Mode != GPIO14Mode_OFF && GlobalConfig.GPIO14asSender)) {
	//    HomeMaticConfig.ChannelNameSender =  "CUxD." + getStateCUxD(String(NetConfig.DeviceName) + ":1", "Address");
	//    ret += " ; CUxD Address Sender = " + HomeMaticConfig.ChannelNameSender;
	//    DEBUG("HomeMaticConfig.ChannelNameSender = " + HomeMaticConfig.ChannelNameSender);
	//  }

	//  if (transmitState == TRANSMITSTATE)
	//    setStateCUxD(HomeMaticConfig.ChannelName + ".SET_STATE", String(getRelayState()));
	return ret;
}
