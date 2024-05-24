#pragma once

#include "glfwWindowWrap.h"
#include "camera.h"
#include "cube.h"
#include "shader.h"
#include "texture.h"

struct ImGuiSizeCallbackData;

class duckWindow: public glfwWindowWrap
{
private:

    enum class wState
    {
        DEFALUT, 

        CAMERA_MOVE,
        CAMERA_ROTATE,
    };

    wState m_windowState;

    camera m_camera;
    glm::vec2 m_mouse_prevPos;

    cube m_testCube;
    shader m_sh_testCube;
    texture m_textTexture;

public:
    // *=*=*=*=*=*=*=*=*=*=
    //   Object Creation
    // *=*=*=*=*=*=*=*=*=*=

    duckWindow();
    ~duckWindow();

private:

    // *=*=*=*=*=*=*=*=*=*=
    //  Wrapper Interface
    // *=*=*=*=*=*=*=*=*=*=

    virtual void RunInit() override;
    virtual void RunRenderTick() override;
    virtual void RunClear() override;

private:



private: 

    void RenderGUI();
    static void InfoWindowSizeCallback(ImGuiSizeCallbackData* data);

private: 

    void SetUpWindowsCallbacks();
    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void MouseButton_RIGHT_Callback(GLFWwindow* window, int action, int mods);
    static void MouseButton_LEFT_Callback(GLFWwindow* window, int action, int mods);
    static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    static duckWindow* GW(GLFWwindow* window);

private: 

    void SetState(wState newState);
    wState GetState();
};
