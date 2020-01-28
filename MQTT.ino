/*
 Name:		MQTT.ino
 Created:	11.01.2020
 Author:	borstel
*/

// uMQTTBroker
// 
// The program simply starts a broker and waits for any client to connect.
// maximal 4 Client possible
// 
///////////////////////////////////////////////////////////////////////////////
// Declare Variable ///////////////////////////////////////////////////////////
struct mqttserver_t {
	char msg[VARSIZE8] = "";
	// MQTT Server
	int mqtt_server1_on = 0;
	char mqtt_server1[VARSIZE16] = "192.168.178.130";
	int mqtt_server2_on = 0;
	char mqtt_server2[VARSIZE16] = "192.168.178.250";
	int mqtt_server3_on = 0;
	char mqtt_server3[VARSIZE16] = "";
	int mqtt_server4_on = 0;
	char mqtt_server4[VARSIZE16] = "";
} mq_Server;
// MQTT Puplic
struct mqttptopic1_t {
	char s1[VARSIZE4] = "1";
	char s2[VARSIZE4] = "0";
	char s3[VARSIZE4] = "0";
	char s4[VARSIZE4] = "0";
	char t1[VARSIZE32] = "State/Pover";
	char b1[VARSIZE64] = "";
	char v1[VARSIZE10] = "on";
	char v2[VARSIZE10] = "off";
} mq_pup_top1;
// MQTT subscribe
struct mqttstopic1_t {
	char s1[VARSIZE4] = "0";
	char s2[VARSIZE4] = "1";
	char s3[VARSIZE4] = "0";
	char s4[VARSIZE4] = "0";
	char t1[VARSIZE64] = "ESP-MQTT_c6/State/Pover";
	char v1[VARSIZE10] = "ON";
	char v2[VARSIZE10] = "OFF";
} mq_sub_top1;
struct mqttstopic2_t {
	char s1[VARSIZE4] = "0";
	char s2[VARSIZE4] = "0";
	char s3[VARSIZE4] = "0";
	char s4[VARSIZE4] = "0";
	char t1[VARSIZE64] = "";
	char v1[VARSIZE10] = "";
	char v2[VARSIZE10] = "";
} mq_sub_top2;
struct mqttstopic3_t {
	char s1[VARSIZE4] = "0";
	char s2[VARSIZE4] = "0";
	char s3[VARSIZE4] = "0";
	char s4[VARSIZE4] = "0";
	char t1[VARSIZE64] = "";
	char v1[VARSIZE10] = "";
	char v2[VARSIZE10] = "";
} mq_sub_top3;
struct mqttstopic4_t {
	char s1[VARSIZE4] = "0";
	char s2[VARSIZE4] = "0";
	char s3[VARSIZE4] = "0";
	char s4[VARSIZE4] = "0";
	char t1[VARSIZE64] = "";
	char v1[VARSIZE10] = "";
	char v2[VARSIZE10] = "";
} mq_sub_top4;

bool BSTAT1 = false;                         // State Button GPIO0	
bool RSTAT1 = false;                         // State Relais GPIO12	
bool TRIGG1 = false;
bool TRIGG2 = false;
bool TRIGG3 = false;
// subscribe
String RECEIVE;                     // Received Topic for logical operations
String RECEVAL;	                    // Received Topic for logical operations
String RECSTOP;
char RECSVAL[VARSIZE6];
char RECSVALalt[VARSIZE6];


// Entprellen
int entprellWert = 200;
int letzteZeit_isr;
//int letzteZeit_off;
int letzteZeit_call;

// init MQTT class
class myMQTTBroker : public uMQTTBroker
{
public:
	virtual bool onConnect(IPAddress addr, uint16_t client_count) {
		Serial.println(addr.toString() + " connected");
		return true;
	}
	virtual bool onAuth(String MQTTUSER, String MQTTPASS) {
		Serial.println("Username/Password: " + MQTTUSER + "/" + MQTTPASS);
		return true;
	}

	virtual void onData(String topic, const char* data, uint32_t length) {
		char data_chr[length + 1];
		os_memcpy(data_chr, data, length);
		data_chr[length] = '\0';
		String data_str = (String)data_chr;
		data_str.toUpperCase();

		RECEVAL = topic + " " + data_str;
		Serial.println("received: " + RECEVAL);
		RECSTOP = topic;
		strcpy(RECSVAL, data);
		//RECSVAL = data_str;
		Serial.print("val : ");
		Serial.println(RECSVAL);
		Serial.println(data);
	}
};
void MQTT_Server_cleanupClientCons();

myMQTTBroker myBroker;

