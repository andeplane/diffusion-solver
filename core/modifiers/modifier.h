#ifndef MODIFIER_H
#define MODIFIER_H
#include <functional>
#include "grid.h"
class Modifier
{
public:
    Modifier();
    virtual void apply(Grid& grid, int propertyIndex) = 0;
};

#endif // MODIFIER_H
