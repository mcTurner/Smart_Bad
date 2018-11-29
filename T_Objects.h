/*T_Objects.h
 * 
 * V0.0
 * 
 * Hier sollen die für die Hausautomation benötigten Objekte definiert werden:
 * Heizobjekt
 * Schaltobjekt
 * Teichobjekt
 *  
 * 
 */
#include "Arduino.h"

class Heizobjekt{
  private: 
  int HW_PIN;
  int Temp_SOLL;
  int Temp_IST;
  int Heiz_DAUER;
  public:
  Heizobjekt(int hardwarepin, int default_temsoll);
  void power_on();
  void power_off();
  void set_Temp_SOLL(int tempsoll);
  void set_Temp_IST(int tempist);
  void set_Heiz_DAUER(int heizdauer);
  int get_Temp_SOLL();
  int get_Heiz_DAUER();
  void handle();
};

Heizobjekt::Heizobjekt(int hp, int default_temsoll)
{
  HW_PIN = hp;
  Temp_SOLL = default_temsoll;
}


void Heizobjekt::power_on(){
  
  digitalWrite(HW_PIN,LOW);
  }


void Heizobjekt::power_off(){
  
  digitalWrite(HW_PIN,HIGH);
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
