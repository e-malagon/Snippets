#include <PinChangeInterrupt.h>
 
#define CHANNEL01 5
#define CHANNEL02 6
#define CHANNEL03 7
#define CHANNEL04 8
#define CHANNEL05 9
#define CHANNEL06 10
#define CHANNEL07 11
#define CHANNEL08 12
#define CHANNEL09 A0
#define CHANNEL10 A1
#define CHANNEL11 A2
#define CHANNEL12 A3
#define CHANNEL13 A4
#define CHANNEL14 A5

#define JITTER_THRESHOLD 4

volatile unsigned int channels[14] = {1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500};
volatile unsigned int minThreshold[14] = {1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000};
volatile unsigned int maxThreshold[14] = {2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000};
volatile unsigned long startTime[14] = {0};

void callbackC1() {
  unsigned long now = micros();
  uint8_t trigger = getPinChangeInterruptTrigger(digitalPinToPCINT(CHANNEL01));
  if(trigger == RISING) {
    startTime[0] = now;
  } else if(trigger == FALLING) {
    unsigned int current = now - startTime[0];
    int diff = channels[0] - current;
    diff = diff < 0? -diff : diff;
    if(JITTER_THRESHOLD < diff)
      channels[0] = current;
  }
}

void callbackC2() {
  unsigned long now = micros();
  uint8_t trigger = getPinChangeInterruptTrigger(digitalPinToPCINT(CHANNEL02));
  if(trigger == RISING) {
    startTime[1] = now;
  } else if(trigger == FALLING) {
    unsigned int current = now - startTime[1];
    int diff = channels[1] - current;
    diff = diff < 0? -diff : diff;
    if(JITTER_THRESHOLD < diff)
      channels[1] = current;
  }
}

void callbackC3() {
  unsigned long now = micros();
  uint8_t trigger = getPinChangeInterruptTrigger(digitalPinToPCINT(CHANNEL03));
  if(trigger == RISING) {
    startTime[2] = now;
  } else if(trigger == FALLING) {
    unsigned int current = now - startTime[2];
    int diff = channels[2] - current;
    diff = diff < 0? -diff : diff;
    if(JITTER_THRESHOLD < diff)
      channels[2] = current;
  }
}

void callbackC4() {
  unsigned long now = micros();
  uint8_t trigger = getPinChangeInterruptTrigger(digitalPinToPCINT(CHANNEL04));
  if(trigger == RISING) {
    startTime[3] = now;
  } else if(trigger == FALLING) {
    unsigned int current = now - startTime[3];
    int diff = channels[3] - current;
    diff = diff < 0? -diff : diff;
    if(JITTER_THRESHOLD < diff)
      channels[3] = current;
  }
}

void callbackC5() {
  unsigned long now = micros();
  uint8_t trigger = getPinChangeInterruptTrigger(digitalPinToPCINT(CHANNEL05));
  if(trigger == RISING) {
    startTime[4] = now;
  } else if(trigger == FALLING) {
    unsigned int current = now - startTime[4];
    int diff = channels[4] - current;
    diff = diff < 0? -diff : diff;
    if(JITTER_THRESHOLD < diff)
      channels[4] = current;
  }
}

void callbackC6() {
  unsigned long now = micros();
  uint8_t trigger = getPinChangeInterruptTrigger(digitalPinToPCINT(CHANNEL06));
  if(trigger == RISING) {
    startTime[5] = now;
  } else if(trigger == FALLING) {
    unsigned int current = now - startTime[5];
    int diff = channels[5] - current;
    diff = diff < 0? -diff : diff;
    if(JITTER_THRESHOLD < diff)
      channels[5] = current;
  }
}

