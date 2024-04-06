#include <string>

constexpr uint32_t FONT_NUMBERS[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};
constexpr uint32_t FONT_LETTERS[26] = {0b10001000, 0b10000011, 0b11000110, 0b10100001, 0b10000110, 0b10001110, 0x82, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x92, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
//constexpr uint32_t FONT_LETTERS[26] = {0x88, 0xC2, 0x87, 0xE0, 0x83, 0x8B, 0x82, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x92, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

uint32_t char_to_disp(char c) {
  if (c >= '0' && c <= '9') {
    return FONT_NUMBERS[c - '0'];
  } else if (c >= 'A' && c <= 'Z') {
    return FONT_LETTERS[c - 'A'];
  }
  return 0xff;
}

uint32_t str_to_disp(const std::string& str, int from = 0) {
  uint32_t disp = 0;
  for (int i = from; i < from + 4; ++i) {
    if (i < str.size()) {
      disp |= char_to_disp(str[i]) << (i * 8);
    } else {
      disp |= 0xff << (i * 8);
    }
  }
  return disp;
}

class Animation {
public:
  Animation() = default;

  void clear() { data.clear(); current_frame = -1; }

  void reset() { current_frame = (data.empty() ? -1 : 0); }

  void parse_anim(const uint32_t* a, int size) {
    data.clear();
    current_frame = 0;
    for (int i = 0; i < size; ++i) {
      data.push_back(a[i]);
    }
  }

  void set_text(const std::string& text) {
    data.clear();
    current_frame = 0;
    data.push_back(str_to_disp(text));
  }

  void set_text(const std::string& a, const std::string& b, int fd = 1) {
    data.clear();
    current_frame = 0;
    data.push_back(str_to_disp(a));
    data.push_back(str_to_disp(b));
    set_auto(fd);
  }

  void setup() {}
  void tick() {
    if (!data.empty() && frame_delay && millis() - change >= frame_delay) {
      change = millis();
      next();
    }
  }

  void set_auto(int fd) {
    frame_delay = fd;
    change = millis();
  }

  void next() {
    current_frame = (current_frame + 1) % data.size();
  }

  uint32_t get_disp() const { return current_frame >= 0 ? data[current_frame]: 0xffffffff; }

private:
  int current_frame = -1;
  std::vector<uint32_t> data;
  int frame_delay;
  unsigned long change;
};

class Display {
public:
  Display(int l, int c, int d) : pin_latch(l), pin_clock(c), pin_data(d) {};

  void set_anim(Animation* a) {
    anim = a;
  }

  void setup() {
    pinMode(pin_latch, OUTPUT);
    pinMode(pin_clock, OUTPUT);
    pinMode(pin_data, OUTPUT);
  }

  void tick() {
    if (anim) {
      show(anim->get_disp());
    } else {
      show(0xffffffff);
    }
  }

private:
  int pin_latch;
  int pin_clock;
  int pin_data;
  
  Animation* anim;

  static constexpr unsigned char char_buff[]  = {0xF1, 0xF2, 0xF4, 0xF8};                                      

  void seg_write(byte pos, unsigned char data)
  {
    digitalWrite(pin_latch, LOW); 
    shiftOut(pin_data, pin_clock, MSBFIRST, data); 
    shiftOut(pin_data, pin_clock, MSBFIRST, char_buff[pos] );
    digitalWrite(pin_latch, HIGH);    
  }

  void show(uint32_t disp) {
    seg_write(0, disp & 0xff);
    seg_write(1, (disp >> 8) & 0xff);
    seg_write(2, (disp >> 16) & 0xff);
    seg_write(3, (disp >> 24) & 0xff);
  }
};
