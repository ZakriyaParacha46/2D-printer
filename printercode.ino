// the setup function runs once when you press reset or power the board
#include <Servo.h>
#define speed 2

#define pixeldensity 35 //1pixel at 20* distance
#define res 32 

#define down 130
#define up 170
#define servodelay 150

bool pixelarray[res][res]={{0}};
Servo myservo;  // create servo object to control a servo

void setup() {
    Serial.begin(9600);

  // initialize digital pin LED_BUILTIN as an output.
    pinMode(LED_BUILTIN, OUTPUT);
    //mot1- yaxis
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    //mot2- xaxis
    pinMode(6, OUTPUT);
    pinMode(7, OUTPUT);
    //status led
    pinMode(8, OUTPUT); //red
    pinMode(9, OUTPUT); //green
    
    digitalWrite(9, HIGH);   
    myservo.attach(3);  // attaches the servo on pin 3 to the servo object
    myservo.write(up); 

  }

void stepout(int mot,int steps)
{
  //led on
  int dir_pin,stp_pin;
  if(mot==1){dir_pin=7;stp_pin=6;}
  else if(mot==2){dir_pin=5;stp_pin=4;}

  //if(curx>31 || curx<0 || cury>31 || cury<0){return;}//error checking

  digitalWrite(dir_pin, HIGH);   
  //steps  
  for(int i = 0; i < steps*pixeldensity; i++){
  digitalWrite(stp_pin, HIGH);
  delay(speed);                   
  digitalWrite(stp_pin, LOW);
  delay(speed);      
  }
  }


void stepin(int mot,int steps)
{
  int dir_pin,stp_pin;
  if(mot==1){dir_pin=7;stp_pin=6;}
  else if(mot==2){dir_pin=5;stp_pin=4;}

  //if(curx>31 || curx<0 || cury>31 || cury<0){return;}//error checking

  digitalWrite(dir_pin, LOW);   
  //steps  
  for(int i = 0; i < steps*pixeldensity; i++){
  digitalWrite(stp_pin, HIGH);
  delay(speed);                   
  digitalWrite(stp_pin, LOW);
  delay(speed);                   
  }
}

void dot()
{
          myservo.write(down);       
          delay(servodelay);
          myservo.write(up);       
          delay(servodelay);
}

//debug code
void printarr()
{
  for (int i=0;i<32;i++){
    for (int j=0;j<32;j++){
        Serial.print(pixelarray[i][j]);
        Serial.print(" ");
  }
    Serial.println();
    }
}


// the loop function runs over and over again forever
void loop() {
  int i= 0;
  int j= 0;
  //waiting for the serial data to be loaded
  Serial.println("Enter the data!");

  while(i<32){
      while (Serial.available()) {
        // get the new byte: 
      char c= (char)Serial.read();
      for (int k=0;k<8;k++)
        {
        pixelarray[i][j] = ((c << k)  & 0x80);
        j++;
        if(j==32){j=0;i++;}
        }
      if(i==32){Serial.flush(); }
      
      }
  }

  //printing out the loaded data
  {
  //debug code for manual input
  //Serial.println("\nyour array is:");
  //printarr();
  //Serial.println("Ready to print!!");
  }

 
  //red led for printing 
  digitalWrite(9, LOW);
  digitalWrite(8,HIGH);

  //acutual printing algorithm 
    for (int i= 0; i < res; i++)
    {
      for (int j = 0; j <= res-1; j++){
          if (pixelarray[i][res-1-j]){dot();}
          if(j!=31)stepout(2,1); 
          } 
      
      stepout(1,1);    
      i++; 

      for (int k= res-1; k >= 0; k--){
          if (pixelarray[i][res-1-k]){dot();}
          if(k!=0)stepin(2,1);  
          }

      stepout(1,1);
     }
    stepin(1,31); 
    //red led for printing 
    Serial.println("Done Printing");
    digitalWrite(9, HIGH);
    digitalWrite(8,LOW);
}