WiFiClient espClient1;
PubSubClient Broker1(espClient1);
WiFiClient espClient2;
PubSubClient Broker2(espClient2);
WiFiClient espClient3;
PubSubClient Broker3(espClient3);
WiFiClient espClient4;
PubSubClient Broker4(espClient4);


// Setup MQTT
void loadMqtt_set() {
	char _buff[] = "";
	if (atoi(mq_pup_top1.s1) == 1 || atoi(mq_sub_top1.s1) == 1 || atoi(mq_sub_top2.s1) == 1 || atoi(mq_sub_top3.s1) == 1 || atoi(mq_sub_top4.s1) == 1) {
		mq_Server.mqtt_server1_on = 1;
	}
	else {
		mq_Server.mqtt_server1_on = 0;
	}
	if (atoi(mq_pup_top1.s2) == 1 || atoi(mq_sub_top1.s2) == 1 || atoi(mq_sub_top2.s2) == 1 || atoi(mq_sub_top3.s2) == 1 || atoi(mq_sub_top4.s2) == 1) {
		mq_Server.mqtt_server2_on = 1;
	}
	else {
		mq_Server.mqtt_server2_on = 0;
	}
	if (atoi(mq_pup_top1.s3) == 1 || atoi(mq_sub_top1.s3) == 1 || atoi(mq_sub_top2.s3) == 1 || atoi(mq_sub_top3.s3) == 1 || atoi(mq_sub_top4.s3) == 1) {
		mq_Server.mqtt_server3_on = 1;
	}
	else {
		mq_Server.mqtt_server3_on = 0;
	}
	if (atoi(mq_pup_top1.s4) == 1 || atoi(mq_sub_top1.s4) == 1 || atoi(mq_sub_top2.s4) == 1 || atoi(mq_sub_top3.s4) == 1 || atoi(mq_sub_top4.s4) == 1) {
		mq_Server.mqtt_server4_on = 1;
	}
	else {
		mq_Server.mqtt_server4_on = 0;
	}

	if (mq_Server.mqtt_server1_on == 1) {
		Serial.println("connect mqtt Server1...");
		Broker1.setServer(mq_Server.mqtt_server1, 1883);
		Broker1.setCallback(callback);
	};
	if (mq_Server.mqtt_server2_on == 1) {
		Serial.println("connect mqtt Server2...");
		Broker2.setServer(mq_Server.mqtt_server2, 1883);
		Broker2.setCallback(callback);
	}
	if (mq_Server.mqtt_server3_on == 1) {
		Serial.println("connect mqtt Server3...");
		Broker3.setServer(mq_Server.mqtt_server3, 1883);
		Broker3.setCallback(callback);
	}
	if (mq_Server.mqtt_server4_on == 1) {
		Serial.println("connect mqtt Server4...");
		Broker4.setServer(mq_Server.mqtt_server4, 1883);
		Broker4.setCallback(callback);
	}
	// Topic in abhängigkeit der Anzahl der dp zur veröfentlichung
	Serial.println("to topic 1");
	strcat(_buff, NetConfig.DeviceName);
	strcat(_buff, "/");
	strcat(_buff, mq_pup_top1.t1);
	strcpy(mq_pup_top1.b1, _buff);
	Serial.println(mq_pup_top1.b1);

	// Start the broker
	Serial.println("Starting uMQTT broker");
	myBroker.init();

	// Subscribe to anything ////////////////////////////////////////////////////
	myBroker.subscribe("#");

}
void mqtt_loop() {
	//uMQTTBroker callback 
	RECEIVE = RECEVAL;

	// connect über PubSubClient.h
	if (mq_Server.mqtt_server1_on == 1) {
		if (!Broker1.connected()) {
			reconnect1();
		};
		Broker1.loop();
	}
	if (mq_Server.mqtt_server2_on == 1) {
		if (!Broker2.connected()) {
			reconnect2();
		};
		Broker2.loop();
	}
	if (mq_Server.mqtt_server3_on == 1) {
		if (!Broker3.connected()) {
			reconnect3();
		};
		Broker3.loop();
	}
	if (mq_Server.mqtt_server4_on == 1) {
		if (!Broker4.connected()) {
			reconnect4();
		};
		Broker4.loop();
	}
}
// MQTT callback PubSubClient.h
void callback(char* topic, byte* payload, unsigned int length) {
	if (millis() > (letzteZeit_call + entprellWert)) {
		Serial.print("Message arrived [");
		Serial.print(topic);
		Serial.print("] ");
		String value;
		for (int i = 0; i < length; i++) {
			value += (char)payload[i];
		}
		char* _RECSVAL = new char[value.length() + 1];
		strcpy(_RECSVAL, value.c_str());
		strcpy(RECSVAL, _RECSVAL);
		Serial.println(length);
		Serial.print("RECS :");
		Serial.println(RECSVAL);
		schalten();
		letzteZeit_call = millis();
	}
}
void reconnect1() {
	// Loop until we're reconnected
	// Server 1
	if (!Broker1.connected()) {
		Serial.print("Attempting MQTT connection Server 1...");
		// Create a random client ID
		String clientId = NetConfig.DeviceName;
		clientId += "-C1";
		// Attempt to connect
		if (Broker1.connect(clientId.c_str())) {
			Serial.println("connected Server 1 " + clientId);
			// Once connected, publish an announcement...
			//client1.publish("outTopic", "hello world");
			//// ... and resubscribe
			if (atoi(mq_sub_top1.s1) == 1) {
				Broker1.subscribe(mq_sub_top1.t1);
			}
			if (atoi(mq_sub_top2.s1) == 1) {
				Broker1.subscribe(mq_sub_top2.t1);
			}
			if (atoi(mq_sub_top3.s1) == 1) {
				Broker1.subscribe(mq_sub_top3.t1);
			}
			if (atoi(mq_sub_top4.s1) == 1) {
				Broker1.subscribe(mq_sub_top4.t1);
			}
		}
		else {
			Serial.print("failed, rc=");
			Serial.print(Broker1.state());
			Serial.println(" try again in 5 seconds Server 1");
			// Wait 5 seconds before retrying
			//delay(5000);
		}
	}
}
void reconnect2() {
	// Loop until we're reconnected
	// Server 2
	if (!Broker2.connected()) {
		Serial.print("Attempting MQTT connection Server 2...");
		// Create a random client ID
		String clientId = NetConfig.DeviceName;
		clientId += "C2";
		// Attempt to connect
		if (Broker2.connect(clientId.c_str(), "admins", "admins")) {
			Serial.println("connected Server 2 " + clientId);
			if (atoi(mq_sub_top1.s2) == 1) {
				Broker1.subscribe(mq_sub_top1.t1);
			}
			if (atoi(mq_sub_top2.s2) == 1) {
				Broker1.subscribe(mq_sub_top2.t1);
			}
			if (atoi(mq_sub_top3.s2) == 1) {
				Broker1.subscribe(mq_sub_top3.t1);
			}
			if (atoi(mq_sub_top4.s2) == 1) {
				Broker1.subscribe(mq_sub_top4.t1);
			}
		}
		else {
			Serial.print("failed, rc=");
			Serial.print(Broker2.state());
			Serial.println(" try again in 5 seconds Server 2");
			// Wait 5 seconds before retrying
			//delay(5000);
		}
	}
}
void reconnect3() {
	// Loop until we're reconnected
	// Server 1
	if (!Broker3.connected()) {
		Serial.print("Attempting MQTT connection Server 3...");
		// Create a random client ID
		String clientId = NetConfig.DeviceName;
		clientId += "C3";
		// Attempt to connect
		if (Broker3.connect(clientId.c_str(), "admins", "admins")) {
			Serial.println("connected Server 3 " + clientId);
			if (atoi(mq_sub_top1.s3) == 1) {
				Broker1.subscribe(mq_sub_top1.t1);
			}
			if (atoi(mq_sub_top2.s3) == 1) {
				Broker1.subscribe(mq_sub_top2.t1);
			}
			if (atoi(mq_sub_top3.s3) == 1) {
				Broker1.subscribe(mq_sub_top3.t1);
			}
			if (atoi(mq_sub_top4.s3) == 1) {
				Broker1.subscribe(mq_sub_top4.t1);
			}
		}
		else {
			Serial.print("failed, rc=");
			Serial.print(Broker3.state());
			Serial.println(" try again in 5 seconds Server 3");
			// Wait 5 seconds before retrying
			//delay(5000);
		}
	}
}
void reconnect4() {
	// Loop until we're reconnected
	// Server 1
	if (!Broker4.connected()) {
		Serial.print("Attempting MQTT connection Server 4...");
		// Create a random client ID
		String clientId = NetConfig.DeviceName;
		clientId += "C4";
		// Attempt to connect
		if (Broker4.connect(clientId.c_str(), "admins", "admins")) {
			Serial.println("connected Server 4 " + clientId);
			if (atoi(mq_sub_top1.s4) == 1) {
				Broker1.subscribe(mq_sub_top1.t1);
			}
			if (atoi(mq_sub_top2.s4) == 1) {
				Broker1.subscribe(mq_sub_top2.t1);
			}
			if (atoi(mq_sub_top3.s4) == 1) {
				Broker1.subscribe(mq_sub_top3.t1);
			}
			if (atoi(mq_sub_top4.s4) == 1) {
				Broker1.subscribe(mq_sub_top4.t1);
			}
		}
		else {
			Serial.print("failed, rc=");
			Serial.print(Broker4.state());
			Serial.println(" try again in 5 seconds Server 4");
			// Wait 5 seconds before retrying
			//delay(5000);
		}
	}
}
void mqtt_public_top1(char* _puptop, char* _val) {
	if (Broker1.state() == 0 && mq_Server.mqtt_server1_on == 1) {
		Serial.println("pup S1");
		Broker1.publish(_puptop, _val);
	}
	if (Broker2.state() == 0 && atoi(mq_pup_top1.s2) == 1) {
		Serial.println("pup S2");
		Broker2.publish(_puptop, _val);
	}
	if (Broker3.state() == 0 && mq_Server.mqtt_server3_on == 1) {
		Serial.println("pup S3");
		Broker3.publish(_puptop, _val);
	}
	if (Broker4.state() == 0 && mq_Server.mqtt_server4_on == 1) {
		Serial.println("pup S4");
		Broker4.publish(_puptop, _val);
	}
}
void handleInterrupt_GPIO0() {

	if (millis() > (letzteZeit_isr + entprellWert)) {
		BSTAT1 = true;
		schalten();
		letzteZeit_isr = millis();
	}

}
//void schalten() {
//	RSTAT1 = digitalRead(GPIO12);
//	int _sch = 1;
//	// Ein
//	//if (strcmp(RECSVALalt, RECSVAL) != 0) { //TRIGG2 = false; 
//	if (RSTAT1 == false) {
//		//&& TRIGG2 == false
//		if (strcmp(RECSVAL, mq_pup_top1.v1) == 0) { _sch = 1; }
//		if (strcmp(RECSVAL, mq_sub_top1.v1) == 0) { _sch = 1; }
//		if (strcmp(RECSVAL, mq_sub_top2.v1) == 0) { _sch = 1; }
//		if (strcmp(RECSVAL, mq_sub_top3.v1) == 0) { _sch = 1; }
//		if (strcmp(RECSVAL, mq_sub_top4.v1) == 0) { _sch = 1; }
//		Serial.print("on ");
//		if (_sch == 1) { BSTAT1 = true; }
//		Serial.println(BSTAT1);
//		//strcpy(RECSVALalt, RECSVAL);
//		strcpy(RECSVALalt, "false");
//		//TRIGG2 = true;
//	}
//	// aus   
//	if (RSTAT1 == true) {
//		// && TRIGG2 == false
//		if (strcmp(RECSVAL, mq_pup_top1.v2) == 0) { _sch = 0; }
//		if (strcmp(RECSVAL, mq_sub_top1.v2) == 0) { _sch = 0; }
//		if (strcmp(RECSVAL, mq_sub_top2.v2) == 0) { _sch = 0; }
//		if (strcmp(RECSVAL, mq_sub_top3.v2) == 0) { _sch = 0; }
//		if (strcmp(RECSVAL, mq_sub_top4.v2) == 0) { _sch = 0; }
//		Serial.print("off ");
//		if (_sch == 0) { BSTAT1 = true; }
//		Serial.println(BSTAT1);
//		//strcpy(RECSVALalt, RECSVAL);
//		strcpy(RECSVALalt, "false");
//		//TRIGG3 = true;
//	}
//	//}
//
//	if (BSTAT1 == true && RSTAT1 == false && TRIGG1 == false) {
//		digitalWrite(GPIO12, 1);
//		digitalWrite(BUILTIN_LED, 0);
//		Serial.println("GPIO0 POWER ON");
//		myBroker.publish(mq_pup_top1.b1, "ON");
//		strcpy(mq_Server.msg, "ON");
//		mqtt_public_top1(mq_pup_top1.b1, mq_pup_top1.v1);
//		TRIGG1 = true;
//	}
//	if (BSTAT1 == true && RSTAT1 == true && TRIGG1 == false) {
//		digitalWrite(GPIO12, 0);
//		digitalWrite(BUILTIN_LED, 1);
//		Serial.println("GPIO0 POWER OFF");
//		myBroker.publish(mq_pup_top1.b1, "OFF");
//		strcpy(mq_Server.msg, "OFF");
//		mqtt_public_top1(mq_pup_top1.b1, mq_pup_top1.v2);
//		TRIGG1 = true;
//	}
//	if (BSTAT1 == true) {
//		TRIGG1 = false;
//		BSTAT1 = false;
//	};
//}
