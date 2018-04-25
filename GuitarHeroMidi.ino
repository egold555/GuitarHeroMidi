
#include <Bounce2.h>

// MIDI channel to use.
const int channel = 1;

// Number of buttons on the fret of the guitar.
const int fretCount = 5;

// Which arduino pins are connected to the fret buttons.
int fretPins[fretCount] = {
  10, 11, 12, 6, 7
};

// Which MIDI notes each of the fret buttons should play.
int note[fretCount] = {
  60, 62, 64, 65, 67    // C, D, E, F, G
};

// Which arduino pin the strum button is connected to.
const int strumPin = 8;

// We use Bounce objects to debounce the inputs from the buttons.
// There are the bounce objects for the strum and fret buttons.
// Note that LOW means the button is pressed in, HIGH is not pressed.
// So fell() means pressed, rose() means released.
Bounce strumBounce;
Bounce fretBounce[fretCount];

// Set to true if the given fret note is playing.
boolean fretPlaying[fretCount];

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the fret input buttons.
  for (int i = 0; i < fretCount; ++i) {
    pinMode(fretPins[i], INPUT_PULLUP);
    fretBounce[i].attach(fretPins[i]);
  }

  // initialize the strum input button.
  pinMode(strumPin, INPUT_PULLUP);
  strumBounce.attach(strumPin); 
 
  Serial.begin(19200); 
}



// the loop routine runs over and over again forever:
void loop() {
  // Update the state of the buttons.
  for (int i = 0; i < fretCount; ++i) {
    fretBounce[i].update();
    if (fretBounce[i].fell()) {
      //Serial.print("fret ");
      //Serial.print(i);
      //Serial.println(" pressed");
    }
  }
  strumBounce.update();
  if (strumBounce.fell()) {
    //Serial.println("strum pressed");
  }

  // When strum is hit, play all frets that are down.
  if (strumBounce.fell()) {
    // The strum was hit.
    for (int fret = 0; fret < fretCount; ++fret) {
      if (fretBounce[fret].read() == LOW) {
        // Play the note 
        noteOn(note[fret]); 
        fretPlaying[fret] = true;
      }
    } 
  }

  // When a fret button is released, stop the note
  // if it was playing.
  for (int fret = 0; fret < fretCount; ++fret) {
    if (fretBounce[fret].rose() && fretPlaying[fret]) {
      // Release the note 
      noteOff(note[fret]); 
      fretPlaying[fret] = false;
    }
  } 
}

void noteOn(int note){
  usbMIDI.sendNoteOn(note, 127, channel);
  //Serial.print(" ON: ");
  //Serial.println(note);
}

void noteOff(int note){
  usbMIDI.sendNoteOff(note, 0, channel);
  //Serial.print("OFF: ");
  //Serial.println(note);
}




