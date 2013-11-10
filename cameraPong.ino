/*
MenuBackend library by Alexander Brevig, LGPL v2.1
documentation (different version): http://wiring.org.co/reference/libraries/MenuBackend/index.html
download: http://www.arduino.cc/playground/uploads/Profiles/MenuBackend_1-4.zip

The version included with Wiring appears to be newer, but by the same author as the 1.4 version used here. 
If I was cool, I'd work out the bugs that prevent the Wiring version from working in Arduino. I'm not cool yet.
*/
#include <MenuBackend.h>

/*
Using examples from: http://www.coagula.org/content/pages/tutorial-manage-menu-and-lcd-display-arduino
Define the menu objects
*/
MenuBackend menu = MenuBackend( menuUseEvent, menuChangeEvent );
  //beneath is list of menu items needed to build the menu
  MenuItem settings_video = MenuItem( "Video" );
    MenuItem settings_video_time = MenuItem( "Cature time" );
    MenuItem settings_video_distance = MenuItem( "Distance" );
    MenuItem settings_video_direction = MenuItem( "Direction" );
      MenuItem settings_video_direction_left = MenuItem( "Left" );
      MenuItem settings_video_direction_right = MenuItem( "Right" );
    MenuItem settings_video_acceleration = MenuItem( "Acceleration" );
      MenuItem settings_video_acceleration_easing = MenuItem( "Easing" );
      MenuItem settings_video_acceleration_linear = MenuItem( "Linear" );
    MenuItem settings_video_start = MenuItem( "Start" );
      MenuItem settings_video_start_pause = MenuItem( "Pause" );
        MenuItem settings_video_start_end = MenuItem( "End recording" );
        MenuItem settings_video_start_return = MenuItem( "Return to start" );
      MenuItem settings_video_start_reverse = MenuItem( "Reverse direction" );
  MenuItem settings_photo = MenuItem( "Timelapse" );
    MenuItem settings_photo_interval = MenuItem( "Interval" );
    MenuItem settings_photo_frames = MenuItem( "Frames" );
    MenuItem settings_photo_distance = MenuItem( "Distance" );
    MenuItem settings_photo_direction = MenuItem( "Direction" );
    MenuItem settings_photo_start = MenuItem( "Start" );

/*
serLCD library by Cody B. Null, GPL v2 (as a derivitive of https://github.com/arduino/Arduino/tree/master/libraries/LiquidCrystal , which appears to be licensed GPL v2 according to the repo)
documentation: http://playground.arduino.cc/Code/SerLCD
download serLCD.h: http://playground.arduino.cc//Code/SerLCD?action=sourceblock&num=1 
download serLCD.cpp: http://playground.arduino.cc//Code/SerLCD?action=sourceblock&num=2

The library requires SoftwareSerial.h to be included previously. I'm not clear why yet, but I think that's a bug.
*/
#include <SoftwareSerial.h>
#include <serLCD.h>

// get and init the LCD driver, assign pin 8 as a soft serial port to send content to the LCD
serLCD lcd( 8 );

/*
IRremote library by Ken Shirriff, LGPL v2.1
repo: https://github.com/shirriff/Arduino-IRremote
download: https://github.com/shirriff/Arduino-IRremote/archive/master.zip
*/
#include <IRremote.h>

/*
Using examples from: https://learn.sparkfun.com/tutorials/ir-control-kit-hookup-guide/example-1-using-the-remote
Define the IR remote button codes. We're only using the least signinficant two bytes of these codes. Each one should actually 
have 0x10EF in front of it. Find these codes by running the IRrecvDump example sketch included with the IRremote library.
*/
const uint16_t irButton_power = 0xD827; // i.e. 0x10EFD827
const uint16_t irButton_a = 0xF807;
const uint16_t irButton_b = 0x7887;
const uint16_t irButton_c = 0x58A7;
const uint16_t irButton_up = 0xA05F;
const uint16_t irButton_down = 0x00FF;
const uint16_t irButton_left = 0x10EF;
const uint16_t irButton_right = 0x807F;
const uint16_t irButton_circle = 0x20DF;

// assign pin 11 as a soft serial port to receive serialized IR signals
IRrecv irRecv( 10 );
decode_results irRecv_results; // This will store our IR received codes
uint16_t irRecv_last = 0; // This keeps track of the last code RX'd


