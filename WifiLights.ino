// WifiLights
// Portions of the animation routines from FastLED DemoReel100 (see LICENSE)
// Configure settings in WifiLightsConfig.h before flashing
//#define USEOLED
#define FASTLED_ALLOW_INTERRUPTS 0
#include "FastLED.h"
#include <ArduinoJson.h>
#include "WifiLightsConfig.h"
//#include <ArduinoOTA.h>
#include <WiFi.h>
#include <Wire.h>
#include "SSD1306.h"
#include <QList.h>


SSD1306  display(0x3c, 33, 27);
FASTLED_USING_NAMESPACE

CRGB leds[NUM_LEDS];
int brightness = BRIGHTNESS;
WiFiClient client;

QList<String> list;
int maxline = 5;

bool printlog(String str = " ") {
  #ifdef USEOLED
  list.push_back(str);
  if(list.length()>maxline) {
    list.pop_front();
  }
  display.clear();
  for(int i=0; i<maxline; i++) {
    display.drawString(0, i*10, list[i]);
  }
  display.display();
  #endif
  Serial.println(str);
}

void setup() {
  Serial.begin(115200);
  delay(100);
  #ifdef USEOLED
  display.init();
  #endif
  connectToWifi();

  /*Setup OTA
  ArduinoOTA.onStart([]() {
    printlog("Starting OTA Update");
  });
  ArduinoOTA.onEnd([]() {
    printlog("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) printlog("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) printlog("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) printlog("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) printlog("Receive Failed");
    else if (error == OTA_END_ERROR) printlog("End Failed");
  });
  ArduinoOTA.begin();

  printlog("OTA Updates Enabled");
  */

  // Setup LEDs
  delay(2400); //Delay for recovery 
  
  // Configure FastLED
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(brightness);
}

// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, confetti, dotsweep, juggle, bpm, solid, twinkle, fader };

uint8_t gCurrentPatternNumber = 8; // Default (startup) pattern is solid
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
uint32_t color1 = 255; //Blue
uint32_t color2 = 65280; //Green

void loop()
{
  //gPatterns[gCurrentPatternNumber](); // Call the current pattern function once, updating the 'leds' array
  
  //FastLED.show();  // send the 'leds' array out to the actual LED strip
  //FastLED.delay(1000/FRAMES_PER_SECOND); // insert a delay to keep the framerate modest
  
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  EVERY_N_SECONDS( 2 ) { pollService(); } // poll service for latest pattern setting
  EVERY_N_MILLISECONDS( 17 ) { animationFrame(); };
  //Check for OTA updates
  //ArduinoOTA.handle();
}

void animationFrame() {
  gPatterns[gCurrentPatternNumber](); // Call the current pattern function once, updating the 'leds' array
  FastLED.show();  // send the 'leds' array out to the actual LED strip
}

void connectToWifi() {
  // Connect to WIFI
  printlog();
  printlog();
  printlog("Connecting to ");
  printlog(WIFISSID);
  WiFi.begin(WIFISSID, PWD);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  printlog("");
  printlog("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

}

bool checkOrEstablishConnection() {
  if(WiFi.status() != WL_CONNECTED) {
    printlog("Network Connection Lost");
    connectToWifi();
  }
  if(client.connected()) {
    Serial.println("Closing connection");
    client.stop();
  }
  Serial.print("Connecting to ");
  Serial.println(HOST);
  if(client.connect(HOST, PORT)) {
    Serial.println("Connected");
    return true;
  }else{
    printlog("Connection failed");
    return false;
  }
}

void pollService()
{
  if(!checkOrEstablishConnection()) {
    //We'll try again next loop
    return;
  }
  
  // We now create a URI for the request
  Serial.print("Requesting URL: ");
  Serial.println(PATH);
  
  // This will send the request to the server
  client.print(String("GET ") + PATH + " HTTP/1.1\r\n" +
               "Host: " + HOST + "\r\n" + 
               "Connection: close\r\n\r\n");

  //Trim Headers
  char endOfHeaders[] = "\r\n\r\n";
  if (!client.find(endOfHeaders)) {
    printlog("Invalid response");
    return;
  }

  //Allocate JSON Buffer
  const size_t BUFFER_SIZE = JSON_OBJECT_SIZE(4) + 70;
  DynamicJsonBuffer jsonBuffer(BUFFER_SIZE);

  //Parse
  JsonObject& root = jsonBuffer.parseObject(client);
  if (!root.success()) {
    printlog("Parsing failed!");
    return;
  }
  
  int pattern = root["pattern"];
  color1 = root["color1"];
  color2 = root["color2"];
  if(root["brightness"]) {
    brightness = root["brightness"];
    printlog("brightness: " + String(brightness));
  }
  printlog("pattern: " + String(pattern));
  printlog("color1: " + String(color1));
  printlog("color2: " + String(color2));
 
  // Set pattern based on response
  switch(pattern) {
    case 0:
      printlog("Setting LEDs to OFF");
      FastLED.setBrightness(0);
      break;
    case 1:
      printlog("Setting LEDs to RANDOM");
      FastLED.setBrightness(brightness);
      nextPattern();
      break;
    case 2:
      printlog("Setting LEDs to RAINBOW");
      FastLED.setBrightness(brightness);
      gCurrentPatternNumber = 0;
      break;
    case 3:
      printlog("Setting LEDs to RAINBOW WITH GLITTER");
      FastLED.setBrightness(brightness);
      gCurrentPatternNumber = 1;
      break;     
    case 4:
      printlog("Setting LEDs to CONFETTI");
      FastLED.setBrightness(brightness);
      gCurrentPatternNumber = 2;
      break;   
    case 5:
      printlog("Setting LEDs to DOTSWEEP");
      FastLED.setBrightness(brightness);
      gCurrentPatternNumber = 3;
      break;
    case 6:
      printlog("Setting LEDs to JUGGLE");
      FastLED.setBrightness(brightness);
      gCurrentPatternNumber = 4;
      break;
    case 7:
      printlog("Setting LEDs to BPM");
      FastLED.setBrightness(brightness);
      gCurrentPatternNumber = 5;
      break;
    case 8:
      printlog("Setting LEDs to SOLID");
      FastLED.setBrightness(brightness);
      gCurrentPatternNumber = 6;
      break;
    case 9:
      printlog("Setting LEDs to TWINKLE");
      FastLED.setBrightness(brightness);
      gCurrentPatternNumber = 7;
      break;
    case 10:
      printlog("Setting LEDs to FADER");
      FastLED.setBrightness(brightness);
      gCurrentPatternNumber = 8;
      break;
    default:
      //Unrecognized response, just increment to the next pattern
      printlog("Unrecognized command. Incrementing pattern.");
      FastLED.setBrightness(brightness);
      nextPattern();
      break;
  }
  
}

// LED ANIMATION PATTERNS===========================================================================================

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void twinkle()
{
  // themed colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += color1; 
  pos = random16(NUM_LEDS);
  leds[pos] += color2;
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void dotsweep()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16(13,0,NUM_LEDS);
  leds[pos] += CHSV( gHue, 255, 192);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16(i+7,0,NUM_LEDS)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

void solid() {
  fill_solid(leds, NUM_LEDS, color1);
}

void fader() {
  fill_solid(leds, NUM_LEDS, CHSV(gHue,200,255));
}
