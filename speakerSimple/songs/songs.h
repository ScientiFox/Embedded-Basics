/*************************************************
 * Public constants
 *************************************************/

#define  NUL 0
#define  c0  16
#define  cs0 17
#define  d0  18
#define  ds0 19
#define  e0  21
#define  f0  22
#define  fs0 23
#define  g0  25
#define  gs0 26
#define  a0  28
#define  as0 29
#define  b0  31
#define  c1  33
#define  cs1 35
#define  d1  37
#define  ds1 39
#define  e1  41
#define  f1  44
#define  fs1 46
#define  g1  49
#define  gs1 52
#define  a1  55
#define  as1 58
#define  b1  62
#define  c2  65
#define  cs2 69
#define  d2  73
#define  ds2 78
#define  e2  82
#define  f2  87
#define  fs2 93
#define  g2  98
#define  gs2 104
#define  a2  110
#define  as2 117
#define  b2  123
#define  c3  131
#define  cs3 139
#define  d3  147
#define  ds3 156
#define  e3  165
#define  f3  175
#define  fs3 185
#define  g3  196
#define  gs3 208
#define  a3  220
#define  as3 233
#define  b3  247
#define  c4  262 //middle c
#define  cs4 277
#define  d4  294
#define  ds4 311
#define  e4  330
#define  f4  349
#define  fs4 370
#define  g4  392
#define  gs4 415
#define  a4  440
#define  as4 466
#define  b4  494
#define  c5  523
#define  cs5 554
#define  d5  587
#define  ds5 622
#define  e5  659
#define  f5  698
#define  fs5 740
#define  g5  784
#define  gs5 831
#define  a5  880
#define  as5 932
#define  b5  988
#define  c6  1047
#define  cs6 1109
#define  d6  1175
#define  ds6 1245
#define  e6  1319
#define  f6  1397
#define  fs6 1480
#define  g6  1568
#define  gs6 1661
#define  a6  1760
#define  as6 1865
#define  b6  1976
#define  c7  2093
#define  cs7 2217
#define  d7  2349
#define  ds7 2489
#define  e7  2637
#define  f7  2794
#define  fs7 2960
#define  g7  3136
#define  gs7 3322
#define  a7  3520
#define  as7 3729
#define  b7  3951
#define  c8  4186
#define  cs8 4435
#define  d8  4699
#define  ds8 4978
#define  e8  5274
#define  f8  5588
#define  fs8 5920
#define  g8  6272
#define  gs8 6645
#define  a8  7040
#define  as8 7459
#define  b8  7902


////    MUsIc    /////
/*
  note durations:
 2 =        half note,
 4 =     quarter note,
 8 =      eighth note,
 4/3 =    half dot note,
 8/3 = quarter dot note
 */

///   default (shave and a Hair cut)   ///
int twobits[] = {
     c4, 4, g3, 8, g3, 8, a3, 4, g3, 4, 0, 4, b3, 4, c4, 4, -1};

//charge (fanfare)
int charge[] = {
	 c5, 1, d5, 1, e5, 1, g5, 2, e5, 1, g5, 3, -1};


  ///   Ode to Joy   ///
  int odetojoy[] = {
     e4, 4, e4, 4, f4, 4, g4, 4, g4, 4, f4, 4, e4, 4, d4, 4, c4, 4, c4, 4, d4, 4, e4, 4, e4, 4, d4, 4, d4, 4,
     e4, 4, e4, 4, f4, 4, g4, 4, g4, 4, f4, 4, e4, 4, d4, 4, c4, 4, c4, 4, d4, 4, e4, 4, d4, 4, c4, 4, c4, 4,
     d4, 4, d4, 4, e4, 4, c4, 4, d4, 4, e4, 4, f4, 4, e4, 4, c4, 4, d4, 4, e4, 4, f4, 4, e4, 4, d4, 4, c4, 4,
	 d4, 4, g3, 4, e4, 4, e4, 4, f4, 4, g4, 4, g4, 4, f4, 4, e4, 4, d4, 4, c4, 4, c4, 4, d4, 4, e4, 4, d4, 4, c4, 4, c4, 4, -1 };


  ///   The Imperial March   ///
  int imperial[] = {
     e3, 4, e3, 4, e3, 4, c3, 5, g3, 16, e3, 4, c3, 5, g3, 16, e3, 2,
     b3, 4, b3, 4, b3, 4, c4, 5, g3, 16, e3, 4, c3, 5, g3, 16, e3,  2, -1};





