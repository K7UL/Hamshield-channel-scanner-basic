/* HamShield Radio sketch
 *  K7UL
 *  for use with the hamshield library
 *  
 *  Current configuration:  Scans memory channels listed below.  Shows freq when scanning stops using arduino serial monitor on the ide.
 *  
 *  
 *  */


#include <HamShield.h>

#define PWM_PIN 3
#define RESET_PIN A3
#define SWITCH_PIN 2

HamShield radio;

int c = 0;
int show = 0;  // show is used to keep the display from flooding with data


// freq list to scan
// edit these for your area

unsigned long mems[] = { 144390,
                          145190, 145290, 145330, 145390,
                          146520, 146640, 
                          147060, 147340,
                          442500, 444250, 
                          0 };  // zero indicates end of the list


 
void setup() {
  // I copied most of the setup routine from the examples in the library
  pinMode (PWM_PIN, OUTPUT);
  digitalWrite (PWM_PIN, LOW);
  
  pinMode (SWITCH_PIN, INPUT_PULLUP);
  
  pinMode (RESET_PIN, OUTPUT);
  digitalWrite (RESET_PIN, HIGH);

  Serial.begin(9600);
  Serial.println("Radio status: ");
  int result = radio.testConnection();
  Serial.println(result, DEC);
  radio.initialize();
  delay(500);  // just for good measure
  
  radio.setModeReceive();
  radio.setSQHiThresh(-110);  // the closer to 0 the stronger the signal
  radio.setSQOn();
  radio.setVolume1(0x9);   // half volume
  radio.setVolume2(0x9);   // not sure why theres 2 of these
  radio.frequency(146520);
   
}

void loop() {
  int16_t rssi = 0;
  rssi = radio.readRSSI();  // read the radio 
                        //  Serial.println(rssi);  // print signal level
  if (rssi < -110)    
  {  // no signal
    radio.setSQOn();
    show = 0;       // see discription at top of file
                        //    Serial.println(rssi); // print signal level
    nextmemory();   // load radio with next memory
  }
  else
  {                    // signal is present
    if (show  < 1) {  
      show = 1;
      Serial.println(mems[c]);
    }
    radio.setSQOff();
  }
  delay(50);
}



void nextmemory() {
  c=c+1;
  if (mems[c] < 1)
  {
    c = 0;
  }
                    //    Serial.println("Freq: ");
                    //    Serial.println(mems[c]);
   radio.frequency(mems[c]);
}




