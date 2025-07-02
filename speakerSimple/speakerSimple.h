#ifndef speakersimnpleh
#define speakerSimpleh

#include <Arduino.h>

class speakerSimple{
		public:
			speakerSimple(int pin);
			void play(int song[]);
			void play(String song);
			
		private:
			int speakerpin;
};

#endif

