#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <LiquidCrystal.h>

SoftwareSerial mySerial(7,8);
//variables to be sent

int sensor_id;
float longitude;
float latitude;
int pipe_id;
String water_pressure_timestamp;

String water_pressure2,sensor_id2,longitude2,latitude2,pipe_id2,reading;


//variables used for by the sensor
int flowPin = 2; //the input pin on the arduino for sensor 1
float flowrate; //the value to calculate
volatile int count; //set to volatile so that it can update during interrupt process
const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 6;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void Flow()
{
  count++; //will increment count by 1
  }

void setup() {
  // put your setup code here, to run once:
  //setup to set up flowrate sensor


  pinMode(flowPin, INPUT);  //setting up the pin as an input
  attachInterrupt(0, Flow, RISING); // this configures the interupt 0(pin 2 on the arduino) to run the function flow
  lcd.begin(16, 2);
  
  //setup to initate connection to GSM module
  Serial.begin(19200); //Setting the baud rate for the serial monitor
  mySerial.begin(19200); //Setting the baudrate for the GSM Module
  delay(1000);

  mySerial.println(F("AT"));
  updateSerial();
  delay(1000);  
  mySerial.println(F("AT+CPIN?"));
  updateSerial();
  delay(1000);
  mySerial.println(F("AT+CREG?"));
  updateSerial();
  delay(1000);
  mySerial.println(F("AT+COPS?"));
  updateSerial();
  delay(1000);
  mySerial.println(F("AT+CSQ"));
  updateSerial();
  delay(1000);
  mySerial.println(F("AT+CGATT=1"));
  updateSerial();
  delay(1000);
  mySerial.println(F("AT+CGATT?"));
  updateSerial();
  delay(1000);
  mySerial.println(F("AT+CGDCONT=1,\"IP\",\"safaricom.com\""));
  updateSerial();
  delay(2000);  
  mySerial.println(F("AT+CGACT=1,1"));
  updateSerial();
  delay(2000);
  mySerial.println(F("AT+CGPADDR=1"));
  updateSerial();
  delay(2000);
  mySerial.println(F("AT+SAPBR=3,1,\"Contype\",\"GPRS\""));
  updateSerial();
  delay(3000);
  mySerial.println(F("AT+SAPBR =1,1"));
  updateSerial();
  delay(3000);
  mySerial.println(F("AT+SAPBR=2,1"));
  updateSerial();
  delay(3000);
  mySerial.println(F("AT+HTTPINIT"));
  updateSerial();
  delay(2000);
  mySerial.println(F("AT+HTTPPARA=\"CID\",1"));
  updateSerial();
  delay(2000);

  //variables that remain constant     
 sensor_id =2 ;
 sensor_id2 = String(sensor_id);
 water_pressure2=String(flowrate);
 longitude = 23.5687;
 longitude2 = String(longitude);
 latitude = 56.89;
 latitude2 = String(latitude);
 pipe_id = 50;
 pipe_id2 = String(pipe_id);
 water_pressure_timestamp="2019-07-18 14:33:40";

 //reading="{  \"censor_id\" :" +sensor_id2+", \"latitude\" :"+latitude2+", \"longitude\" :" +longitude2+", \"water_pressure_timestamp\" :" +water_pressure_timestamp+",\"pipe_id\":"+pipe_id2+",\"water_pressure\":"+water_pressure2+"}";
}

void loop() {
  // put your main code here, to run repeatedly
      //Flowrate sensor

      // count = 0; //this resets the counter so we start counting from zero
      
       //waterDetect();
      
        flowrate=47;
         water_pressure2=String(flowrate);
       lcdPrint();
       

       reading="{\"censor_id\" :" +sensor_id2+", \"latitude\" :"+latitude2+", \"longitude\" :" +longitude2+", \"water_pressure_timestamp\" :" +water_pressure_timestamp+",\"pipe_id\":"+pipe_id2+",\"water_pressure\":"+water_pressure2+"}";

      //GSM Module
        mySerial.println("AT+HTTPPARA=\"URL\",\"ptsv2.com/t/csnmp-1563388795/post\"");
        updateSerial();
        delay(3000);
        mySerial.println("AT+HTTPPARA=\"CONTENT\",\"application/json\"");
        updateSerial();
        delay(3000);
        mySerial.println("AT+HTTPDATA=" + String(reading.length()) + ",10000");
        updateSerial();
        delay(3000);
        mySerial.println(reading);
        updateSerial();
        delay(5000);
       
        mySerial.println(F("AT+HTTPACTION=1")); 
        updateSerial();
         delay(10000);
         // mySerial.println(F("AT+HTTPTERM")); 
          //delay(2000);
         // updateSerial();

          
     
    
}
void updateSerial(){
while(Serial.available())
  {


    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
   // Serial.read();
    //delay(1000);
  }
  
  while (mySerial.available())
  {

    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
    //Serial.write(mySerial.read());
    //delay(1000);
    }
}

void waterDetect(){
  
       interrupts(); //Enabling interrupts on the Arduino
       delay(1000); //waiting for a second
       noInterrupts(); //disables interrupts on the arduino

  //mathematical calculation

       flowrate = (count * 2.25); //the taken number of pulses from the last second multiplied by 2.25ml
       flowrate = (flowrate * 60); //converts the rate from /sec to /min
       flowrate = flowrate / 1000; //this changes the value from ml to l
  
  
  
  }

void lcdPrint(){
  lcd.setCursor(0,0);
       lcd.print("Sensor 1: ");
       lcd.print(flowrate);
  
  }
