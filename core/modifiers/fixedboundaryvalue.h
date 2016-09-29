#ifndef FIXEDBOUNDARYVALUE_H
#define FIXEDBOUNDARYVALUE_H
#include "modifier.h"
#include "common.h"

class FixedBoundaryValue : public Modifier
{
public:
    FixedBoundaryValue(real value1, real value2);

    // Modifier interface
    virtual void apply(Grid& gridPtr) override;
private:
    real m_value1;
    real m_value2;
};

#endif // FIXEDBOUNDARYVALUE_H
