LILYGO T-Display-S3 KY-004 Push Button Module Project with an Interrupt Service Routine (ISR)

This code demonstrates the use of hardware interrupts to detect button presses and releases using a KY-004 module.
The button retains its natural momentary behavior, and the display shows the current state of the button.
The ESP32 is active low, so the interrupt is triggered on both falling (button press) and rising (button release) edges.
The button state is displayed on the built-in screen using the TFT_eSPI library.

Real-time Responsiveness: 
 ISRs are ideal for applications requiring immediate response to events, such as button presses,
 sensor triggers, safety triggers, or communication signals. They ensure the system reacts instantly
 without waiting for the main loop to process the event.

Pin Connections: (had to swap V & GND pins to get the button to work)
 - KY-004 '-'       -> 3.3V
 - KY-004 'middle'  -> GND
 - KY-004 'S'       -> GPIO1

KY-004 Specifications:
 - Protocol: Digital
 - Operating Voltage: 3.3V to 5.5V
 - Operating Temperature: -25°C to 105°C

Notes:
 - Signal on GPIO1 is active low, this means that when the button is pressed there is 0V on the pin.
    If the button is released there is 3.3V on the pin.
 - Try not to touch the 4 button pins on the underside of the module as this will cause the button to
      switch. Mount or hold the module on the edges when pressing the button.
 - Since our loop is simply reacting to button presses and display redraw, we don't need to add a delay
    at the end of the loop to slow down the loop cycle. The loop should be as responsive as possible.
