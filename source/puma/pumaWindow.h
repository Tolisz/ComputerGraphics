#pragma once

#include "glfwWindowWrap.h"

class pumaWindow : public glfwWindowWrap
{

private:

    // *=*=*=*=*=*=*=*=*=*=
    //  Wrapper Interface
    // *=*=*=*=*=*=*=*=*=*=
    
    virtual void RunInit() override;
    virtual void RunRenderTick() override;
    virtual void RunClear() override;

private:

    
};