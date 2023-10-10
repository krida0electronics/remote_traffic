////////////////////////////////////////
//
//  output RED color    -  D9 | PB1
//  output YELLOW color - D10 | PB2
//  output GREEN color  - D11 | PB3
//
//  input RX480e D0 - D8 | PB0 
//  input RX480e D1 - D7 | PD7
//  input RX480e D2 - D6 | PD6
//  input RX480e D3 - D5 | PD5     
//
//  input SWITCH A1 - D3 | PD3
//  input SWITCH A2 - D2 | PD2
//  input SWITCH A3 - D4 | PD4
//
//  MODE 0 - 0000 0111 - 0x07
//  MODE 1 - 0000 0011 - 0x03
//  MODE 2 - 0000 0101 - 0x05
//  MODE 3 - 0000 0001 - 0x01
//  MODE 4 - 0000 0110 - 0x06
//  MODE 5 - 0000 0010 - 0x02
//  MODE 6 - 0000 0100 - 0x04
//  MODE 7 - 0000 0000 - 0x00
//
/////////////////////////////////////////

#include "OneButton.h"
#include <TimerOne.h>
#include <EEPROM.h>

#define flashing 1000

#define RED 9             //    on the pcb 9   PINB.1
#define YELLOW 10         //    on the pcb 10  PINB.2
#define GREEN 11          //    on the pcb 11  PINB.3


OneButton button1(8, false, false);         //  Button A 5 -> 6
OneButton button2(7, false, false);         //  Button B 6 -> 5
OneButton button3(6, false, false);         //  Button C 7 -> 8
OneButton button4(5, false, false);         //  Button D 8 -> 7

long eeprom_RED;
long eeprom_YELLOW;
long eeprom_GREEN;

unsigned char mode, prev_mode, type;
unsigned int delay_time;
unsigned char SWITCH_MODE;
unsigned char New_FLAG;
unsigned char button1_click, button2_click, button3_click, button4_click;
unsigned long int t_millis;
unsigned char NewEvent1, NewEvent2, NewEvent3, NewEvent4;
unsigned char SPEED, i;
unsigned char ON_OFF;

unsigned char mas[4] = {100,150,200,250};

unsigned long timer_tick;

unsigned char programming_mode;
unsigned char programming_color = 0; // RED = 1; YELLOW = 2; GREEN = 3;
unsigned char click_count;
unsigned char color_flash = 1;
//unsigned char i = 1;



void setup() {

  button1_click = 0;
  button2_click = 0;
  button3_click = 0;
  button4_click = 0;
  
  pinMode(RED, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(GREEN, OUTPUT);

  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);

  //  MODE 1 - 0000 0111 - 0x07
  //  MODE 2 - 0000 0011 - 0x03
  //  MODE 3 - 0000 0101 - 0x05
  //  MODE 4 - 0000 0001 - 0x01
  //  MODE 5 - 0000 0110 - 0x06
  //  MODE 6 - 0000 0010 - 0x02
  //  MODE 7 - 0000 0100 - 0x04
  //  MODE 8 - 0000 0000 - 0x00

  SWITCH_MODE = digitalRead(4) | digitalRead(2)<<1 | digitalRead(3) <<2;

  SWITCH_MODE = 0x02;
  ON_OFF = 1;
  programming_mode = 0;
  i=1;
  
  button1.attachClick(click1);
  button1.attachDoubleClick(doubleclick1);
  button1.attachLongPressStart(longPressStart1);

  button2.attachClick(click2);
  button2.attachDoubleClick(doubleclick2);
  button2.attachLongPressStart(longPressStart2);

  button3.attachClick(click3);
  button3.attachDoubleClick(doubleclick3);
  button3.attachLongPressStart(longPressStart3);

  button4.attachClick(click4);
  button4.attachDoubleClick(doubleclick4);
  button4.attachLongPressStart(longPressStart4);

  Timer1.attachInterrupt(timerIsr);
  Timer1.initialize(1000); 

  eeprom_RED = EEPROMReadlong(0); // 4 bytes
  eeprom_YELLOW = EEPROMReadlong(4); // 4 bytes
  eeprom_GREEN = EEPROMReadlong(8); // 4 bytes

 
}


void timerIsr() {

  timer_tick++;
    
  button1.tick();
  button2.tick();
  button3.tick();
  button4.tick();
}


