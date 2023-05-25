#include <WiFi.h>
#include <HTTPClient.h>

#define ONBOARD_LED 2

// digital pins for soaker relay control
#define SOAKER_RELAY_1  12 
#define SOAKER_RELAY_2  14 

// name of sensor that is controlling relay state
#define SENSOR_1 "beds_1_&_2"
#define SENSOR_2 "beds_3_&_4"

// wifi credentials
#define STASSID ""
#define STAPSK  ""

// TODO: don't hardcode sensor name
// url for opengrower-sensor-server
#define HOST "http://192.168.1.100:8080/irrigation/beds_1_&_2"

#define LOOP_DELAY 120e3

const char* ssid = STASSID;
const char* password = STAPSK;
const char* host = HOST;

void setup() {
  // Set ONBOARD_LED to output mode
  pinMode(ONBOARD_LED, OUTPUT);

  // Set ONBOARD LED to HIGH on setup
  digitalWrite(ONBOARD_LED, HIGH);

  // Set SOAKER_RELAY_X to output mode
  pinMode(SOAKER_RELAY_1, OUTPUT);
  pinMode(SOAKER_RELAY_2, OUTPUT);

  // Set SOAKER_RELAY_X to low on startup
  digitalWrite(SOAKER_RELAY_1, LOW);
  digitalWrite(SOAKER_RELAY_2, LOW);

  // Connect to network
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  
  // Set ONBOARD LED to low on setup complete
  digitalWrite(ONBOARD_LED, LOW);
}

void loop() {
  if ((WiFi.status() == WL_CONNECTED)) {
    HTTPClient http;
    http.begin(host);
    int httpCode = http.GET();
    String response = http.getString();
    if(httpCode > 0){
      int state = response.toInt();
      if(state == 1 || state == 0){
        digitalWrite(SOAKER_RELAY_1, state);
      } else {
         // state == -1  ERROR
         // state ==  2  HOLD_ON
         // state ==  3  HOLD_OFF
      }
    }
    http.end();  
  }

  delay(LOOP_DELAY);
}
