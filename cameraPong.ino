/*
MenuBackend library by Alexander Brevig, LGPL v2.1
documentation (different version): http://wiring.org.co/reference/libraries/MenuBackend/index.html
download (previous version): http://www.arduino.cc/playground/uploads/Profiles/MenuBackend_1-4.zip
download (my forked version): https://github.com/misterbisson/MenuBackendPlus
*/
#include <MenuBackendPlus.h>

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
Adafruit_SSD1306 and Adafruit_GFX libraries by Adafruit Industries, BSD licensed
documentation: http://learn.adafruit.com/adafruit-gfx-graphics-library/overview 
               and http://learn.adafruit.com/monochrome-oled-breakouts/
download Adafruit_GFX: https://github.com/adafruit/Adafruit-GFX-Library
download Adafruit_SSD1306: https://github.com/adafruit/Adafruit_SSD1306

The libraries require SPI and Wire as well.
*/
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// init the display driver using software SPI, pin assignments as follows
// display( OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS )
Adafruit_SSD1306 display( 9, 10, 6, 7, 8 );



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
const uint16_t irButton_power  = 0xD827; // i.e. 0x10EFD827
const uint16_t irButton_a      = 0xF807;
const uint16_t irButton_b      = 0x7887;
const uint16_t irButton_c      = 0x58A7;
const uint16_t irButton_up     = 0xA05F;
const uint16_t irButton_down   = 0x00FF;
const uint16_t irButton_left   = 0x10EF;
const uint16_t irButton_right  = 0x807F;
const uint16_t irButton_circle = 0x20DF;

// assign pin 5 as a soft serial port to receive serialized IR signals
IRrecv irRecv( 5 );
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



/*
SimpleTimer library by Marcello Romani, LGPL 2.1+
documentation: http://playground.arduino.cc//Code/SimpleTimer
download SimpleTimer.h: http://playground.arduino.cc//Code/SimpleTimer?action=sourceblock&num=2
download SimpleTimer.cpp: http://playground.arduino.cc//Code/SimpleTimer?action=sourceblock&num=3
*/

#include <SimpleTimer.h>
SimpleTimer timer;



/*
Positioning switches
The absolution position of the camera sled is determined when reed 
switches in the sled are triggered by magnets at each end of the rail
*/
int posLeft_pin    = 0;
int posLeft_state  = 0;
int posRight_pin   = 1;
int posRight_state = 0;



// this function builds the menu and connects the correct items together
void menuSetup()
{

  // all the top-level menus
  menu.getRoot().add( settings_video );
  settings_video.addRight( settings_photo );

  // the submenus of settings_video
  settings_video.add( settings_video_time ).addRight( settings_video_distance ).addRight( settings_video_direction ).addRight( settings_video_acceleration ).addRight( settings_video_start );
    settings_video_direction.add( settings_video_direction_left ).addRight( settings_video_direction_right );
    settings_video_acceleration.add( settings_video_acceleration_easing ).addRight( settings_video_acceleration_linear );
    settings_video_start.add( settings_video_start_pause ).addRight( settings_video_start_reverse );
    settings_video_start_pause.add( settings_video_start_end ).addRight( settings_video_start_return );

  // the submenus of settings_photo
  settings_photo.add( settings_photo_interval ).addRight( settings_photo_frames ).addRight( settings_photo_distance ).addRight( settings_photo_direction ).addRight( settings_photo_start );
}

// menu item behaviors
void menuUseEvent( MenuUseEvent used )
{
  display.clearDisplay();
  display.println( "Using menu: " );
  display.println( used.item.getName() );
}

// menu item navigation
void menuChangeEvent( MenuChangeEvent changed )
{
  display.clearDisplay();
  display.println( "Current menu: " );
  display.println( changed.to.getName() );
}

// read the input from the IR
void navigate()
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
        menu.moveUp();
/*
        display.clearDisplay();
        display.println( "Power" );
*/
        break;

      case irButton_a :
        display.clearDisplay();
        display.println( "A" );
        break;

      case irButton_b :
        display.clearDisplay();
        display.println( "B" );
        break;

      case irButton_c :
        display.clearDisplay();
        display.println( "C" );
        break;

      case irButton_up :
        menu.moveUp();
/*
        display.clearDisplay();
        display.println( "Up" );
*/
        break;

      case irButton_down :
        menu.moveDown();
/*
        display.clearDisplay();
        display.println( "Down" );
*/
        break;

      case irButton_left :
        menu.moveLeft();
/*
        display.clearDisplay();
        display.println( "Left" );
*/
        break;

      case irButton_right :
        menu.moveRight();
/*
        display.clearDisplay();
        display.println( "Right" );
*/
        break;

      case irButton_circle :
        menu.use();
/*
        display.clearDisplay();
        display.println( "Circle" );
*/
        break;

      default :
        display.clearDisplay();
        display.println( "Unrecognized code received: 0x" );
        break;        
    }    
    irRecv.resume(); // Receive the next value
  }
}

void setup()
{
  // set the display to generate the high voltage from the Vin line
  display.begin( SSD1306_SWITCHCAPVCC );
  display.clearDisplay();   // clears the screen and buffer

  display.setTextSize( 2 );
  display.setTextColor( WHITE );
  display.setCursor( 5, 35 );
  display.println( "cameraPong" );
  display.display();

  menuSetup();

  irRecv.enableIRIn(); // Start the receiver

/*
  pinMode( posLeft_pin, INPUT_PULLUP );
  pinMode( posRight_pin, INPUT_PULLUP );
*/

/*
  stepper.setMaxSpeed(300);
  stepper.setAcceleration( 15 );
  stepper.runToNewPosition( 1200 );

  display.clearDisplay();
  display.println( superplan.positionNow() );
*/
}

void loop()
{

  navigate();

/*
  posLeft_state  = digitalRead( posLeft_pin );
  posRight_state = digitalRead( posRight_pin );
*/
  delay( 200 );
}