void loop() 
{
 
  switch (SWITCH_MODE) {
                         case 0x07: 
                                    switch (type) {
                                                   case 1: digitalWrite(RED, HIGH); digitalWrite(YELLOW, LOW); digitalWrite(GREEN, LOW);
                                                           break;

                                                   case 2: digitalWrite(RED, LOW); digitalWrite(YELLOW, HIGH); digitalWrite(GREEN, LOW);
                                                           break;
                                                                 
                                                   case 3: digitalWrite(RED, LOW); digitalWrite(YELLOW, LOW); digitalWrite(GREEN, HIGH);
                                                           break;

                                                   case 4: if ( digitalRead(RED) == 1 && New_FLAG == 1){
                                                                                                        digitalWrite(RED, LOW);
                                                                                                        digitalWrite(YELLOW, LOW);
                                                                                                        digitalWrite(GREEN, HIGH);
                                                                                                        
                                                                                                        delay(30000); 
                                                                                                        
                                                                                                        digitalWrite(YELLOW, HIGH);
                                                                                                        digitalWrite(GREEN, LOW);

                                                                                                        delay(3000);

                                                                                                        digitalWrite(RED, HIGH);
                                                                                                        digitalWrite(YELLOW, LOW); 

                                                                                                        New_FLAG = 0;
                                                                                                       }
                                                           break;  

                                                   case 5: digitalWrite(RED, HIGH); digitalWrite(YELLOW, LOW); digitalWrite(GREEN, LOW);
                                                           delay(1000);
                                    
                                                           digitalWrite(RED, LOW); digitalWrite(YELLOW, LOW); digitalWrite(GREEN, LOW);
                                                           delay(1000);
                                                           
                                                           break;  

                                                   case 6: digitalWrite(RED, LOW); digitalWrite(YELLOW, HIGH); digitalWrite(GREEN, LOW);
                                                           delay(1000);
                                    
                                                           digitalWrite(RED, LOW); digitalWrite(YELLOW, LOW); digitalWrite(GREEN, LOW);
                                                           delay(1000);
                                                              
                                                           break;  
                                                           
                                                   case 7: digitalWrite(RED, LOW); digitalWrite(YELLOW, LOW); digitalWrite(GREEN, HIGH);
                                                           delay(1000);
                                    
                                                           digitalWrite(RED, LOW); digitalWrite(YELLOW, LOW); digitalWrite(GREEN, LOW);
                                                           delay(1000);
                                                           
                                                           break;  

                                                   case 8: digitalWrite(RED, HIGH); digitalWrite(YELLOW, HIGH); digitalWrite(GREEN, HIGH);
                                                           delay(1000);
                                    
                                                           digitalWrite(RED, LOW); digitalWrite(YELLOW, LOW); digitalWrite(GREEN, LOW);
                                                           delay(1000);
                                                           
                                                           break;  
                                                                 
                                                   case 9: digitalWrite(RED, LOW); digitalWrite(YELLOW, LOW); digitalWrite(GREEN, LOW);
                                                           break;

                                                  case 10: digitalWrite(RED, LOW); digitalWrite(YELLOW, LOW); digitalWrite(GREEN, LOW);
                                                           break;  

                                                  case 11: digitalWrite(RED, LOW); digitalWrite(YELLOW, LOW); digitalWrite(GREEN, LOW);
                                                           break;  

                                                  case 12: digitalWrite(RED, LOW); digitalWrite(YELLOW, LOW); digitalWrite(GREEN, LOW);
                                                           break;                     
                                    }
                                    
                                    break;

                         case 0x03: 
                                    switch (type) {
                                                   case 1: if (NewEvent1){
                                                                          if (digitalRead(9) == 0) {digitalWrite(RED, HIGH); NewEvent1 = 0; break;}
                                                                           else {digitalWrite(RED, LOW); NewEvent1 = 0; break;}  
                                                                         }
                                                           break;                                      

                                                   case 2: if (NewEvent2){
                                                                          if (digitalRead(10) == 0) {digitalWrite(YELLOW, HIGH); NewEvent2 = 0; break;}
                                                                           else {digitalWrite(YELLOW, LOW); NewEvent2 = 0; break;}
                                                                         }
                                                           break;   

                                                   case 3: if (NewEvent3){
                                                                          if (digitalRead(11) == 0) {digitalWrite(GREEN, HIGH); NewEvent3 = 0; break;}
                                                                           else {digitalWrite(GREEN, LOW); NewEvent3 = 0; break;}
                                                                         }
                                                           break;   

                                                   case 4: if (NewEvent4){ if ( digitalRead(9) || digitalRead(10) || digitalRead(11) ) {digitalWrite(RED, LOW); digitalWrite(YELLOW, LOW); digitalWrite(GREEN, LOW); NewEvent4 = 0; break;}
                                                                            else {digitalWrite(RED, HIGH); digitalWrite(YELLOW, HIGH); digitalWrite(GREEN, HIGH); NewEvent4 = 0; break;}
                                                                         }                         
                                                           break;  

                                                   case 5: digitalWrite(RED, HIGH);
                                                           delay(500);
                                    
                                                           digitalWrite(RED, LOW);
                                                           delay(500);

                                                           break;

                                                   case 6: digitalWrite(YELLOW, HIGH);
                                                           delay(500);
                                    
                                                           digitalWrite(YELLOW, LOW);
                                                           delay(500);
                                                           break;     

                                                   case 7: digitalWrite(GREEN, HIGH);
                                                           delay(500);
                                    
                                                           digitalWrite(GREEN, LOW);
                                                           delay(500);
                                                           break;

                                                   case 8: digitalWrite(RED, HIGH); digitalWrite(YELLOW, HIGH); digitalWrite(GREEN, HIGH);
                                                           delay(500);
                                    
                                                           digitalWrite(RED, LOW); digitalWrite(YELLOW, LOW); digitalWrite(GREEN, LOW);
                                                           delay(500);
                                                           break; 

                                                   case 9: digitalWrite(RED, LOW); digitalWrite(YELLOW, LOW); digitalWrite(GREEN, LOW);
                                                           break;

                                                  case 10: digitalWrite(RED, LOW); digitalWrite(YELLOW, LOW); digitalWrite(GREEN, LOW);
                                                           break;     

                                                  case 11: digitalWrite(RED, LOW); digitalWrite(YELLOW, LOW); digitalWrite(GREEN, LOW);
                                                           break;

                                                  case 12: digitalWrite(RED, LOW); digitalWrite(YELLOW, LOW); digitalWrite(GREEN, LOW);
                                                           break;                                


                                    }
                                    break; 

                         case 0x05:  
                                     if (programming_mode == 0 && ON_OFF == 1) { 
                                                                                if (i == 1) {digitalWrite(RED, HIGH); i = 2;}
                                                                                if (i == 2) {if (timer_tick >= eeprom_RED*1000 ) {timer_tick = 0; i = 3; digitalWrite(RED, LOW);}}
 
                                                                                if (i == 3) {digitalWrite(GREEN, HIGH); i = 4;}
                                                                                if (i == 4) {if (timer_tick >= eeprom_GREEN*1000 ) {timer_tick = 0; i = 5; digitalWrite(GREEN, LOW);}}
 
                                                                                if (i == 5) {digitalWrite(YELLOW, HIGH); i = 6;}
                                                                                if (i == 6) {if (timer_tick >= eeprom_YELLOW*1000 ) {timer_tick = 0; i = 1; digitalWrite(YELLOW, LOW);}}
                                                                               }
                                    
                                    break;   

                         case 0x01: if (programming_mode == 0 && ON_OFF == 1) { 
                                                                                if (i == 1) {digitalWrite(RED, HIGH); i = 2;}
                                                                                if (i == 2) {if (timer_tick >= eeprom_RED*1000 ) {timer_tick = 0; i = 3; digitalWrite(RED, LOW);}}
 
                                                                                if (i == 3) {digitalWrite(YELLOW, HIGH); i = 4;}
                                                                                if (i == 4) {if (timer_tick >= eeprom_YELLOW*1000 ) {timer_tick = 0; i = 5; digitalWrite(YELLOW, LOW);}}
 
                                                                                if (i == 5) {digitalWrite(GREEN, HIGH); i = 6;}
                                                                                if (i == 6) {if (timer_tick >= eeprom_GREEN*1000 ) {timer_tick = 0; i = 1; digitalWrite(GREEN, LOW);}}
                                                                               }
                                    
                                    break;    

                         case 0x06: if (programming_mode == 0 && ON_OFF == 1) { 
                                                                                if (i == 1) {digitalWrite(RED, HIGH); i = 2;}
                                                                                if (i == 2) {if (timer_tick >= eeprom_RED*1000 ) {timer_tick = 0; i = 3; }}
 
                                                                                if (i == 3) {digitalWrite(YELLOW, HIGH); i = 4;}
                                                                                if (i == 4) {if (timer_tick >= eeprom_YELLOW*1000 ) {timer_tick = 0; i = 5; digitalWrite(YELLOW, LOW); digitalWrite(RED, LOW);}}
 
                                                                                if (i == 5) {digitalWrite(GREEN, HIGH); i = 6;}
                                                                                if (i == 6) {if (timer_tick >= eeprom_GREEN*1000 ) {timer_tick = 0; i = 7; digitalWrite(GREEN, LOW);}}

                                                                                if (i == 7) {digitalWrite(YELLOW, HIGH); i = 8;}
                                                                                if (i == 8) {if (timer_tick >= eeprom_YELLOW*1000 ) {timer_tick = 0; i = 1; digitalWrite(YELLOW, LOW);}}
                                                                               }

                                    break;                                           

                         case 0x02: if (ON_OFF == 1)
                                     {
                                      digitalWrite(RED, HIGH); delay(750); digitalWrite(RED, LOW);
                                      digitalWrite(GREEN, HIGH); delay(750); digitalWrite(GREEN, LOW);
                                     }
                                    
                                    break;    

                         case 0x04: if (ON_OFF == 1)
                                     {
                                      digitalWrite(RED, HIGH); delay(500); digitalWrite(RED, LOW);
                                      digitalWrite(GREEN, HIGH); delay(500); digitalWrite(GREEN, LOW);
                                     }
                                    
                                    break;    

                         case 0x00: switch (type) {
                                                   case 1: 
                                                          for (i=0; i<3; i++) {
                                                                               digitalWrite(RED, HIGH);
                                                                               delay(mas[0]);

                                                                               digitalWrite(RED, LOW);
                                                                               delay(mas[0]);

                                                                              }          

                                                           for (i=0; i<3; i++) {
                                                                               digitalWrite(GREEN, HIGH);
                                                                               delay(mas[0]);


                                                                               digitalWrite(GREEN, LOW);
                                                                               delay(mas[0]);

                                                                              }                              
                                                           break;

                                                   case 2: for (i=0; i<3; i++) {
                                                                               digitalWrite(RED, HIGH);
                                                                               delay(mas[1]);


                                                                               digitalWrite(RED, LOW);
                                                                               delay(mas[1]);
                                                                              }          

                                                           for (i=0; i<3; i++) {
                                                                               digitalWrite(GREEN, HIGH);
                                                                               delay(mas[1]);

                                                                               digitalWrite(GREEN, LOW);
                                                                               delay(mas[1]);
                                                                              }                              
                                                           break;

                                                                 
                                                   case 3: for (i=0; i<3; i++) {
                                                                               digitalWrite(RED, HIGH);
                                                                               delay(mas[2]);

                                                                               digitalWrite(RED, LOW);
                                                                               delay(mas[2]);
                                                                              }          

                                                           for (i=0; i<3; i++) {
                                                                               digitalWrite(GREEN, HIGH);
                                                                               delay(mas[2]);

                                                                               digitalWrite(GREEN, LOW);
                                                                               delay(mas[2]);
                                                                              }                              
                                                           break;


                                                   case 4: for (i=0; i<3; i++) {
                                                                                digitalWrite(RED, HIGH);
                                                                                delay(mas[3]);

                                                                                digitalWrite(RED, LOW);
                                                                                delay(mas[3]);
                                                                               }          

                                                           for (i=0; i<3; i++) {
                                                                                digitalWrite(GREEN, HIGH);
                                                                                delay(mas[3]);

                                                                                digitalWrite(GREEN, LOW);
                                                                                delay(mas[3]);
                                                                               }                              
                                                           break;
 

                                                   case 5: for (i=0; i<2; i++) {
                                                                                digitalWrite(RED, HIGH);
                                                                                delay(mas[0]);

                                                                                delay(mas[0]);
                                                                               }          

                                                           for (i=0; i<2; i++) {
                                                                                digitalWrite(GREEN, HIGH);
                                                                                delay(mas[0]); 

                                                                                digitalWrite(GREEN, LOW);
                                                                                delay(mas[0]);
                                                                               }         

                                                           for (i=0; i<2; i++) {
                                                                                digitalWrite(RED, HIGH);
                                                                                delay(mas[0]);

                                                                                digitalWrite(RED, LOW);
                                                                                delay(mas[0]);

                                                                                digitalWrite(GREEN, HIGH);
                                                                                delay(mas[0]);

                                                                                digitalWrite(GREEN, LOW);
                                                                                delay(mas[0]);
                                                                               }                                             
                                                           break;  

                                                   case 6: for (i=0; i<2; i++) {
                                                                                digitalWrite(RED, HIGH);
                                                                                delay(mas[1]);

                                                                                digitalWrite(RED, LOW);
                                                                                delay(mas[1]);
                                                                               }          

                                                           for (i=0; i<2; i++) {
                                                                                digitalWrite(GREEN, HIGH);
                                                                                delay(mas[1]);

                                                                                digitalWrite(GREEN, LOW);
                                                                                delay(mas[1]);
                                                                               }         

                                                           for (i=0; i<2; i++) {
                                                                                digitalWrite(RED, HIGH);
                                                                                delay(mas[1]);

                                                                                digitalWrite(RED, LOW);
                                                                                delay(mas[1]);
                                                                                
                                                                                digitalWrite(GREEN, HIGH);
                                                                                delay(mas[1]);

                                                                                digitalWrite(GREEN, LOW);
                                                                                delay(mas[1]);
                                                                               }                                             
                                                           break;   
                                                           
                                                   case 7:                                                            
                                                           for (i=0; i<2; i++) {
                                                                                digitalWrite(RED, HIGH);
                                                                                delay(mas[2]);

                                                                                digitalWrite(RED, LOW);
                                                                                delay(mas[2]);
                                                                               }          

                                                           for (i=0; i<2; i++) {
                                                                                digitalWrite(GREEN, HIGH);
                                                                                delay(mas[2]);

                                                                                digitalWrite(GREEN, LOW);
                                                                                delay(mas[2]);
                                                                               }         

                                                           for (i=0; i<2; i++) {
                                                                                digitalWrite(RED, HIGH);
                                                                                delay(mas[2]);

                                                                                digitalWrite(RED, LOW);
                                                                                delay(mas[2]);

                                                                                digitalWrite(GREEN, HIGH);
                                                                                delay(mas[2]);

                                                                                digitalWrite(GREEN, LOW);
                                                                                delay(mas[2]);
                                                                               }                                             
                                                           break;  
                                                           
                                                   case 8: for (i=0; i<2; i++) {
                                                                                digitalWrite(RED, HIGH);
                                                                                delay(mas[3]);

                                                                                digitalWrite(RED, LOW);
                                                                                delay(mas[3]);
                                                                               }          

                                                           for (i=0; i<2; i++) {
                                                                                digitalWrite(GREEN, HIGH);
                                                                                delay(mas[3]);

                                                                                digitalWrite(GREEN, LOW);
                                                                                delay(mas[3]);
                                                                               }         

                                                           for (i=0; i<2; i++) {
                                                                                digitalWrite(RED, HIGH);
                                                                                delay(mas[3]);

                                                                                digitalWrite(RED, LOW);
                                                                                delay(mas[3]);

                                                                                digitalWrite(GREEN, HIGH);
                                                                                delay(mas[3]);

                                                                                digitalWrite(GREEN, LOW);
                                                                                delay(mas[3]);
                                                                               }                                             
                                                           break;  
                                                           
                                                   case 9: digitalWrite(RED, LOW); digitalWrite(YELLOW, LOW); digitalWrite(GREEN, LOW);
                                                           break;

                                                  case 10: digitalWrite(RED, LOW); digitalWrite(YELLOW, LOW); digitalWrite(GREEN, LOW);
                                                           break;  

                                                  case 11: digitalWrite(RED, LOW); digitalWrite(YELLOW, LOW); digitalWrite(GREEN, LOW);
                                                           break;  

                                                  case 12: digitalWrite(RED, LOW); digitalWrite(YELLOW, LOW); digitalWrite(GREEN, LOW);
                                                           break;                     
                                    }
                                    
                                    break;                                                    
   
  }
  
}



