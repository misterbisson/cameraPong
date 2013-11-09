/*
MenuBackend library by Alexander Brevig 
documentation (different version): http://wiring.org.co/reference/libraries/MenuBackend/index.html
download: http://www.arduino.cc/playground/uploads/Profiles/MenuBackend_1-4.zip

The version included with Wiring appears to be newer, but by the same author as the 1.4 version used here. 
If I was cool, I'd work out the bugs that prevent the Wiring version from working in Arduino. I'm not cool yet.
*/
#include <MenuBackend.h>

/*
serLCD library by Cody B. Null
documentation: http://playground.arduino.cc/Code/SerLCD
download serLCD.h: http://playground.arduino.cc//Code/SerLCD?action=sourceblock&num=1 
download serLCD.cpp: http://playground.arduino.cc//Code/SerLCD?action=sourceblock&num=2

The library requires SoftwareSerial.h to be included previously. I'm not clear why yet, but I think that's a bug.
*/
#include <SoftwareSerial.h>
#include <serLCD.h>

/*
IRremote library by Ken Shirriff
repo: https://github.com/shirriff/Arduino-IRremote
download: https://github.com/shirriff/Arduino-IRremote/archive/master.zip
*/
#include <IRremote.h>


// Define the menu objects
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
  Serial.print("Menu use ");
  Serial.println(used.item.getName());
}

// menu item navigation
void menuChangeEvent( MenuChangeEvent changed )
{
  Serial.print( "Menu change " );
  Serial.print( changed.from.getName() );
  Serial.print( " " );
  Serial.println( changed.to.getName() );
}


// get and init the LCD driver
serLCD lcd( 8 );

void setup()
{
  menuSetup();
  lcd.print( "cameraPong" );
}

void loop()
{
}
