#ifndef MODIFIER_H
#define MODIFIER_H
#include <functional>

class Modifier
{
public:
    Modifier();
    virtual void apply(std::shared_ptr<class Grid> grid) = 0;
};

#endif // MODIFIER_H