void click1() {
               type = 1; NewEvent1 = 1; if (SWITCH_MODE == 0x03) { button1_click++; } 

                 if (programming_mode == 1) { 
                                                     switch (programming_color){
                                                                                case 1: digitalWrite(RED, LOW); delay(50000); digitalWrite(RED, HIGH);
                                                                                        eeprom_RED++;
                                                                                        break;

                                                                                case 2: digitalWrite(YELLOW, LOW); delay(50000); digitalWrite(YELLOW, HIGH);
                                                                                        eeprom_YELLOW++;
                                                                                        break;

                                                                                case 3: digitalWrite(GREEN, LOW); delay(50000); digitalWrite(GREEN, HIGH);
                                                                                        eeprom_GREEN++;
                                                                                        break;                
                                                                               }
                                                                                          

                          
                                                   }
               
               
              }


                
void doubleclick1() {type = 5;} 
void longPressStart1() {type = 9; } 

void click2() {
               type = 2; NewEvent2 = 1; if (SWITCH_MODE == 0x03) {button2_click++;} 

               if (programming_mode == 1) { 
                                                     switch (programming_color){
                                                                                case 1: eeprom_RED--;
                                                                                        if (eeprom_RED == 0)  { eeprom_RED = 1;
                                                                                                                digitalWrite(RED, LOW); delay(20000); digitalWrite(RED, HIGH); delay(20000);   
                                                                                                                digitalWrite(RED, LOW); delay(20000); digitalWrite(RED, HIGH); delay(20000);                          
                                                                                                              }
                                                                                        else { digitalWrite(RED, LOW); delay(50000); digitalWrite(RED, HIGH); }                      
                                                                                        break;            

                                                                                case 2: eeprom_YELLOW--;
                                                                                        if (eeprom_YELLOW == 0)  { eeprom_YELLOW = 1;
                                                                                                                   digitalWrite(YELLOW, LOW); delay(20000); digitalWrite(YELLOW, HIGH); delay(20000);   
                                                                                                                   digitalWrite(YELLOW, LOW); delay(20000); digitalWrite(YELLOW, HIGH); delay(20000);                          
                                                                                                                 }
                                                                                        else { digitalWrite(YELLOW, LOW); delay(50000); digitalWrite(YELLOW, HIGH); }                      
                                                                                        break;  

                                                                                case 3: eeprom_GREEN--;
                                                                                        if (eeprom_GREEN == 0)  { eeprom_GREEN = 1;
                                                                                                                  digitalWrite(GREEN, LOW); delay(20000); digitalWrite(GREEN, HIGH); delay(20000);   
                                                                                                                  digitalWrite(GREEN, LOW); delay(20000); digitalWrite(GREEN, HIGH); delay(20000);                          
                                                                                                                }
                                                                                        else { digitalWrite(GREEN, LOW); delay(50000); digitalWrite(GREEN, HIGH); }                      
                                                                                        break;                    
                                                                               }
                                                   }

              }

