#include "forwardeuler.h"
#include <iostream>
#include "../grid.h"

using namespace std;
ForwardEuler::ForwardEuler(shared_ptr<class Grid> previous, shared_ptr<class Grid> current) : Integrator(),
    m_previous(previous), m_current(current)
{

}


double K(int poreSize) {
    const double Ka = 0.1586;
    const double Kb = -0.2389;
    const double Kc = -0.04202;
    return Ka*pow(poreSize,Kb)+Kc;
}

double DSelf(int poreSize) {
    double D1 = 0.000000000007016;
    double D2 = 0.000000001542;
    double D3 = -0.000000003727;
    double D4 = 0.0000000303;
    return (D1*(poreSize)*(poreSize)*(poreSize)+D2*(poreSize)*(poreSize)+D3*(poreSize)+D4); //[m2/s]
}

double fugacity(double concentration, int poreSize) {
    return concentration/K(poreSize);
}

double concentration(double fugacity, int poreSize) {
    return fugacity*K(poreSize);
}

void ForwardEuler::tick(double dt)
{
    if(!m_previous || !m_current) {
        cerr << "Error, ForwardEuler doesn't have initial grids set up." << endl;
        terminate();
    }

    int CONCENTRATION = 0;
    int PORESIZE = 0;

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
                    int poreSizeA = cellA[PORESIZE];
                    int poreSizeB = cellB[PORESIZE];

                    double concentrationA = cellA[CONCENTRATION];
                    double concentrationB = cellB[CONCENTRATION];
                    double fugacityA = fugacity(concentrationA, poreSizeA);
                    double fugacityB = fugacity(concentrationB, poreSizeB);

                    double flowDirection = fugacityB>fugacityA ? 1 : -1; // +1 if flow goes from cell2 to cell1, -1 otherwise

                    double DA = DSelf(poreSizeA);
                    double DB = DSelf(poreSizeB);

                    double DAB= 2*DA*DB/(DA+DB); //L-cell size
                    double JAB=DAB*df/L; //flux btw 2 sites
                    double DC=dt*JAB/L;

                    current(current.index(i,j,k)) += DC*flowDirection;

                }
            }
        }
    }
}
