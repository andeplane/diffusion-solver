#include "fixedboundaryvalue.h"
#include "grid.h"
#include <iostream>
using namespace std;

FixedBoundaryValue::FixedBoundaryValue(real value1, real value2) : Modifier(),
    m_value1(value1), m_value2(value2)
{

}

void FixedBoundaryValue::apply(Grid &grid)
{
    createTables(); // TODO: get rid of this crazy static hell
    for(int i=0; i<grid.ny(); i++) {
        for(int j=0; j<grid.nz(); j++) {
            int cj = i;
            int ck = j;
            int index = grid.index(0, cj, ck);
            int poreSize = grid.poreSize(index);
            grid[index] = concentration(m_value1, poreSize);

            index = grid.index(grid.nx()-1, cj, ck);
            poreSize = grid.poreSize(index);
            grid[index] = concentration(m_value2, poreSize);
        }
    }
}
