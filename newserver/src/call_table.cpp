#include "call_table.hpp"
#include <unistd.h>
#include <string.h>
CallTable::CallTable::CallTable(unsigned int size,CommandFunction _default)
{
    this->reserve(size);
    for(unsigned int i=0;i<size;++i)
    {
        operator[](i) = _default;
    }
    default_cell = _default;
}
CallTable::CallTable::~CallTable()
{
}
