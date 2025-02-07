// #include <Arduino.h>

#include <FastLED.h>
#include <Wire.h>
#include <RTClib.h>
#include "planets.h"
#include "eccentric_anomaly.h"

RTC_DS1307 rtc;

#define LED_PIN     2
#define NUM_LEDS    241
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
#define UNIX_TO_J2K_OFFSET_S 946728000

CRGB leds[NUM_LEDS];
const uint8_t led_count[] = {60, 48, 40, 32, 24, 16, 12, 8, 1};

#define NUM_RINGS (sizeof(led_count) / sizeof(led_count[0]))
#define NUM_PLANETS NUM_RINGS-1

// Load the Keplerian elements for all planets
keplerian_elements planets[NUM_PLANETS]={
  Mercury,
  Venus,
  Earth_Moon_barycenter,
  Mars,
  Jupiter,
  Saturn,
  Uranus,
  Neptune
};

enum ClockMode{
  LIVE=0,
  PLAYBACK
};

enum ClockMode clock_mode=LIVE;
double days_since_j2k_epoch=0;
DateTime last_frame_time;
double playback_speedup=100000;

CRGB getPlanetColor(int planetIndex);




void setup() {
  Serial.begin(115200);

  Wire.begin();
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  
  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // This line sets the RTC with an explicit date & time, for example to set
    // January 1, 2024 at 00:00 you would call:
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // rtc.adjust(DateTime(2024, 1, 1, 0, 0, 0));
  }

  // Set up LED matrix
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  last_frame_time = rtc.now();
  
}

void loop() {
  // Get time
  DateTime now = rtc.now();
  char clock_buffer[] = "DDD, DD MMM YYYY hh:mm:ss";
  Serial.println(now.toString(clock_buffer));

  if (clock_mode==LIVE){  
    uint32_t seconds_since_unix_epoch = now.unixtime();
    uint32_t seconds_since_j2k_epoch = seconds_since_unix_epoch-UNIX_TO_J2K_OFFSET_S;
    days_since_j2k_epoch = seconds_since_j2k_epoch/(60.*60.*24.);
  } else {
    // // Increment the time by time since last frame, multiplied by playback speedup, in days.
    // days_since_j2k_epoch += (now-last_frame_time).totalseconds()*playback_speedup/(60*60*24);
    // // days_since_j2k_epoch += 0.0001*playback_speedup/(60*60*24);
    // last_frame_time = now;
  }

  // Initialize LEDs
  CRGB *led = leds;
  // uint8_t ring = 0;
  uint8_t ring_offset = 0;
  Serial.print("Days since J2k:");
  Serial.println(days_since_j2k_epoch);
  // Cycle across planets from outside in
  for(uint8_t ring_id=0; ring_id<NUM_PLANETS; ring_id++) {
    uint8_t planet_id = (NUM_PLANETS-1)-ring_id; // Skip the Sun, and -1 to count from 0.
    uint8_t count = led_count[ring_id];

    Serial.print("Planet: ");
    Serial.print(planet_id);
    Serial.print(": ");
    double longitude_planet = longitude_at_date(planets[planet_id], days_since_j2k_epoch);
    // Wrapping is imperfect for negative numbers.
    if (longitude_planet<-M_PI){
      longitude_planet+=M_PI;
    }
    Serial.print(longitude_planet);
    Serial.print(" rad or ");
    Serial.print(longitude_planet*180./M_PI);
    Serial.print(" deg, corresponding to LED ");

    uint8_t ring_planet_id = round(count*(longitude_planet+M_PI)/(2*M_PI));
    
    Serial.print(ring_planet_id);
    
    // Wrap if we find the final LED
    if (ring_planet_id==count){
      ring_planet_id-=count;
    }
    // Black out the ring
    for (uint8_t i = 0; i < count ; i++) {
      *led++ = CRGB::Black;
    }
    // Color the relevant planet
    // uint8_t ring_index = seconds_since_unix_epoch%count;
    Serial.print(". We will light LED ");
    Serial.println(ring_offset+ring_planet_id);
    leds[ring_offset+ring_planet_id] = getPlanetColor(planet_id);
    ring_offset += count;
    
  }
  // Color the Sun
  leds[NUM_LEDS-1] = 0xf28322; //getPlanetColor(0);

  FastLED.show();
  sleep(1);
}

// void lightPlanet(int planetIndex, float year) {
//   float position = fmod(year / orbitalPeriods[planetIndex], 1.0); // Get orbit progress (0-1)
//   int totalLEDsInRing = ringEnd[planetIndex] - ringStart[planetIndex] + 1;
//   int ledIndex = ringStart[planetIndex] + (int)(position * totalLEDsInRing); // Map to ring LED
//   leds[ledIndex] = getPlanetColor(planetIndex);
// }
// Planet colors, inspired by https://i.imgur.com/3pL0vJq.png
CRGB getPlanetColor(int planetIndex) {
  switch (planetIndex) {
    // case 0: return 0xf28322; //CRGB::Yellow;
    case 0: return 0xBFBEBD; //CRGB::Orange;   // Mercury
    case 1: return 0xD9B391; //CRGB::Green;    // Venus
    case 2: return 0xBACBD9; //CRGB::Blue;     // Earth
    case 3: return 0xF27A5E; //CRGB::Red;      // Mars
    case 4: return 0xBF8136; //CRGB::Purple;   // Jupiter
    case 5: return 0xF2CD88; //CRGB::Cyan;     // Saturn
    case 6: return 0xCEF2F2; //CRGB::Magenta;  // Uranus
    case 7: return 0x789EBF; //CRGB::White;    // Neptune
    default: return CRGB::Black;   // Default to off
  }
}