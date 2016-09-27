#include "fixedboundaryvalue.h"
#include "grid.h"

FixedBoundaryValue::FixedBoundaryValue(int dimension, real value1, real value2) : Modifier(),
    m_dimension(dimension), m_value1(value1), m_value2(value2)
{

}

void FixedBoundaryValue::apply(std::shared_ptr<Grid> gridPtr)
{
    Grid &grid = *gridPtr;
    int N1 = m_dimension==0 ? grid.ny() : (m_dimension==1 ? grid.nx() : grid.nx()); // if boundary fix is in x, use YZ, if y, use XZ, if z, use XY
    int N2 = m_dimension==0 ? grid.nz() : (m_dimension==1 ? grid.nz() : grid.ny());
    for(int i=0; i<N1; i++) {
        for(int j=0; j<N2; j++) {
            // lower dimension "left"
            int ci = m_dimension==0 ? 0 : i;
            int cj = m_dimension==0 ? i : (m_dimension==1 ? 0 : j);
            int ck = m_dimension==0 ? j : (m_dimension==1 ? j : 0);
            grid(ci, cj, ck) = m_value1;

            // higher dimension "right"
            ci = m_dimension==0 ? (grid.nx()-1) : i;
            cj = m_dimension==0 ? i : (m_dimension==1 ? (grid.ny()-1) : j);
            ck = m_dimension==0 ? j : (m_dimension==1 ? j : (grid.nz()-1));

            grid(ci, cj, ck) = m_value2;
        }
    }
}
