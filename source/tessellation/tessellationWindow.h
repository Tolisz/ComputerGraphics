#pragma once

#include "glfwWindowWrap.h"
#include "fpsCounter.h"

#include <imgui.h>
#include <string>

class tessellationWindow: public glfwWindowWrap
{
private: 

    // *=*=*=*=*=*=*=*=*=*=
    //         GUI
    // *=*=*=*=*=*=*=*=*=*=

    fpsCounter m_gui_fps;
    std::string m_gui_renderer;
    ImVec4 m_gui_hintColor;

public:

    // *=*=*=*=*=*=*=*=*=*=
    //   Object Creation
    // *=*=*=*=*=*=*=*=*=*=

    tessellationWindow();
    ~tessellationWindow();

private:

    // *=*=*=*=*=*=*=*=*=*=
    //  Wrapper Interface
    // *=*=*=*=*=*=*=*=*=*=
    
    virtual void RunInit() override;
    virtual void RunRenderTick() override;
    virtual void RunClear() override;

private:

    void RenderGUI();
    static void InfoWindowSizeCallback(ImGuiSizeCallbackData* data);

    void GenGUI_AppStatistics();
};