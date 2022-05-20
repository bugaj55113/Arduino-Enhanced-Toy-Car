#include <SPI.h> 
#include <RFID.h>
#include<Servo.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        6 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 16 // Popular NeoPixel ring size

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 200 // Time (in milliseconds) to pause between pixels

#define trigPin 7
#define echoPin 8

//button1
int LEDState = 0;
int buttonPin = 0;
int buttonNew;
int buttonOld=1;

//button2
int LEDState2 = 0;
int buttonPin2 = 5;
int buttonNew2;
int buttonOld2=1;

//button3
int LEDState3 = 0;
int buttonPin3 = 4;
int buttonNew3;
int buttonOld3=1;

//button4
int LEDState4 = 0;
int buttonPin4 = 2;
int buttonNew4;
int buttonOld4=1;

int clearTemp=0;
int temp1=0;
int b=1;
int h=1;

RFID rfid(10, 9);       //D10:pin of tag reader SDA. D9:pin of tag reader RST 
unsigned char status; 
unsigned char str[MAX_LEN]; //MAX_LEN is 16: size of the array 

String accessGranted [2] = {"121211107718", "19612012715"};  //RFID serial numbers to grant access to
int accessGrantedSize = 2;                                //The number of serial numbers

Servo lockServo;                //Servo for locking mechanism
int lockPos = 15;               //Locked position limit
int unlockPos = 75;             //Unlocked position limit
boolean locked = true;

int redLEDPin = A0;


void setup() 
{ 
  
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  
    // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  
      // initialize the pushbutton2 pin as an input:
  pinMode(buttonPin2, INPUT);
  
        // initialize the pushbutton3 pin as an input:
  pinMode(buttonPin3, INPUT);
  
        // initialize the pushbutton4 pin as an input:
  pinMode(buttonPin4, INPUT);  
  
  Serial.begin(9600);     //Serial monitor is only required to get tag ID numbers and for troubleshooting
  SPI.begin();            //Start SPI communication with reader
  rfid.init();            //initialization 
  pinMode(redLEDPin, OUTPUT);     //LED startup sequence
  
  digitalWrite(redLEDPin, HIGH);
  delay(200);
  
  delay(200);
  digitalWrite(redLEDPin, LOW);
  delay(200);
  
  lockServo.attach(3);
  lockServo.write(lockPos);         //Move servo into locked position
  Serial.println("Place card/tag near reader...");

  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
} 

