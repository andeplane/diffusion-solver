#include <iostream>
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
    if(numArgs > 1) {
        numThreads = atoi(arguments[1]);
    }

    createTables();
    int N = 32;
    int poreSize = 19;
    // auto gridPtr = Geometry::initialWallX(N, N, N, poreSize, 1.0, 0.0);
    // auto gridPtr = Geometry::linearGridX(N, N, N, poreSize, 1.0, 0.0);
    // auto gridPtr = Geometry::cubeGridX(N, N, N, 1, 19, 1.0, 0.0, false);
    string planeGeometryFile = "/Users/anderhaf/Dropbox/uio/phd/2016/zeolite/3dmodel/data_SPPA_20/SPPA_N=20_xMin=1.5_xMax=19.5_T=1.0_19/geometry.txt";
    // string planeGeometryFile = "/projects/geometry.txt";
    // auto gridPtr = Geometry::planeGeometryGrid(N, N, N, planeGeometryFile, 1, 0, true);
    // auto gridPtr = Geometry::linearGridX(N, N, N, poreSize, 1.0, 0.0);
    auto gridPtr = Geometry::initialWallX(N, N, N, poreSize, 1.0, 0.0);

    cout << "Initializing grid " << endl;
    Grid &grid = *gridPtr;

    real dx = 1;
    real dt = dx*dx / 2.0;
    real L = dx*N;

    System system;
    system.setGrid(gridPtr);
    system.setLength(L, L, L);

    real deltaC = (concentration(1.0, poreSize) - concentration(0.0, poreSize));
    real gradC = deltaC / (dx * (N - 1));
    real D = DSelf(poreSize);
    cout << "Theoretical flux = " << D * gradC << endl;

    auto boundaryCondition = make_shared<FixedBoundaryValue>( FixedBoundaryValue(1, 0) );
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
    int printCounter = 0;
    bool writeVTK = true;

    grid.writeGeometryVTK("/projects/poregenerator/vtk/geometry.vtk");
    int timesteps = 10000;
    for(int i=0; i<timesteps; i++) {
        if(i % printEvery == 0) {
            double percentage = double(i) / timesteps * 100;

            double endTime = omp_get_wtime();
            double elapsedSecs = endTime-startTime;
            double estimatedTotalTime = elapsedSecs / (i+1) * timesteps;
            double estimatedTotalTimeLeft = estimatedTotalTime - elapsedSecs;
            cout << "Step " << i << " / " << timesteps << " (" << percentage << "\%). Estimated time left: " << estimatedTotalTimeLeft << " seconds." << endl;

            if(writeVTK) {
                char filename[1000];
                sprintf(filename, "/projects/poregenerator/vtk/concentration%d.vtk", printCounter);
                grid.writeConcentrationVTK(string(filename));
                sprintf(filename, "/projects/poregenerator/vtk/fugacity%d.vtk", printCounter++);
                grid.writeFugacityVTK(string(filename));
            }

            integrator.computeFlux = true;
            integrator.tick(make_shared<System>(system), dt);
        } else {
            integrator.computeFlux = false;
            integrator.tick(make_shared<System>(system), dt);
        }
    }
    grid.writeConcentrationVTK(string("/projects/poregenerator/vtk/final.vtk"));

    double endTime = omp_get_wtime();
    double elapsedSecs = endTime-startTime;
    cout << "Simulation finished using " << elapsedSecs << " seconds." << endl;

    return 0;
}
