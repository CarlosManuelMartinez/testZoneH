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
Zone zone2("Children bedroom", 5, 6, 28);
Zone zone3("Study", 8, 9, 25);
Zone zone4("Living room", 11, 12, 28);

Zone zones[4] = {zone1, zone2, zone3, zone4};
DHT sensors[4] = {sensor1, sensor2, sensor3, sensor4};


boolean print = true;

void printInfo(Zone zone, DHT sensor) {

  String name = zone.getName();
  float setTemperature = zone.getTemperature();
  bool state = zone.getStateDamp();
  float temperature = sensor.readTemperature();
  float humidity = sensor.readHumidity();
  float heatIndex = sensor.computeHeatIndex();
  
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

  //Serial.println("----------------------------");
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
    dampState = true;
    Serial.println("------------------");
    Serial.println("ABRO");
  }else if (action == false && dampState == true){ //Closed damp and change state
    stopDamp(zone);
    pinMode(pinClosed, HIGH);
    delay(timeMotor);
    pinMode(pinClosed, LOW);
    delay(timeAction);
    dampState = false;
    Serial.println("------------------");
    Serial.println("CIERRO");
  }else{
  
    Serial.println("------------------");
    Serial.println("NI CIERRO NI ABRO");
  } 

  return dampState;
}

bool climateZone(bool mode, Zone zone, DHT sensor){

  bool stateDamp = zone.getStateDamp();
  float temperature = sensor.readTemperature();
  int t = round(temperature);
  float temperatureTarget = zone.getTemperature();
  bool onDemand;
  
  if(mode == true && t > temperatureTarget ){//Summer on demand
    stateDamp =  moveDamp(zone, sensor, true);
    Serial.print("Sumer on demand: ");
    Serial.println(stateDamp);
  }else if(mode == true && t <= temperatureTarget){//Summer out of demand
    stateDamp = moveDamp(zone, sensor, false);
    Serial.print("Summer out of demand: ");
    Serial.println(stateDamp);
  }else if(mode == false && t < temperatureTarget){//Winter on demand
    stateDamp = moveDamp(zone, sensor, true);
    Serial.print("Winter on demand: ");
    Serial.println(stateDamp);
  }else if(mode == false && t >= temperatureTarget  ){//Winter out of demand
    stateDamp = moveDamp(zone, sensor, false);
    Serial.print("Winter out of demand: ");
    Serial.println(stateDamp);
  }else{
    String n = zone.getName();
    Serial.print(n);
    Serial.println(" hace cosas raras");
  }
  return stateDamp;
}



void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:

  for (int i = 0 ; i < 4 ; i++){
    sensors[i].begin();
    delay(2000);
  }
 
}

void loop() {
  // put your main code here, to run repeatedly:

  //Zone zones[4] = {zone1, zone2, zone3, zone4};
  //DHT sensors[4] = {sensor1, sensor2, sensor3, sensor4};

  for(int i = 0 ; i < 4 ; i++){
    bool stateDamp;
    stateDamp = climateZone(mode, zones[i], sensors[i]);
    delay(6000);
    zones[i].setStateDamp(stateDamp);
    printInfo(zones[i], sensors[i]);
  }
  
}
