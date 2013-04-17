/*
  Hexbright demo firmware: Fades
  Andrew Magill  9/2012
  
  1. Hold button down.  Light fades up and down.
  2. Let go of button.  Light holds current brightness.
  3. Press button.  Light turns off.
*/

#include <hexbright.h>
#include <Wire.h>

hexbright hb;

// Modes
#define MODE_OFF                0
#define MODE_FADE               1
#define MODE_ON                 2

byte mode;
char brightness_direction = -1;

void setup()
{
  hb.init_hardware();
  mode = MODE_OFF;
}

void loop()
{
  hb.update();
  switch (mode)
  {
  case MODE_OFF:
    if(hb.button_pressed_time()>200) {
      mode = MODE_FADE;
    }
    break;
  case MODE_FADE:
    if(hb.button_pressed())
    {
      if(!hb.light_change_remaining())
        if(brightness_direction<0)
        { // the light is low, go up from here
          hb.set_light(CURRENT_LEVEL, 1000, 1000-(hb.get_light_level()));  // go from low to high over 1 second
          brightness_direction = 1;
        }
        else
        {
          hb.set_light(CURRENT_LEVEL, 1, hb.get_light_level()); // go from high to low over 1 second
          brightness_direction = -1;
        }
    }
    else // released!
    { // we backtrack a little to help compensate for human reaction time
      int brightness = hb.get_light_level()+(50*brightness_direction);
      brightness = brightness<1 ? 1 : brightness; // if our adjust took us to 0, undo.
      hb.set_light(CURRENT_LEVEL, brightness, 50);
      mode = MODE_ON;
    }
    break;
  case MODE_ON:
    if (!hb.button_pressed() && hb.button_just_released())
    {
      hb.set_light(0, OFF_LEVEL, NOW);
      mode = MODE_OFF;
    }
    else if (hb.button_pressed() && hb.button_pressed_time()>200)
    {
      mode = MODE_FADE;
      // so we continue going the same way as before
      brightness_direction = -brightness_direction;
    }
    break;
  }
}

