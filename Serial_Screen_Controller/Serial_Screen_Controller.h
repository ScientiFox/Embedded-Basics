#ifndef SSCh
#define SSCh

#include "Arduino.h"
#include "SoftwareSerial.h"

#define VERTICAL 0
#define HORIZONTAL 1
#define TOP_LEFT 0
#define TOP_RIGHT 1
#define BOTTOM_LEFT 2
#define BOTTOM_RIGHT 3

class Serial_Screen_Controller{

	public:
		Serial_Screen_Controller();
		void draw_rect(int h, int w, int crn, int dir);
		void draw_rect_fill(int h, int w, int crn, int dir);
		void draw_rect_invis(int h, int w, int crn, int dir);
		void draw_circle(int r, int crn, int dir);
		void draw_circle_fill(int r, int crn, int dir);
		void draw_string(String str, int crn, int dir);
		void draw_graph(int data[], int len, int h, int M, int crn, int dir);
		void draw_thermometer(int temp, int crn, int dir);
		void draw_sun(int crn, int dir);
		void draw_sun_full(int crn, int dir);
		void draw_cloud(int crn, int dir);
		void draw_clock(int hr, int min, int crn, int dir);
		void clr();
		void draw();
		
};

#endif
