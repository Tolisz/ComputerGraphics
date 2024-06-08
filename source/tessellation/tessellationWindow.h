#pragma once

#include "glfwWindowWrap.h"
#include "fpsCounter.h"
#include "camera.h"
#include "quad.h"
#include "shader.h"
#include "uniformBufferObject.h"
#include "keyboardManager.h"

#include <imgui.h>
#include <string>

class tessellationWindow: public glfwWindowWrap
{
private: 

    static std::string m_shaderBasePath;

    // *=*=*=*=*=*=*=*=*=*=
    //        Scene
    // *=*=*=*=*=*=*=*=*=*=
    
    enum class wState
    {
        DEFALUT, 

        CAMERA_MOVE,
        CAMERA_ROTATE,
    };
    wState m_windowState;
    camera m_camera;
    glm::vec2 m_mousePrevPos;

    // quads
    quad m_obj_quad;
    shader m_sh_quad;

    // UBOs
    uniformBufferObject m_MatriciesUBO;

    keyboardMenager m_keyboardMenager;

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

    static std::string shPath(std::string fileName);

private:

    void RenderGUI();
    static void InfoWindowSizeCallback(ImGuiSizeCallbackData* data);

    void GenGUI_AppStatistics();
    void GenGUI_Tessellation();

private:

    void SetUpWindowsCallbacks();
    // typedef void (* GLFWkeyfun)(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void MouseButton_RIGHT_Callback(GLFWwindow* window, int action, int mods);
    static void MouseButton_LEFT_Callback(GLFWwindow* window, int action, int mods);
    static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    static tessellationWindow* GW(GLFWwindow* window);

    void SetPolyMode(unsigned i);


private: 

    void SetState(wState newState);
    wState GetState();

};