void callbackC7() {
  unsigned long now = micros();
  uint8_t trigger = getPinChangeInterruptTrigger(digitalPinToPCINT(CHANNEL07));
  if(trigger == RISING) {
    startTime[6] = now;
  } else if(trigger == FALLING) {
    unsigned int current = now - startTime[6];
    int diff = channels[6] - current;
    diff = diff < 0? -diff : diff;
    if(JITTER_THRESHOLD < diff)
      channels[6] = current;
  }
}

void callbackC8() {
  unsigned long now = micros();
  uint8_t trigger = getPinChangeInterruptTrigger(digitalPinToPCINT(CHANNEL08));
  if(trigger == RISING) {
    startTime[7] = now;
  } else if(trigger == FALLING) {
    unsigned int current = now - startTime[7];
    int diff = channels[7] - current;
    diff = diff < 0? -diff : diff;
    if(JITTER_THRESHOLD < diff)
      channels[7] = current;
  }
}

void callbackC9() {
  unsigned long now = micros();
  uint8_t trigger = getPinChangeInterruptTrigger(digitalPinToPCINT(CHANNEL09));
  if(trigger == RISING) {
    startTime[8] = now;
  } else if(trigger == FALLING) {
    unsigned int current = now - startTime[8];
    int diff = channels[8] - current;
    diff = diff < 0? -diff : diff;
    if(JITTER_THRESHOLD < diff)
      channels[8] = current;
  }
}

void callbackC10() {
  unsigned long now = micros();
  uint8_t trigger = getPinChangeInterruptTrigger(digitalPinToPCINT(CHANNEL10));
  if(trigger == RISING) {
    startTime[9] = now;
  } else if(trigger == FALLING) {
    unsigned int current = now - startTime[9];
    int diff = channels[9] - current;
    diff = diff < 0? -diff : diff;
    if(JITTER_THRESHOLD < diff)
      channels[9] = current;
  }
}

void callbackC11() {
  unsigned long now = micros();
  uint8_t trigger = getPinChangeInterruptTrigger(digitalPinToPCINT(CHANNEL11));
  if(trigger == RISING) {
    startTime[10] = now;
  } else if(trigger == FALLING) {
    unsigned int current = now - startTime[10];
    int diff = channels[10] - current;
    diff = diff < 0? -diff : diff;
    if(JITTER_THRESHOLD < diff)
      channels[10] = current;
  }
}

void callbackC12() {
  unsigned long now = micros();
  uint8_t trigger = getPinChangeInterruptTrigger(digitalPinToPCINT(CHANNEL12));
  if(trigger == RISING) {
    startTime[11] = now;
  } else if(trigger == FALLING) {
    unsigned int current = now - startTime[11];
    int diff = channels[11] - current;
    diff = diff < 0? -diff : diff;
    if(JITTER_THRESHOLD < diff)
      channels[11] = current;
  }
}

void callbackC13() {
  unsigned long now = micros();
  uint8_t trigger = getPinChangeInterruptTrigger(digitalPinToPCINT(CHANNEL13));
  if(trigger == RISING) {
    startTime[12] = now;
  } else if(trigger == FALLING) {
    unsigned int current = now - startTime[12];
    int diff = channels[12] - current;
    diff = diff < 0? -diff : diff;
    if(JITTER_THRESHOLD < diff)
      channels[12] = current;
  }
}

void callbackC14() {
  unsigned long now = micros();
  uint8_t trigger = getPinChangeInterruptTrigger(digitalPinToPCINT(CHANNEL14));
  if(trigger == RISING) {
    startTime[13] = now;
  } else if(trigger == FALLING) {
    unsigned int current = now - startTime[13];
    int diff = channels[13] - current;
    diff = diff < 0? -diff : diff;
    if(JITTER_THRESHOLD < diff)
      channels[13] = current;
  }
}

byte ibusBuffer[32] = { 0 };

