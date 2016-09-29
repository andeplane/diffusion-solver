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
    for(int i=0; i<grid.ny(); i++) {
        for(int j=0; j<grid.nz(); j++) {
            int cj = i;
            int ck = j;
            grid(0, cj, ck) = m_value1;
            grid(grid.nx()-1, cj, ck) = m_value2;
        }
    }
}
