/*
This library provides a simple control method for a speaker, allowing
you to play a song in the form of a list of notes and durations.
*/

#include "Arduino.h"
#include "speakerSimple.h"

speakerSimple::speakerSimple(int pin){
	speakerpin = pin; //pin you're using for the speaker
}

void speakerSimple::play(int song[]){
	//Plays a song
	//A song is an array which alternates elements between frequencies and
	//	durations. The 'song' [220,400,350,200] plays 220Hz for 400 ms and
	//	the 350Hz for 200 ms.
	int i = 0; //song array index
	int duration; //Variable to hold how long the current note should be played
	while (song[i] != -1){ //While not at the end of the song
		duration = 100*song[i+1]; //Get the time to play this note
		if (song[i] != 0){ //If the current note is not a 'hold'
	    	tone(speakerpin,song[i]); //Put the note as a tone on speaker
	    	delay(duration);} //and wait the duration - we don't use noTone because we want to avoid gaps
	    else{ //If the dcurrent node is a hold
			noTone(speakerpin); //Take the sond off the speaker
			delay(duration);} //And delay the duration
		i+=2; //Increment the song index- 2 because we used the note and the duration together
	}
	noTone(speakerpin); //Once the song is over, turn off the speaker
} 
