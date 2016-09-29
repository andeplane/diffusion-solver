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
    Grid &grid = *gridPtr;


    System system;
    system.setGrid(gridPtr);
    cout << "System exists" << endl;
    real dx = 1;
    real dt = dx*dx / 10.;


    auto boundaryCondition = make_shared<FixedBoundaryValue>( FixedBoundaryValue(1, 0) );
    grid.iterate([&](real &, short &poreSize, int i, int , int ) {
        if(i == 0 || i == grid.nx()-1) {
            poreSize = 50;
        }
    });

    system.setLength(1.0, 1.0, 1.0);
    ForwardEuler integrator;
    integrator.addModifier(boundaryCondition);
    clock_t begin = clock();
    int printEvery = 100;
    int printCounter = 0;

    int N = 10000;
    for(int i=0; i<N; i++) {
        if(i % printEvery == 0) {
            double percentage = double(i) / N * 100;
            cout << "Step " << i << " / " << N << " (" << percentage << "\%)" << endl;
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

    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout << "Simulation finished using " << 1000*elapsed_secs << " ms." << endl;

    return 0;
}
