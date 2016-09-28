#ifndef GEOMETRY_H
#define GEOMETRY_H
#include "grid.h"
#include <functional>
using namespace std;

namespace Geometry {
    Grid voidGrid(int nx, int ny, int nz, int numProperties);
    Grid cylinderGrid(int nx, int ny, int nz, int numProperties, float radius);
    Grid boxGrid(int nx, int ny, int nz, int numProperties);
    shared_ptr<Grid> linearGridX(int nx, int ny, int nz, int numProperties, int poreSize, real value1, real value2);
    shared_ptr<Grid> initialWallX(int nx, int ny, int nz, int numProperties, int poreSize, real value1, real value2);
}

#endif // GEOMETRY_H
