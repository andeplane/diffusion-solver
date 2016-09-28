#include <iostream>
#include <functional>
#include "integrators/forwardeuler.h"
#include "modifiers/fixedboundaryvalue.h"
#include "geometry.h"
#include "common.h"
#include "system.h"

using namespace std;

int main(int argc, char *argv[])
{
    auto gridPtr = Geometry::initialWallX(32, 32, 32, 2, 10, 1.0, 0.0);
    Grid &grid = *gridPtr;

    System system;
    system.setGrid(gridPtr);
    real dx = 1;
    real dt = dx*dx / 10.;


    auto boundaryCondition = make_shared<FixedBoundaryValue>( FixedBoundaryValue(0, 1, 0) );

    system.setLength(1.0, 1.0, 1.0);
    ForwardEuler integrator;
    integrator.addModifier(boundaryCondition);
    for(int i=0; i<100; i++) {
        cout << i << endl;
        char filename[1000];
        sprintf(filename, "/projects/poregenerator/vtk/data%d.vtk", i);
        grid.writeVTK(string(filename), CONCENTRATION);
        integrator.tick(make_shared<System>(system), dt);
    }

    return 0;
}
