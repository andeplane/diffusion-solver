#ifndef CELL_H
#define CELL_H
#include <vector>
#include "common.h"

class Cell
{
public:
    Cell(int numProperties);
    std::vector<real> &contents();
    real operator()(int index) { return m_contents[index]; }
    real &operator[](int index) { return m_contents[index]; }
    int type() const { return m_type; }
    void setType(int type);
    int poreSize() const { return m_poreSize; }
    void setPoreSize(int poreSize);

private:
    int m_type;
    int m_poreSize;
    std::vector<real> m_contents;
};

#endif // CELL_H
