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
    auto gridPtr = Geometry::initialWallX(32, 32, 32, 19, 1.0, 0.0);
    cout << "Initializing grid " << endl;
    // auto gridPtr = Geometry::initialWallX(500, 500, 500, 19, 1.0, 0.0);
    Grid &grid = *gridPtr;
    cout << "Doing stuff with grid " << endl;
    grid.iterate([&](real &, short &poreSize, int i, int , int ) {
        if(i > 3) {
            poreSize = 2;
        }
    });

    System system;
    system.setGrid(gridPtr);
    cout << "System exists" << endl;
    real dx = 1;
    real dt = dx*dx / 3.;


    auto boundaryCondition = make_shared<FixedBoundaryValue>( FixedBoundaryValue(1, 0) );

    system.setLength(1.0, 1.0, 1.0);
    ForwardEuler integrator;
    integrator.addModifier(boundaryCondition);
    clock_t begin = clock();
    int printEvery = 1;
    int printCounter = 0;
    cout << "Starting timesteps" << endl;
    for(int i=0; i<100; i++) {
        cout << i << endl;
        if(i % printEvery == 0) {
            char filename[1000];
            sprintf(filename, "/projects/poregenerator/vtk/data%d.vtk", printCounter++);
            grid.writeVTK(string(filename));
        }
        integrator.tick(make_shared<System>(system), dt);
    }

    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout << "Simulation finished using " << 1000*elapsed_secs << " ms." << endl;

    return 0;
}