/*
AccelStepper library by Mike McCauley, GPL v2
documentation: http://www.airspayce.com/mikem/arduino/AccelStepper/
download: http://www.airspayce.com/mikem/arduino/AccelStepper/AccelStepper-1.37.zip
*/
#include <AccelStepper.h>

/*
using examples from: http://bildr.org/2012/11/big-easy-driver-arduino/
*/
int motorSpeed = 9600; //maximum steps per second (about 3rps / at 16 microsteps)
int motorAccel = 80000; //steps/second/second to accelerate

int motorDirPin = 2; //digital pin 2
int motorStepPin = 3; //digital pin 3

//set up the accelStepper intance
//the "1" tells it we are using a driver
AccelStepper stepper( 1, motorStepPin, motorDirPin ); 



// this function builds the menu and connects the correct items together
void menuSetup()
{

  // all the top-level menus
  menu.getRoot().add( settings_video );
  settings_video.addRight( settings_photo );

  // the submenus of settings_video
  settings_video.add( settings_video_time ).addRight( settings_video_distance ).addRight( settings_video_direction ).addRight( settings_video_acceleration ).addRight( settings_video_start );
    settings_video_direction.add( settings_video_direction_left ).add( settings_video_direction_left );
    settings_video_acceleration.add( settings_video_acceleration_easing ).add( settings_video_acceleration_linear );
    settings_video_start.add( settings_video_start_pause ).add( settings_video_start_reverse );
    settings_video_start_pause.add( settings_video_start_end ).add( settings_video_start_return );

  // the submenus of settings_photo
  settings_photo.add( settings_photo_interval ).addRight( settings_photo_frames ).addRight( settings_photo_distance ).addRight( settings_photo_direction ).addRight( settings_photo_start );
}

// menu item behaviors
void menuUseEvent( MenuUseEvent used )
{
  Serial.print( "Menu use " );
  Serial.println( used.item.getName() );
}

// menu item navigation
void menuChangeEvent( MenuChangeEvent changed )
{
  Serial.print( "Menu change " );
  Serial.print( changed.from.getName() );
  Serial.print( " " );
  Serial.println( changed.to.getName() );
}

// read the input from the IR
void readIR()
{
  if ( irRecv.decode( &irRecv_results ) ) 
  {
    /* read the RX'd IR into a 16-bit variable: */
    uint16_t resultCode = ( irRecv_results.value & 0xFFFF );

    /* The remote will continue to spit out 0xFFFFFFFF if a 
     button is held down. If we get 0xFFFFFFF, let's just
     assume the previously pressed button is being held down */
    if ( resultCode == 0xFFFF )
    {
      resultCode = irRecv_last;
    }
    else
    {
      irRecv_last = resultCode;
    }

    // This switch statement checks the received IR code against
    // all of the known codes. Each button press produces a 
    // serial output, and has an effect on the LED output.
    switch ( resultCode )
    {
      case irButton_power :
        lcd.clear();
        lcd.print( "Power" );
        break;

      case irButton_a :
        lcd.clear();
        lcd.print( "A" );
        break;

      case irButton_b :
        lcd.clear();
        lcd.print( "B" );
        break;

      case irButton_c :
        lcd.clear();
        lcd.print( "C" );
        break;

      case irButton_up :
        lcd.clear();
        lcd.print( "Up" );
        break;

      case irButton_down :
        lcd.clear();
        lcd.print( "Down" );
        break;

      case irButton_left :
        lcd.clear();
        lcd.print( "Left" );
        break;

      case irButton_right :
        lcd.clear();
        lcd.print( "Right" );
        break;

      case irButton_circle :
        lcd.clear();
        lcd.print( "Circle" );
        break;

      default :
        lcd.clear();
        lcd.print( "Unrecognized code received: 0x" );
        break;        
    }    
    irRecv.resume(); // Receive the next value
  }
}

void setup()
{
  menuSetup();

  irRecv.enableIRIn(); // Start the receiver


  lcd.clear();
  lcd.print( "   cameraPong   " );
  lcd.setSplash();
}

void loop()
{
  readIR();
//  navigateMenus();
}
