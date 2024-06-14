#include "pumaWindow.h"

void pumaWindow::RunInit()
{
    glClearColor(0.5f, 0.5f, 1.0f, 1.0f);
}

void pumaWindow::RunRenderTick()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void pumaWindow::RunClear()
{
    
}
