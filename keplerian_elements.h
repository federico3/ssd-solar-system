#ifndef KEPLERIAN_ELEMENTS_H
#define KEPLERIAN_ELEMENTS_H

typedef struct keplerian_elements {
    double a_au;        // Semi-major axis, AU
    double e;           // Eccentricity
    double I_deg;       // Inclination, degrees
    double L_deg;       // Mean longitude, degrees
    double lon_periapsis_deg;   // longitude of the perihelion, degrees
    double Omega_deg;   // Longitude of the ascending node, degrees

    double adot;        // SMA Precession, AU/century
    double edot;        // e precession, per century
    double Idot;        // Precession of inclination, degrees/century
    double Ldot;        // Precession of mean longitude, degrees/century
    double lon_periapsisdot;    // Precession of lph, degrees/century
    double Omegadot;    // Precessio of the lan, degrees/century;

    double b;           // Higher-order correction factor for Jupiter-Neptune
    double c;           // Higher-order correction factor for Jupiter-Neptune
    double s;           // Higher-order correction factor for Jupiter-Neptune
    double f;           // Higher-order correction factor for Jupiter-Neptune
} keplerian_elements;

#endif