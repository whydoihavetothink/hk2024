#include <string>

class Listener {
public:
  Listener(int baud, void (*c)(const std::string&)) : baudrate(baud), callback(c) {}

  void setup() { Serial.begin(baudrate); }

  void tick() {
    while (Serial.available()) {
      buff += (char)Serial.read();
      if (buff[buff.size() - 1] == '\n') {
        process();
        break;
      }
    }
  }

  void process() {
    callback(buff);
    buff.clear();
  }

private:
  int baudrate;
  std::string buff;
  void (*callback)(const std::string&);
};
