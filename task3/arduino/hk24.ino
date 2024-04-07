#include <vector>
#include <button.h>
#include <notes.h>
#include <animation.h>
#include <player.h>
#include <resources.h>
#include <songlistener.h>

Player player(3);

Animation anim_song;
Animation anim_text;
Display disp(4, 7, 8);

std::vector<int> parse_str(const std::string& str) {
  int current = 0;
  bool empty = true;
  std::vector<int> result;
  for (char c : str) {
    if (c >= '0' && c <= '9') {
      current = current * 10 + c - '0';
      empty = false;
    } else {
      if (!empty) {
        result.push_back(current);
      }
      current = 0;
      empty = true;
    }
  }
  if (!empty) {
    result.push_back(current);
  }
  return result;
}

class Main {
public:
  enum class State { WAITING, TONE, SCALE, SONG };
public:

  void setup() {
    melodies.emplace_back(std::vector<int>(std::begin(MELODY_TETRIS), std::end(MELODY_TETRIS)), 144);
    melodies.emplace_back(std::vector<int>(std::begin(MELODY_DOOM), std::end(MELODY_DOOM)), 225);
    melodies.emplace_back(std::vector<int>(std::begin(MELODY_NGGYU), std::end(MELODY_NGGYU)), 114);
    melodies.emplace_back(std::vector<int>(std::begin(MELODY_TAKEONME), std::end(MELODY_TAKEONME)), 140);
  }
  void tick() {}

  void song_received(const std::string& s) {
    Melody m;
    auto vec = parse_str(s);
    m.tempo = vec.back();
    vec.pop_back();
    m.data = vec;

    state = State::WAITING;
    start();
    melodies.push_back(m);
    melody_id = melodies.size() - 1;
    state = State::SONG;
    start();
  }

  void button_pressed(int btn) {
    if (state == State::WAITING) {
      if (btn == 1) {
        state = State::TONE;
      } else if (btn == 2) {
        state = State::SCALE;
      } else if (btn == 3) {
        state = State::SONG;
      }
      start();
    } else {
      if (btn == 1) {
        state = State::WAITING;
        start();
      } else if (btn == 2 && state != State::SONG) {
        if (start_note_id+ 1 < N_NOTES) {
          start_note_id++;
          start();
        }
      } else if (btn == 3 && state != State::SONG) {
        if (start_note_id > 0) {
          start_note_id--;
          start();
        }
      } else if (btn == 2) {
        melody_id = (melody_id + 1) % melodies.size();
        start();
      }
    }
  }

private:
  State state;
  int start_note_id = 46; // A4
  int melody_id = 0;

  std::vector<Melody> melodies;

  void start() {
    if (state == State::WAITING) {
      player.pause();
      anim_text.clear();
      disp.set_anim(&anim_text);
    } else if (state == State::TONE) {
      player.single_tone(start_note_id);
      anim_text.set_text(NOTE_NAMES[start_note_id], std::to_string(FREQUENCIES[start_note_id]), 1000);
      disp.set_anim(&anim_text);
      player.resume();
    } else if (state == State::SCALE) {
      player.dur_scale(start_note_id,120);
      anim_text.set_text(NOTE_NAMES[start_note_id]);
      disp.set_anim(&anim_text);
      player.resume();
    } else if (state == State::SONG) {
      player.parse_melody(melodies[melody_id]);
      anim_song.reset();
      disp.set_anim(&anim_song);
      player.resume();
    }
  }
};

Main control;

Button b1(A1, []() { control.button_pressed(1); }, 0);
Button b2(A2, []() { control.button_pressed(2); }, 0);
Button b3(A3, []() { control.button_pressed(3); }, 0);

Listener lis(115200, [](const std::string& s) { control.song_received(s); });

void setup() {
  player.setup();
  anim_song.setup();
  anim_text.setup();
  disp.setup();
  b1.setup();
  b2.setup();
  b3.setup();
  control.setup();
  lis.setup();

  anim_song.parse_anim(animation3, sizeof(animation3)/sizeof(animation3[0]));
  player.set_anim(&anim_song);
}

void loop() {
  player.tick();
  anim_song.tick();
  anim_text.tick();
  disp.tick();
  b1.tick();
  b2.tick();
  b3.tick();
  control.tick();
  lis.tick();
}