void loop() 
{ 
long czas;

digitalWrite(trigPin, HIGH);
delay(10);
digitalWrite(trigPin, LOW);

czas = pulseIn(echoPin, HIGH);
Serial.println(czas/58);

delay(0);
  
  int r=0;
  int j=1;
  int z_test_kierunkowskazy=3;
  int k=1;
  
  if(clearTemp==0){
    for(int i=0;i<=16;i++)
    {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
      pixels.show();
    }
    clearTemp=1;
  }

long distance=czas/58;
if(distance<15 && distance>10){
    digitalWrite(redLEDPin, HIGH);      //Red LED sequence
    delay(500);
    digitalWrite(redLEDPin, LOW);
    delay(500);
  }
if(distance<10 && distance>5){
    digitalWrite(redLEDPin, HIGH);      //Red LED sequence
    delay(100);
    digitalWrite(redLEDPin, LOW);
    delay(100);
  }
if(distance<5 && distance>0){
    digitalWrite(redLEDPin, HIGH);      //Red LED sequence
    delay(10);
    digitalWrite(redLEDPin, LOW);
    delay(10);
  }  


  
  //POWITANIE
  if (rfid.findCard(PICC_REQIDL, str) == MI_OK)   //Wait for a tag to be placed near the reader
  { 
    Serial.println("Card found"); 
    String temp = "";                             //Temporary variable to store the read RFID number
    if (rfid.anticoll(str) == MI_OK)              //Anti-collision detection, read tag serial number 
    { 
      Serial.print("The card's ID number is : "); 
      for (int i = 0; i < 4; i++)                 //Record and display the tag serial number 
      { 
        temp = temp + (0x0F & (str[i] >> 4)); 
        temp = temp + (0x0F & str[i]); 
      } 
      Serial.println (temp);
      checkAccess (temp);     //Check if the identified tag is an allowed to open tag
    } 
    rfid.selectTag(str); //Lock card to prevent a redundant read, removing the line will make the sketch read cards continually
  }
  rfid.halt();

  //KONIEC POWITANIA

  //PRZÓD STAŁE BIAŁE
  if(temp1!=0){
  if(b==1){
  pixels.setPixelColor(8, pixels.Color(150, 150, 250));
  pixels.setPixelColor(9, pixels.Color(150, 150, 250));
  pixels.setPixelColor(10, pixels.Color(0, 0, 0));
  pixels.setPixelColor(11, pixels.Color(0, 0, 0));
  pixels.setPixelColor(12, pixels.Color(150, 150, 250));
  pixels.setPixelColor(13, pixels.Color(150, 150, 250));
    b=2;
  }
  
  //TYŁ STAŁE CZERWONE   
  pixels.setPixelColor(3, pixels.Color(150, 0, 0));
  pixels.setPixelColor(4, pixels.Color(150, 0, 0));
    pixels.show();
    delay(70);
    pixels.setPixelColor(2, pixels.Color(150, 0, 0));
  pixels.setPixelColor(5, pixels.Color(150, 0, 0));
    pixels.show();
    delay(70);
      pixels.setPixelColor(1, pixels.Color(150, 0, 0));
  pixels.setPixelColor(6, pixels.Color(150, 0, 0));
    pixels.show();
    delay(70);
      pixels.setPixelColor(0, pixels.Color(150, 0, 0));
  pixels.setPixelColor(7, pixels.Color(150, 0, 0));
    pixels.show();
    delay(70); 
  }


  
  //KIERUNEK W LEWO
  buttonNew2 = digitalRead(buttonPin2);
  if(buttonOld2==0 && buttonNew2==1){
    if(LEDState2==0){
   
      r=0;
      while(r<3){  //kierunkowskaz ile razy 
  pixels.clear();
    k=1;
    j=1;
    int a=9;
  for(int i=3; i>=0; i--) {
    
    pixels.setPixelColor(i, pixels.Color(255, 90, 0));
  pixels.setPixelColor(a, pixels.Color(250, 90, 0));
    if(i==2){
    a=8;
    }
    pixels.show();   // Send the updated pixel colors to the hardware.

    delay(DELAYVAL); // Pause before next pass through loop
    }
        r++;
  }
      LEDState2=1;
    }
    else{
      LEDState2=0;
    }
    
    //WŁĄCZ DŁUGIE LUB KRÓTKIE PO KIERUNKU
    if(h==1){
    pixels.setPixelColor(8, pixels.Color(150, 150, 250));
    pixels.setPixelColor(9, pixels.Color(150, 150, 250));
    pixels.setPixelColor(10, pixels.Color(0, 0, 0));
    pixels.setPixelColor(11, pixels.Color(0, 0, 0));
    pixels.setPixelColor(12, pixels.Color(150, 150, 250));
    pixels.setPixelColor(13, pixels.Color(150, 150, 250)); 
    pixels.show(); 
    }
    else{
    pixels.setPixelColor(8, pixels.Color(150, 150, 250));
    pixels.setPixelColor(9, pixels.Color(150, 150, 250));
    pixels.setPixelColor(10, pixels.Color(0, 0, 250));
    pixels.setPixelColor(11, pixels.Color(0, 0, 250));
    pixels.setPixelColor(12, pixels.Color(150, 150, 250));
    pixels.setPixelColor(13, pixels.Color(150, 150, 250)); 
    pixels.show(); 
    }
    
  }
    buttonOld2 = buttonNew2;
  
  
  
  
    
  //KIERUNEK W PRAWO
  buttonNew3 = digitalRead(buttonPin3);
  if(buttonOld3==0 && buttonNew3==1){
    if(LEDState==0){

      
      r=0;
      while(r<3){ //kierunkowskaz ile razy 
        
    pixels.clear();
        int a=12;
        
        for(int s=4;s<=7;s++){
          
  pixels.setPixelColor(s, pixels.Color(250, 90, 0));
    pixels.setPixelColor(a, pixels.Color(250, 90, 0));
    if(s==5){
    a=13;
    }
          
    pixels.show();   // Send the updated pixel colors to the hardware.

    delay(DELAYVAL); // Pause before next pass through loop
      }
        r++;
  }
  
      LEDState3=1;
    }
    else{
      LEDState3=0;
    }
    
    //WŁĄCZ DŁUGIE LUB KRÓTKIE PO KIERUNKU
    if(h==1){
    pixels.setPixelColor(8, pixels.Color(150, 150, 250));
    pixels.setPixelColor(9, pixels.Color(150, 150, 250));
    pixels.setPixelColor(10, pixels.Color(0, 0, 0));
    pixels.setPixelColor(11, pixels.Color(0, 0, 0));
    pixels.setPixelColor(12, pixels.Color(150, 150, 250));
    pixels.setPixelColor(13, pixels.Color(150, 150, 250)); 
    pixels.show();
    }
    else{
    pixels.setPixelColor(8, pixels.Color(150, 150, 250));
    pixels.setPixelColor(9, pixels.Color(150, 150, 250));
    pixels.setPixelColor(10, pixels.Color(0, 0, 250));
    pixels.setPixelColor(11, pixels.Color(0, 0, 250));
    pixels.setPixelColor(12, pixels.Color(150, 150, 250));
    pixels.setPixelColor(13, pixels.Color(150, 150, 250)); 
    pixels.show(); 
    }
    
    
  }
    buttonOld3 = buttonNew3;


  
  //ŚWIATŁA DŁUGIE/KRÓTKIE
  buttonNew4 = digitalRead(buttonPin4);
  if(buttonOld4==0 && buttonNew4==1){
    if(LEDState4==0){
            //KRÓTKIE
    pixels.setPixelColor(8, pixels.Color(150, 150, 250));
    pixels.setPixelColor(9, pixels.Color(150, 150, 250));
    pixels.setPixelColor(10, pixels.Color(0, 0, 0));
    pixels.setPixelColor(11, pixels.Color(0, 0, 0));
    pixels.setPixelColor(12, pixels.Color(150, 150, 250));
    pixels.setPixelColor(13, pixels.Color(150, 150, 250)); 
    pixels.show();
      
       h=1;
      
      LEDState4=1;
    }
    else{   //DŁUGIE
      
    pixels.setPixelColor(8, pixels.Color(150, 150, 250));
    pixels.setPixelColor(9, pixels.Color(150, 150, 250));
    pixels.setPixelColor(10, pixels.Color(0, 0, 250));
    pixels.setPixelColor(11, pixels.Color(0, 0, 250));
    pixels.setPixelColor(12, pixels.Color(150, 150, 250));
    pixels.setPixelColor(13, pixels.Color(150, 150, 250)); 
    pixels.show(); 
      
       h=2;
      
      LEDState4=0;
    }
  }
    buttonOld4 = buttonNew4;
  
      
}

