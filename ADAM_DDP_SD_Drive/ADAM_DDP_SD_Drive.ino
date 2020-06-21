//*****************************************************************************************
//
// Coleco ADAM SD DDP source code version 1.4
//07/28/2019 - Start, designed DDP riser interface PCB to TAP signals. Waiting for PCB.
//09/09/2019 - Was able to read a sync byte today.
//11/26/2019 - Successfully read block header from tape.
//11/27/2019 - Successful read of data from tape 6801.
//12/16/2019 - Successful write to tape 6801.
//             Successful read of directory - intermittent.
//12/23/2019 - Successful read of directory - consistent.
//             Unable to load multi-block program from cold boot.
//12/29/2019 - Stabilized block loading code.
//             Consistent header reads on first try.
//             Consistent block reads on first try.
//             Multi-block reads successful.
//             Multi-block program load from cold boot.
//01/01/2020 - Fixed bug when loading tracks.
//           - Initial release version 1.0
//01/12/2020 - Version 1.1
//           - Fixed SD card detection and display. Added option to insert card and press 'Mount' button to reinit SD.
//           - Added STOP detection to prevent SD DDP from moving when physical DDP is in motion.
//           - Modified ADAM_DDP_SD_Drive, Forward, ProcessButtons, SDCardSetup, Stop.
//01/13/2020 - Version 1.2
//           - Fixed problem with reading blocks after FastFoward.
//05/27/2020 - Version 1.3
//           - Changed virtual disk format from right directory (GW) to center directory (HE) which now allows for the SD DDP to work with a real DDP.
//           - Successful block copy from real DDP with both left and center directory format to SD DDP.
//           - Added SN74LS7406N to ensure HI-Z state when SD DDP is stopped.
//06/17/2020 - Version 1.4
//           - Added "Tape Mode" button to allow the use of both center (HE) and right directory (GW) real DDPs with the SD DDP.
//             Whatever format tape you have in a real DDP drive you should also select on the SD DDP using the "Tape Mode" button.
//*****************************************************************************************
//
// Emulates ADAM Digital Data Drive (DDP)
// Full Compatibility: Works with all existing Adams. It bridges the ADAM Tape 6801 to
//    the FAT16/FAT32 file system on the SD Card. This means no software to install on the ADAM.
// Drive Emulation: Acts as a single ADAM DDP drive. Because of hard coded timing in the ADAM the SD DDP
//    block load speed is equivalent to a real DDP. The SD DDP is a little faster at fast forwarding and
//    rewinding.
// Disk Selection: Selects which of the 200 "ddp tapes" to use. This is selectable by using the buttons.
// Disks are Files: These "ddp tapes" are simply files on the SD Card, and can be manipulated as such
//    when the SD card is plugged in to a PC/Mac.
// Existing Disks: Works with .ddp images found on the Internet. Put the SD Card in a
//    PC/Mac and copy the files on to the SD card. Put the SD Card in to the ADAM SD DDP and go!
//
//*****************************************************************************************
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <util/delay.h>
#include <SPI.h>
#include <SdFat.h>
#define  GPIO2_PREFER_SPEED  1
#include <DIO2.h>               // include the fast I/O 2 functions
#define SCREEN_WIDTH 128        // OLED display width, in pixels
#define SCREEN_HEIGHT 64        // OLED display height, in pixels
#define OLED_RESET     8        // OLED reset pin
#define MODE          2         //read/write mode pin 2-7 (INT4)
#define STOP          3         //stop indicator pin 1-4 (INT5)
#define STATUSLED     13        //13 is the internal LED on the Mega
#define TXpin         18        //pin to transmit to tape 6801 pin 2-5  TX communication pin. (18 = PORTD = PD3).
#define RXpin         19        //pin to receive from tape 6801 pin 2-1 RX communication pin. (19 = PORTD = INT2 = PD2).
#define REVERSE       32        //move backwards pin 1-2
#define FORWARD       34        //move forward pin 1-3
#define SPEED         36        //speed select pin 1-5
#define MSENSE        38        //motion sense pin 1-7
#define TAPEIN        40        //tape indicator pin 1-9
#define TRACK         42        //track a/b pin 2-2
//↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
//↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓   Only modify the following variables   ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
const unsigned int maxfiles = 200;         // The maximum number of file names to load into the directory index array.
const unsigned int namelength = 100;       // Length of file name to display.
//↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑
//↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑
//oled display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
const int chipSelect = 10;                 // Chip select (CS) pin for the SD Card, (53)
byte refreshscreen = 1;                    // Flag to refresh the LED display
String motor_status[] = {"STOP", " >  ", " <  ", " >> ", " << "};
int motorstatus = 0;
String tapemode = "CD";

//arrays for sending and receiving data
byte headerdata[20];                       //array for header data to be sent to tape 6801
byte blockdata[1040];                      //array for 1K of block data read from sd to be sent to tape 6801
byte crcdata[505];                         //array for calculated checksum for 1K of data to be sent to tape 6801
//byte writedata[1040];                    //and for data received from tape 6801 to be written
                                           //↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
                                           //Format of data tape 6801 sends:
                                           //0 byte | 0 byte | 0 byte | 0 byte | SYNC byte (16H) | DATA 1024 bytes | 0 byte | 0 byte | SYNC Byte (16H) | CRC High Byte | CRC Low byte | 0 byte
                                           //↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑
