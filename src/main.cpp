/*********************************************************************************************************
* LILYGO T-Display-S3 KY-004 Push Button Module Project with an Interrupt Service Routine (ISR)
*
* This code demonstrates the use of hardware interrupts to detect button presses and releases
*  using a KY-004 module.
* The button retains its natural momentary behavior, and the display shows the current state of the button.
* The ESP32 is active low, so the interrupt is triggered on both falling (button press)
*  and rising (button release) edges.
* The button state is displayed on the built-in screen using the TFT_eSPI library.
*
* Real-time Responsiveness: 
*  ISRs are ideal for applications requiring immediate response to events, such as button presses,
*  sensor triggers, safety triggers, or communication signals. They ensure the system reacts instantly
*  without waiting for the main loop to process the event.
*
* Pin Connections: (had to swap V & GND pins to get the button to work)
*  - KY-004 '-'       -> 3.3V
*  - KY-004 'middle'  -> GND
*  - KY-004 'S'       -> GPIO1
*
* KY-004 Specifications:
*  - Protocol: Digital
*  - Operating Voltage: 3.3V to 5.5V
*  - Operating Temperature: -25°C to 105°C
*
* Notes:
*  - Signal on GPIO1 is active low, this means that when the button is pressed there is 0V on the pin.
      If the button is released there is 3.3V on the pin.
   - Try not to touch the 4 button pins on the underside of the module as this will cause the button to
      switch. Mount or hold the module on the edges when pressing the button.
   - Since our loop is simply reacting to button presses and display redraw, we don't need to add a delay
      at the end of the loop to slow down the loop cycle. The loop should be as responsive as possible.
**********************************************************************************************************/

/*************************************************************
******************* INCLUDES & DEFINITIONS *******************
**************************************************************/

#include <Arduino.h>
#include <TFT_eSPI.h>

// TFT_eSPI
TFT_eSPI tft = TFT_eSPI();

// Pin definitions
const int buttonPin = 1; // digital pin for push button

// Variables for debouncing
volatile bool buttonPressed = false;  // flag to indicate button press
volatile bool buttonReleased = false; // flag to indicate button release
volatile bool buttonState = false;    // current button state
unsigned long lastDebounceTime = 0;   // last time the button state was toggled
const long debounceInterval = 10;     // debounce interval (ms)

// Global variables
bool redrawRequired = true;  // flag to redraw the screen


/*************************************************************
********************** HELPER FUNCTIONS **********************
**************************************************************/

// Function to draw static elements on the TFT screen
void drawStaticElements() {
  // Clear the screen and set text properties
  tft.fillScreen(TFT_BLACK);
  tft.setTextFont(2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  // Reset cursor to the top-left corner
  tft.setCursor(0, 0);

  // Display header
  tft.println("----------------------------");
  tft.println("KY004 Push Button Module");
  tft.println("----------------------------");

  // Display static labels
  tft.setCursor(0, 70); // position for button state label
  tft.print("Button State:");
}

// Function to update dynamic elements on the TFT screen
void updateDynamicElements() {
  // Update button state
  tft.setCursor(0, 90);      // position for button state value (below the label)
  tft.print("            "); // clear previous value (12 spaces for padding)
  tft.setCursor(0, 90);      // reset cursor
  tft.print(buttonState ? "RELEASED" : "PRESSED");
}


/*************************************************************
*********************** MAIN FUNCTIONS ***********************
**************************************************************/

// Interrupt Service Routine (ISR) for button press and release
void IRAM_ATTR handleButtonChange() {
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();

  // Debounce the button press/release
  if (interruptTime - lastInterruptTime > debounceInterval) {
    bool currentState = !digitalRead(buttonPin); // read the current state (active low)

    if (currentState != buttonState) {
      buttonState = currentState; // update the button state

      // Set the appropriate flag based on the button state
      if (buttonState) {
        buttonPressed = true;  // button is pressed
      } else {
        buttonReleased = true; // button is released
      }
    }
  }
  lastInterruptTime = interruptTime;
}


// SETUP
void setup() {
  // Set pin modes with internal pull-up resistor
  pinMode(buttonPin, INPUT);

  // Initialize the button state based on the actual button state at startup
  buttonState = !digitalRead(buttonPin); // read the initial state of the button

  // Attach the interrupt to the button pin for both FALLING and RISING edges
  attachInterrupt(digitalPinToInterrupt(buttonPin), handleButtonChange, CHANGE);

  // Initialize the TFT display
  tft.init();
  tft.setRotation(0);                     // adjust rotation (0 & 2 portrait | 1 & 3 landscape)
  tft.fillScreen(TFT_BLACK);              // black background
  tft.setTextFont(2);                     // set the font size
  tft.setTextColor(TFT_WHITE, TFT_BLACK); // set text colour (white) and background colour (black)

  // Draw static elements once
  drawStaticElements();

  // Initial display update
  redrawRequired = true; // update display
}


// MAIN LOOP
void loop() {
  // Check if the button press flag is set
  if (buttonPressed) {
    // Handle button press event
    redrawRequired = true; // update display
    buttonPressed = false; // reset the flag
  }

  // Check if the button release flag is set
  if (buttonReleased) {
    // Handle button release event
    redrawRequired = true;  // update display
    buttonReleased = false; // reset the flag
  }

  // Update the display if the button state has changed
  if (redrawRequired) {
    updateDynamicElements();
    redrawRequired = false; // reset the flag
  }
}
