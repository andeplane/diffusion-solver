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

private:
    int m_type;
    std::vector<real> m_contents;
};

#endif // CELL_H
