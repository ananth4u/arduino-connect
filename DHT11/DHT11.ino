#include <MQ135.h>
#include <DHT.h>
#include <DHT_U.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#define DHTPIN 2
#define DHTTYPE DHT11
#define ANALOGPIN A0
MQ135 gasSensor = MQ135(ANALOGPIN);
DHT dht(DHTPIN, DHTTYPE);
char myssid[] = "";         // your network SSID (name)
char mypass[] = "";          // your network password
String jsonString = "{\n";
String humidity,temp,rzero1,ppm1;
String hu,te,rzero2,ppm2;

int status = WL_IDLE_STATUS;

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));
  dht.begin();
  Serial.println("Start");
  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  Serial.println("Setup done");
  // We start by connecting to a WiFi network
  Serial.print("Connecting to ");
  Serial.println(myssid);
  WiFi.begin(myssid, mypass);
  Serial.println(WiFi.localIP());

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("Not connected");
  }
  Serial.println(".");
}

void loop() {
  delay(2000);
     float h = dht.readHumidity();
     humidity = String(h);
     hu = "\""+humidity+"\"";
    float t = dht.readTemperature();
     temp = String(t);
    te = "\""+temp+"\"";
    float f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));
  Serial.println("##############################################################");
  float rzero = gasSensor.getRZero(); //this to get the rzero value, uncomment this to get ppm value
  float ppm = gasSensor.getPPM(); // this to get ppm value, uncomment this to get rzero value
  rzero1 = String(rzero);
  rzero2 = "\""+rzero1+"\"";
  ppm1 = String(ppm);
  ppm2 = "\""+ppm1+"\"";
  Serial.println(rzero); // this to display the rzero value continuously, uncomment this to get ppm value
  Serial.println(ppm); // this to display the ppm value continuously, uncomment this to get rzero value
     jsonString = "{\n";
     jsonString += "\"temp\": "+te+",\n"; // this is a real UK MCC
     jsonString += "\"humidity\": "+hu+",\n"; // this is a real UK MCC
     jsonString += "\"rzero\": "+rzero2+",\n"; // this is a real UK MCC
     jsonString += "\"ppm\": "+ppm2+"\n"; // this is a real UK MCC
     jsonString += "}\n";
     Serial.println("");
     HTTPClient http;
     http.begin("http://{ipaddress}:{port}/sensor/data");
     http.addHeader("Content-Type", "application/json");
     http.PUT(jsonString);
     http.writeToStream(&Serial);
     http.end();
     delay(10000);
}
