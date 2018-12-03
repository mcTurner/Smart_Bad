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
#ifndef T_Objects_h
#define T_Objects_h
#include "Arduino.h"
class Heizobjekt{
  private: 
  int HW_PIN;
  int Temp_SOLL;
  int Temp_IST;
  int Heiz_DAUER;
  public:
  Heizobjekt(int hp, int default_temsoll);
  void power_on();
  void power_off();
  void set_Temp_SOLL(int tempsoll);
  void set_Temp_IST(int tempist);
  void set_Heiz_DAUER(int heizdauer);
  int get_Temp_SOLL();
  int get_Heiz_DAUER();
  void handle();
};
#endif
