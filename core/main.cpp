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
    int N = 128;
    int poreSize = 19;
    // auto gridPtr = Geometry::initialWallX(N, N, N, poreSize, 1.0, 0.0);
    // auto gridPtr = Geometry::linearGridX(N, N, N, poreSize, 1.0, 0.0);
    auto gridPtr = Geometry::cubeGridX(N, N, N, 1, 19, 1.0, 0.0);
    cout << "Initializing grid " << endl;
    Grid &grid = *gridPtr;


    System system;
    system.setGrid(gridPtr);
    real dx = 1;
    real dt = dx*dx / 4;
    real L = dx*(N-1);

    cout << "c(1) = " << concentration(1.0, poreSize) << endl;
    cout << "c(0) = " << concentration(0.0, poreSize) << endl;
    cout << "D = " << DSelf(poreSize)*K(poreSize) << endl;
    real deltaC = (concentration(1.0, poreSize) - concentration(0.0, poreSize));
    real gradC = deltaC / L;
    real D = DSelf(poreSize);
    cout << "Theoretical flux = " << D * gradC << endl;

    auto boundaryCondition = make_shared<FixedBoundaryValue>( FixedBoundaryValue(1, 0) );
    grid.iterate([&](real &, short &poreSize, int i, int , int ) {
        if(i == 0 || i == grid.nx()-1) {
            poreSize = 19;
        }
    });
    system.setLength(1.0, 1.0, 1.0);
    ForwardEuler integrator;
    integrator.setNumThreads(numThreads);
    integrator.addModifier(boundaryCondition);
    integrator.applyModifiers(grid);
    double startTime = omp_get_wtime();
    int printEvery = 10;
    int printCounter = 0;

    int timesteps = 100;
    for(int i=0; i<timesteps; i++) {
        if(i % printEvery == 0) {
            double percentage = double(i) / timesteps * 100;

            double endTime = omp_get_wtime();
            double elapsedSecs = endTime-startTime;
            double estimatedTotalTime = elapsedSecs / (i+1) * timesteps;
            double estimatedTotalTimeLeft = estimatedTotalTime - elapsedSecs;
            cout << "Step " << i << " / " << timesteps << " (" << percentage << "\%). Estimated time left: " << estimatedTotalTimeLeft << " seconds." << endl;
            char filename[1000];
            sprintf(filename, "/projects/poregenerator/vtk/data%d.vtk", printCounter++);
            // grid.writeVTK(string(filename));
            // integrator.computeFlux = true;
            integrator.tick(make_shared<System>(system), dt);
        } else {
            integrator.computeFlux = false;
            integrator.tick(make_shared<System>(system), dt);
        }
    }

    grid.writeGeometryVTK("/projects/poregenerator/vtk/geometry.vtk");
    double endTime = omp_get_wtime();
    double elapsedSecs = endTime-startTime;
    cout << "Simulation finished using " << elapsedSecs << " seconds." << endl;

    return 0;
}
