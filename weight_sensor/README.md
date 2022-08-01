This repository contains code for an arduino based weight sensor with SMS notification when weight falls below a particular threshold.


# COMPONENTS & SET UP

   > SET UP FOR HX711
         - Arduino pin D6 -> HX711 SCK
         - Arduino pin D5 -> HX711 DT
         - Arduino pin 5V -> HX711 VCC
         - Arduino pin GND -> HX711 GND
   

   > SET UP FOR LCD (16x2)
         - Arduino pin D10 -> LCD D4
         - Arduino pin D9 -> LCD D5
         - Arduino pin D8 -> LCD D6
         - Arduino pin D7 -> LCD D7
         - Arduino pin D12 -> LCD RS
         - Arduino pin D11 -> LCD E
         - Arduino pin 5V -> LCD VDD, A
         - Arduino pin GND -> LCD VSS, RW, K


   > SET UP FOR MQ2
         - Arduino pin A6 -> MQ2 A0
         - Arduino pin D4 -> MQ2 D0
         - Arduino pin 5V -> MQ2 VCC
         - Arduino pin GND -> MQ2 GND


   > SET UP FOR BUZZER
         - Arduino pin D3 -> BUZZER VCC
         - Arduino pin GND -> BUZZER GND