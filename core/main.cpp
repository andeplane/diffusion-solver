#include <iostream>
#include <fstream>
#include <functional>
#include <ctime>
#include <omp.h>
#include "integrators/forwardeuler.h"
#include "modifiers/fixedboundaryvalue.h"
#include "geometry.h"
#include "common.h"
#include "system.h"

using namespace std;

int main(int numArgs, char **arguments)
{
    int numThreads = 1;
    if(numArgs < 2) {
        cout << "Error, you need to provide geometry file" << endl;
        exit(1);
    }

    string fileName = arguments[1];
    int N = 64;
    if(numArgs > 2) {
        N = atoi(arguments[2]);
    }
    if(numArgs > 3) {
        numThreads = atoi(arguments[3]);
    }

    createTables();
    int poreSize = 19;
    // auto gridPtr = Geometry::initialWallX(N, N, N, poreSize, 1.0, 0.0);
    // auto gridPtr = Geometry::linearGridX(N, N, N, poreSize, 1.0, 0.0);
    // auto gridPtr = Geometry::cubeGridX(N, N, N, 1, 19, 1.0, 0.0, false);

    // string planeGeometryFile = "/Users/anderhaf/Dropbox/uio/phd/2016/zeolite/3dmodel/data_SPPA_20/SPPA_N=20_xMin=1.5_xMax=19.5_T=1.0_19/geometry.txt";
    // string planeGeometryFile = "/projects/geometry.txt";
    // auto gridPtr = Geometry::planeGeometryGrid(N, N, N, planeGeometryFile, 1, 0.9, true);
    auto gridPtr = Geometry::fromSpheres(N, 0.1, fileName, 0.0, 1.0, true);
    // auto gridPtr = Geometry::linearGridX(N, N, N, poreSize, 1.0, 0.0);
    // auto gridPtr = Geometry::initialWallX(N, N, N, poreSize, 1.0, 0.0);

    cout << "Initializing grid " << endl;
    Grid &grid = *gridPtr;

    real dx = 1;
    real dt = dx*dx / 3.0;
    real L = dx*N;

    auto systemPtr = make_shared<System>();
    System &system = *systemPtr;
    system.setGrid(gridPtr);
    system.setLength(L, L, L);

    real deltaC = (concentration(1.0, poreSize) - concentration(0.0, poreSize));
    real gradC = deltaC / (dx * (N - 1));
    real D = DSelf(poreSize);
    cout << "Theoretical flux = " << D * gradC << endl;

    auto boundaryCondition = make_shared<FixedBoundaryValue>( FixedBoundaryValue(1, 0.9) );
    grid.iterate([&](real &, short &poreSize, int i, int , int ) {
        if(i == 0 || i == grid.nx()-1) {
            poreSize = 19;
        }
    });

    ForwardEuler integrator;
    integrator.setNumThreads(numThreads);
    integrator.addModifier(boundaryCondition);
    integrator.applyModifiers(grid);
    double startTime = omp_get_wtime();
    int printEvery = 100;
    int saveEvery = 100000;
    int printCounter = 0;
    bool writeVTK = true;
    ofstream log("log.txt");
    grid.writeGeometryVTK("geometry.vtk");
    grid.writePoresVTK("pores.vtk");
    grid.writeConcentrationVTK("concentration.vtk");
    return 0; // TODO: REMOVE
    int timesteps = 1000000;
    for(int i=0; i<timesteps; i++) {
        if(i % printEvery == 0) {
            integrator.computeFlux = true;
            integrator.tick(systemPtr, dt);

            double percentage = double(i) / timesteps * 100; // How far are we in the simulation?
            double endTime = omp_get_wtime(); // Current time
            double elapsedSecs = endTime-startTime; // Total elapsed time
            double estimatedTotalTime = elapsedSecs / (i+1) * timesteps;
            double estimatedTotalTimeLeft = estimatedTotalTime - elapsedSecs;
            double ratio = (-system.fluxX0()/system.fluxX1());

            cout << "Step " << i << " / " << timesteps << " (" << percentage << "\%). Fluxes: " << system.fluxX0() << " and " << system.fluxX1() << " (ratio " << ratio << "). Estimated time left: " << estimatedTotalTimeLeft << " seconds." << endl;
            log << "Step " << i << " / " << timesteps << " (" << percentage << "\%). Fluxes: " << system.fluxX0() << " and " << system.fluxX1() << " (ratio " << ratio << "). Estimated time left: " << estimatedTotalTimeLeft << " seconds." << endl;

            if(i > timesteps*0.1 && fabs(ratio - 1.0) < 1e-5) { // Initially we might have the correct flux, so don't let the program stop
                cout << "Simulation has reached equilibrium with ratio = " << ratio << ". Stopping simulation." << endl;
                log << "Simulation has reached equilibrium with ratio = " << ratio << ". Stopping simulation." << endl;
                break;
            }
        } else {
            integrator.computeFlux = false;
            integrator.tick(make_shared<System>(system), dt);
        }

        if(writeVTK && (i % saveEvery==0) ) {
            char filename[1000];
            sprintf(filename, "concentration%d.vtk", printCounter);
            grid.writeConcentrationVTK(string(filename));
        }
    }
    grid.writeConcentrationVTK(string("concentration_final.vtk"));

    double endTime = omp_get_wtime();
    double elapsedSecs = endTime-startTime;
    cout << "Simulation finished using " << elapsedSecs << " seconds." << endl;

    return 0;
}
