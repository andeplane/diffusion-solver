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

void ForwardEuler::tick(System *system, real dt)
{
    if(computeFlux) {
        doTick<1>(system, dt);
    } else {
        doTick<0>(system, dt);
    }
}

void ForwardEuler::tick(std::shared_ptr<System> systemPtr, real dt) {
    if(computeFlux) {
        doTick<1>(systemPtr.get(), dt);
    } else {
        doTick<0>(systemPtr.get(), dt);
    }
}

template <int COMPUTEFLUX>
void ForwardEuler::doTick(System *systemPtr, real dt)
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

    real dr = current.nx() / system.lx(); // TODO: don't assume equal length in all dimensions
    real oneOverDr = 1.0 / dr;
#pragma omp parallel shared(next, current) num_threads(m_numThreads)
    {
        const int NX = current.nx();
        const int NY = current.ny();
        const int NZ = current.nz();
#pragma omp for
        for(int i=0; i<NX; i++) {
            for(int j=0; j<NY; j++) {
                for(int k=0; k<NZ; k++) {
                    // Loop through the 3 dimensions and the nearest neighbor in each
                    // 14 flops?
                    // N^3 * 6 * 14 = 10500000000 = 1e10 (N=500)

                    int deltaIs[] = {-1, 1, 0, 0, 0, 0};
                    int deltaJs[] = {0, 0, -1, 1, 0, 0};
                    int deltaKs[] = {0, 0, 0, 0, -1, 1};

                    for(int a = 0; a<6; a++) {
                        const int di = deltaIs[a];
                        const int dj = deltaJs[a];
                        const int dk = deltaKs[a];
                        const int indexA = current.index(i,j,k);
                        const int indexB = current.indexPeriodic(i+di,j+dj,k+dk);
                        const short poreSizeA = current.poreSize(indexA);
                        const short poreSizeB = current.poreSize(indexB);

                        const real concentrationA = current[indexA];
                        const real concentrationB = current[indexB];

                        const real fugacityA = fugacity(concentrationA, poreSizeA);
                        const real fugacityB = fugacity(concentrationB, poreSizeB);

                        const real deltaFugacity = fugacityB - fugacityA;

                        const real DA = D_tabulated[poreSizeA] * K_tabulated[poreSizeA]; // units [m^2/s * s^2/(m^2 kg)] = [s/kg]
                        const real DB = D_tabulated[poreSizeB] * K_tabulated[poreSizeB];

                        const real DAB= 2.0*DA*DB/(DA+DB);
                        const real JAB=DAB*deltaFugacity*oneOverDr; // units [s/kg * kg/(m s^2) * 1/m] = [1/(m^2 s)]
                        const real deltaConcentration=dt*JAB*oneOverDr; // units [s * 1/(m^2 s) * 1/m] = [1/m^3]
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
#pragma omp atomic
                                fluxX0 += JAB;
                            } if(i==current.nx()-2 && di==1) {
#pragma omp atomic
                                fluxX1 += JAB;
                            }
                        }
                    }
                }
            }
        }
    }

    if(COMPUTEFLUX) {
        fluxX0 /= (current.ny() * current.nz());
        fluxX1 /= (current.ny() * current.nz());
        system.setFluxX0(fluxX0);
        system.setFluxX1(fluxX1);
    }

    applyModifiers(next);
    swap(current.grid(), next.grid());
}
