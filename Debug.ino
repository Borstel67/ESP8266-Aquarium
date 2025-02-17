// Serial oder syslog oder telnet
void DEBUG(String DebugMessage) {
  DEBUG(DebugMessage, "DummyTag", _slInformational);
}
void DEBUG(String DebugMessage, String Tag, byte Severity) {
  //debug SYSLOG
#ifdef UDPDEBUG
  if (UDPReady) {
    UDPClient.UDP.beginPacket(SYSLOGIP, SYSLOGPORT);
    byte severity = 8 + Severity;
    UDPClient.UDP.write(String("<" + String(severity) + ">" + String(NetConfig.DeviceName) + " " + Tag + " " + DebugMessage).c_str());
    UDPClient.UDP.endPacket();
  }
#endif
  // debug serial
  #ifdef SERIALDEBUG
  Serial.println(DebugMessage);
  #endif
  // debug serial
#ifdef TELNET
  TM(DebugMessage);
#endif
}

// Telnet
void TM(String text) {
  for (int i = 0; i < MAX_TELNET_CLIENTS; i++)
  {
    if (TelnetClient[i] || TelnetClient[i].connected())
    {
      TelnetClient[i].println(text);
    }
  }
  delay(10);  // to avoid strange characters left in buffer
}

void Telnet() {
  // Cleanup disconnected session
  uint8_t i;
  bool ConnectionEstablished; // Flag for successfully handled connection

  for (i = 0; i < MAX_TELNET_CLIENTS; i++) {
    if (TelnetClient[i] && !TelnetClient[i].connected()) {
      //Serial.print("Client disconnected ... terminate session "); //Serial.println(i + 1);
      TelnetClient[i].stop();
    }
  }

  // Check new client connections
  if (TelnetServer.hasClient()) {
    ConnectionEstablished = false; // Set to false
    for (i = 0; i < MAX_TELNET_CLIENTS; i++) {
      // Serial.print("Checking telnet session "); //Serial.println(i+1);

      // find free socket
      if (!TelnetClient[i]) {
        TelnetClient[i] = TelnetServer.available();

        //Serial.print("New Telnet client connected to session "); //Serial.println(i + 1);

        TelnetClient[i].flush();  // clear input buffer, else you get strange characters
        TelnetClient[i].println("Welcome!");

        TelnetClient[i].print("Millis since start: ");
        TelnetClient[i].println(millis());

        TelnetClient[i].print("Free Heap RAM: ");
        TelnetClient[i].println(ESP.getFreeHeap());

        TelnetClient[i].println("----------------------------------------------------------------");

        ConnectionEstablished = true;

        break;
      } else {
        // //Serial.println("Session is in use");
      }
    }
    if (ConnectionEstablished == false) {
      //Serial.println("No free sessions ... drop connection");
      TelnetServer.available().stop();
      // TM("An other user cannot connect ... MAX_TELNET_CLIENTS limit is reached!");
    }
  }

  for (i = 0; i < MAX_TELNET_CLIENTS; i++) {
    if (TelnetClient[i] && TelnetClient[i].connected()) {
      if (TelnetClient[i].available()) {
        //get data from the telnet client
        while (TelnetClient[i].available()) {
          Serial.write(TelnetClient[i].read());
        }
      }
    }
  }
}
