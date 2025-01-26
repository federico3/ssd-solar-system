/*
Compute the position of the planets
following the JPL SSD guide at
https://ssd.jpl.nasa.gov/planets/approx_pos.html
*/

#ifndef KEPLERIAN_ANOMALY_H
#define KEPLERIAN_ANOMALY_H

#include "keplerian_elements.h"
#include "math.h"
#include <stdio.h>
typedef double jd; 

#define NEWTON_EPSILON 0.000001*M_PI/180 // SSD suggests 1e-6 degrees
#define MAX_NEWTON_ITERATIONS 100

double epoch_j2k = 2451545.0;

// Solve Kepler equation for eccentric anomaly.
// The equation is M = E-esin(E). 
// We use Newton-Rapson to find a fixed point and following
// https://en.wikipedia.org/w/index.php?title=Kepler%27s_equation&section=9
double Enextf(double E, double e, double M){
    return E-(E-e*sin(E)-M)/(1-e*cos(E));
};

double eccentric_anomaly_at_date(const keplerian_elements planet, const double days_since_j2k){
    // Compute the time since epoch, T
    // double time_since_epoch_centuries = (julian_date-epoch_j2k)/36525;
    double time_since_epoch_centuries = days_since_j2k/36525;
    
    // Compute the relevant orbital parameters at T

    // Compute the argument of periapsis from the longitude of periapsis
    // See https://en.wikipedia.org/wiki/Longitude_of_periapsis
    double lon_periapsis_deg = planet.lon_periapsis_deg+planet.lon_periapsisdot*time_since_epoch_centuries;
    double Omega_deg = planet.Omega_deg+planet.Omegadot*time_since_epoch_centuries;

    double L = planet.L_deg+planet.Ldot*time_since_epoch_centuries; // This carries the motion along the orbit
    double e = planet.e + planet.edot*time_since_epoch_centuries;

    // Compute argument of the perihelion
    double argument_of_perihelion = lon_periapsis_deg-Omega_deg;

    // Compute the mean anomaly
    double mean_anomaly_deg = L  
                           - lon_periapsis_deg
                           + planet.b*(time_since_epoch_centuries*time_since_epoch_centuries)
                           + planet.c * cos(planet.f*M_PI/180.*time_since_epoch_centuries)
                           + planet.s * sin(planet.f*M_PI/180.*time_since_epoch_centuries);
    // Reduce mean anomaly to [-180, 180]
    mean_anomaly_deg = fmod(mean_anomaly_deg+180., 360.)-180.;

    double mean_anomaly_rad = mean_anomaly_deg*M_PI/180;

    // printf ("Mean anomaly: %f degrees or %f rad\n", mean_anomaly_deg, mean_anomaly_rad);

    // Solve Kepler equation for eccentric anomaly.
    // The equation is M = E-esin(E). 
    // We use Newton-Rapson to find a fixed point/

    // Initial guess from https://ssd.jpl.nasa.gov/planets/approx_pos.html
    double eccentric_anomaly_rad = mean_anomaly_rad + e*sin(mean_anomaly_rad);

    // printf ("Eccentric anomaly (initial guess): %f rad\n", eccentric_anomaly_rad);

    // Newton
    for (int i=0; i<MAX_NEWTON_ITERATIONS; i++){
        eccentric_anomaly_rad = Enextf(eccentric_anomaly_rad, e, mean_anomaly_rad);
        if ((eccentric_anomaly_rad-e*sin(eccentric_anomaly_rad)-mean_anomaly_rad)<NEWTON_EPSILON){
            break;
        }
    }

    // printf ("Eccentric anomaly (Newton-Rapson): %f rad after %d iterations\n", eccentric_anomaly_rad, i);

    return eccentric_anomaly_rad;
};

double true_anomaly_at_date(const keplerian_elements planet, const double days_since_j2k){
    // Eccentric anomaly
    double eccentric_anomaly_rad = eccentric_anomaly_at_date(planet, days_since_j2k);
    // Eccentricity
    double time_since_epoch_centuries = days_since_j2k/36525;
    double e = planet.e + planet.edot*time_since_epoch_centuries;
    double true_anomaly_rad = atan2(sqrt(1-e*e)*sin(eccentric_anomaly_rad),cos(eccentric_anomaly_rad)-e);
    // printf(" From eccentric anomaly %f, we find a true anomaly of %f rad\n", eccentric_anomaly_rad, true_anomaly_rad);
    
    return true_anomaly_rad;
}

double longitude_at_date(const keplerian_elements planet, const double days_since_j2k){
    double true_anomaly_rad = true_anomaly_at_date(planet, days_since_j2k);

    double time_since_epoch_centuries = days_since_j2k/36525;
    double lon_periapsis_deg = planet.lon_periapsis_deg+planet.lon_periapsisdot*time_since_epoch_centuries;
    // With respect to the vernal equinox, if i=0, then you rotate by Omega to
    // find the RAAN; then rotate by omega to find the periapsis; and finally
    // run along the orbit by the true anomaly to find the location of the body.
    // And remember that the longitude of the periapsis IS Omega+omega.
    double longitude_rad = lon_periapsis_deg*M_PI/180. + true_anomaly_rad;
    return longitude_rad;
}

#endif