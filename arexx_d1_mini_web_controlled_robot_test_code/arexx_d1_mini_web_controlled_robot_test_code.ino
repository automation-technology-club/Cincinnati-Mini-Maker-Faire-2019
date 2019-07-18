/*********
  Rui Santos
  Complete project details at http://randomnerdtutorials.com  
*********/

// Load Wi-Fi library
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <LOLIN_I2C_MOTOR.h>

LOLIN_I2C_MOTOR motor; //I2C address 0x30

// Replace with your network credentials
const char* ssid     = "ESPRobotAP";
const char* password = "123456789";

uint8_t dutyCycle = 25;

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String FWDState = "off";
String BCWState = "off";
String LFTState = "off";
String RGTState = "off";

void setup() {
  Serial.begin(9600);
  while (motor.PRODUCT_ID != PRODUCT_ID_I2C_MOTOR) //wait motor shield ready.
  {
    motor.getInfo();
  }
  motor.changeFreq(MOTOR_CH_BOTH, 1000); //Change A & B 's Frequency to 1000Hz.

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  //WiFi.begin(ssid, password);
  WiFi.softAP(ssid, password);
  /*while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }*/
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  //Serial.println(WiFi.localIP());
  Serial.println(WiFi.softAPIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            //Forward
            if (header.indexOf("GET /5/on") >= 0) {
              //Serial.println("GPIO 5 on");
              FWDState = "on";
              BCWState = "off";
              LFTState = "off";
              RGTState = "off";
              forward();
              } else if (header.indexOf("GET /5/off") >= 0) {
              //Serial.println("GPIO 5 off");
              FWDState = "off";
              BCWState = "off";
              LFTState = "off";
              RGTState = "off";
              stop();
            } 
            //Backward
            else if (header.indexOf("GET /4/on") >= 0) {
              //Serial.println("GPIO 4 on");
              FWDState = "off";
              BCWState = "on";
              LFTState = "off";
              RGTState = "off";
              backward();
            } else if (header.indexOf("GET /4/off") >= 0) {
              //Serial.println("GPIO 4 off");
              FWDState = "off";
              BCWState = "off";
              LFTState = "off";
              RGTState = "off";
              stop();
            }
            //Left
            else if (header.indexOf("GET /6/on") >= 0) {
              //Serial.println("GPIO 4 on");
              FWDState = "off";
              BCWState = "off";
              LFTState = "on";
              RGTState = "off";
              left();
            } else if (header.indexOf("GET /6/off") >= 0) {
              //Serial.println("GPIO 4 off");
              FWDState = "off";
              BCWState = "off";
              LFTState = "off";
              RGTState = "off";
              stop();
            }
            //Right
            else if (header.indexOf("GET /7/on") >= 0) {
              //Serial.println("GPIO 4 on");
              FWDState = "off";
              BCWState = "off";
              LFTState = "off";
              RGTState = "on";
              right();
            } else if (header.indexOf("GET /7/off") >= 0) {
              //Serial.println("GPIO 4 off");
              FWDState = "off";
              BCWState = "off";
              LFTState = "off";
              RGTState = "off";
              stop();
            }
            else if (header.indexOf("GET /8/stop") >= 0) {
              //Serial.println("GPIO 4 off");
              FWDState = "off";
              BCWState = "off";
              LFTState = "off";
              RGTState = "off";
              stop();
            }
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>Robot Demo</h1>");
            
            // Display current state, and ON/OFF buttons for Forward 
            client.println("<p>Forward " + FWDState + "</p>");
            // If the FWDState is off, it displays the ON button       
            if (FWDState=="off") {
              client.println("<a href=\"/5/on\"><button class=\"button\">ON</button></a>");
            } else {
              client.println("<a href=\"/5/off\"><button class=\"button button2\">OFF</button></a>");
            } 
               
            // Display current state, and ON/OFF buttons for Backward  
            client.println("<p>Backward " + BCWState + "</p>");
            // If the BCWState is off, it displays the ON button       
            if (BCWState=="off") {
              client.println("<a href=\"/4/on\"><button class=\"button\">ON</button></a>");
            } else {
              client.println("<a href=\"/4/off\"><button class=\"button button2\">OFF</button></a>");
            }
            // Display current state, and ON/OFF buttons for Left 
            client.println("<p>Left " + LFTState + "</p>");
            // If the BCWState is off, it displays the ON button       
            if (LFTState=="off") {
              client.println("<a href=\"/6/on\"><button class=\"button\">ON</button></a>");
            } else {
              client.println("<a href=\"/6/off\"><button class=\"button button2\">OFF</button></a>");
            }

            // Display current state, and ON/OFF buttons for right  
            client.println("<p>Right " + RGTState + "</p>");
            // If the BCWState is off, it displays the ON button       
            if (RGTState=="off") {
              client.println("<a href=\"/7/on\"><button class=\"button\">ON</button></a>");
            } else {
              client.println("<a href=\"/7/off\"><button class=\"button button2\">OFF</button></a>");
            }

            /* client.println("<p>Stop </p>"); */ client.println("<p><a href=\"/8/stop\"><button class=\"button\">Stop</button></a></p>");
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}

void forward() {
  stop();
  motor.changeDuty(MOTOR_CH_BOTH, dutyCycle);
  motor.changeStatus(MOTOR_CH_A, MOTOR_STATUS_CCW);
motor.changeStatus(MOTOR_CH_B, MOTOR_STATUS_CW);
}

void backward() {
  stop();
  motor.changeDuty(MOTOR_CH_BOTH, dutyCycle);
  motor.changeStatus(MOTOR_CH_A, MOTOR_STATUS_CW);
motor.changeStatus(MOTOR_CH_B, MOTOR_STATUS_CCW);
}

void stop() {
  motor.changeStatus(MOTOR_CH_BOTH, MOTOR_STATUS_STOP);
}

void left() {
  stop();
  motor.changeDuty(MOTOR_CH_BOTH, dutyCycle);
  motor.changeStatus(MOTOR_CH_B, MOTOR_STATUS_CW);
}

void right() {
  stop();
  motor.changeDuty(MOTOR_CH_BOTH, dutyCycle);
  motor.changeStatus(MOTOR_CH_A, MOTOR_STATUS_CCW);
}
