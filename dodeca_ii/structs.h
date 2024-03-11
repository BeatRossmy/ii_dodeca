#define PPQN 24

enum Mode {VOLT=0, CLOCK=1, LFO=2};

struct Outlet {
  int pin;
  //int value;

  Mode mode;

  int clock_div;
  int clock_off;
  
  float value;
  float target_value;
  float slew;
  float delta;
  float lfo_freq;
  int steps;

  bool changed;

  void init () {
    pinMode(pin, OUTPUT);
    analogWriteFrequency(pin, 200000);
    slew = 2*CTRLRATE; // ms
    mode = VOLT;
    clock_div = 6;
    clock_off = 3;

    lfo_freq = 1;
  }

  void set_mode (int m) {
    if (m==CLOCK) mode = CLOCK;
    else if (m==LFO) mode = LFO;
    else mode = VOLT;
  }

  void set_division (int d) {
    clock_div = constrain(d,2,100);
    clock_off = clock_div/2;
  }

  void set (int v) {
    target_value = constrain(v,0,MAX);
    if (slew<=0) {
      value = target_value;
      write(value);
    }
    else {
      //steps = int(slew/CTRLRATE);
      steps = slew; // because timer runs with 1kHz => slew in ms == steps
      steps = steps<=0 ? 1 : steps;
      delta = float(target_value-value)/steps;
    }
  }

  void set_slew (int ms) { // in ms
    if (ms<0) return;
    slew = ms;
    if (steps>0) set(target_value);
  }

  void set_lfo_freq (float hz) {
    lfo_freq = constrain(hz,0.015,500);
  }

  void process () {
    if (steps>0) {
      steps--;
      value += delta;
      write(value);
    }
  }

  void process (unsigned long cycles, int ticks) {
    if (mode==CLOCK) {
      if (ticks%clock_div == 0) {
        write(MAX);
      }
      else if (ticks%clock_div == clock_off) {
        write(0);
      }
    }
    else if (mode==LFO) {
      float val = (sin(cycles/(1000. / lfo_freq) * 2 * PI) + 1) * (MAX/2);
      write(val);
    }
    else {
      process();
    }
  }

  void write (float v) {
    analogWrite(pin, v);
  }
};