void doubleclick2() {type = 6;} 
void longPressStart2() {type = 10; } 

void click3() {
               type = 3; NewEvent3 = 1; if (SWITCH_MODE == 0x03) { button3_click++; } 

                if (programming_mode == 1) { 
                                                     click_count++;
                                                    
                                                     switch (click_count) {
                                                                           case 0: digitalWrite(RED, HIGH); digitalWrite(YELLOW, LOW); digitalWrite(GREEN, LOW);
                                                                                   programming_color = 1; // RED color
                                                                                   break;
                                                                                   
                                                                           case 1: digitalWrite(RED, LOW); digitalWrite(YELLOW, HIGH); digitalWrite(GREEN, LOW);
                                                                                   programming_color = 2; // YELLOW color
                                                                                   break;
                                                                                   
                                                                           case 2: digitalWrite(RED, LOW); digitalWrite(YELLOW, LOW); digitalWrite(GREEN, HIGH);
                                                                                   programming_color = 3; // GREEN color
                                                                                   break;
                                                                                   
                                                                           case 3: digitalWrite(RED, HIGH); digitalWrite(YELLOW, LOW); digitalWrite(GREEN, LOW);
                                                                                   click_count = 0;
                                                                                   programming_color = 1; // RED color
                                                                                   break;                        
                                                                   
                                                                          }
                                                     
                                                   }

              }

