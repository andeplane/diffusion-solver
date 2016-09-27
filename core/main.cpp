#include <iostream>
#include <functional>
#include "integrators/forwardeuler.h"
#include "geometry.h"
#include "common.h"
#include "system.h"

using namespace std;

int main(int argc, char *argv[])
{
    Grid grid = Geometry::linearGridX(32, 32, 32, 2, 1.0, 0.0);
    grid.writeVTK("/projects/testGrid.vtk", CONCENTRATION);

    System system;
    ForwardEuler integrator;
    integrator.initialize(1.0);
    system.setIntegrator(make_shared<ForwardEuler>(integrator));

    return 0;
}
