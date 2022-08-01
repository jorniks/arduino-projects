/*
 SET UP FOR HX711
   Arduino pin D6 -> HX711 SCK
   Arduino pin D5 -> HX711 DT
   Arduino pin 5V -> HX711 VCC
   Arduino pin GND -> HX711 GND
   
 SET UP FOR LCD (16x2)
   Arduino pin D10 -> LCD D4
   Arduino pin D9 -> LCD D5
   Arduino pin D8 -> LCD D6
   Arduino pin D7 -> LCD D7
   Arduino pin D12 -> LCD RS
   Arduino pin D11 -> LCD E
   Arduino pin 5V -> LCD VDD, A
   Arduino pin GND -> LCD VSS, RW, K
      
 SET UP FOR MQ2
   Arduino pin A6 -> MQ2 A0
   Arduino pin D4 -> MQ2 D0
   Arduino pin 5V -> MQ2 VCC
   Arduino pin GND -> MQ2 GND
         
 SET UP FOR BUZZER
   Arduino pin D3 -> BUZZER VCC
   Arduino pin GND -> BUZZER GND
   
*/

//Library for load cell
#include "HX711.h"
//Library for sending SMS
#include <SoftwareSerial.h>
//Library for 16x2 LCD
#include <LiquidCrystal.h>



// HX711 VARIABLES
HX711 scale(5, 6); // Load cell module port
float calibration_factor = -122440; // this calibration factor is adjusted according to my load cell
float currentValue;
float previousValue;


// LCD VARIABLES
const int rs = 12, en = 11, d4 = 10, d5 = 9, d6 = 8, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


// MQ2 VARIABLE
int smokeA0 = A6;
int sensorThreshold = 500; // Your threshold value

// BUZZER VARIABLE
int buzzer = 3; //D10


SoftwareSerial sms(0, 1); //GSM module port
//SoftwareSerial load(3, 4); 


// CHANGE PHONE NUMBERS TO ANY NUMBER OF YOUR CHOICE (INTERNATIONAL FORMAT)
char myPhone[] = ""; //User's phone number
char supplyPhone[] = ""; //Supplier's phone number


// CHANGE THE STRINGS BELOW TO SUIT THE TYPE OF MESSAGE YOU DESIRE
String alertForMe = "You are running out of gas, please call for refill."; //Notification for user
String messageForSupplier = "This user needs a gas refill."; //Message for gas supplier
String leakageMessage = "System has detected a leakage in your gas cylinder. Go and resolve it.";
int sent = 0;
int sentLeakage = 0;






void setup() {
  pinMode(buzzer, OUTPUT);
  pinMode(smokeA0, INPUT);
  
  Serial.begin(115200);
  
  lcd.begin(16, 2);
  showOnLCD("Remove items", "from the scale");
  delay(2000);
  
  noTone(buzzer);
  scale.set_scale();
  scale.tare();  //Reset the scale to 0
  scale.set_scale(calibration_factor); //Adjust to this calibration factor
  //Get a baseline reading
  scale.read_average(); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.


  delay(5000);
  lcd.clear();
  showOnLCD("Place an item", "on the scale");


  delay(3000);
  lcd.clear();
  showOnLCD("Weight of item", "");
}











void loop() {
  checkForLeakageAndRingBuzzer();
  

  currentValue = scale.get_units(), 10;
  if (currentValue <= 0) {
    lcd.clear();
    showOnLCD("Place an item", "on the scale");
  } else {
    if(currentValue < 7.05 && previousValue > currentValue && sent < 1) {
      sendSMS(myPhone, alertForMe);
      sent += 1;
//      load.listen();
    }
    
    lcd.clear();
    Serial.print("Weight of item: ");
    Serial.print(currentValue);
    Serial.println("");
    
    lcd.print("Weight of item");
    lcd.setCursor(0,1);
    lcd.print(currentValue);
    lcd.print(" kg");
    previousValue = currentValue;
  }
}



void checkForLeakageAndRingBuzzer(){
  int analogSensor = analogRead(smokeA0);
  
  if (analogSensor > sensorThreshold) {
    if(sentLeakage < 1) {
      sendSMS(myPhone, leakageMessage); // SEND A NOTIFICATION TO ANOUNCE LEAKAGE
      sentLeakage += 1;
    }
    tone(buzzer, 1000, 200);
  } else {
    sentLeakage = 0;
    noTone(buzzer);
  }
  delay(100);
}



void sendSMS(char* number, String text){
  sms.listen();

  Serial.println("AT+CMGF=1"); //Sets GSM module in text mode
  delay(3000);
  Serial.print("AT+CMGS=\"");
  Serial.print(number);
  Serial.println("\"\r");
  delay(500);
  Serial.print(text);
  delay(500);
  Serial.println(char(26)); // the ASCII code for the ctrl z is 26
}



void showOnLCD(String firstLineText, String secondLineText) {
  Serial.print(firstLineText);
  Serial.print(": ");
  Serial.print(secondLineText);
  Serial.println("");
  
  lcd.print(firstLineText);
  lcd.setCursor(0,1);
  lcd.print(secondLineText);
}
