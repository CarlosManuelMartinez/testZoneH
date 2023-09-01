//Carlos Manuel Martinez Pomares.

#include <Zone.h>
#include <DHT.h>

//Zone(String name, int pinOpen, int pinClosed, int pinSensor)
int temperatureTargetInit = 25;
//bool createArrays = false;
bool mode = true;//Mode summer
//bool mode = false;//Mode winter

int timeMotor = 1200;
int timeAction = 2000;

DHT sensor1(4,DHT11);
DHT sensor2(7,DHT11);
DHT sensor3(10,DHT11);
DHT sensor4(13,DHT11);

Zone zone1("Principal bedroom", 2, 3, 22);
Zone zone2("Children bedroom", 5, 6, 25);
Zone zone3("Study", 8, 9, 25);
Zone zone4("Living room", 11, 12, 25);


boolean print = true;

void printInfo(Zone zone, DHT sensor) {

  String name = zone.getName();
  float setTemperature = zone.getTemperature();
  bool state = zone.getStateDamp();
  float temperature = sensor.readTemperature();
  float humidity = sensor.readHumidity();
  float heatIndex = sensor.computeHeatIndex();
  
  Serial.println("----------------------------");
  Serial.print("Zone: ");
  Serial.println(name);

  Serial.print("Set temperature: ");
  Serial.print(setTemperature);
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

bool stopDamp(Zone zone){
  int pinOpen = zone.getPinOpen();
  int pinClosed = zone.getPinClosed();
  pinMode(pinOpen, LOW);
  pinMode(pinClosed, LOW);
  delay(timeAction);
}

bool moveDamp(Zone zone, DHT sensor, bool action){

  int pinOpen = zone.getPinOpen();
  int pinClosed = zone.getPinClosed();
  bool dampState = zone.getStateDamp();
  float temperatureTarget = zone.getTemperature();
  float temperature = sensor.readTemperature(); 

  if(action == true && dampState == false)
  { //Open damp and change state
    stopDamp(zone);
    delay(timeAction);
    pinMode(pinOpen, HIGH);
    delay(timeMotor);
    pinMode(pinOpen, LOW);
    delay(timeAction);
    zone.setStateDamp(true);
    Serial.print("ABRO");
  }else if (action == false && dampState == true){ //Closed damp and change state
    stopDamp(zone);
    pinMode(pinClosed, HIGH);
    delay(timeMotor);
    pinMode(pinClosed, LOW);
    delay(timeAction);
    zone.setStateDamp(false);
    Serial.print("CIERRO");
  }else{
    Serial.print("NO SETEO EL ESTADO");
  } 

  return dampState;
}

void climateZone(bool mode, Zone zone, DHT sensor){

  float temperature = sensor.readTemperature();
  float temperatureTarget = zone.getTemperature();

  if(mode == true && temperature > temperatureTarget){//Summer on demand
    moveDamp(zone, sensor, true);
  }else if(mode == false && temperature < temperatureTarget){//Winter on demand
    moveDamp(zone, sensor, true);
  }else if(mode == true && temperature <= temperatureTarget){//Summer out of demand
    moveDamp(zone, sensor, false);
  }else if(mode == false && temperature <= temperatureTarget){//Winter out of demand
    moveDamp(zone, sensor, false);
  }else{
    Serial.print("COSA RARA");
  }
}
void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  sensor1.begin();
  delay(2000);
  sensor2.begin();
  delay(2000);
  sensor3.begin();
  delay(2000);
  sensor4.begin();
  delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  /*delay(6000);
  printInfo(zone1,sensor1);
  delay(6000);
  printInfo(zone2,sensor2);
  delay(6000);
  printInfo(zone3, sensor3);
  delay(6000);
  printInfo(zone4, sensor4);*/

  Zone zones[4] = {zone1, zone2, zone3, zone4};
  DHT sensors[4] = {sensor1, sensor2, sensor3, sensor4};

  

  /*for(int i = 0 ; i < 4 ; i++){
    DHT s =sensors[i];
    Zone z = zones[i];
    printInfo(z, s);
    delay(6000);
  }*/

  for(int i = 0 ; i < 4 ; i++){
    DHT s = sensors[i];
    Zone z = zones[i];
    climateZone(mode, z, s);
    delay(6000);
    printInfo(z, s);
  }

  
}
