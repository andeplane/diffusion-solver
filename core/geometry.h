#ifndef GEOMETRY_H
#define GEOMETRY_H
#include "grid.h"
#include <functional>

namespace Geometry {
    std::shared_ptr<Grid> linearGridX(int nx, int ny, int nz, int poreSize, real value1, real value2);
    std::shared_ptr<Grid> initialWallX(int nx, int ny, int nz, int poreSize, real value1, real value2);
    std::shared_ptr<Grid> cubeGridX(int nx, int ny, int nz, int poreSize1, int poreSize2, real value1, real value2, bool linear);
    std::shared_ptr<Grid> planeGeometryGrid(int nx, int ny, int nz, std::string filename, real value1, real value2, bool linear);
}

#endif // GEOMETRY_H
