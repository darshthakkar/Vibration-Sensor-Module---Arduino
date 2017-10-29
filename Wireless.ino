

#include <SPI.h>
#include "RF24.h"

byte addresses[][6] = {"1Node","2Node"};


/***      Set this radio as radio number 0 or 1         ***/
bool radioNumber = 1;

//radioNumber=1 => Trasmitter; radioNumber=0=>Reciever
/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(9,10);

// Used to control whether this node is sending or receiving
bool role = 0;

/**
* Create a data structure for transmitting and receiving data
* This allows many variables to be easily sent and received in a single transmission
*/

struct dataStruct{
  int Sum;
  int Read;
  int SamplingCount;
  int scale;
  unsigned long _micros;
  int rec;
} ;

dataStruct myData;

//myData.Sum = 0;
//myData.Read = 0;
//myData.SamplingCount = 0;
//myData.scale = 0;
int iCount =0;
int Sum2 = 0;
int SensePin = A2;
int sr = 150;
int sc = 0;


void setup() 
{
  myData.Sum = 0;
  myData.Read = 0;
  myData.SamplingCount = 0;
  myData.scale = 0;
  Serial.begin(115200);
  //Serial.println(F("RF24/examples/GettingStarted_HandlingData"));
  Serial.println(F("*** PRESS 'T' to begin transmitting to the other node"));
  
  pinMode(SensePin, INPUT);
  
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  
  // Open a writing and reading pipe on each radio, with opposite addresses
  if(radioNumber){
    radio.openWritingPipe(addresses[1]);
    radio.openReadingPipe(1,addresses[0]);
  }else{
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1,addresses[1]);
  }
  
  //Start the radio listening for data
  radio.startListening();
}




void loop() {
  
  
/****************** Ping Out Role ***************************/  
if (role == 1)  {
    
    Serial.print("\n Your Experience is Begining to start in few seconds");
    delay(1000);
    Serial.print("\n Start now : \n");
    delay(3000);
//    myData.SamplingCount = 0;
      for(iCount=0;iCount<sr;iCount++)
    {
       myData.Read = digitalRead(SensePin);
       Serial.print(myData.Read);
       Serial.print(',');
       //myData.Sum = myData.Sum + myData.Read;
       Sum2 = Sum2 + myData.Read;
       myData.SamplingCount = myData.SamplingCount + 1;
       Serial.print("    \t    ");
       sc=sc+1;
       Serial.println(sc);
       Serial.print("\n"); 
       delay(100);
       
       radio.stopListening();                                    // First, stop listening so we can talk.
       Serial.print(F("Now sending"));
       myData._micros = micros();
       myData.rec = 0;
       myData.scale = 0;
       if (!radio.write( &myData, sizeof(myData) )){
          Serial.println(F("failed"));
       }
        
      radio.startListening();                                    // Now, continue listening
    
      unsigned long started_waiting_at = micros();               // Set up a timeout period, get the current microseconds
      boolean timeout = false;                                   // Set up a variable to indicate if a response was received or not
    
      while ( ! radio.available() ){                             // While nothing is received
        if (micros() - started_waiting_at > 200000 ){            // If waited longer than 200ms, indicate timeout and exit while loop
            timeout = true;
            break;
        }        
      }
        
      if ( timeout ){                                             // Describe the results
          Serial.println(F("Failed, response timed out."));
      }
      else
      
      {
                                                                // Grab the response, compare, and send to debugging spew
        radio.read( &myData, sizeof(myData) );
        //unsigned long time = micros();
        
        // Spew it
        Serial.print(F(" -- Sent "));
        Serial.print(F(", Got response: "));
        Serial.println(myData.rec);
        //Serial.print(F(", Round-trip delay "));
        //Serial.print(time-myData._micros);
        //Serial.print(F(" microseconds Value "));
        //Serial.println(myData.value);
      }
    }
    
    
    if(Sum2<=15)
    {
      Serial.print("Your Vibrations Is of Scale : 1");
      myData.scale=1;
    }  
   else if(Sum2<=30)
    {
      Serial.print("Your Vibrations Is of Scale : 2");
      myData.scale=2;
    }
   else if(Sum2<=45)
    {
      Serial.print("Your Vibrations Is of Scale : 3");
      myData.scale=3;
    }   
   else if(Sum2<=60)
    {
      Serial.print("Your Vibrations Is of Scale : 4");
      myData.scale=4;
    }   
   else if(Sum2<=75)
    {
      Serial.print("Your Vibrations Is of Scale : 5");
      myData.scale=5;
    }
   else if(Sum2<=90)
    {
      Serial.print("Your Vibrations Is of Scale : 6");
      myData.scale=6;
    }
   else if(Sum2<=105)
    {
      Serial.print("Your Vibrations Is of Scale : 7");
      myData.scale=7;
    }
   else if(Sum2<=120)
    {
      Serial.print("Your Vibrations Is of Scale : 8");
      myData.scale=8;
    }
   else if(Sum2<=135)
    {
      Serial.print("Your Vibrations Is of Scale : 9");
      myData.scale=9;
    }
   else
    {
      Serial.print("Your Vibrations Is of Scale : 10");
      myData.scale=10;
    }
   delay(1000);
   Serial.print("\n");
   
   iCount=0;
   myData.Sum=0;
   Sum2 = 0;
   radio.stopListening();   
       radio.write(&myData, sizeof(myData));
   radio.startListening();                                    
   
   // Try again 1s later
    delay(1000);
  }



/****************** Pong Back Role ***************************/

  if ( role == 0 )
  {
    
    if( radio.available()){                                 // Variable for the received timestamp
      while (radio.available()) {                          // While there is data ready
        radio.read( &myData, sizeof(myData) );             // Get the payload
      }
     
      radio.stopListening();                               // First, stop listening so we can talk  
      myData.rec = 1;                                    // Increment the float value
      radio.write( &myData, sizeof(myData) );              // Send the final one back.      
      radio.startListening();                           // Now, resume listening so we catch the next packets.     
      if(myData.scale==0)
      {
       Serial.print(myData.Read);
       Serial.print(',');
       Serial.print("    \t    ");
       sc=sc+1;
       Serial.println(sc);
       //Serial.print(myData.SamplingCount);
       Serial.println("\n"); 
      }
       if(myData.scale!=0)
       {
             Serial.print("Your Vibration is of Scale : ");
             Serial.println(myData.scale);
       }
      //Serial.print(F("Sent response "));
      //Serial.print(myData._micros);  
      //Serial.print(F(" : "));
      //Serial.println(myData.value);
   }
 }




/****************** Change Roles via Serial Commands ***************************/

  if ( Serial.available() )
  {
    char c = toupper(Serial.read());
    if ( c == 'T' && role == 0 ){      
      Serial.println(F("*** CHANGING TO TRANSMIT ROLE -- PRESS 'R' TO SWITCH BACK"));
      role = 1;                  // Become the primary transmitter (ping out)
    
   }
   else if ( c == 'R' && role == 1 ){
       Serial.println(F("*** CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK"));      
       role = 0;                // Become the primary receiver (pong back)
       radio.startListening();
       
    }
  }


} // Loop
