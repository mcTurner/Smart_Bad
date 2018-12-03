
#include "T_Objects.h"
#include "Arduino.h"

Heizobjekt::Heizobjekt(int hp, int default_tempsoll)
{
  HW_PIN = hp;
  Temp_SOLL = default_tempsoll;
  Serial.println("Hardwarepin");
  Serial.println(HW_PIN);
}


void Heizobjekt::power_on(){
  
  digitalWrite(HW_PIN,LOW);
  Serial.println("Heizobjekt power ON");
  }


void Heizobjekt::power_off(){
  
  digitalWrite(HW_PIN,HIGH);
   Serial.println(HW_PIN);
  Serial.println("Heizobjekt power OFF");
  }

void Heizobjekt::set_Temp_SOLL(int tempsoll){
  
  Temp_SOLL = tempsoll;
  }

void Heizobjekt::set_Temp_IST(int tempist){
  
  Temp_IST = tempist;
  }

void Heizobjekt::set_Heiz_DAUER(int heizdauer){
  
  Heiz_DAUER = heizdauer;
  }

void Heizobjekt::handle(){
  
  if(Temp_IST >= Temp_SOLL){
    power_off();
    }
  }
