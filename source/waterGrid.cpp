#include "waterGrid.h"

waterGrid::waterGrid() 
    : waterGrid(256, 2.0f, 1.0f)
{ }

waterGrid::waterGrid(int N, float a, float c)
    : N{N}, a{a}, c{c}
{
    h = a / (N - 1);
    dt = 1.0f / N;
}

waterGrid::~waterGrid()
{

}

void waterGrid::InitGL()
{
    
}