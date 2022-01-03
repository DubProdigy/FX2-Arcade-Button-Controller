#include <Bounce2.h>

/* Nano Arduino Code
   license: Public Domain - please use this code however you'd like.
   This code is to turn an arduino into an HID keyboard controller for arcade games 
*/

//---------
// USE THESE FOR THE buttonPresets ARRAY BELOW
#define KEY_LEFT_CTRL  0x80
#define KEY_LEFT_SHIFT 0x81
#define KEY_LEFT_ALT   0x82
#define KEY_LEFT_GUI   0x83  
#define KEY_RIGHT_CTRL 0x84
#define KEY_RIGHT_SHIFT    0x85
#define KEY_RIGHT_ALT  0x86
#define KEY_RIGHT_GUI  0x87
#define KEY_UP_ARROW   0xDA
#define KEY_DOWN_ARROW 0xD9
#define KEY_LEFT_ARROW 0xD8
#define KEY_RIGHT_ARROW    0xD7
#define KEY_BACKSPACE  0xB2
#define KEY_TAB        0xB3
#define KEY_RETURN 0xB0
#define KEY_ESC        0xB1
#define KEY_INSERT 0xD1
#define KEY_DELETE 0xD4
#define KEY_PAGE_UP    0xD3
#define KEY_PAGE_DOWN  0xD6
#define KEY_HOME   0xD2
#define KEY_END        0xD5
#define KEY_CAPS_LOCK  0xC1
#define KEY_F1     0xC2
#define KEY_F2     0xC3
#define KEY_F3     0xC4
#define KEY_F4     0xC5
#define KEY_F5     0xC6
#define KEY_F6     0xC7
#define KEY_F7     0xC8
#define KEY_F8     0xC9
#define KEY_F9     0xCA
#define KEY_F10        0xCB
#define KEY_F11        0xCC
#define KEY_F12        0xCD

//---------

#define LED_PIN 13
#define BOUNCE_LOCK-OUT //activate the alternative debouncing method. This method is a lot more responsive, but does not cancel noise.

//========== CONFIGURATION SETTINGS ==========
#define BOUNCE_WAIT 10
#define BOUNCE_COUNT 1

int buttonPins[] = {
  9, // Alt Key
  8, // F4
  2, // Slow Motion
  4, // Nudge Left
  6, // Nudge Right
  5, // Nudge Foward
  18, // Change View
  19, // Left Flipper
  20, // Right Flipper
  21, // Launch Ball
  10, // Up Arrow
  16, // Down Arrow
  14, // Left Arrow
  15 // Right Arrow
};

char buttonPresets[] = { 
  KEY_LEFT_ALT, // Alt Key
  KEY_F4, // F4
  's', // Slow Motion
  KEY_LEFT_CTRL, // Nudge Left
  KEY_RIGHT_CTRL, // Nudge Right
  ' ', // Nudge Foward
  'c', // Change View
  KEY_LEFT_SHIFT, // Left Flipper
  KEY_RIGHT_CTRL, // Right Flipper
  KEY_RETURN, // Launch Ball
  KEY_UP_ARROW, // Up Arrow
  KEY_DOWN_ARROW, // Down Arrow
  KEY_LEFT_ARROW, // Left Arrow
  KEY_RIGHT_ARROW // Right Arrow
};

//========== END CONFIGURATION SETTINGS ==========

int RXLED = 2;  // The RX LED has a defined Arduino pin
// Instantiate button state array
boolean buttonPressed[14];
// Instantiate a Bounce object array to store each debouncer in
Bounce debouncers[14];
//Instantiate a counter array for each button to debounce count timer
int debounceCount[14];

void setup() {
  // put your setup code here, to run once:

  pinMode(RXLED, OUTPUT);  // Set RX LED as an output
 // TX LED is set as an output behind the scenes 
 
   //Setup the LED :
 pinMode(LED_PIN,OUTPUT);
 digitalWrite(RXLED, LOW);
  
 
 Keyboard.begin();
 
  // Create debounce instances :
   for (int i = 0; i < 14; i++) {
     debouncers[i] = Bounce();
     debounceCount[i] = 0;
     pinMode(buttonPins[i],INPUT_PULLUP);
     (debouncers[i]).attach(buttonPins[i]);
     (debouncers[i]).interval(BOUNCE_WAIT);
        delay(100);
     buttonPressed[i] = false;, 
   }
     
}

void loop() {
  
  for (int j = 0; j < 14; j++) { //iterate over each button (pin)
    
     (debouncers[j]).update(); //check current value
     int value = (debouncers[j]).read();
     
     if ( value == LOW ) { //if button pressed
       
       if(debounceCount[j] == BOUNCE_COUNT && buttonPressed[j] == false) { //the button has been held down long enough and it hasn't been previously registered as pressed
          Keyboard.press(char(buttonPresets[j])); //Keyboard.write('1');
          buttonPressed[j] = true;
        } else {
            if(debounceCount[j] < BOUNCE_COUNT) { 
              debounceCount[j] = debounceCount[j] + 1; //else increment the count
            }
        }
        
      } else { //button is not pressed
        
        if(debounceCount[j] > 0) {
          debounceCount[j] = debounceCount[j] - 1; //keep decreasing count unless 0
        } else {
           Keyboard.release(char(buttonPresets[j])); //if 0 then release button
           buttonPressed[j] = false;
        }
        
      }

  }

}
