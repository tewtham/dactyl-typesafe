#ifndef FiveXSix_h
#define FiveXSix_h

#define NUM_KEYS 56

#define NUM_ROWS 6
#define NUM_COLUMNS 12

// ======================= LAYER 0 ==============================
#define LAYER_0_LEFT \
esc    , keyQ   , keyW   , keyE   , keyR   , keyT   , \
minus  , keyA   , keyS   , keyD   , keyF   , keyG   , \
tab    , keyZ   , keyX   , keyC   , keyV   , keyB   , \
         gui    , tilde  , equal  , unused , unused , \
                           backsp , del    , home   , \
gui, shift, ctrl, alt

#define LAYER_0_RIGHT \
keyY   , keyU   , keyI   , keyO   , keyP   , backsl , \
keyH   , keyJ   , keyK   , keyL   , semicl , quote  , \
keyN   , keyM   , comma  , period , slash  , shift  , \
ply    , lftBrc , rgtBrc    , gui    , gui    ,          \
enter  , space  , layer

// ======================= LAYER 1 ==============================
#define LAYER_1_LEFT \
esc    , scrollX, unused , up     , unused , unused, \
minus  , scrollY, left   , down   , right  , unused, \
tab    , unused , keyX   , keyC   , keyV   , unused, \
         gui    , unused , unused , unused , unused, \
                           backsp , del    , home  , \
gui, shift, ctrl, alt

#define LAYER_1_RIGHT \
scrollX, unused , unused , unused , unused , unused, \
scrollY, mouseL , mouseM , mouseR , unused , unused, \
unused , unused , unused , unused , unused , unused, \
unused , unused , alt    , gui    , unused ,         \
enter  , space  , layer

// ======================= LAYER 2 ==============================
#define LAYER_2_LEFT \
minus  , bang   , at     , hash   , dola   , perc   , \
esc    , key1   , key2   , key3   , key4   , key5   , \
shift  , ply    , vdwn   , vup    , prev   , nex    , \
         gui    , unused , unused , unused , unused , \
                           backsp , del    , home   , \
gui, shift, ctrl, alt

#define LAYER_2_RIGHT \
cart   , amp    , ast    , lpar   , rpar   , unde   , \
key6   , key7   , key8   , key9   , key0   , minus  , \
unused , unused , unused , period , unused , unused , \
unused , unused , alt    , gui    , unused ,          \
unused , unused , layer

#define LAYER_KEYS {{LAYER_0_LEFT,LAYER_0_RIGHT},{LAYER_1_LEFT,LAYER_1_RIGHT},{LAYER_2_LEFT,LAYER_2_RIGHT}}

// Map state col/row to an index in the layout
#define KEY_MAP { \
  { 0, 1, 2, 3, 4, 5,     30,31,32,33,34,35}, \
  { 6, 7, 8, 9,10,11,     36,37,38,39,40,41}, \
  {12,13,14,15,16,17,     42,43,44,45,46,47}, \
  {18,19,20,21,22,-1,     -1,48,49,50,51,52}, \
  {-1,-1,-1,25,24,23,     53,54,55,-1,-1,-1}, \
  {-1,-1,28,29,27,26,     -1,-1,-1,-1,-1,-1}  \
}

#endif
