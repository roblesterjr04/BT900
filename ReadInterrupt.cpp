/*
  Clarion C-Bus Driver
  Prototyped off 1995 Saab 900 Clarion stereo.
  Read the Readme for more information
  Rob Lester
  GNU General Public License, version 2.0 (GPL-2.0)
*/

#define dt_pin 7 // Data Pin
#define cl_pin 2 // Clock pin on INT0


volatile bool reading = true;
volatile int counter = 0;

volatile byte lastbytein = 0x00;
volatile byte nextbyteout = 0xF7;

volatile int bitsin[8];

void setup() {
  pinMode(dt_pin, INPUT_PULLUP);
  pinMode(cl_pin, INPUT);
  attachInterrupt(digitalPinToInterrupt(cl_pin), shift_dt, RISING);
  Serial.begin(115200);
}

void loop() {
  if (counter > 500) {
    noInterrupts();
    counter = 0;
    for (int i = 0; i < 8; i++) {
      lastbytein = (lastbytein << 1) | bitsin[i];
    }
    bitWrite(PORTD, dt_pin, 0);
    delayMicroseconds(21);
    bitWrite(PORTD, dt_pin, 1);
    reading = !reading;
    nextbyteout = lastbytein;
    Serial.println(lastbytein, HEX);
    interrupts();
  }
  if (counter > 505) {
    Serial.println("Clock still bursting...");
  }
}

void shift_dt() {
  if (reading) bitsin[counter] = bitRead(PORTD, dt_pin);
  else bitWrite(PORTD, dt_pin, bitRead(nextbyteout, (counter < 8 ? counter - 7 : 8)));
  counter++;
}