//sd card variables
SdFat sd;                                  // Setup SD Card
SdFile file;                               // Setup SD Card
int sd_status = 0;
unsigned int filesindex[maxfiles + 1];     // Index for the files on the SD card
unsigned int numberoffiles = 0;            // The number of files on the SD card
unsigned int currentfile = 1;              // The current file index number that being displayed
String ddpfilename = "none";               //current ddp image filename to be displayed
int ddpregisterIt = 0;                     //has a ddp image been mounted
unsigned int ddpfileIndex = 0;             //current ddp image sd index number
unsigned int ddpfileNumber = 0;            //current ddp image file number

//led pin
GPIO_pin_t STATUSLEDpin = DP13;

//tape drive pins
GPIO_pin_t MODEpin = DP2;
GPIO_pin_t STOPpin = DP3;
GPIO_pin_t TX_pin = DP18;
GPIO_pin_t RX_pin = DP19;
GPIO_pin_t REVERSEpin = DP32;
GPIO_pin_t FORWARDpin = DP34;
GPIO_pin_t SPEEDpin = DP36;
GPIO_pin_t MSENSEpin = DP38;
GPIO_pin_t TAPEINpin = DP40;
GPIO_pin_t TRACKpin = DP42;

int readSTOP;
int readSPEED;
int readFORWARD;
int readREVERSE;
int readTRACK;
int readTAPEIN;

//flags
bool MOTORMOTION = false;
bool FORWARDMOTION = false;
bool REVERSEMOTION = false;
bool STOPPED = true;
bool NOTDONE = true;
bool UPDATESTATUS = false;
bool READSTOP = false;
bool TAPEGWMODE = false;

//butons
int reading;
//cycle files up button
int buttonUPpin = 22;
GPIO_pin_t bUPpin = DP22;
int buttonstateUP;
int lastbUPstate = LOW;
unsigned long lastDebounceTimeUP = 0;      // the last time the output pin was toggled
//cycle files down button
int buttonDOWNpin = 24;
GPIO_pin_t bDOWNpin = DP24;
int buttonstateDOWN;
int lastbDOWNstate = LOW;
unsigned long lastDebounceTimeDOWN = 0;    // the last time the output pin was toggled
//mount unmount/file
int buttonMOUNTpin = 26;
GPIO_pin_t bMOUNTpin = DP26;
int buttonstateMOUNT;
int lastbMOUNTstate = LOW;
unsigned long lastDebounceTimeMOUNT = 0;   // the last time the output pin was toggled
//mode toggle between GW and HE tape formats
int buttonMODEpin = 28;
GPIO_pin_t bMODEpin = DP28;
int buttonstateMODE;
int lastbMODEstate = LOW;
unsigned long lastDebounceTimeMODE = 0;   // the last time the output pin was toggled
unsigned long debounceDelay = 50;          // the debounce time; increase if the output flickers

//timers
unsigned long CommandDelay = 0;
unsigned long LastCommandTime = 0;
unsigned long CurrentTime = 0;
unsigned long TrackTimer = 0;

int CurrentBlock = 128;
int PreviousBlock = 1;
unsigned int CurrentTrack = 1;
int Direction = 0;
byte clockbyte = 1;
byte xorclock = 1;
  
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  //set pin modes
  //set RX and TX pins
  pinMode(RXpin,INPUT);               // Setup RXPin 19 PD2 to input
  EIFR = bit (INTF2);                 // Clear flag for interrupt 2

  pinMode(TXpin,OUTPUT);
  digitalWrite(TXpin, LOW);
  
  //set LED pin
  pinMode(STATUSLED,OUTPUT);          // Set the status LED as output
  digitalWrite(STATUSLED,LOW);        // Turn off the status LED

  //set SD card pin
  pinMode(10, OUTPUT);                // CS for SD card
  digitalWrite(10, HIGH);             

  //set tape indicator pins
  pinMode(REVERSE, INPUT);
  pinMode(FORWARD, INPUT);
  pinMode(STOP, INPUT);
  pinMode(SPEED, INPUT);
  pinMode(TRACK, INPUT);
  pinMode(MODE, INPUT);
  pinMode(MSENSE, OUTPUT);
  digitalWrite(MSENSE, LOW);
  pinMode(TAPEIN, OUTPUT);
  digitalWrite(TAPEIN, HIGH);

  //set button pins
  pinMode(buttonUPpin, INPUT_PULLUP);
  pinMode(buttonDOWNpin, INPUT_PULLUP);
  pinMode(buttonMOUNTpin, INPUT_PULLUP);
  
  //start led display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))      // Address for Adafruit 128x64 OLED is 0x3D
  {
    Serial.println(F("SSD1306 allocation failed"));
  }
  display.display();
  delay(2000);
  Serial.println("Coleco ADAM SD DDP"); 
  Serial.println("Starting up...");
    
  //initialize sd card
  reinitsd:
  sd_status = SDCardSetup();
  while (sd_status == 1)
  {
    reading = digitalRead2f(bMOUNTpin);
    if (reading == HIGH)
    {
      goto reinitsd;
    }
  }
  
  LastCommandTime = millis();                        // Setup initial time for last command in ms
}

