#include "common.h"
#include "library.h"
#include "system.h"
#include "grid.h"
#include "geometry.h"
#include "integrators/forwardeuler.h"
#include "modifiers/modifiers.h"

extern "C" {
    System* createSystem() {
        return new System();
    }

    void createPlaneGeometry(System *system, int N, string filename, real value1, real value2, bool linear) {
        std::shared_ptr<Grid> grid = Geometry::planeGeometryGrid(N, N, N, filename, value1, value2, linear);
        system->setGrid(grid);
    }

    void createSphereGeometry(System *system, int N, real dr, string filename, real value1, real value2, bool linear) {
        std::shared_ptr<Grid> grid = Geometry::fromSpheres(N, dr, filename, value1, value2, linear);
        system->setGrid(grid);
    }

    ForwardEuler* createForwardEuler() {
        return new ForwardEuler();
    }

    void setOMPThreads(Integrator *integrator, int numThreads) {
        integrator->setNumThreads(numThreads);
    }

    void setXBoundaryConditions(Integrator *integrator, System *system, real value1, real value2) {
        auto boundaryCondition = make_shared<FixedBoundaryValue>( FixedBoundaryValue(value1, value2) );
        system->grid()->iterate([&](real &, short &poreSize, int i, int , int ) {
            if(i == 0 || i == system->grid()->nx()-1) {
                poreSize = 19;
            }
        });

        integrator->addModifier(boundaryCondition);
    }

    void writeConcentrationVTK(System *system, string filename) {
        system->grid()->writeConcentrationVTK(filename);
    }

    void writeGeometryVTK(System *system, string filename) {
        system->grid()->writeGeometryVTK(filename);
    }

    void writeFugacityVTK(System *system, string filename) {
        system->grid()->writeFugacityVTK(filename);
    }

    void writePoresVTK(System *system, string filename) {
        system->grid()->writePoresVTK(filename);
    }

    void tick(Integrator *integrator, System *system, real dt) {
        integrator->tick(system, dt);
    }
}
