
void handleInterrupt_GPIO0() {

	if (millis() > (letzteZeit_isr + entprellWert)) {
		interruptCounter++;
		BSTAT1 = true;
		schalten();
		letzteZeit_isr = millis();
	}

}
void schalten() {
	RSTAT1 = digitalRead(GPIO12);
	int _sch = 1;
	// Ein
	//if (strcmp(RECSVALalt, RECSVAL) != 0) { //TRIGG2 = false; 
	if (RSTAT1 == false) {
		//&& TRIGG2 == false
		if (strcmp(RECSVAL, mq_pup_top1.v1) == 0) { _sch = 1; }
		if (strcmp(RECSVAL, mq_sub_top1.v1) == 0) { _sch = 1; }
		if (strcmp(RECSVAL, mq_sub_top2.v1) == 0) { _sch = 1; }
		if (strcmp(RECSVAL, mq_sub_top3.v1) == 0) { _sch = 1; }
		if (strcmp(RECSVAL, mq_sub_top4.v1) == 0) { _sch = 1; }
		Serial.print("on ");
		if (_sch == 1) { BSTAT1 = true; }
		Serial.println(BSTAT1);
		//strcpy(RECSVALalt, RECSVAL);
		strcpy(RECSVALalt, "false");
		//TRIGG2 = true;
	}
	// aus   
	if (RSTAT1 == true) {
		// && TRIGG2 == false
		if (strcmp(RECSVAL, mq_pup_top1.v2) == 0) { _sch = 0; }
		if (strcmp(RECSVAL, mq_sub_top1.v2) == 0) { _sch = 0; }
		if (strcmp(RECSVAL, mq_sub_top2.v2) == 0) { _sch = 0; }
		if (strcmp(RECSVAL, mq_sub_top3.v2) == 0) { _sch = 0; }
		if (strcmp(RECSVAL, mq_sub_top4.v2) == 0) { _sch = 0; }
		Serial.print("off ");
		if (_sch == 0) { BSTAT1 = true; }
		Serial.println(BSTAT1);
		//strcpy(RECSVALalt, RECSVAL);
		strcpy(RECSVALalt, "false");
		//TRIGG3 = true;
	}
	//}

	if (BSTAT1 == true && RSTAT1 == false && TRIGG1 == false) {
		digitalWrite(GPIO12, 1);
		digitalWrite(BUILTIN_LED, 0);
		Serial.println("GPIO0 POWER ON");
		myBroker.publish(mq_pup_top1.b1, "ON");
		strcpy(mq_Server.msg, "ON");
		mqtt_public_top1(mq_pup_top1.b1, mq_pup_top1.v1);
		TRIGG1 = true;
	}
	if (BSTAT1 == true && RSTAT1 == true && TRIGG1 == false) {
		digitalWrite(GPIO12, 0);
		digitalWrite(BUILTIN_LED, 1);
		Serial.println("GPIO0 POWER OFF");
		myBroker.publish(mq_pup_top1.b1, "OFF");
		strcpy(mq_Server.msg, "OFF");
		mqtt_public_top1(mq_pup_top1.b1, mq_pup_top1.v2);
		TRIGG1 = true;
	}
	if (BSTAT1 == true) {
		TRIGG1 = false;
		BSTAT1 = false;
	};
}