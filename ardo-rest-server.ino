/* 
  This a simple example of the aREST Library for Arduino (Uno/Mega/Due/Teensy)
  using the Ethernet library (for example to be used with the Ethernet shield). 
  See the README file for more details.
 
  Written in 2014 by Marco Schwartz under a GPL license. 
*/

// Libraries
#include <SPI.h>
#include <Ethernet.h>
#include <aREST.h>
#include <avr/wdt.h>

// Enter a MAC address for your controller below.
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0E, 0xFE, 0x40 };

// IP address in case DHCP fails
IPAddress ip(10,0,0,3);

// Ethernet server
EthernetServer server(80);

// Create aREST instance
aREST rest = aREST();

// Variables to be exposed to the API
int temperature;
int humidity;

/* MUSIC */
int speakerPin = 9;

void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(speakerPin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(speakerPin, LOW);
    delayMicroseconds(tone);
  }
}

void playNote(char note, int duration) {
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };
  
  // play the tone corresponding to the note name
  for (int i = 0; i < 8; i++) {
    if (names[i] == note) {
      playTone(tones[i], duration);
    }
  }
}

void setup(void)
{  
  pinMode(speakerPin, OUTPUT);

  // Start Serial
  Serial.begin(9600);
  
  // Init variables and expose them to REST API
  temperature = 24;
  humidity = 40;
  rest.variable("temperature",&temperature);
  rest.variable("humidity",&humidity);

  // Function to be exposed
  rest.function("play", playSong);
  
  // Give name and ID to device
  rest.set_id("008");
  rest.set_name("dapper_drake");

  // Start the Ethernet connection and the server
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());

  // Start watchdog
//  wdt_enable(WDTO_4S);
}

void loop() {  
  
  // listen for incoming clients
  EthernetClient client = server.available();
  rest.handle(client);
//  wdt_reset();
  
}

// Custom function accessible by the API
int playSong(String command) {
  char song[command.length() + 1];
  command.toCharArray(song, command.length() + 1); // a space represents a rest
  int beats[]    = { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4 };
  int songLength = (unsigned)strlen(song); // the number of notes
  int tempo      = 300;

  Serial.println(song);
  Serial.println(command.length());
  Serial.println(command);

  // Get state from command
//  int state = command.toInt();
  
  for (int i = 0; i < songLength; i++) {
    if (song[i] == ' ') {
      delay(beats[i] * tempo); // rest
    } else {
      playNote(song[i], 1 * tempo);
    }
    
    // pause between notes
    delay(tempo / 2); 
  }

  return 1;
}

// Custom function accessible by the API
int ledControl(String command) {
  
  // Get state from command
  int state = command.toInt();
  
  digitalWrite(6,state);
  return 1;
}