void checkAccess (String temp)    //Function to check if an identified tag is registered to allow access
{
  boolean granted = false;
  for (int i=0; i <= (accessGrantedSize-1); i++)    //Runs through all tag ID numbers registered in the array
  {
    if(accessGranted[i] == temp)            //If a tag is found then open/close the lock
    {
      Serial.println ("Access Granted");
      granted = true;
      if (locked == true)         //If the lock is closed then open it
      {
          lockServo.write(unlockPos);
          locked = false;
      }
      else if (locked == false)   //If the lock is open then close it
      {
          lockServo.write(lockPos);
          locked = true;
      }

temp1++;
      
    pixels.clear();
      pixels.show();
  //PRZÓD    
  pixels.setPixelColor(8, pixels.Color(0, 0, 0));
  pixels.setPixelColor(9, pixels.Color(0, 0, 0));
  pixels.setPixelColor(12, pixels.Color(0, 0, 0));
  pixels.setPixelColor(13, pixels.Color(0, 0, 0)); 
    pixels.show();
    delay(200);     
  pixels.setPixelColor(8, pixels.Color(250, 250, 250));
  pixels.setPixelColor(9, pixels.Color(250, 250, 250));
  pixels.setPixelColor(12, pixels.Color(250, 250, 250));
  pixels.setPixelColor(13, pixels.Color(250, 250, 250)); 
    pixels.show();
    delay(200); 
  pixels.setPixelColor(8, pixels.Color(0, 0, 0));
  pixels.setPixelColor(9, pixels.Color(0, 0, 0));
  pixels.setPixelColor(12, pixels.Color(0, 0, 0));
  pixels.setPixelColor(13, pixels.Color(0, 0, 0)); 
    pixels.show();
    delay(200);  
  pixels.setPixelColor(8, pixels.Color(255, 255, 255));
  pixels.setPixelColor(9, pixels.Color(255, 255, 255));
  pixels.setPixelColor(12, pixels.Color(255, 255, 255));
  pixels.setPixelColor(13, pixels.Color(255, 255, 255));    
    pixels.show();
    delay(200); 
      
  //TYŁ
  pixels.setPixelColor(3, pixels.Color(150, 0, 0));
  pixels.setPixelColor(4, pixels.Color(150, 0, 0));
    pixels.show();
    delay(150);
    pixels.setPixelColor(2, pixels.Color(150, 0, 0));
  pixels.setPixelColor(5, pixels.Color(150, 0, 0));
    pixels.show();
    delay(150);
      pixels.setPixelColor(1, pixels.Color(150, 0, 0));
  pixels.setPixelColor(6, pixels.Color(150, 0, 0));
    pixels.show();
    delay(150);
      pixels.setPixelColor(0, pixels.Color(150, 0, 0));
  pixels.setPixelColor(7, pixels.Color(150, 0, 0));
    pixels.show();
    delay(150);   
    
  pixels.setPixelColor(7, pixels.Color(0, 0, 0));
  pixels.setPixelColor(0, pixels.Color(0, 0, 0));
    pixels.show();
    delay(150);
    pixels.setPixelColor(6, pixels.Color(0, 0, 0));
  pixels.setPixelColor(1, pixels.Color(0, 0, 0));
    pixels.show();
    delay(150);
      pixels.setPixelColor(5, pixels.Color(0, 0, 0));
  pixels.setPixelColor(2, pixels.Color(0, 0, 0));
    pixels.show();
    delay(150);
      pixels.setPixelColor(4, pixels.Color(0, 0, 0));
  pixels.setPixelColor(3, pixels.Color(0, 0, 0));
    pixels.show();
    delay(150);
    }
  }
  if (granted == false)     //If the tag is not found
  {
    Serial.println ("Access Denied");
    digitalWrite(redLEDPin, HIGH);      //Red LED sequence
    delay(200);
    digitalWrite(redLEDPin, LOW);
    delay(200);
    digitalWrite(redLEDPin, HIGH);
    delay(200);
    digitalWrite(redLEDPin, LOW);
    delay(200);
  }
}
