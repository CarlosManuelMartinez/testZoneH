#include <Zone.h>
#include <DHT.h>

//Zone(String name, int pinOpen, int pinClosed, int pinSensor)

DHT sensor1(4,DHT11);
DHT sensor2(7,DHT11);
DHT sensor3(10,DHT11);
DHT sensor4(13,DHT11);

DHT sensors[4] = {sensor1, sensor2, sensor3, sensor4};

Zone zone1("Principal bedroom", 2, 3);
Zone zone2("Children bedroom", 5, 6);
Zone zone3("Study", 8, 9);
Zone zone4("Living room", 11, 12);

Zone zones[4] = {zone1, zone2, zone3, zone4};

boolean print = true;

void printInfo(Zone zone, DHT sensor) {

  String name = zone.getName();
  //float setTemperature = zone.getTemperature();
  bool state = zone.getStateDamp();
  float temperature = sensor.readTemperature();
  float humidity = sensor.readHumidity();
  float heatIndex = sensor.computeHeatIndex();
  
  Serial.println("----------------------------");
  Serial.print("Zone: ");
  Serial.println(name);

  Serial.print("Set temperature: ");
  //Serial.print(setTemperature);
  Serial.println("ºC");

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println("ºC");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  Serial.print("Heat Index: ");
  Serial.print(heatIndex);
  Serial.println("ºC");

  if (state == true) {
    Serial.println("Damp OPEN");
  } else if (state == false) {
    Serial.println("Damp CLOSED");
  } else {
    Serial.println("Damp ERROR");
  }

  Serial.println("----------------------------");
}

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
 
    
    delay(6000);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  
  delay(6000);//
    printInfo(zone1,sensor1);
    delay(6000);
     printInfo(zone2,sensor2);
    delay(6000);
  printInfo(zone3, sensor3);
   delay(6000);
  printInfo(zone4, sensor4);
}
