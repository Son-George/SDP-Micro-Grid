const int Battery_Discharge_relay = 23;
const int Battery_Charge_relay = 25;
const int Solar_Panel_relay = 27;
const int Fuel_Cell_relay = 29;

//case statement for day, Conversion(day to night), nightime, night-to_day 
const int daytime = 1;
const int day_to_night = 2;
const int nightime = 3;
const int night_to_day = 4;
  
// VOltage reading Valeus
float Solar_Voltage = 24;
float Fuel_Cell_Voltage = 24;
float Battery_Voltage = 24;
const byte Solar_Pin = A0; 
// ideal reference value;
const int bus_voltage = 24;
const float cutoff_voltage = 23.3;

int state;
void setup() {
  // Initialize the output pins for the relays
  pinMode(Battery_Discharge_relay, OUTPUT);
  pinMode(Battery_Charge_relay, OUTPUT);
  pinMode(Solar_Panel_relay, OUTPUT);
  pinMode(Fuel_Cell_relay, OUTPUT);
  // Set the relay to low for commonly open 
  digitalWrite(Battery_Discharge_relay, LOW);
  digitalWrite(Battery_Charge_relay, LOW);
  digitalWrite(Solar_Panel_relay, LOW);
  digitalWrite(Fuel_Cell_relay, LOW);
  // assuming the system will beging in the first state daytime.
  int state = daytime;
  Serial.begin(9600);
}

void loop() {
  delay(500); // delay is set to have the all system to be powered on/been in the operating state

  while(true){
    switch(state){
      case daytime:
        State_Daytime();
        break;
      case day_to_night:
        State_day_night();
        break;
      case nightime:
        State_nightime();
        break;
      case night_to_day:
        State_night_day();
        break;

    }// end of switch/case statement
  Serial.println(state);
  }// end of while loop

}// of of main loop

// daytime logic
void State_Daytime(){
  digitalWrite(Solar_Panel_relay, HIGH);// closed the relay for the solar panel
  digitalWrite(Battery_Charge_relay, HIGH);// closed the relay to charge the battery
  Solar_Voltage = Read_Volt(Solar_Pin); // read solar voltage
  if(Solar_Voltage < cutoff_voltage){
    delay(500);// half a second delay to check if the voltage drop was a random or the solar power is actually going down 
    if(Solar_Voltage < cutoff_voltage){
      state = day_to_night;
    }// end of second voltage test verify
  }//end of second voltage test verify
}//end daytime

//transition from day to night logic
void State_day_night(){
  digitalWrite(Battery_Discharge_relay, LOW);// open the relay from the battery charger
  digitalWrite(Battery_Charge_relay, LOW);// open the relay from the battery charger
  delay(500);// delay before we closed the battery charger to prevent overlap
}

// state during nightime
void State_nightime(){

}

//
void State_night_day(){

}

float Read_Volt(byte POS){
  float relay = analogRead(POS);
  float volt = relay * (25.8 / 1023.0) ;
  Serial.print("Voltage Relay 1: ");
  Serial.println(volt);
}
