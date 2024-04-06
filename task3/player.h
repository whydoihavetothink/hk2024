#include <vector>

constexpr int DUR_SCALE[] = {0,2,4,5,7,9,11,12};

struct Note {
  int freq;
  double dur;

  Note(int f, double d) : freq(f), dur(d) {}
};

struct Melody {
  std::vector<int> data;
  int tempo = 60;

  Melody() = default;
  Melody(const std::vector<int>& d, int t) : data(d), tempo(t) {}
};

double parse_dur(int dur) {
  if (dur >= 0) {
    return 1.0 / dur;
  }
  return 1.5 / -dur;
}

class Player {
public:
  Player(int p) : pin(p) {}

  void parse_melody(const Melody& melody) {
    tempo = melody.tempo;
    notes.clear();
    for (int i = 0; i < melody.data.size(); i += 2) {
      notes.emplace_back(melody.data[i], parse_dur(melody.data[i + 1]));
    }
    restart();
  }

  void single_tone(int tone_id, double dur = 1.0) {
    tempo = 1;
    notes.clear();
    notes.emplace_back(FREQUENCIES[tone_id], dur);
    restart();
  }

  void dur_scale(int tone_id, int tmp) {
    tempo = tmp;
    notes.clear();
    for (int i = 0; i < 8; ++i) {
      notes.emplace_back(FREQUENCIES[tone_id + DUR_SCALE[i]], 0.25);
    }
    for (int i = 7; i >= 0; --i) {
      notes.emplace_back(FREQUENCIES[tone_id + DUR_SCALE[i]], 0.25);
    }
    restart();
  }

  void setup() {
    pinMode(pin, OUTPUT);
  }

  void restart() {
    current_note = 0;
    playing = false;
  }

  void playpause() {
    if (playing) {
      pause();
    } else {
      resume();
    }
  }

  void resume() {
    playing = true;
    play();
  }

  void pause() {
    playing = false;
  }

  void set_anim(Animation* a) {
    anim = a;
  }

  void tick() {
    if (!playing) {
      noTone(pin);
      return;
    }
    long curr = millis();
    auto note = notes[current_note];
    double whole_note = 60000.0 * 4 / tempo;
    if (curr - note_started >= whole_note * note.dur) {
      ++current_note;
      if (current_note >= notes.size()) {
        restart();
      } else {
        play();
        auto next = notes[current_note];
        if (next.freq > note.freq) {
          anim_next = true;
        } else if (next.freq < note.freq) {
          anim_next = false;
        }
        if (anim) {
          if (anim_next) {
            anim->next();
          } else {
            anim->prev();
          }
        }
      }
    }
  }

private:
  int pin;
  std::vector<Note> notes;
  int tempo = 60;
  bool playing = false;
  int current_note = 0;
  long note_started = 0;
  bool anim_next = true;
  Animation* anim = 0;

  void play() {
    double whole_note = 60000.0 * 4 / tempo;
    tone(pin, notes[current_note].freq, whole_note * notes[current_note].dur * 0.9);
    note_started = millis();
  }
};
