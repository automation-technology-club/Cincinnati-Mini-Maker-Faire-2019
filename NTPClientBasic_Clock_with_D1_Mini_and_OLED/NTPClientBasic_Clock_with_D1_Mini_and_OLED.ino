/*
Copyright 2018 German Martin (gmag11@gmail.com). All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are
permitted provided that the following conditions are met :

1. Redistributions of source code must retain the above copyright notice, this list of
conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list
of conditions and the following disclaimer in the documentation and / or other materials
provided with the distribution.

THIS SOFTWARE IS PROVIDED BY <COPYRIGHT HOLDER> ''AS IS'' AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.IN NO EVENT SHALL <COPYRIGHT HOLDER> OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
	SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
	ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT(INCLUDING
		NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
	ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

	The views and conclusions contained in the software and documentation are those of the
	authors and should not be interpreted as representing official policies, either expressed
	or implied, of German Martin
*/

/*
 Name:		NtpClient.ino
 Created:	22/07/2018
 Author:	gmag11@gmail.com
 Editor:	http://www.visualmicro.com
*/

#include <TimeLib.h> //TimeLib library is needed https://github.com/PaulStoffregen/Time
#include <NtpClientLib.h> //Include NtpClient library header
#include <ESP8266WiFi.h>
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h"
//#include "SH1106Wire.h"
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>   

//SSD1306Wire  display(0x3c, D2, D1);
SSD1306Wire  display(0x3c, D1, D2);
//SH1106Wire display(0x3c, D1, D2);

int8_t timeZone = -4;
int8_t minutesTimeZone = 0;

void setup () {
	Serial.begin (9600);
	Serial.println ();
WiFiManager wifiManager;
wifiManager.autoConnect("AutoConnectAP");
	// NTP begin with default parameters:
	//   NTP server: pool.ntp.org
	//   TimeZone: UTC
	//   Daylight saving: off
	NTP.begin ("pool.ntp.org", timeZone, true, minutesTimeZone);
// Initialising the UI will init the display too.
  display.init();

  display.flipScreenVertically();
  display.invertDisplay();
  display.setFont(ArialMT_Plain_10);
}

void loop () {
	//To keep time updated you need to call now() from time to time inside loop
	//in this case getTimeDateString() implies a call to now()
	display.clear();
	
	Serial.println (NTP.getTimeDateString ());
 // display.setTextAlignment(TEXT_ALIGN_LEFT);
 display.setFont(ArialMT_Plain_16); 
 display.drawString(32,15,NTP.getTimeStr());
 display.setFont(ArialMT_Plain_10);
 display.drawString(37,45,NTP.getDateStr());
//	display.drawString(10,15,NTP.getTimeDateString());
 display.display();
	delay (1000);
}
