#include "doctest/doctest.h"
#include "time.h"
#include "../planets.h"
#include "../planets_1800-2050.h"
#include "../orbits.h"

typedef struct alignment {
    int planet1;
    int planet2;
    tm date;
} alignment;

TEST_CASE("Checking conjunctions and oppositions"){
    struct tm j2000_tm = {
        .tm_sec=0,    // Seconds after the minute
        .tm_min=0,    // Minutes after the hour
        .tm_hour=12,  // Hours since midnight
        .tm_mday=1,   // Day of the month, 1-31
        .tm_mon=0,    // Months since Jan, 0-11
        .tm_year=100, // Years since 1900
    };
    time_t j2000_localtime = mktime(&j2000_tm);
    time_t j2000_utctime = j2000_localtime-timezone;

    keplerian_elements planets_lr[8]={Mercury, Venus, Earth_Moon_barycenter, Mars, Jupiter, Saturn, Uranus, Neptune};
    keplerian_elements planets_sr[8]={Mercury_sr, Venus_sr, Earth_Moon_barycenter_sr, Mars_sr, Jupiter_sr, Saturn_sr, Uranus_sr, Neptune_sr};
    // This is a container of containers? It's all pointers anyway.
    keplerian_elements * all_planets[2]={planets_lr, planets_sr};

    SUBCASE("Oppositions"){
        // Oppositions from https://www.rmg.co.uk/stories/topics/what-planet-opposition
        alignment oppositions[6]={
            {2,6, {
                    .tm_sec=0,    // Seconds after the minute
                    .tm_min=0,    // Minutes after the hour
                    .tm_hour=12,  // Hours since midnight
                    .tm_mday=17,   // Day of the month, 1-31
                    .tm_mon=10,    // Months since Jan, 0-11
                    .tm_year=124, // Years since 1900
                }
            }, // Uranus
            {2,4, {
                    .tm_sec=0,    // Seconds after the minute
                    .tm_min=0,    // Minutes after the hour
                    .tm_hour=12,  // Hours since midnight
                    .tm_mday=7,   // Day of the month, 1-31
                    .tm_mon=11,    // Months since Jan, 0-11
                    .tm_year=124, // Years since 1900
                }
            }, // Jupiter
            {2,3, {
                    .tm_sec=0,    // Seconds after the minute
                    .tm_min=0,    // Minutes after the hour
                    .tm_hour=12,  // Hours since midnight
                    .tm_mday=16,   // Day of the month, 1-31
                    .tm_mon=0,    // Months since Jan, 0-11
                    .tm_year=125, // Years since 1900
                }
            }, // Mars
            {2,5, {
                    .tm_sec=0,    // Seconds after the minute
                    .tm_min=0,    // Minutes after the hour
                    .tm_hour=12,  // Hours since midnight
                    .tm_mday=21,   // Day of the month, 1-31
                    .tm_mon=8,    // Months since Jan, 0-11
                    .tm_year=125, // Years since 1900
                }
            }, // Saturn
            {2,7, {
                    .tm_sec=0,    // Seconds after the minute
                    .tm_min=0,    // Minutes after the hour
                    .tm_hour=12,  // Hours since midnight
                    .tm_mday=23,   // Day of the month, 1-31
                    .tm_mon=8,    // Months since Jan, 0-11
                    .tm_year=125, // Years since 1900
                }
            }, // Neptune
            {2,6, {
                    .tm_sec=0,    // Seconds after the minute
                    .tm_min=0,    // Minutes after the hour
                    .tm_hour=12,  // Hours since midnight
                    .tm_mday=21,   // Day of the month, 1-31
                    .tm_mon=10,    // Months since Jan, 0-11
                    .tm_year=125, // Years since 1900
                }
            }, // Uranus
        };
        for (int j=0; j<2; j++){
            // Iterate over sets of planets
            keplerian_elements * planets = all_planets[j]; 
            for (int i=0; i<6; i++){ // Iterate over oppositions
                time_t opposition_timer = mktime(&oppositions[i].date);
                double seconds_since_j2k = difftime(opposition_timer, j2000_utctime);
                double days_since_j2k = seconds_since_j2k/(60*60*24);
                double p1_lon = longitude_at_date(planets[oppositions[i].planet1], days_since_j2k);
                double p2_lon = longitude_at_date(planets[oppositions[i].planet2], days_since_j2k);
                CHECK(abs(p1_lon-p2_lon)<1./180.*M_PI);
                // printf("Planet longitudes: %f and %f\n Difference %f rad or %f deg\n",p1_lon, p2_lon, abs(p1_lon-p2_lon), abs(p1_lon-p2_lon)*180./M_PI);
            }
        }
    }

    SUBCASE("Solar conjunctions"){
        // From https://in-the-sky.org/article.php?amp=1&term=solar_conjunction
        alignment conjunctions[11]={
            {2,0, {
                    .tm_sec=0,    // Seconds after the minute
                    .tm_min=19,    // Minutes after the hour
                    .tm_hour=4,  // Hours since midnight
                    .tm_mday=9,   // Day of the month, 1-31
                    .tm_mon=1,    // Months since Jan, 0-11
                    .tm_year=125, // Years since 1900
                }
            }, // Mercury
            {2,5, {
                    .tm_sec=0,    // Seconds after the minute
                    .tm_min=19,    // Minutes after the hour
                    .tm_hour=3,  // Hours since midnight
                    .tm_mday=12,   // Day of the month, 1-31
                    .tm_mon=2,    // Months since Jan, 0-11
                    .tm_year=125, // Years since 1900
                }
            }, // Saturn
            {2,7, {
                    .tm_sec=0,    // Seconds after the minute
                    .tm_min=16,    // Minutes after the hour
                    .tm_hour=16,  // Hours since midnight
                    .tm_mday=19,   // Day of the month, 1-31
                    .tm_mon=2,    // Months since Jan, 0-11
                    .tm_year=125, // Years since 1900
                }
            }, // Neptune
            {2,1, {
                    .tm_sec=0,    // Seconds after the minute
                    .tm_min=2,    // Minutes after the hour
                    .tm_hour=18,  // Hours since midnight
                    .tm_mday=22,   // Day of the month, 1-31
                    .tm_mon=2,    // Months since Jan, 0-11
                    .tm_year=125, // Years since 1900
                }
            }, // Venus
            {2,0, {
                    .tm_sec=0,    // Seconds after the minute
                    .tm_min=43,    // Minutes after the hour
                    .tm_hour=12,  // Hours since midnight
                    .tm_mday=24,   // Day of the month, 1-31
                    .tm_mon=2,    // Months since Jan, 0-11
                    .tm_year=125, // Years since 1900
                }
            }, // Mercury
            {2,6, {
                    .tm_sec=0,    // Seconds after the minute
                    .tm_min=23,    // Minutes after the hour
                    .tm_hour=16,  // Hours since midnight
                    .tm_mday=17,   // Day of the month, 1-31
                    .tm_mon=4,    // Months since Jan, 0-11
                    .tm_year=125, // Years since 1900
                }
            }, // Uranus
            {2,0, {
                    .tm_sec=0,    // Seconds after the minute
                    .tm_min=19,    // Minutes after the hour
                    .tm_hour=21,  // Hours since midnight
                    .tm_mday=29,   // Day of the month, 1-31
                    .tm_mon=4,    // Months since Jan, 0-11
                    .tm_year=125, // Years since 1900
                }
            }, // Mercury
            {2,4, {
                    .tm_sec=0,    // Seconds after the minute
                    .tm_min=5,    // Minutes after the hour
                    .tm_hour=8,  // Hours since midnight
                    .tm_mday=24,   // Day of the month, 1-31
                    .tm_mon=5,    // Months since Jan, 0-11
                    .tm_year=125, // Years since 1900
                }
            }, // Jupiter
            {2,0, {
                    .tm_sec=0,    // Seconds after the minute
                    .tm_min=36,    // Minutes after the hour
                    .tm_hour=16,  // Hours since midnight
                    .tm_mday=31,   // Day of the month, 1-31
                    .tm_mon=6,    // Months since Jan, 0-11
                    .tm_year=125, // Years since 1900
                }
            }, // Mercury
            {2,0, {
                    .tm_sec=0,    // Seconds after the minute
                    .tm_min=00,    // Minutes after the hour
                    .tm_hour=4,  // Hours since midnight
                    .tm_mday=13,   // Day of the month, 1-31
                    .tm_mon=8,    // Months since Jan, 0-11
                    .tm_year=125, // Years since 1900
                }
            }, // Mercury
            {2,0, {
                    .tm_sec=0,    // Seconds after the minute
                    .tm_min=19,    // Minutes after the hour
                    .tm_hour=1,  // Hours since midnight
                    .tm_mday=20,   // Day of the month, 1-31
                    .tm_mon=10,    // Months since Jan, 0-11
                    .tm_year=125, // Years since 1900
                }
            }, // Mercury
        };

        for (int j=0; j<2; j++){
            // Iterate over sets of planets
            keplerian_elements * planets = all_planets[j]; 
            for (int i=0; i<11; i++){
                time_t conjunction_timer = mktime(&conjunctions[i].date);
                double seconds_since_j2k = difftime(conjunction_timer, j2000_utctime);
                double days_since_j2k = seconds_since_j2k/(60*60*24);
                double p1_lon = longitude_at_date(planets[conjunctions[i].planet1], days_since_j2k);
                double p2_lon = longitude_at_date(planets[conjunctions[i].planet2], days_since_j2k);
                double longitude_difference = abs(p1_lon-p2_lon);
                if (conjunctions[i].planet2>2){ // For Mars and onward, oppositions definitely happen on opposite sides of the Sun
                    CHECK(longitude_difference>179./180.*M_PI);
                    CHECK(longitude_difference<181./180.*M_PI);
                } else { // For Mercury or Venus, conjunction may also well be opposition
                    if (longitude_difference<M_PI/2.){
                        CHECK(longitude_difference<1./180.*M_PI);
                    } else {
                        CHECK(longitude_difference>179./180.*M_PI);
                        CHECK(longitude_difference<181./180.*M_PI);
                    }
                }
                // printf("Planet longitudes: %f and %f\n Difference %f rad or %f deg\n",p1_lon, p2_lon, abs(p1_lon-p2_lon), abs(p1_lon-p2_lon)*180./M_PI);
            }
        }
    }
}