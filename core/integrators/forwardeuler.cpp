#include "forwardeuler.h"
#include <iostream>
#include "../grid.h"

using namespace std;
ForwardEuler::ForwardEuler(shared_ptr<class Grid> previous, shared_ptr<class Grid> current) : Integrator(),
    m_previous(previous), m_current(current)
{

}


void ForwardEuler::tick(double dt)
{
    if(!m_previous || !m_current) {
        cerr << "Error, ForwardEuler doesn't have initial grids set up." << endl;
        terminate();
    }


}
