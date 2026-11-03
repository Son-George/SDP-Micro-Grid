//Note for using Relay connection
//*when the relay has a LOW signal
// Com to NC: Closed (continuity exists; current flows between COM and NC).
// Com to NO: Open (no continuity; current does not flow between COM and NO).

//*When the relay receives a HIGH signal
// com to NC: Open (the internal switch breaks the connection).
// com to NO: Closed (the internal switch makes a connection; current flows between COM and NO).

// Arduino pinout for the realys at the power side

#include <ezButton.h>

ezButton power_switch(3);
ezButton day_nigth_switch(4);
const int Solar_Panel_relay = 23;
const int Fuel_Cell_relay = 25;
const int Battery_Discharge_relay = 27;
const int Battery_Charge_relay= 29;

// Arduino Pinout for the switches of power on/off and day/night time


// State for day and night
const int daytime = 1;
const int nighttime = 2;

//state for power on and off
const int power_on = 1;
const int power_off = 2;

// used to store the current state day 
int power_state = 0;
int time_state = 0;

void setup() {
  // Initialize the output pins for the relays
  pinMode(Battery_Discharge_relay, OUTPUT);
  pinMode(Battery_Charge_relay, OUTPUT);
  pinMode(Solar_Panel_relay, OUTPUT);
  pinMode(Fuel_Cell_relay, OUTPUT);

  // Initialized input pin for power and day/night time

  // Have the relays open for Com to NO: Set the relay signal to low
  digitalWrite(Battery_Discharge_relay, LOW);
  digitalWrite(Battery_Charge_relay, LOW);
  digitalWrite(Solar_Panel_relay, LOW);
  digitalWrite(Fuel_Cell_relay, LOW);

  power_switch.setDebounceTime(50);
  day_nigth_switch.setDebounceTime(50);

  Serial.begin(9600);

}

void loop() 
{
  delay(500); //delay to have all the system powered on, delay can be remove if needed
  while(true){
    power_switch.loop();
    //int state1 = power_switch.getState();
    //if (state1 == HIGH)
    //Serial.println("The power switch: OFF");
    //else Serial.println("The power switch: ON");
    //int state2 = day_nigth_switch.getState();
    // if (state2 == HIGH)
    //Serial.println("The day night switch: OFF");
    //else Serial.println("The night switch: ON");
    // reading at the power switch
    while(power_switch.getState() == LOW)
    { // switch power is on
        power_switch.loop();
        day_nigth_switch.loop();
      // not reading the day night switch
      if(day_nigth_switch.isPressed()){
        // switch of day/night is on go to state daytime operation function
        State_Daytime();
        Serial.println("Relays ON");
        Serial.println("DayTime");
      }
      else if(day_nigth_switch.isReleased())
      {// switch of day/night is off go to state nighttime operation function
        State_nighttime();
        Serial.println("Relays ON");
        Serial.println("Nighttime");
      }
      if(power_switch.isReleased())
       {
        Serial.println("YOU ARE STUPID");// yes but no
        break;
       }
    }
    if(power_switch.getState() == HIGH)
    {// if power switch is off have all the relays open (disconnected from power)
      Serial.println("Relays OFF");
      digitalWrite(Battery_Discharge_relay, LOW);
      digitalWrite(Battery_Charge_relay, LOW);
      digitalWrite(Solar_Panel_relay, LOW);
      digitalWrite(Fuel_Cell_relay, LOW);
    }
    // print out the state for power and time to verify the switch are working 
    delay(1000);// delay can be remove add more time if needed to verify time switching 
  }
}
void State_Daytime(){
  digitalWrite(Battery_Discharge_relay, LOW); // open
  digitalWrite(Solar_Panel_relay, HIGH); // closed
  digitalWrite(Fuel_Cell_relay, LOW); // open
  digitalWrite(Battery_Charge_relay, HIGH); // closed
}
void State_nighttime(){
  digitalWrite(Battery_Discharge_relay, LOW); // open
  digitalWrite(Fuel_Cell_relay, HIGH); // closed
  digitalWrite(Solar_Panel_relay, LOW); // open
  digitalWrite(Battery_Charge_relay, HIGH); // closed
}