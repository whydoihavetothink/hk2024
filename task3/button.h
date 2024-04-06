
class Button {
public:
  Button(int p, void (*onp)(), void (*onr)()) : pin(p), on_change{onp, onr} {}

  void setup() { 
    pinMode(pin, INPUT);
    new_state = state = digitalRead(pin);
    change = 0;
  };

  void tick() {
    int curr = digitalRead(pin);
    if (new_state != curr) {
      new_state = curr;
      change = millis();
    }
    if (new_state != state && (millis() - change) >= 20) {
      state = new_state;
      if (on_change[state]) {
        on_change[state]();
      }
    }
  }

private:
  int pin;
  int state;
  int new_state;
  long change = 0;

  void (*on_change[2])();
};
