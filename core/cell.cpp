#include "cell.h"
using namespace std;

Cell::Cell(int numProperties) :
    m_type(0),
    m_contents(numProperties, 0)
{

}

vector<real> &Cell::contents()
{
    return m_contents;
}

void Cell::setType(int type)
{
    m_type = type;
}

void Cell::setPoreSize(int poreSize)
{
    m_poreSize = poreSize;
}
