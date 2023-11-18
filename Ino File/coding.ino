#define BLYNK_TEMPLATE_ID "Template id"
#define BLYNK_TEMPLATE_NAME "Air Monitoring System"
#define BLYNK_AUTH_TOKEN "Auth token"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h> 
#include <BlynkSimpleEsp8266.h>
#include "DHT.h"
              
char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "WiFi SSID";  // type your wifi name
char pass[] = "WiFi Password";  // type your wifi password

BlynkTimer timer;

int GasSensor = A0;
#define DHTPIN 2
#define DHTTYPE DHT11
#define GreenLED 14
#define RedLED 12
#define Buzzer 13

DHT dht(DHTPIN, DHTTYPE);

int sensorThreshold = 100;
 


void sendSensor()
{
   int gasValue = analogRead(GasSensor);
  Blynk.virtualWrite(V0, gasValue);
  Serial.print("Gas Value: ");
    Serial.println(gasValue);
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  
    if(gasValue<100)
    {
      digitalWrite(GreenLED, HIGH);
      digitalWrite(RedLED, LOW);
      digitalWrite(Buzzer, LOW);
  }
  else if(gasValue>100)
  {
    digitalWrite(GreenLED, LOW);
    digitalWrite(RedLED, HIGH);
    digitalWrite(Buzzer, HIGH);
    Blynk.logEvent("gas_alert","Gas is High");
   
  }

  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

     if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
   }
  Blynk.virtualWrite(V1, t);
  Blynk.virtualWrite(V2, h);
  Blynk.virtualWrite(V3, hic);



  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));

}
void setup()
{   
  
  Serial.begin(115200);
  dht.begin();
  pinMode(GasSensor, INPUT);
  pinMode(GreenLED, OUTPUT);
  pinMode(RedLED, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  Blynk.begin(auth, ssid, pass);
   
  timer.setInterval(1000L, sendSensor);

  }

void loop()
{
  Blynk.run();
  timer.run();
 
}
