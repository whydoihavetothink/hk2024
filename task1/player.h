#include <vector>

struct Note {
  int freq;
  double dur;

  Note(int f, double d) : freq(f), dur(d) {}
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

  void parse_melody(const int* melody, int size, int tmp = 60) {
    tempo = tmp;
    notes.clear();
    for (int i = 0; i < size; i += 2) {
      notes.emplace_back(melody[i], parse_dur(melody[i + 1]));
    }
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

  void set_anim(Animation& a) {
    anim = &a;
  }

  void tick() {
    if (!playing)
      return;
    long curr = millis();
    auto note = notes[current_note];
    double whole_note = 60000.0 * 4 / tempo;
    if (curr - note_started >= whole_note * note.dur) {
      ++current_note;
      if (current_note >= notes.size()) {
        restart();
      } else {
        play();
        if (anim) {
          anim->next();
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
  Animation* anim = 0;

  void play() {
    double whole_note = 60000.0 * 4 / tempo;
    tone(pin, notes[current_note].freq, whole_note * notes[current_note].dur * 0.9);
    note_started = millis();
  }
};
