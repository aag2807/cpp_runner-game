#include "dimension.h"

Dimensions CreateNewDimension(int x, int y)
{
    Dimensions value;
    value.width = x;
    value.height = y;

    return value;
}
