
class Animation {
public:
  Animation(int l, int c, int d) : pin_latch(l), pin_clock(c), pin_data(d) {};

  void parse_anim(const uint32_t* a, int size) {
    data.clear();
    for (int i = 0; i < size; ++i) {
      data.push_back(a[i]);
    }
    current_frame = 0;
  }

  void setup() {
    pinMode(pin_latch, OUTPUT);
    pinMode(pin_clock, OUTPUT);
    pinMode(pin_data, OUTPUT);
  }

  void tick() {
    uint32_t disp = current_frame >= 0 ? data[current_frame]: 0xffffffff;
    seg_write(0, disp & 0xff);
    seg_write(1, (disp >> 8) & 0xff);
    seg_write(2, (disp >> 16) & 0xff);
    seg_write(3, (disp >> 24) & 0xff);
  }

  void next() {
    current_frame = (current_frame + 1) % data.size();
  }

private:
  int pin_latch;
  int pin_clock;
  int pin_data;
  int current_frame = -1;
  std::vector<uint32_t> data;

  static constexpr unsigned char char_buff[]  = {0xF1, 0xF2, 0xF4, 0xF8};                                      

  void seg_write(byte pos, unsigned char data)
  {
    digitalWrite(pin_latch, LOW); 
    shiftOut(pin_data, pin_clock, MSBFIRST, data); 
    shiftOut(pin_data, pin_clock, MSBFIRST, char_buff[pos] );
    digitalWrite(pin_latch, HIGH);    
  }
};
