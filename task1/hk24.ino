#include <vector>
#include <button.h>
#include <notes.h>
#include <animation.h>
#include <player.h>

int melody[] = {

  //Based on the arrangement at https://www.flutetunes.com/tunes.php?id=192
  
  NOTE_E5, 4,  NOTE_B4,8,  NOTE_C5,8,  NOTE_D5,4,  NOTE_C5,8,  NOTE_B4,8,
  NOTE_A4, 4,  NOTE_A4,8,  NOTE_C5,8,  NOTE_E5,4,  NOTE_D5,8,  NOTE_C5,8,
  NOTE_B4, -4,  NOTE_C5,8,  NOTE_D5,4,  NOTE_E5,4,
  NOTE_C5, 4,  NOTE_A4,4,  NOTE_A4,8,  NOTE_A4,4,  NOTE_B4,8,  NOTE_C5,8,

  NOTE_D5, -4,  NOTE_F5,8,  NOTE_A5,4,  NOTE_G5,8,  NOTE_F5,8,
  NOTE_E5, -4,  NOTE_C5,8,  NOTE_E5,4,  NOTE_D5,8,  NOTE_C5,8,
  NOTE_B4, 4,  NOTE_B4,8,  NOTE_C5,8,  NOTE_D5,4,  NOTE_E5,4,
  NOTE_C5, 4,  NOTE_A4,4,  NOTE_A4,4, REST, 4,

  NOTE_E5, 4,  NOTE_B4,8,  NOTE_C5,8,  NOTE_D5,4,  NOTE_C5,8,  NOTE_B4,8,
  NOTE_A4, 4,  NOTE_A4,8,  NOTE_C5,8,  NOTE_E5,4,  NOTE_D5,8,  NOTE_C5,8,
  NOTE_B4, -4,  NOTE_C5,8,  NOTE_D5,4,  NOTE_E5,4,
  NOTE_C5, 4,  NOTE_A4,4,  NOTE_A4,8,  NOTE_A4,4,  NOTE_B4,8,  NOTE_C5,8,

  NOTE_D5, -4,  NOTE_F5,8,  NOTE_A5,4,  NOTE_G5,8,  NOTE_F5,8,
  NOTE_E5, -4,  NOTE_C5,8,  NOTE_E5,4,  NOTE_D5,8,  NOTE_C5,8,
  NOTE_B4, 4,  NOTE_B4,8,  NOTE_C5,8,  NOTE_D5,4,  NOTE_E5,4,
  NOTE_C5, 4,  NOTE_A4,4,  NOTE_A4,4, REST, 4,
  

  NOTE_E5,2,  NOTE_C5,2,
  NOTE_D5,2,   NOTE_B4,2,
  NOTE_C5,2,   NOTE_A4,2,
  NOTE_GS4,2,  NOTE_B4,4,  REST,8, 
  NOTE_E5,2,   NOTE_C5,2,
  NOTE_D5,2,   NOTE_B4,2,
  NOTE_C5,4,   NOTE_E5,4,  NOTE_A5,2,
  NOTE_GS5,2,

};

uint32_t animation[] = {
  0xC0FFFFFF,
  0xFFC0FFFF,
  0xFFFFC0FF,
  0xFFFFFFC0,
  0xFFFFC0FF,
  0xFFC0FFFF
};

Player player(3);

Animation anim_song;
Animation anim_text;
Display disp(4, 7, 8);

class Main {
public:
  enum class State { WAITING, TONE, SCALE, SONG };
public:

  void setup() {}
  void tick() {}

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
      }
    }
  }

private:
  State state;
  int start_note_id= 46; // A4

  void start() {
    if (state == State::WAITING) {
      player.pause();
      anim_text.clear();
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
      player.parse_melody(melody, sizeof(melody)/sizeof(melody[0]), 144);
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

void setup() {
  player.setup();
  anim_song.setup();
  anim_text.setup();
  disp.setup();
  b1.setup();
  b2.setup();
  b3.setup();
  control.setup();

  anim_song.parse_anim(animation, sizeof(animation)/sizeof(animation[0]));
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
}
