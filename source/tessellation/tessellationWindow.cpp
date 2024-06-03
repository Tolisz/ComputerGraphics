#include "tessellationWindow.h"

tessellationWindow::tessellationWindow()
{

}

tessellationWindow::~tessellationWindow()
{

}

void tessellationWindow::RunInit()
{
    glClearColor(0.5f, 0.5f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
}

void tessellationWindow::RunRenderTick()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void tessellationWindow::RunClear()
{

}
