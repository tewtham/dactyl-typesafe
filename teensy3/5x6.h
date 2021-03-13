#ifndef FiveXSix_h
#define FiveXSix_h

#define NUM_KEYS 64

#define NUM_ROWS 6
#define NUM_COLUMNS 12

// ======================= LAYER 0 ==============================
#define LAYER_0_LEFT \
esc    , key1   , key2   , key3   , key4   , key5   , \
tab    , keyQ   , keyW   , keyE   , keyR   , keyT   , \
minus  , keyA   , keyS   , keyD   , keyF   , keyG   , \
shift  , keyZ   , keyX   , keyC   , keyV   , keyB   , \
                  gui    , tilde  , equal  , unused , \
                           backsp , alt    , home   , \
                           ctrl   , end

#define LAYER_0_RIGHT \
key6   , key7   , key8   , key9   , key0   , minus  , \
keyY   , keyU   , keyI   , keyO   , keyP   , backsl , \
keyH   , keyJ   , keyK   , keyL   , semicl , quote  , \
keyN   , keyM   , comma  , period , slash  , shift  , \
lftBrc , rgtBrc , alt , gui , \
enter  , \
layer  , space

// ======================= LAYER 1 ==============================
#define LAYER_1_LEFT \
unused , unused , unused , unused , unused , unused, \
esc    , scrollX, unused , up     , unused , unused, \
minus  , scrollY, left   , down   , right  , unused, \
shift  , unused , keyX   , keyC   , keyV   , unused, \
                  gui    , unused , unused , unused, \
                           backsp , alt    , home  , \
                           ctrl   , end

#define LAYER_1_RIGHT \
unused , unused , unused , unused , unused , unused, \
scrollX, unused , unused , unused , unused , unused, \
scrollY, mouseL , mouseM , mouseR , unused , unused, \
unused , unused , unused , unused , unused , unused, \
unused , unused , alt    , gui    , \
enter  , \
layer  , space

// ======================= LAYER 2 ==============================
#define LAYER_2_LEFT \
unused , unused , unused , unused , unused , unused , \
unused , unused , unused , unused , unused , unused , \
unused , unused , unused , unused , unused , unused , \
shift  , ply    , vdwn   , vup    , prev   , nex    , \
                  gui    , unused , unused , unused , \
                           backsp , alt    , home   , \
                           ctrl   , end

#define LAYER_2_RIGHT \
unused , unused , unused , unused , unused , unused , \
unused , unused , unused , unused , unused , unused , \
unused , unused , unused , unused , unused , unused , \
unused , unused , unused , unused , unused , unused , \
unused , unused , alt    , gui    , \
unused , \
layer  , unused

#define LAYER_KEYS {{LAYER_0_LEFT,LAYER_0_RIGHT},{LAYER_1_LEFT,LAYER_1_RIGHT},{LAYER_2_LEFT,LAYER_2_RIGHT}}

// Map state col/row to an index in the layout
#define KEY_MAP { \
  { 0, 1, 2, 3, 4, 5,     33,34,35,36,37,38}, \
  { 6, 7, 8, 9,10,11,     39,40,41,42,43,44}, \
  {12,13,14,15,16,17,     45,46,47,48,49,50}, \
  {18,19,20,21,22,23,     51,52,53,54,55,56}, \
  {24,25,26,27,-1,28,     -1,-1,57,58,59,60}, \
  {-1,-1,29,30,32,31,     63,62,61,-1,-1,-1}  \
}

#endif
