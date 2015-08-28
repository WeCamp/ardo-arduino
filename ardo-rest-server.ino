// include Libraries
#include <SPI.h>
#include <Ethernet.h>
#include <aREST.h>

// Enter a MAC address for your controller below.
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0E, 0xFE, 0x40 };

// IP address in case DHCP fails
IPAddress ip(10, 0, 0, 3);

// Ethernet server
EthernetServer server(80);

// Create aREST instance
aREST rest = aREST();

/* SET PINS */
int speakerPin     = 9;
int temperaturePin = 0;

void setup(void)
{
  pinMode(speakerPin, OUTPUT);

  // Start Serial
  Serial.begin(9600);

  // Function to be exposed
  rest.function("temp", temperature);
  rest.function("play", playSong);
      
  // Give name and ID to device
  rest.set_id("001");
  rest.set_name("Ardo");

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
}

void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();

  rest.handle(client);
}



/* Route: /play */

// Custom function accessible by the API
int playSong(String command) {

  int songSize   = command.length() + 1;
  int songLength = command.length();
  int tempo      = 150;

  char song[songSize];
  command.toCharArray(song, songSize); // a space represents a rest

  for (int i = 0; i < songLength; i++) {
    if (song[i] == ' ') {
      delay(1 * tempo); // rest
    } else {
      playNote(song[i], 1 * tempo);
    }

    // pause between notes
    delay(tempo / 2);
  }

  return 1;
}

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

/* Route: temperature*/
int temperature(String command) {
  Serial.println("Starting temperature");
  //getting the voltage reading from the tem
  float temperature = getVoltage(temperaturePin);

  // temperature sensor
  temperature = (temperature - .5) * 100;
  
  return (int)temperature;
}

float getVoltage(int pin) {
  //converting from a 0 to 1023 digital range
  // to 0 to 5 volts (each 1 reading equals ~ 5 millivolts
  return (analogRead(pin) * .004882814);
}
