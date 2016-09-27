#include "forwardeuler.h"
#include "../grid.h"
#include <iostream>
#include <cmath>
using namespace std;
ForwardEuler::ForwardEuler(shared_ptr<class Grid> previous, shared_ptr<class Grid> current, real dr) : Integrator(),
    m_previous(previous), m_current(current), m_dr(dr)
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
    return (D1*(poreSize)*(poreSize)*(poreSize)+D2*(poreSize)*(poreSize)+D3*(poreSize)+D4)*1000; //[µ2/ns]
}

real fugacity(real concentration, int poreSize) {
    return concentration/K(poreSize);
}

real concentration(real fugacity, int poreSize) {
    return fugacity*K(poreSize);
}

void ForwardEuler::tick(real dt)
{
    if(!m_previous || !m_current) {
        cerr << "Error, ForwardEuler doesn't have initial grids set up." << endl;
        terminate();
    }

    int CONCENTRATION = 0;

    Grid &current = *m_current;
    Grid &previous = *m_previous;
    for(int i=0; i<current.nx(); i++) {
        for(int j=0; j<current.ny(); j++) {
            for(int k=0; k<current.nz(); k++) {
                // Loop through the 3 dimensions and the nearest neighbor in each
                for(int a=0; a<3; a++) {
                    int di = a==0 ? 1 : 0;
                    int dj = a==1 ? 1 : 0;
                    int dk = a==2 ? 1 : 0;
                    Cell &cellA = previous(i,j,k);
                    Cell &cellB = previous[ previous.indexPeriodic(i+di,j+dj,k+dk) ];

                    real concentrationA = cellA[CONCENTRATION];
                    real concentrationB = cellB[CONCENTRATION];
                    real fugacityA = fugacity(concentrationA, cellA.poreSize());
                    real fugacityB = fugacity(concentrationB, cellB.poreSize());
                    real deltaFugacity = fugacityA - fugacityB;
                    real flowDirection = fugacityB>fugacityA ? 1 : -1; // +1 if flow goes from cell2 to cell1, -1 otherwise

                    real DA = DSelf(cellA.poreSize());
                    real DB = DSelf(cellB.poreSize());

                    real DAB= 2.0*DA*DB/(DA+DB); //L-cell size
                    real JAB=DAB*deltaFugacity/m_dr; //flux btw 2 sites
                    real DC=dt*JAB/m_dr;

                    current(i,j,k)[CONCENTRATION] += DC*flowDirection;

                }
            }
        }
    }
}

real ForwardEuler::dr() const
{
    return m_dr;
}

void ForwardEuler::setDr(real dr)
{
    m_dr = dr;
}
