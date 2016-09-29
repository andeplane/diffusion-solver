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

real K_tabulated[20];
real K_inv_tabulated[20];
bool filled = false;

real K(int poreSize) { // Units: 1 / (nm^3*bar)
    const real Ka = 0.1586;
    const real Kb = -0.2389;
    const real Kc = -0.04202;
    return Ka*pow(poreSize,Kb)+Kc;
}

real DSelf(int poreSize) {
    // Note: multiplied each coefficient by 1e6 to convert end result from m^2/s to nm^2/ps
    real D1 = 0.000007016;
    real D2 = 0.001542;
    real D3 = -0.003727;
    real D4 = 0.0303;
    return (D1*(poreSize)*(poreSize)*(poreSize)+D2*(poreSize)*(poreSize)+D3*(poreSize)+D4); //[nm2/ps]
}

real fugacity(real concentration, int poreSize) {
    return concentration*K_inv_tabulated[poreSize];
}

real concentration(real fugacity, int poreSize) {
    return fugacity*K_tabulated[poreSize];
}

void ForwardEuler::tick(std::shared_ptr<System> systemPtr, real dt)
{
    if(!filled) {
        for(int i=0; i<20; i++) {
            K_tabulated[i] = K(i);
            K_inv_tabulated[i] = 1.0 / K_tabulated[i];
        }
        filled = true;
    }
    System &system = *systemPtr;
    Grid &current = *system.grid();
    if(!m_grid) {
        Grid grid = current;
        m_grid = make_shared<Grid>(grid);
    }

    Grid &next = *m_grid;
    next = current;

    real dr = system.lx(); // TODO: don't assume equal length in all dimensions
    real oneOverDr = 1.0 / dr;
    for(int i=0; i<current.nx(); i++) {
        cout << "  Working on i = " << i << endl;
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

                        real DA = DSelf(cellA.poreSize()) * K_tabulated[cellA.poreSize()];
                        real DB = DSelf(cellB.poreSize()) * K_tabulated[cellB.poreSize()];

                        real DAB= 2.0*DA*DB/(DA+DB); //L-cell size // m^2 / s
                        real JAB=DAB*deltaFugacity*oneOverDr; // m^2/s*bar/r
                        real deltaConcentration=dt*JAB*oneOverDr;

                        if(next(i,j,k)(CONCENTRATION)+deltaConcentration<0) {
                            cout << "Working with (" << i << "," << j << "," << k << ") and (" << i+di << "," << j+dj << "," << k+dk << ")" << endl;
                            cout << "Concentrations: " << concentrationA << " and " << concentrationB << endl;
                            cout << "Fugacities: " << fugacityA << " and " << fugacityB << endl;
                            cout << "Self diffusion: " << DA << " and " << DB << endl;
                            cout << "Effective DAB: " << DAB << endl;
                            cout << "Flux between sites: " << JAB << endl;
                            cout << "DeltaC = " << deltaConcentration << " and CONC_A = " << cellA(CONCENTRATION) << " and CONC_B = " << cellB(CONCENTRATION) << endl;
                            cout << "next(i,j,k)[CONCENTRATION] = " << next(i,j,k)(CONCENTRATION) << endl << endl;
                            cout << "Error, DC was larger than concentration in one of the cells. You need to reduce timestep." << endl;
                            terminate();
                        }
                        
                        next(i,j,k)[CONCENTRATION] += deltaConcentration; // the other cell will get its contribution at a later stage.
                        // TODO: use "newton's third law" here
                    }
                }
            }
        }
    }
    for(auto modifierPtr : m_modifiers) {
        Modifier &modifier = *modifierPtr;
        modifier.apply(next, CONCENTRATION);
    }
    swap(current.cells(), next.cells());
}
