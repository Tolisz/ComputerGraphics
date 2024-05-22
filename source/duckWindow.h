#pragma once

#include "glfwWindowWrap.h"

class duckWindow: public glfwWindowWrap
{
private:

    

public:

    virtual void RunInit() override;
    virtual void RunRenderTick() override;
    virtual void RunClear() override;
};
