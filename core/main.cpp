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
    int N = 8;
    int poreSize = 19;
    auto gridPtr = Geometry::initialWallX(N, N, N, poreSize, 1.0, 0.0);
    cout << "Initializing grid " << endl;
    Grid &grid = *gridPtr;


    System system;
    system.setGrid(gridPtr);
    real dx = 1;
    real dt = dx*dx / 10;
    real L = dx*N;

    cout << "c(1) = " << concentration(1.0, poreSize) << endl;
    cout << "c(0) = " << concentration(0.0, poreSize) << endl;
    cout << "D = " << DSelf(poreSize)*K(poreSize) << endl;
    real deltaC = (concentration(1.0, poreSize) - concentration(0.0, poreSize));
    real gradC = deltaC / (L-1);
    real D = DSelf(poreSize);
    cout << "Theoretical flux = " << D * gradC << endl;

    auto boundaryCondition = make_shared<FixedBoundaryValue>( FixedBoundaryValue(1, 0) );
//    grid.iterate([&](real &, short &poreSize, int i, int , int ) {
//        if(i == 0 || i == grid.nx()-1) {
//            poreSize = 19;
//        }
//    });
    system.setLength(1.0, 1.0, 1.0);
    ForwardEuler integrator;
    integrator.addModifier(boundaryCondition);
    clock_t begin = clock();
    int printEvery = 100;
    int printCounter = 0;

    int timsteps = 10000;
    for(int i=0; i<timsteps; i++) {
        if(i % printEvery == 0) {
            double percentage = double(i) / timsteps * 100;
            cout << "Step " << i << " / " << timsteps << " (" << percentage << "\%)" << endl;
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
