
#include <TimerOne.h> 

//#include <Timer2.h>

int mux_ctrl_pins[3]={7,8,9};
int length_mux_ctrl_pins=sizeof(mux_ctrl_pins)/sizeof(int);
int hw_interrupt_pin=2;
int flag_hw_detected=0; 
int out=0;
int prevOut=0;
int u=0;
int freq=1000;
int chara[8]={0,0,0,0,0,0,0,0};
int charaOut=0;
void sw_ISR(){
  if(flag_hw_detected==0){
  int n=0;
    for(int k=0; k<3; k++){
      n=(u >> k) & 1;
      digitalWrite(mux_ctrl_pins[k], n);  
    } 
    u++;
    if(u>=8){
      u=0;
    }
    //Serial.println("1");
  }
  else{
    Timer1.stop();
    detachInterrupt(digitalPinToInterrupt(hw_interrupt_pin));
    u=0;
    int n=0;
    while(u<8){
      for(int k=0; k<3; k++){
        n=(u >> k) & 1;
        digitalWrite(7+k, n);
      }
      out=out+pow(2, u)*digitalRead(hw_interrupt_pin);
      u++;
    }
    
    out=1;
    while(out!=0){
    u=0;
    out=0;
    while(u<8){
      for(int k=0; k<3; k++){
        n=(u >> k) & 1;
        digitalWrite(7+k, n);
      }
      chara[u]=chara[u]+digitalRead(hw_interrupt_pin);
      out=out+pow(2, u)*digitalRead(hw_interrupt_pin);
      u++;
    }
    
    }
    charaOut=0;
    for(int i=0; i<8; i++){
      if(chara[i]!=0){
        chara[i]=1;
      }
      charaOut=charaOut+pow(2, i)*chara[i];
      chara[i]=0;
    }
    //Serial.print("charaOut: ");
    if(charaOut!=0){
      Keyboard.print(char(charaOut));
    }
    //end test
    //Keyboard.print(char(charaOut));
    prevOut=out;
    out=0;
    flag_hw_detected=0;
    attachInterrupt(digitalPinToInterrupt(hw_interrupt_pin), hw_ISR, RISING);
    
    Timer1.initialize(1000);
    Timer1.restart();
    }
}
void hw_ISR(){
  //Timer1.stop();
  //freq=1000;
  flag_hw_detected=1;
  Timer1.initialize(50000);
  Timer1.restart();
  
}

void setup() {
  for(int i=0; i<length_mux_ctrl_pins; i++){
    pinMode(mux_ctrl_pins[i], OUTPUT);
  }
  pinMode(hw_interrupt_pin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(hw_interrupt_pin), hw_ISR, RISING);
  Timer1.initialize(1000);
  Timer1.attachInterrupt(sw_ISR);
  Timer1.start();
  Keyboard.begin();
}

void loop(){
}
