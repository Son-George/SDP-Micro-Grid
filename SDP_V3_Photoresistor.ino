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
const int Solar_Panel_relay = 23;
const int Fuel_Cell_relay = 25;
const int Battery_Discharge_relay = 27;
const int Battery_Charge_relay= 29;
const byte IT_Load_Pin = A0; 
const byte FC_Pin = A1;
const int photoPin = A2;  // Photoresistor pin

// Arduino Pinout for the switches of power on/off and day/night time

//photoresistor
int Photo_R = 0;
const int light_threshold = 550; // Adjust this cutoff value for your lighting 
// lower value means more light

// Voltage parameter
float IT_Voltage;
float Fuel_Cell_Voltage;
float Battery_Voltage;
// ideal reference value;
const int bus_voltage = 24;
const float cutoff_voltage = 23.9;
const int fc_nominal_voltage = 12;

void setup() {
  // Initialize the output pins for the relays
  pinMode(Battery_Discharge_relay, OUTPUT);
  pinMode(Battery_Charge_relay, OUTPUT);
  pinMode(Solar_Panel_relay, OUTPUT);
  pinMode(Fuel_Cell_relay, OUTPUT);

  // Initialized input pin for power and day/night time

  // Have the relays open for Com to NO: Set the relay signal to low for open
  // Battery_Charge_relay will be the only relay connected to NC
  digitalWrite(Battery_Discharge_relay, LOW);//HIGH IS OPEN; LOW IS CLOSED
  digitalWrite(Battery_Charge_relay, LOW);// LOW IS OPEN;  HIGH IS CLOSED
  digitalWrite(Solar_Panel_relay, LOW);// LOW IS OPEN;  HIGH IS CLOSED
  digitalWrite(Fuel_Cell_relay, LOW);// LOW IS OPEN;  HIGH IS CLOSED

  power_switch.setDebounceTime(50);

  Serial.begin(9600);
}
int i = 0;

void loop() 
{
  power_switch.loop();//??
	if(i<=5)
	{
    Serial.println(i);
		Start_up();
		delay(1000);
		i=i+1;
	}
	else if (i>5)
	{
    while(power_switch.getState() == LOW){ // switch power is on
      Serial.print(" Fuel Cell");
      Fuel_Cell_Voltage = Read_Volt(FC_Pin);
      Serial.print(" IT Load");
      IT_Voltage = Read_Volt(IT_Load_Pin);
      if( Fuel_Cell_Voltage >= cutoff_voltage){// fuel cell is at full operation FC >= 24V
        Photo_R = analogRead(photoPin);
        Serial.print("Photo Resistor: ");
        Serial.println(Photo_R);
        if(Photo_R < light_threshold){// low value means lower light
         State_Daytime();
        }
        else{
          State_nighttime();
        }
      }
      else{// fuel cell does not reach the 24V needed switch 
        FC_Down();
      }
      power_switch.loop();
      Serial.println(power_switch.getState());
	   	delay(1000);
    }// end of while loop when the power is on
    power_switch.loop();
    Serial.println(power_switch.getState());
    if(power_switch.getState() == HIGH);
    {// if power switch is off have all the relays open (disconnected from power)

      Serial.println("Relays OFF");
      digitalWrite(Battery_Discharge_relay, HIGH);
      digitalWrite(Battery_Charge_relay, LOW);
      digitalWrite(Solar_Panel_relay, LOW);
      digitalWrite(Fuel_Cell_relay, LOW);
      delay(1000);
    }
    // print out the state for power and time to verify the switch are working 
    //delay(1000);// delay can be remove add more time if needed to verify time switching 
	}
  
}

//initial start up condition when turning on the circuit
void Start_up(){
  Serial.println("State: starting BAttery discharging");
  digitalWrite(Battery_Discharge_relay, LOW); // closed
  digitalWrite(Fuel_Cell_relay, HIGH); // closed
  digitalWrite(Solar_Panel_relay, LOW); // Open
  digitalWrite(Battery_Charge_relay, LOW); // open

}


void State_Daytime(){
  Serial.println("State: daytime");
  digitalWrite(Battery_Discharge_relay, HIGH); // open
  digitalWrite(Solar_Panel_relay, HIGH); // closed
  digitalWrite(Fuel_Cell_relay, HIGH); // closed
  digitalWrite(Battery_Charge_relay, HIGH); // closed
}

//transitioning from start up to either daytime or nighttime
void State_nighttime(){
  Serial.print("State: nighttime");
  digitalWrite(Battery_Discharge_relay, HIGH); // open
  digitalWrite(Fuel_Cell_relay, HIGH); // closed
  digitalWrite(Solar_Panel_relay, LOW); // open
  digitalWrite(Battery_Charge_relay, HIGH); // closed
}

void FC_Down(){//fuel cell is down
  Serial.print("State: Fuel Cell Down  BAttery Discharge");
  digitalWrite(Battery_Charge_relay, LOW); // open
  digitalWrite(Fuel_Cell_relay, LOW); // open
  digitalWrite(Battery_Discharge_relay, LOW); // Closed

  Serial.print(" IT Load");
  IT_Voltage = Read_Volt(IT_Load_Pin);

  Photo_R = analogRead(photoPin);
  Serial.print("Photo Resistor: ");
  Serial.println(Photo_R);
  if(Photo_R < light_threshold){// lower Photoresister value means more light
    digitalWrite(Solar_Panel_relay, HIGH); // closed 
  }
  else{
    digitalWrite(Solar_Panel_relay, LOW); // open 
  }
}

float Read_Volt(byte POS){
  float relay = analogRead(POS);
  float volt = relay * (25 / 1023.0) ;// change denomitanor 
  Serial.print(" Voltage Relay: ");
  Serial.println(volt);
  return volt;
}

int Photo_R_Value(){
  return analogRead(photoPin);
}


void Print_All_Volt(){

}