void sendIBUSData() {
  unsigned int value;
  uint16_t checksum = ((uint16_t)0xFFFF);  
  ibusBuffer[0] = (byte)0x20;
  ibusBuffer[1] = (byte)0x40;
  for(int i = 0; i < 14; i++) {
    minThreshold[i] = channels[i] < minThreshold[i] ? channels[i] : minThreshold[i];
    maxThreshold[i] = maxThreshold[i] < channels[i] ? channels[i] : maxThreshold[i];
    value = map(channels[i], minThreshold[i], maxThreshold[i], 1000, 2000);
    ibusBuffer[i + 2] = (byte)(value & 0xFF);
    ibusBuffer[i + 3] = (byte)((value >> 8) & 0xFF);
  }
  for (int i = 0; i < 30; i++)
    checksum -= (uint16_t)ibusBuffer[i];
  ibusBuffer[30] = (byte)(checksum & 0xFF);
  ibusBuffer[31] = (byte)((checksum >> 8) & 0xFF);
  Serial.write(ibusBuffer, 32);  
}

void setup() {
  pinMode(CHANNEL01, INPUT);
  pinMode(CHANNEL02, INPUT);
  pinMode(CHANNEL03, INPUT);
  pinMode(CHANNEL04, INPUT);
  pinMode(CHANNEL05, INPUT);
  pinMode(CHANNEL06, INPUT);
  pinMode(CHANNEL07, INPUT);
  pinMode(CHANNEL08, INPUT);
  pinMode(CHANNEL09, INPUT);
  pinMode(CHANNEL10, INPUT);
  pinMode(CHANNEL11, INPUT);
  pinMode(CHANNEL12, INPUT);
  pinMode(CHANNEL13, INPUT);
  pinMode(CHANNEL14, INPUT);
  digitalWrite(CHANNEL01, HIGH);
  digitalWrite(CHANNEL02, HIGH);
  digitalWrite(CHANNEL03, HIGH);
  digitalWrite(CHANNEL04, HIGH);
  digitalWrite(CHANNEL05, HIGH);
  digitalWrite(CHANNEL06, HIGH);
  digitalWrite(CHANNEL07, HIGH);
  digitalWrite(CHANNEL08, HIGH);
  digitalWrite(CHANNEL09, HIGH);
  digitalWrite(CHANNEL10, HIGH);
  digitalWrite(CHANNEL11, HIGH);
  digitalWrite(CHANNEL12, HIGH);
  digitalWrite(CHANNEL13, HIGH);
  digitalWrite(CHANNEL14, HIGH);
  attachPCINT(digitalPinToPCINT(CHANNEL01), &callbackC1, CHANGE);
  attachPCINT(digitalPinToPCINT(CHANNEL02), &callbackC2, CHANGE);
  attachPCINT(digitalPinToPCINT(CHANNEL03), &callbackC3, CHANGE);
  attachPCINT(digitalPinToPCINT(CHANNEL04), &callbackC4, CHANGE);
  attachPCINT(digitalPinToPCINT(CHANNEL05), &callbackC5, CHANGE);
  attachPCINT(digitalPinToPCINT(CHANNEL06), &callbackC6, CHANGE);
  attachPCINT(digitalPinToPCINT(CHANNEL07), &callbackC7, CHANGE);
  attachPCINT(digitalPinToPCINT(CHANNEL08), &callbackC8, CHANGE);
  attachPCINT(digitalPinToPCINT(CHANNEL09), &callbackC9, CHANGE);
  attachPCINT(digitalPinToPCINT(CHANNEL10), &callbackC10, CHANGE);
  attachPCINT(digitalPinToPCINT(CHANNEL11), &callbackC11, CHANGE);
  attachPCINT(digitalPinToPCINT(CHANNEL12), &callbackC12, CHANGE);
  attachPCINT(digitalPinToPCINT(CHANNEL13), &callbackC13, CHANGE);
  attachPCINT(digitalPinToPCINT(CHANNEL14), &callbackC14, CHANGE);
  Serial.begin(115200);
}

void loop() {
  sendIBUSData();
  delay(10);
}