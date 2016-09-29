#include <iostream>
#include <functional>
#include <ctime>

#include "integrators/forwardeuler.h"
#include "modifiers/fixedboundaryvalue.h"
#include "geometry.h"
#include "common.h"
#include "system.h"

using namespace std;

int main(int , char **)
{
    createTables();
    int N = 32;
    int poreSize = 19;
    // auto gridPtr = Geometry::initialWallX(N, N, N, poreSize, 1.0, 0.0);
    // auto gridPtr = Geometry::linearGridX(N, N, N, poreSize, 1.0, 0.0);
    auto gridPtr = Geometry::cubeGridX(N, N, N, 2, 19, 1.0, 0.0);
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
    integrator.addModifier(boundaryCondition);
    integrator.applyModifiers(grid);
    clock_t begin = clock();
    int printEvery = 10;
    int printCounter = 0;

    int timesteps = 2000;
    for(int i=0; i<timesteps; i++) {
        if(i % printEvery == 0) {
            double percentage = double(i) / timesteps * 100;

            clock_t end = clock();
            double elapsedSecs = double(end - begin) / CLOCKS_PER_SEC;
            double estimatedTotalTime = elapsedSecs / (i+1) * timesteps;
            double estimatedTotalTimeLeft = estimatedTotalTime - elapsedSecs;

            cout << "Step " << i << " / " << timesteps << " (" << percentage << "\%). Estimated time left: " << estimatedTotalTimeLeft << " seconds." << endl;
            char filename[1000];
            sprintf(filename, "/projects/poregenerator/vtk/data%d.vtk", printCounter++);
            grid.writeVTK(string(filename));
            integrator.computeFlux = true;
            integrator.tick(make_shared<System>(system), dt);
        } else {
            integrator.computeFlux = false;
            integrator.tick(make_shared<System>(system), dt);
        }
    }

    grid.writeGeometryVTK("/projects/poregenerator/vtk/geometry.vtk");
    clock_t end = clock();
    double elapsedSecs = double(end - begin) / CLOCKS_PER_SEC;
    cout << "Simulation finished using " << 1000*elapsedSecs << " ms." << endl;

    return 0;
}
