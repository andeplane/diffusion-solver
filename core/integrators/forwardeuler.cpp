#include "forwardeuler.h"
#include "../grid.h"
#include "../system.h"
#include "../modifiers/modifier.h"
#include <iostream>
#include <cmath>
using namespace std;
ForwardEuler::ForwardEuler() : Integrator(),
    computeFlux(false)
{

}

void ForwardEuler::tick(std::shared_ptr<System> systemPtr, real dt) {
    if(computeFlux) {
        doTick<1>(systemPtr, dt);
    } else {
        doTick<0>(systemPtr, dt);
    }
}

template <int COMPUTEFLUX>
void ForwardEuler::doTick(std::shared_ptr<System> systemPtr, real dt)
{
    if(!filled) {
        createTables();
    }
    System &system = *systemPtr;
    Grid &current = *system.grid();
    if(!m_grid) {
        Grid grid = current;
        m_grid = make_shared<Grid>(grid);
    }

    Grid &next = *m_grid;
    next = current;

    real fluxX0 = 0;
    real fluxX1 = 0;
//    real fluxY0 = 0;
//    real fluxY1 = 0;
//    real fluxZ0 = 0;
//    real fluxZ1 = 0;

    real dr = system.lx(); // TODO: don't assume equal length in all dimensions
    real oneOverDr = 1.0 / dr;
    for(int i=0; i<current.nx(); i++) {
        for(int j=0; j<current.ny(); j++) {
            for(int k=0; k<current.nz(); k++) {
                // Loop through the 3 dimensions and the nearest neighbor in each
                // 14 flops?
                // N^3 * 6 * 14 = 10500000000 = 1e10 (N=500)
                for(int lowerDimension=-1; lowerDimension<=1; lowerDimension+=2) {
                    for(int a=0; a<3; a++) {
                        const int di = a==0 ? lowerDimension : 0;
                        const int dj = a==1 ? lowerDimension : 0;
                        const int dk = a==2 ? lowerDimension : 0;
                        const int indexA = current.index(i,j,k);
                        const int indexB = current.indexPeriodic(i+di,j+dj,k+dk);
                        const short poreSizeA = current.poreSize(indexA);
                        const short poreSizeB = current.poreSize(indexB);

                        const real concentrationA = current[indexA];
                        const real concentrationB = current[indexB];

                        const real fugacityA = fugacity(concentrationA, poreSizeA);
                        const real fugacityB = fugacity(concentrationB, poreSizeB);

                        const real deltaFugacity = fugacityB - fugacityA;

                        const real DA = D_tabulated[poreSizeA] * K_tabulated[poreSizeA];
                        const real DB = D_tabulated[poreSizeB] * K_tabulated[poreSizeB];

                        const real DAB= 2.0*DA*DB/(DA+DB); //L-cell size // m^2 / s
                        const real JAB=DAB*deltaFugacity*oneOverDr; // m^2/s*bar/r
                        const real deltaConcentration=dt*JAB*oneOverDr;
#ifdef DEBUG
                        if(next(i,j,k)+deltaConcentration<0) {
                            cout << "Working with (" << i << "," << j << "," << k << ") and (" << i+di << "," << j+dj << "," << k+dk << ")" << endl;
                            cout << "Concentrations: " << concentrationA << " and " << concentrationB << endl;
                            cout << "Fugacities: " << fugacityA << " and " << fugacityB << endl;
                            cout << "Self diffusion: " << DA << " and " << DB << endl;
                            cout << "Effective DAB: " << DAB << endl;
                            cout << "Flux between sites: " << JAB << endl;
                            cout << "DeltaC = " << deltaConcentration << " and CONC_A = " << concentrationA << " and CONC_B = " << concentrationB << endl;
                            cout << "next(i,j,k)[CONCENTRATION] = " << next(i,j,k) << endl << endl;
                            cout << "Error, DC was larger than concentration in one of the cells. You need to reduce timestep." << endl;
                            terminate();
                        }
#endif
                        next(i,j,k) += deltaConcentration; // the other cell will get its contribution at a later stage.
                        // TODO: use "newton's third law" here
                        if(COMPUTEFLUX) {
                            if(i==1 && di==-1) {
                                fluxX0 += deltaConcentration;
                            } if(i==current.nx()-2 && di==1) {
                                fluxX1 += deltaConcentration;
                            }
                        }
                    }
                }
            }
        }
    }

    if(COMPUTEFLUX) {
        cout << "Fluxes: " << fluxX0 << " and " << fluxX1 << endl;
    }

    for(auto modifierPtr : m_modifiers) {
        Modifier &modifier = *modifierPtr;
        modifier.apply(next);
    }
    swap(current.grid(), next.grid());
}
