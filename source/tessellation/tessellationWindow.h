#pragma once

#include "glfwWindowWrap.h"
#include "fpsCounter.h"
#include "camera.h"
#include "quad.h"
#include "shader.h"
#include "uniformBufferObject.h"
#include "keyboardManager.h"
#include "controlPoints.h"

#include <imgui.h>
#include <string>

#define CONTROL_POINTS_SETS 5

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

    // control points of Bezier
    controlPoints m_obj_controlPoints;
    shader m_sh_controlPoints;

    // Options
    bool m_bShowControlPoints;
    bool m_bUsePhong;

    // UBOs
    uniformBufferObject m_MatriciesUBO;
    uniformBufferObject m_ControlPointsUBO;
    
    keyboardMenager m_keyboardMenager;

    // Tessellation params
    glm::vec4 m_tessLevelOuter;
    glm::vec2 m_tessLevelInner;
    int m_bezierShape;

    // *=*=*=*=*=*=*=*=*=*=
    //         GUI
    // *=*=*=*=*=*=*=*=*=*=

    fpsCounter m_gui_fps;
    std::string m_gui_renderer;
    ImVec4 m_gui_hintColor;
    float m_gui_menuWidth;

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
    void PopulateBezierPointsUBO();

private:

    void RenderGUI();
    static void InfoWindowSizeCallback(ImGuiSizeCallbackData* data);

    void GenGUI_AppStatistics();
    void GenGUI_Tessellation();

private:

    void SetUpWindowsCallbacks();
    static void WindowSizeCallback(GLFWwindow* window, int width, int height);
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void MouseButton_RIGHT_Callback(GLFWwindow* window, int action, int mods);
    static void MouseButton_LEFT_Callback(GLFWwindow* window, int action, int mods);
    static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    static tessellationWindow* GW(GLFWwindow* window);

    void SetPolyMode(unsigned i);
    void SetBezierPointsShape(unsigned i);
    void SetShowBezierPoints(unsigned i);
    void SetPhongShading(unsigned i);


private: 

    void SetState(wState newState);
    wState GetState();

};