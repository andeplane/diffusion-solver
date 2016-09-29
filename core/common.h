#ifndef COMMON_H
#define COMMON_H

#include <vector>
#include <memory>
#include <cmath>
typedef double real;

static real K_tabulated[20];
static real K_inv_tabulated[20];
static real D_tabulated[20];
static bool filled = false;

inline real K(int poreSize) { // Units: 1 / (nm^3*bar)
    const real Ka = 0.1586;
    const real Kb = -0.2389;
    const real Kc = -0.04202;
    return Ka*pow(poreSize,Kb)+Kc;
}

inline real DSelf(int poreSize) {
    // Note: multiplied each coefficient by 1e6 to convert end result from m^2/s to nm^2/ps
    real D1 = 0.000007016;
    real D2 = 0.001542;
    real D3 = -0.003727;
    real D4 = 0.0303;
    return (D1*(poreSize)*(poreSize)*(poreSize)+D2*(poreSize)*(poreSize)+D3*(poreSize)+D4); //[nm2/ps]
}

inline real fugacity(real concentration, int poreSize) {
    return concentration*K_inv_tabulated[poreSize];
}

inline real concentration(real fugacity, int poreSize) {
    return fugacity*K_tabulated[poreSize];
}

#endif // COMMON_H
