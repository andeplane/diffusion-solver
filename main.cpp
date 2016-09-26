#include <iostream>
#include "geometry.h"

using namespace std;

enum PROPERTIES {
    VOID = 0,
    WALL = 1
} PROPERTIES;

int main(int argc, char *argv[])
{
    Grid grid = Geometry::voidGrid(128, 128, 128, 2);
    cout << "size: " << grid.cells().size() << endl;
    return 0;
}
