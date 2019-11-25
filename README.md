# esp8266-Aquarium

Beschreibung Projekt

Mit diesem Projekt habe ich meine Aquarien Beleuchtung auf LED Umgestellt.

Die Aquarien Beleuchtungssteuerung basiert auf einem ESP8266 und einem WS2812b Led Strip.
Der ESP übernimmt dabei die Ansteuerung des Strips und beinhaltet den Webserver mit der Bedienoberfläche zur Anzeige der Zustände
und Konfiguration der Beleuchtung.

Funktionen:
-	WLAN
1.	Config Mode
2.	AP Mode
3.	STA Mode

-	Uhr
1.	Synchronisation mit NTP Server
2.	Zeitzonen Einstellung Uhr
3.	Manuelle Zeiteinstellung
4.	Sommer Winterzeit Umschaltung Uhr

-	Sonne und Mond
1.	Sonnenaufgangs und Sonnenuntergangs Berechnung
2.	Mondphasen Berechnung zur Ansteuerung eines Teiles des Strip für die Mondbeleuchtung

-	Steuerung
1.	Für jede 4te LED (4 Kanäle) kann eine andere Farbe für Tag und Nacht vorgewählt werden 
2.	Jeder Kanal hat zwei Schulzeiten 
3.	Die maximale Anzahl der des LED Strip kann eingestellt werden
4.	Für die Mondbeleuchtung können beliebige LED ausgewählt werden

-	Eingebundenen Protokolle
1.	Syslog
2.	FTP
3.	telnet

-	Konfiguration und Bedienung
1.	Die Konfiguration erfolgt über den Integrierten WEB- Server
2.	Die Konfigurationsdaten werden in einen JSON File auf dem ESP abgelegt.

Offen Punkte 
-	UDP Kopplung mehrerer ESP zur Ansteuerung weiteren Geräten wie CO2 Generator
-	HomeMatic Kopplung
-	MQTT Einbindung 
