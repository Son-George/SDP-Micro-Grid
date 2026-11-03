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
const byte Solar_Pin = A0; 
const byte FC_Pin = A1;

// Arduino Pinout for the switches of power on/off and day/night time


int state;
//case statement for day, Conversion(day to night), nightime, night-to_day 
const int daytime = 1;
const int day_to_night = 2;
const int nighttime = 3;
const int night_to_day = 4;

//state for power on and off
const int power_on = 1;
const int power_off = 2;

// used to store the current state day 
int power_state = 0;
int time_state = 0;

// Voltage parameter
float Solar_Voltage;
float Fuel_Cell_Voltage;
float Battery_Voltage;
// ideal reference value;
const int bus_voltage = 24;
const float cutoff_voltage = 23.3;

void setup() {
  // Initialize the output pins for the relays
  pinMode(Battery_Discharge_relay, OUTPUT);
  pinMode(Battery_Charge_relay, OUTPUT);
  pinMode(Solar_Panel_relay, OUTPUT);
  pinMode(Fuel_Cell_relay, OUTPUT);

  // Initialized input pin for power and day/night time

  // Have the relays open for Com to NC: Set the relay signal to high
  digitalWrite(Battery_Discharge_relay, HIGH);
  digitalWrite(Battery_Charge_relay, HIGH);
  digitalWrite(Solar_Panel_relay, HIGH);
  digitalWrite(Fuel_Cell_relay, HIGH);

  power_switch.setDebounceTime(50);
  day_nigth_switch.setDebounceTime(50);

  Serial.begin(9600);
  state = daytime;
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
      /* commented out theday and night switch to add the switch case for
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
      */
      switch(state){
      case daytime://1
        State_Daytime();
        break;
      case day_to_night://2
        Day_to_Night();
        break;
      case nighttime://3
        State_nighttime();
        break;
      case night_to_day://4
        Night_to_Day();
        break;
      }
      if(power_switch.isReleased())
       {
        Serial.println("YOU ARE STUPID");// yes but no
        break;
       }
    }// end of while loop when the power is on
    if(power_switch.getState() == HIGH)
    {// if power switch is off have all the relays open (disconnected from power)
      Serial.println("Relays OFF");
      digitalWrite(Battery_Discharge_relay, HIGH);
      digitalWrite(Battery_Charge_relay, HIGH);
      digitalWrite(Solar_Panel_relay, HIGH);
      digitalWrite(Fuel_Cell_relay, HIGH);
    }
    // print out the state for power and time to verify the switch are working 
    delay(1000);// delay can be remove add more time if needed to verify time switching 
  }
}


void State_Daytime(){
  digitalWrite(Battery_Discharge_relay, HIGH); // open
  digitalWrite(Solar_Panel_relay, LOW); // closed
  digitalWrite(Fuel_Cell_relay, HIGH); // open
  digitalWrite(Battery_Charge_relay, LOW); // closed

  Solar_Voltage = Read_Volt(Solar_Pin); // read solar voltage
  if(Solar_Voltage < cutoff_voltage){
    delay(500);// half a second delay to check if the voltage drop was a random or the solar power is actually going down 
    Solar_Voltage = Read_Volt(Solar_Pin); // read solar voltage second time
    if(Solar_Voltage < cutoff_voltage){
      //cutoff reachand go to the transition state
      state = day_to_night;
    }// end of second voltage test verify
  }
}

void Day_to_Night(){//transition period to night
  digitalWrite(Battery_Discharge_relay, HIGH); // open
  digitalWrite(Battery_Charge_relay, HIGH); // open
  digitalWrite(Solar_Panel_relay, LOW); // closed
  digitalWrite(Fuel_Cell_relay, LOW); // Closed

  //delay for half a second before the battery start to discharge
  delay(500);// can be remove to improve response time 
  digitalWrite(Battery_Discharge_relay, LOW); // closed

  Fuel_Cell_Voltage = Read_Volt(FC_Pin);
  if(Fuel_Cell_Voltage >= bus_voltage){// FC has reach the 24 voltage required, may need to charge for lower voltage just in case of loss in voltage reader
    //Open solar and discharge and closed charging
    state = nighttime;
    digitalWrite(Solar_Panel_relay, HIGH); // open
    digitalWrite(Battery_Discharge_relay, HIGH); // open
    delay(500);// before we start to charge the battery have the delay for safety 
    digitalWrite(Battery_Charge_relay, LOW); // closed
    
  }
}

void State_nighttime(){
  digitalWrite(Battery_Discharge_relay, HIGH); // open
  digitalWrite(Fuel_Cell_relay, LOW); // closed
  digitalWrite(Solar_Panel_relay, HIGH); // open
  digitalWrite(Battery_Charge_relay, LOW); // closed

  Solar_Voltage = Read_Volt(Solar_Pin); // read solar voltage
  if(Solar_Voltage >= bus_voltage){
    state = night_to_day;
  }
}

void Night_to_Day(){//transition period
  digitalWrite(Battery_Charge_relay, HIGH); // open
  digitalWrite(Battery_Discharge_relay, HIGH); // open
  digitalWrite(Solar_Panel_relay, LOW); // closed
  digitalWrite(Fuel_Cell_relay, LOW); // closed
  
  Solar_Voltage = Read_Volt(Solar_Pin); // read solar voltage
  if(Solar_Voltage >= bus_voltage){
    state = daytime;
    digitalWrite(Fuel_Cell_relay, HIGH); // open
    delay(500);//delay for safety but can be remove for efficiency
    digitalWrite(Battery_Charge_relay, low); // closed
  }
}


float Read_Volt(byte POS){
  float relay = analogRead(POS);
  float volt = relay * (25.8 / 1023.0) ;
  Serial.print("Voltage Relay 1: ");
  Serial.println(volt);
}
