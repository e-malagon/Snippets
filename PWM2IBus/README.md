# PWM to Ibus

This is an Arduino sketch to convert PWM signals from an RC receiver to FlySky IBus protocol.

Modern RC servo position is not defined by the PWM duty cycle (i.e., ON vs OFF time) but only by the width of the pulse [Wikipedia](https://en.wikipedia.org/wiki/Servo_control). The length of each pulse may vary, but normally it has a length between 1ms and 2ms. A pulse of 1.5ms normally represents the center position. With this design, the length of the signal never will be low or high all the time, so an interrupt is a good option to measure the length of the signal.

It is possible to use the I/O pins of an Arduino to trigger a state change interrupt. It is a little bit complicated to do it from scratch but using the library [PinChangeInterrupt](https://github.com/NicoHood/PinChangeInterrupt) is so easy to configure almost any I/O pin to call a custom function to measure the length of the pulse.


The first thing to do is change the I/O pin to INPUT_PULLUP. After that attach the callback function to the state change interrupt over that pin.
```cpp
void setup() {
    pinMode(CHANNEL01, INPUT);
    digitalWrite(CHANNEL01, HIGH);
    attachPCINT(digitalPinToPCINT(CHANNEL01), &callbackC1, CHANGE);
}
```

The callback function measures the length of the pulse calculating the time between the rising and the falling events.
```cpp
void callbackC1() {
  unsigned long now = micros();
  uint8_t trigger = getPinChangeInterruptTrigger(digitalPinToPCINT(CHANNEL01));
  if(trigger == RISING) {
    startTime = now;
  } else if(trigger == FALLING) {
    length = now - startTime;
  }
}
```

In the loop, build the IBus buffer based on the values read by each callback and send it by the serial port.
```cpp
void loop() {
  sendIBUSData();
  delay(10);
}
```

## License

MIT License

Copyright (c) 2021 Edgar Malagón Calderón

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.