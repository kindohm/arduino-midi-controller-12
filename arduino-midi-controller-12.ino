#include <MIDIUSB.h>

// The number of knobs. 
const int numPins = 6;

// Change to the MIDI channel you want to output to
const int midiChannel = 15;

// Each pin outputs to MIDI CC control number of 
// its pin number plus this value. e.g. A0 == CC 1,
// A1 == CC 2, etc.
const int ccOffset = 1;

// Pin for the LED "on" indicator.
const int ledPin = 7;

const int serialRate = 9600;
const int minAnalog = 0;
const int maxAnalog = 1023;
const int minMIDI = 0;
const int maxMIDI = 127;
int currentValues[numPins];
int ccValue = 0;
int outputValue = 0;
bool on = false;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(serialRate);
}

void loop() {
  // only need to turn the LED on once.
  if (!on){
    on = true;
    digitalWrite(ledPin, HIGH);
  }

  for (int i = 0; i < numPins; i++) {
    ccValue = map(analogRead(i), minAnalog, maxAnalog, minMIDI, maxMIDI);
    
    if (currentValues[i] != ccValue) {
      currentValues[i] = ccValue;
      controlChange(midiChannel, i + ccOffset, ccValue);
    }
  }

  MidiUSB.flush();
}

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}