void loop() {
  // put your main code here, to run repeatedly:

  readSTOP = digitalRead2f(STOPpin);
  readSPEED = digitalRead2f(SPEEDpin);
  readFORWARD = digitalRead2f(FORWARDpin);
  readREVERSE = digitalRead2f(REVERSEpin);
  readTRACK = digitalRead2f(TRACKpin);
  readTAPEIN = digitalRead2f(TAPEINpin);
    
  if (UPDATESTATUS)
  {
    Update_Motor_Status();
    UPDATESTATUS = false;
  }
    
  if (readTRACK == HIGH) //track A
  {
    if (CurrentTrack == 0)
    {
      CurrentBlock = 128 + CurrentBlock;
      CurrentTrack = 1;
      LoadBlock(CurrentBlock,ddpfileIndex);
      //Serial.print("Current Block= ");
      //Serial.println(CurrentBlock);
      //Serial.print("Current Track= ");
      //Serial.println(CurrentTrack);
    }
  }
  else                            //track B
  {
    if (CurrentTrack == 1)
    {
      CurrentBlock = CurrentBlock - 128;
      CurrentTrack = 0;
      LoadBlock(CurrentBlock,ddpfileIndex);
      //Serial.print("Current Block= ");
      //Serial.println(CurrentBlock);
      //Serial.print("Current Track= ");
      //Serial.println(CurrentTrack);
    }
  }

  if (readTAPEIN == LOW && readSTOP == LOW) //STOP pin HIGH prevents tape from moving and data output when other drive is active
  {
    if ((readFORWARD == LOW) && (readSPEED == HIGH))
    {
      LastCommandTime = millis();
      attachInterrupt(digitalPinToInterrupt(3), Stop, RISING);
      READSTOP = false;
      STOPPED = false;
      MOTORMOTION = true;
      REVERSEMOTION = false;
      FORWARDMOTION = false;
      motorstatus = 3;
      Update_Motor_Status();
      PORTD |= 0b10000000;                        // Set MSENSE PD7 = HIGH
      FastForward();
    }
    else if (readFORWARD == LOW)
    {
      LastCommandTime = millis();
      //Serial.println("B");
      if (FORWARDMOTION == true)
      {
          delay(200);
          Forward();
          goto breakout;
      }
      attachInterrupt(digitalPinToInterrupt(2), ReadFrom6801, LOW);
      attachInterrupt(digitalPinToInterrupt(3), Stop, RISING);
      if (readTRACK == HIGH) //track A
      {
       if (CurrentTrack == 0)
        {
          CurrentBlock = 128 + CurrentBlock;
          CurrentTrack = 1;
          LoadBlock(CurrentBlock,ddpfileIndex);
        }
      }
      else                            //track B
      {
        if (CurrentTrack == 1)
        {
          CurrentBlock = CurrentBlock - 128;
          CurrentTrack = 0;
          LoadBlock(CurrentBlock,ddpfileIndex);
        }
      }
      READSTOP = false;
      STOPPED = false;
      MOTORMOTION = true;
      FORWARDMOTION = true;
      REVERSEMOTION = false;
      PORTD |= 0b10000000;                // Set MSENSE PD7 = HIGH
      motorstatus = 1;
      Update_Motor_Status();
      delay(210);
      Forward();
    }
    else if (readREVERSE == LOW)
    {
      LastCommandTime = millis();
      if (REVERSEMOTION == true)
      {
              Rewind();
              goto breakout;
      }
      attachInterrupt(digitalPinToInterrupt(3), Stop, RISING);
      READSTOP = false;
      STOPPED = false;
      MOTORMOTION = true;
      REVERSEMOTION = true;
      FORWARDMOTION = false;
      motorstatus = 2;
      Update_Motor_Status();
      PORTD |= 0b10000000;                        // Set MSENSE PD7 = HIGH
      delay(110);
      Rewind();
    }
    else
    {
      if ((FORWARDMOTION == true) || (REVERSEMOTION == true))
      {
        UPDATESTATUS = true;
        MOTORMOTION = false;
        FORWARDMOTION = false;
        REVERSEMOTION = false;
        motorstatus = 0;
        digitalWrite(TXpin, LOW);
        PORTD = PORTD & 0b01111111;                 // Set MSENSE PD7 = LOW
      }
    }
  }
breakout:
  CommandDelay = millis() - LastCommandTime;
  if (CommandDelay >= 500){                // Has it been at least 500 ms since the drive processed a command?
                                           // This ensures that we are not changing files in the middle of processing
    ProcessButtons();                      // Has the button delay passed and do we have a button press?
  }
}
