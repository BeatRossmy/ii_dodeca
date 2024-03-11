

/*
   Neutron magpie dodeca: 

Right Side Dodceca - DUBDECA layout

1) Velocity (synth voice) Midi Channel 4
2) Note out (synth voice) Midi Channel 4
3) Gate (synth voice) Midi Channel 4
4) 12) MIDI Clock out
5) Note to Gate/Trig - C Midi Channel 2
6) CC #72 MIDI Channel 2
7) Note to Gate/Trig - D Midi Channel 2
😎 CC #73 MIDI Channel 2
9) Note to Gate/Trig - E Midi Channel 2
10) CC #74 MIDI Channel 2
11) CC #75 MIDI Channel 2
12) CC #76 MIDI Channel 2

  
 * */

#include <elapsedMillis.h>

#define CTRLRATE 4
#define BITS 12

const int MAX = pow(2,BITS);

unsigned long cpu_cycles;

elapsedMillis ctrlRate;

IntervalTimer writeTimer;
const int writeRate = 1000;

int clock_ticks = 0;

#include "structs.h"

// i2c Wire Library (for Teensy)
#include <i2c_t3.h>

#include "TxHelper.h"
int configID = 0x7F;
bool enablePullups = false;


#include <MIDI.h>
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);

/*
const uint8_t CHANA = 2;  //set the MIDI channel here!
const uint8_t CHANB = 4;

uint8_t out2pin[] = { 23, 0, 22, 25, 20, 6, 21, 5, 9, 4, 10, 3 };  //output number to actual teensy pin, dont change.
uint8_t whitekeys[] = { 4, 0, 6, 0, 8, 0, 0, 0, 0, 0, 0, 0 };      //non zero keys sent to output number.
uint8_t pulses;
uint8_t sixteenthnotes;
uint8_t quartertoggle;
uint8_t wholetoggle;
bool playing;
*/


uint8_t cc2active[] = { 72, 73, 74, 75, 76 };
uint8_t cc2out[] = { 5, 7, 9, 10, 11 };

Outlet outlets[12] = {
  { 23 },
  { A14 },
  { 22 },
  { 25 },
  { 20 },
  { 6 },
  { 21 },
  { 5 },
  { 9 },
  { 4 },
  { 10 },
  { 3 }
};

void setup() {
  // Initiate MIDI communications, listen to all channels

  for (int i=0; i<12; i++) {
    outlets[i].init();
  }
  analogWriteResolution(BITS);

  for (int i = 0; i < 12; i++) {
    outlets[i].write(MAX);
    delay(100);
    outlets[i].write(0);
  }

  /*MIDI.begin(MIDI_CHANNEL_OMNI);
  // Connect the Handlers to the library, so it is called upon reception.
  MIDI.setHandleNoteOn(HandleNoteOn);  // Put only the name of the function
  MIDI.setHandleControlChange(HandleControlChange);
  MIDI.setHandleNoteOff(HandleNoteOff);
  MIDI.setHandleClock(HandleClock);
  MIDI.setHandleStart(HandleStart);
  MIDI.setHandleStop(HandleStop);
  MIDI.setHandleContinue(HandleContinue);*/

  // START TIMER
  writeTimer.begin(writeOutputs, writeRate);

  // I2C
  // initialize the teensy optimized wire library
  Wire.begin(I2C_SLAVE, configID, I2C_PINS_18_19, enablePullups ? I2C_PULLUP_INT : I2C_PULLUP_EXT, I2C_RATE_400);  // I2C_RATE_2400
  Wire.onReceive(receiveEvent);

  float base_freq = 0.058;

  outlets[4].set_lfo_freq(1*base_freq);
  outlets[4].set_mode(LFO);

  outlets[5].set_lfo_freq(2*base_freq);
  outlets[5].set_mode(LFO);

  outlets[6].set_lfo_freq(3*base_freq);
  outlets[6].set_mode(LFO);

  outlets[7].set_lfo_freq(5*base_freq);
  outlets[7].set_mode(LFO);

  outlets[8].set_lfo_freq(8*base_freq);
  outlets[8].set_mode(LFO);

  outlets[9].set_lfo_freq(13*base_freq);
  outlets[9].set_mode(LFO);

  outlets[10].set_lfo_freq(21*base_freq);
  outlets[10].set_mode(LFO);

  outlets[11].set_lfo_freq(34*base_freq);
  outlets[11].set_mode(LFO);

  //outlets[0].set(4000);
}

void loop() {
  // Call MIDI.read the fastest you can for real-time performance.
  MIDI.read();

  /*if (ctrlRate > CTRLRATE) {
    ctrlRate = 0;
    for (int i=0; i<12; i++) {
      outlets[i].process(clock_ticks);
    }
  }*/
}

void writeOutputs () {
  cpu_cycles ++;
  for (int i=0; i<12; i++) {
    outlets[i].process(cpu_cycles, clock_ticks);
  }
}

void receiveEvent(size_t len) {
  TxResponse response = TxHelper::Parse(len);
  actOnCommand(response.Command, response.Output, response.Value);
}

void actOnCommand(byte cmd, byte out, int value) {
  if (out<0 || out>11) return;

  if (cmd == 0) {
    outlets[out].set(value);
  }
  else if (cmd == 1) {
    outlets[out].set_slew(value);
  }
  else if (cmd == 2) {
    outlets[out].set_mode(value);
  }
  else if (cmd == 3) {
    outlets[out].set_division(value);
  }
  else if (cmd == 4) {
    tick(value);
  }
}

void tick (int t) {
  clock_ticks = t;
}

