#ifndef Keys_h
#define Keys_h

#include <Keyboard.h>
#include "./constants.h"

#define MOUSE_L 0
#define MOUSE_M 1
#define MOUSE_R 2

#define STANDARD_KEY 0x01 // standard key
#define MOUSE_BUTTON 0x02 // mouse button
#define MOUSE_MOD 0x03 // mouse modifier
#define KEY_MOD 0x04 // modifier
#define TO_LAYER 0x05 // to layer. when pressed, switch to layer 2. when released, switch to layer 0.
#define UNUSED 0x06

class Key {
  public:
    Key(uint8_t t) {
        code = 0;
        type = t;
    }
    Key(uint8_t t, int c) {
      code = c;
      type = t;
    }
    static Key standard(int code) {
      return Key(STANDARD_KEY, code);
    }
    uint8_t type;
    int code;
};

// when pressed, switch to layer 2. when released, switch to layer 0.
// layer 1 is reached by moving the mouse
Key layer = Key(TO_LAYER);
Key unused = Key(UNUSED);

Key mouseL = Key(MOUSE_BUTTON, MOUSE_L);
Key mouseM = Key(MOUSE_BUTTON, MOUSE_M);
Key mouseR = Key(MOUSE_BUTTON, MOUSE_R);

Key scrollX = Key(MOUSE_MOD, MOUSEMOD_SCROLLX);
Key scrollY = Key(MOUSE_MOD, MOUSEMOD_SCROLLY);

Key alt     = Key(KEY_MOD, MODIFIERKEY_ALT);
Key ctrl    = Key(KEY_MOD, MODIFIERKEY_CTRL);
Key gui     = Key(KEY_MOD, MODIFIERKEY_GUI);
Key shift   = Key(KEY_MOD, MODIFIERKEY_SHIFT);

Key backsl  = Key::standard(KEY_BACKSLASH);
Key backsp  = Key::standard(KEY_BACKSPACE);
Key comma   = Key::standard(KEY_COMMA);
Key del     = Key::standard(KEY_DELETE);
Key down    = Key::standard(KEY_DOWN);
Key end     = Key::standard(KEY_END);
Key enter   = Key::standard(KEY_ENTER);
Key equal   = Key::standard(KEY_EQUAL);
Key esc     = Key::standard(KEY_ESC);
Key home    = Key::standard(KEY_HOME);
Key left    = Key::standard(KEY_LEFT);
Key lftBrc  = Key::standard(KEY_LEFT_BRACE);
Key minus   = Key::standard(KEY_MINUS);
Key period  = Key::standard(KEY_PERIOD);
Key quote   = Key::standard(KEY_QUOTE);
Key rgtBrc  = Key::standard(KEY_RIGHT_BRACE);
Key right   = Key::standard(KEY_RIGHT);
Key semicl  = Key::standard(KEY_SEMICOLON);
Key slash   = Key::standard(KEY_SLASH);
Key space   = Key::standard(KEY_SPACE);
Key tab     = Key::standard(KEY_TAB);
Key tilde   = Key::standard(KEY_TILDE);
Key up      = Key::standard(KEY_UP);

Key f1  = Key::standard(KEY_F1);
Key f2  = Key::standard(KEY_F2);
Key f3  = Key::standard(KEY_F3);
Key f4  = Key::standard(KEY_F4);
Key f5  = Key::standard(KEY_F5);
Key f6  = Key::standard(KEY_F6);
Key f7  = Key::standard(KEY_F7);
Key f8  = Key::standard(KEY_F8);
Key f9  = Key::standard(KEY_F9);
Key f10 = Key::standard(KEY_F10);

Key key0 = Key::standard(KEY_0);
Key key1 = Key::standard(KEY_1);
Key key2 = Key::standard(KEY_2);
Key key3 = Key::standard(KEY_3);
Key key4 = Key::standard(KEY_4);
Key key5 = Key::standard(KEY_5);
Key key6 = Key::standard(KEY_6);
Key key7 = Key::standard(KEY_7);
Key key8 = Key::standard(KEY_8);
Key key9 = Key::standard(KEY_9);
Key keyA = Key::standard(KEY_A);
Key keyB = Key::standard(KEY_B);
Key keyC = Key::standard(KEY_C);
Key keyD = Key::standard(KEY_D);
Key keyE = Key::standard(KEY_E);
Key keyF = Key::standard(KEY_F);
Key keyG = Key::standard(KEY_G);
Key keyH = Key::standard(KEY_H);
Key keyI = Key::standard(KEY_I);
Key keyJ = Key::standard(KEY_J);
Key keyK = Key::standard(KEY_K);
Key keyL = Key::standard(KEY_L);
Key keyM = Key::standard(KEY_M);
Key keyN = Key::standard(KEY_N);
Key keyO = Key::standard(KEY_O);
Key keyP = Key::standard(KEY_P);
Key keyQ = Key::standard(KEY_Q);
Key keyR = Key::standard(KEY_R);
Key keyS = Key::standard(KEY_S);
Key keyT = Key::standard(KEY_T);
Key keyU = Key::standard(KEY_U);
Key keyV = Key::standard(KEY_V);
Key keyW = Key::standard(KEY_W);
Key keyX = Key::standard(KEY_X);
Key keyY = Key::standard(KEY_Y);
Key keyZ = Key::standard(KEY_Z);

Key bang = Key::standard(0x21);
Key at = Key::standard(0x40);
Key hash = Key::standard(0x23);
Key dola = Key::standard(0x24);
Key perc = Key::standard(0x25);
Key cart = Key::standard(0x5E);
Key amp = Key::standard(0x26);
Key ast = Key::standard(0x2A);
Key lpar = Key::standard(0x28);
Key rpar = Key::standard(0x29);
Key unde = Key::standard(0x5F);

Key ply = Key::standard(KEY_MEDIA_PLAY_PAUSE);
Key vup = Key::standard(KEY_MEDIA_VOLUME_INC);
Key vdwn = Key::standard(KEY_MEDIA_VOLUME_DEC);
Key nex = Key::standard(KEY_MEDIA_NEXT_TRACK);
Key prev = Key::standard(KEY_MEDIA_PREV_TRACK);


#endif