void doubleclick3() { type = 7; } 

void longPressStart3() {
                        type = 11; 

                        if (SWITCH_MODE == 0x05 || SWITCH_MODE == 0x01 || SWITCH_MODE == 0x06 || SWITCH_MODE == 0x02 || SWITCH_MODE == 0x04)
                        {
                          if (ON_OFF == 1) { ON_OFF = 0; digitalWrite(RED, LOW); digitalWrite(YELLOW, LOW); digitalWrite(GREEN, LOW);}
                           else { ON_OFF = 1; timer_tick = 0; i = 1;}
                        }

                       } 




//  MODE 2 - 0000 0101 - 0x05
//  MODE 3 - 0000 0001 - 0x01
//  MODE 4 - 0000 0110 - 0x06
//  MODE 5 - 0000 0010 - 0x02
//  MODE 6 - 0000 0100 - 0x04
void click4() {type = 4; NewEvent4 = 1; New_FLAG = 1; if (SWITCH_MODE == 0x03) { button4_click++;} }

void doubleclick4() {type = 8; } 

void longPressStart4() { 
                        type = 12; 

                        if (SWITCH_MODE == 0x05 || SWITCH_MODE == 0x01 || SWITCH_MODE == 0x06 || SWITCH_MODE == 0x02 || SWITCH_MODE == 0x04)
                        {                       
 
                         switch (programming_mode) { 
                                                    case 0: programming_mode = 1;                                 // fast flash ENTER to programming mode
                                                            digitalWrite(RED, HIGH); digitalWrite(YELLOW, HIGH); digitalWrite(GREEN, HIGH); delay(40000);
                                                            digitalWrite(RED, LOW); digitalWrite(YELLOW, LOW); digitalWrite(GREEN, LOW);    delay(40000);
                                                            digitalWrite(RED, HIGH); digitalWrite(YELLOW, HIGH); digitalWrite(GREEN, HIGH); delay(40000);
                                                            digitalWrite(RED, LOW); digitalWrite(YELLOW, LOW); digitalWrite(GREEN, LOW);    delay(40000);
                                                            digitalWrite(RED, HIGH); digitalWrite(YELLOW, HIGH); digitalWrite(GREEN, HIGH); delay(40000);
                                                            digitalWrite(RED, LOW); digitalWrite(YELLOW, LOW); digitalWrite(GREEN, LOW);    delay(99000);

                                                            
                                                            digitalWrite(RED, HIGH); digitalWrite(YELLOW, LOW); digitalWrite(GREEN, LOW);
                                                            programming_color = 1; // RED color
                                                            click_count = 0;
                                                            
                                                            break;

                                                    case 1: programming_mode = 0;                                // slow flashing OUT OF programming mode
                                                            digitalWrite(RED, LOW); digitalWrite(YELLOW, LOW); digitalWrite(GREEN, LOW);    delay(99000);
                                                            digitalWrite(RED, HIGH); digitalWrite(YELLOW, HIGH); digitalWrite(GREEN, HIGH); delay(90000);
                                                            digitalWrite(RED, LOW); digitalWrite(YELLOW, LOW); digitalWrite(GREEN, LOW);    delay(90000);
                                                            digitalWrite(RED, HIGH); digitalWrite(YELLOW, HIGH); digitalWrite(GREEN, HIGH); delay(90000);
                                                            digitalWrite(RED, LOW); digitalWrite(YELLOW, LOW); digitalWrite(GREEN, LOW);    delay(90000);
                                                            digitalWrite(RED, HIGH); digitalWrite(YELLOW, HIGH); digitalWrite(GREEN, HIGH); delay(90000);
                                                            digitalWrite(RED, LOW); digitalWrite(YELLOW, LOW); digitalWrite(GREEN, LOW);    delay(90000);
                                                            programming_color = 0; // no color for programming

                                                            EEPROMWritelong(0,eeprom_RED);
                                                            EEPROMWritelong(4,eeprom_YELLOW);
                                                            EEPROMWritelong(8,eeprom_GREEN);

                                                            timer_tick = 0; i = 1;
                                                            
                                                            break;

                                                  } // end of switch
                        }
                       } 


long EEPROMReadlong(long address) {
  long four = EEPROM.read(address);
  long three = EEPROM.read(address + 1);
  long two = EEPROM.read(address + 2);
  long one = EEPROM.read(address + 3);
  
  return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}

void EEPROMWritelong(int address, long value) {
  byte four = (value & 0xFF);
  byte three = ((value >> 8) & 0xFF);
  byte two = ((value >> 16) & 0xFF);
  byte one = ((value >> 24) & 0xFF);
  
  EEPROM.write(address, four);
  EEPROM.write(address + 1, three);
  EEPROM.write(address + 2, two);
  EEPROM.write(address + 3, one);
}
