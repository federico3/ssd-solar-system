#include <time.h>
#include <stdio.h>
#include <math.h>
#include "planets.h"
#include "orbits.h"
extern long int timezone; // Seconds between localtime and UTC.

int main(){
    // Get time, inspired by time(&timer);  /* get current time; same as: timer = time(NULL)  */
    time_t timer;
    time(&timer);  // get current time as seconds since UTC epoch

    /* We would like the current time in days (defined as 24*60*60s) since the
    J2000 epoch, which is 12 noon on Jan 1, 2000 in the UTC timezone. 
    */

    // J2K time is 2000-1-1 12:00 *IN GMT TIMEZONE*. 
    // This computes a time in the LOCAL timezone
    struct tm j2000_tm = {
        .tm_sec=0,    // Seconds after the minute
        .tm_min=0,    // Minutes after the hour
        .tm_hour=12,  // Hours since midnight
        .tm_mday=1,   // Day of the month, 1-31
        .tm_mon=0,    // Months since Jan, 0-11
        .tm_year=100, // Years since 1900
    };

    // struct tm unix_reference_but_local_tm = {
    //     .tm_sec=0,    // Seconds after the minute
    //     .tm_min=0,    // Minutes after the hour
    //     .tm_hour=0,  // Hours since midnight
    //     .tm_mday=1,   // Day of the month, 1-31
    //     .tm_mon=0,    // Months since Jan, 0-11
    //     .tm_year=70, // Years since 1900
    // };

    struct tm mars_opposition_tm = { // Jan 16 2025, https://www.skyatnightmagazine.com/advice/skills/mars-opposition
        .tm_sec=0,    // Seconds after the minute
        .tm_min=0,    // Minutes after the hour
        .tm_hour=0,  // Hours since midnight
        .tm_mday=16,   // Day of the month, 1-31
        .tm_mon=0,    // Months since Jan, 0-11
        .tm_year=125, // Years since 1900
    };

    struct tm mars_conjunction_tm = { // Sept 2 2019, https://www.jpl.nasa.gov/news/whats-mars-solar-conjunction-and-why-does-it-matter/
        .tm_sec=0,    // Seconds after the minute
        .tm_min=0,    // Minutes after the hour
        .tm_hour=0,  // Hours since midnight
        .tm_mday=2,   // Day of the month, 1-31
        .tm_mon=8,    // Months since Jan, 0-11
        .tm_year=119, // Years since 1900
    };
    
    time_t j2000_localtime = mktime(&j2000_tm);
    time_t j2000_utctime = j2000_localtime-timezone; 
    // time_t julian_epoch_time = mktime(&julian_epoch_tm);
    // time_t unix_epoch_local_time = mktime(&unix_reference_but_local_tm);
    
    double seconds_since_j2k = difftime(timer, j2000_utctime);
    double days_since_j2k = seconds_since_j2k/(60*60*24);

    double true_anomaly_Earth = true_anomaly_at_date(Earth_Moon_barycenter, days_since_j2k);
    double longitude_Earth = longitude_at_date(Earth_Moon_barycenter, days_since_j2k);
    // double seconds_j2k_to_julian_epoch = difftime(j2000_time, julian_epoch_time);
    // double seconds_unix_to_j2k_all_local = difftime(j2000_utctime, unix_epoch_local_time);

    printf ("%.f seconds since J2K in the current timezone\n", seconds_since_j2k);
    printf ("%f days since J2K in the current timezone\n", days_since_j2k);
    printf ("%f ish years since J2K in the current timezone\n", seconds_since_j2k/(60*60*24*365.25));
    printf ("The Earth's true anomaly is %f rad or %f degrees \n", true_anomaly_Earth, true_anomaly_Earth*180./M_PI);
    printf ("The Earth's longitude today is %f rad or %f degrees \n", longitude_Earth, longitude_Earth*180./M_PI);

    keplerian_elements planets[8]={Mercury, Venus, Earth_Moon_barycenter, Mars, Jupiter, Saturn, Uranus, Neptune};

    for (int i=0; i<8; i++){
        double longitude_planet = longitude_at_date(planets[i], days_since_j2k);
        printf("Longitude for planet %d: %f rad, %f deg\n", i, longitude_planet, longitude_planet*180./M_PI);
    }

    double mars_opposition_j2k_seconds = difftime(mktime(&mars_opposition_tm), j2000_utctime);
    double mars_opposition_25_date = mars_opposition_j2k_seconds/(60*60*24);

    double earth_lon_at_mars_opposition = longitude_at_date(Earth_Moon_barycenter, mars_opposition_25_date);
    double mars_lon_at_mars_opposition = longitude_at_date(Mars, mars_opposition_25_date);

    printf (" On Jan 16 2025, Earth longitude is %f and Mars longitude is %f. Mars is supposed to be at opposition\n", earth_lon_at_mars_opposition*180./M_PI, mars_lon_at_mars_opposition*180./M_PI);

    double mars_conjunction_j2k_seconds = difftime(mktime(&mars_conjunction_tm), j2000_utctime);
    double mars_conjunction_j2k_date = mars_conjunction_j2k_seconds/(60*60*24);

    double earth_lon_at_mars_conjunction = longitude_at_date(Earth_Moon_barycenter, mars_conjunction_j2k_date);
    double mars_lon_at_mars_conjunction = longitude_at_date(Mars, mars_conjunction_j2k_date);

    printf (" On Sept 2 2019, Earth longitude is %f and Mars longitude is %f. Mars is supposed to be at conjunction\n", earth_lon_at_mars_conjunction*180./M_PI, mars_lon_at_mars_conjunction*180./M_PI);


    return 0;
}