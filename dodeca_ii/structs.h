#define PPQN 24

enum Mode {VOLT=0, CLOCK=1};

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
  int steps;

  bool changed;

  void init () {
    pinMode(pin, OUTPUT);
    analogWriteFrequency(pin, 200000);
    slew = 2*CTRLRATE; // ms
    mode = VOLT;
    clock_div = 6;
    clock_off = 3;
  }

  void set_mode (int m) {
    if (m==CLOCK) mode = CLOCK;
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
      steps = int(slew/CTRLRATE);
      steps = steps<=0 ? 1 : steps;
      delta = float(target_value-value)/steps;
    }
  }

  void set_slew (int ms) { // in ms
    if (ms<0) return;
    slew = ms;
    if (steps>0) set(target_value);
  }

  void process () {
    if (steps>0) {
      steps--;
      value += delta;
      //analogWrite(pin, value);
      write(value);
    }
  }

  void process (int t) {
    if (mode==CLOCK) {
      if (t%clock_div == 0) {
        write(MAX);
      }
      else if (t%clock_div == clock_off) {
        write(0);
      }
    }
    else {
      process();
    }
  }

  void write (float v) {
    analogWrite(pin, v);
  }
};