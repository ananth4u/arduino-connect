#include <TinyGPS++.h>
#include <SoftwareSerial.h>


static const int RXPin = 12, TXPin = 13;
static const uint32_t GPSBaud = 9600;

String latitude,longitude;
String lati,longi;

// The TinyGPS++ object
TinyGPSPlus gps;



// The serial connection to the GPS device
SoftwareSerial ss(RXPin,TXPin);

void setup(){
  ss.begin(GPSBaud);
  Serial.println("Start");
}

void loop(){
  while (ss.available() > 0){
    gps.encode(ss.read());
    if (gps.location.isUpdated()){
     latitude = String(gps.location.lat(),6);
     longitude = String(gps.location.lng(),6);
     Serial.println("lat:");
     Serial.println(latitude);
     Serial.println("long:");
     Serial.println(longitude);
     delay(10000);
     }
   }
}
