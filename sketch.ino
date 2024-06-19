#include <DallasTemperature.h>
#include <OneWire.h>

struct details{
      int Temperature_of_hall;
      char Chiller_power_status[3];
      uint16_t No_of_people_in_hall=0;
};

const int buzzerIN = 12;
const int buzzerOUT = 13;
const int PIRin = 2;  
const int PIRout = 3;            
            
const int relay = 7;
int flag=0;

int count=60;
const int oneWireBus = 4;
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);
struct details info;

void setup() {
  // put your setup code here, to run once:
    
    Serial.begin(9600);
    sensors.begin();
    pinMode(buzzerIN, OUTPUT);
    pinMode(buzzerOUT, OUTPUT);
    pinMode(PIRin, INPUT);
    pinMode(PIRout, INPUT);
    pinMode(relay, OUTPUT);
    attachInterrupt(digitalPinToInterrupt(PIRin),pirInInt,RISING);
    attachInterrupt(digitalPinToInterrupt(PIRout),pirOutInt,RISING);
    
}

void loop() {
  // put your main code here, to run repeatedly:

      if(count==60)
      {
        sensors.requestTemperatures();
        info.Temperature_of_hall = sensors.getTempCByIndex(0);
        if(info.Temperature_of_hall>25){
        digitalWrite(relay, HIGH);
        strcpy(info.Chiller_power_status, "ON");
        Serial.println("chiller is ON");

      }
      else{
        digitalWrite(relay, LOW);
        strcpy(info.Chiller_power_status, "OFF");
        Serial.println("chiller is OFF");
      }
        count=0;
        
      } 
     
      if(info.No_of_people_in_hall>10 & flag==0){
        tone(buzzerOUT, 3000);
        flag=1;
        Serial.println("Entry Gate Closed");
        delay(1000);
        noTone(buzzerOUT);
      }

      if(info.No_of_people_in_hall<9 & flag==1){
          tone(buzzerIN, 1000);
          flag=0;
          Serial.println("Entry Gate Reopen");
          delay(1000);
          noTone(buzzerIN);
      }

      Serial.print("Temparature of the Hall is  ");
      Serial.println(info.Temperature_of_hall);
      Serial.print("Status of Chiller "); 
      Serial.println(info.Chiller_power_status);
      Serial.print("Number Of People in the Hall is ");
      Serial.println(info.No_of_people_in_hall); 
      count++;
      delay(1000);


}

void pirInInt(){
 
    info.No_of_people_in_hall++;  
}

void pirOutInt(){

    info.No_of_people_in_hall--;
}