
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
  
  if(Temp_IST <= Temp_SOLL){
    digitalWrite(HW_PIN,HIGH);
    Serial.println("Heizobjekt power ON");
  }
  else{
    Serial.println("Temperatur zu niedrieg um zu Heizen");  
  }
 }


void Heizobjekt::power_off(){
  
  digitalWrite(HW_PIN,LOW);
   Serial.println(HW_PIN);
  Serial.println("Heizobjekt power OFF");
  }

void Heizobjekt::set_Temp_SOLL(int tempsoll){
  
  Temp_SOLL = tempsoll;
  Serial.println("Heizobjekt set Value");
  Serial.print(tempsoll);
  Serial.println();
  }

void Heizobjekt::set_Temp_IST(int tempist){
  
  Temp_IST = tempist;
  Serial.println("Heizobjekt set Temp_Soll");
  Serial.print(Temp_IST);
  Serial.println();
  }

void Heizobjekt::set_Heiz_DAUER(int heizdauer){
  
  Heiz_DAUER = heizdauer;
  }

void Heizobjekt::handle(){
  
  if(Temp_IST >= Temp_SOLL){
    power_off();
    }
  Serial.println("handle heizobjekt");  
  }
