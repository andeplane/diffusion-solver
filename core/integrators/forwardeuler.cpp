#include "forwardeuler.h"
#include "../grid.h"
#include "../system.h"
#include "../modifiers/modifier.h"
#include <iostream>
#include <cmath>
using namespace std;
ForwardEuler::ForwardEuler() : Integrator()
{

}


real K(int poreSize) {
    const real Ka = 0.1586;
    const real Kb = -0.2389;
    const real Kc = -0.04202;
    return Ka*pow(poreSize,Kb)+Kc;
}

real DSelf(int poreSize) {
    real D1 = 0.000000000007016;
    real D2 = 0.000000001542;
    real D3 = -0.000000003727;
    real D4 = 0.0000000303;
    //return (D1*(poreSize)*(poreSize)*(poreSize)+D2*(poreSize)*(poreSize)+D3*(poreSize)+D4); //[m2/s]
    return (D1*(poreSize)*(poreSize)*(poreSize)+D2*(poreSize)*(poreSize)+D3*(poreSize)+D4)*1e6; //[nm2/ps]
}

real fugacity(real concentration, int poreSize) {
    // cout << "fugacity(" << concentration << "," << poreSize << ") K = " << K(poreSize) << endl;
    return concentration/K(poreSize);
}

real concentration(real fugacity, int poreSize) {
    return fugacity*K(poreSize);
}

void ForwardEuler::tick(std::shared_ptr<System> systemPtr, real dt)
{
    System &system = *systemPtr;
    Grid &current = *system.grid();
    Grid next = current;
    real dr = system.lx(); // TODO: don't assume equal length in all dimensions
    real oneOverDr = 1.0 / dr;
    for(int i=0; i<current.nx(); i++) {
        for(int j=0; j<current.ny(); j++) {
            for(int k=0; k<current.nz(); k++) {
                // Loop through the 3 dimensions and the nearest neighbor in each
                for(int lowerDimension=-1; lowerDimension<=1; lowerDimension+=2) {
                    for(int a=0; a<3; a++) {
                        int di = a==0 ? lowerDimension : 0;
                        int dj = a==1 ? lowerDimension : 0;
                        int dk = a==2 ? lowerDimension : 0;
                        Cell &cellA = current(i,j,k);
                        Cell &cellB = current[ current.indexPeriodic(i+di,j+dj,k+dk) ];

                        real concentrationA = cellA[CONCENTRATION];
                        real concentrationB = cellB[CONCENTRATION];

                        real fugacityA = fugacity(concentrationA, cellA.poreSize());
                        real fugacityB = fugacity(concentrationB, cellB.poreSize());


                        real deltaFugacity = fugacityB - fugacityA;

                        real DA = DSelf(cellA.poreSize());
                        real DB = DSelf(cellB.poreSize());

                        real DAB= 2.0*DA*DB/(DA+DB); //L-cell size
                        real JAB=DAB*deltaFugacity*oneOverDr; //flux btw 2 sites
                        real DC=dt*JAB*oneOverDr;
                        if(DC<0 && fabs(DC)>cellA[CONCENTRATION]) {
                            cout << "Working with (" << i << "," << j << "," << k << ") and (" << i+di << "," << j+dj << "," << k+dk << ")" << endl;
                            cout << "Concentrations: " << concentrationA << " and " << concentrationB << endl;
                            cout << "Fugacities: " << fugacityA << " and " << fugacityB << endl;
                            cout << "Self diffusion: " << DA << " and " << DB << endl;
                            cout << "Effective DAB: " << DAB << endl;
                            cout << "Flux between sites: " << JAB << endl;
                            cout << "DC = " << DC << " and CA = " << cellA[CONCENTRATION] << " and CB = " << cellB[CONCENTRATION] << endl << endl;
                            cout << "Error, DC was larger than concentration in one of the cells. You need to reduce timestep." << endl;
                            terminate();
                        }

                        next(i,j,k)[CONCENTRATION] += DC; // the other cell will get its contribution at a later stage.
                        // TODO: use "newton's third law" here
                    }
                }
            }
        }
    }
    for(auto modifierPtr : m_modifiers) {
        Modifier &modifier = *modifierPtr;
        modifier.apply(next);
    }
    next.iterate([&](Cell &cell, int i, int j, int k) {
        cout << i << "," << j << "," << k << ": " << cell(CONCENTRATION) << endl;
    });
    swap(current.cells(), next.cells());
}
