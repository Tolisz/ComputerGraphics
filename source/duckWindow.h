#pragma once

#include "glfwWindowWrap.h"
#include "camera.h"
#include "shader.h"
#include "waterGrid.h"
#include "light.h"
#include "material.h"
#include "skyBox.h"

#include <vector>
#include <map>
#include <random>

struct ImGuiSizeCallbackData;

class duckWindow: public glfwWindowWrap
{
private:

    // Window
    enum class wState
    {
        DEFALUT, 

        CAMERA_MOVE,
        CAMERA_ROTATE,
    };
    wState m_windowState;

    camera m_camera;
    glm::vec2 m_mouse_prevPos;

    // Materials
    std::map<std::string, material> m_materials;

    // Lights
    const int m_maxLightsNum = 5;
    glm::vec3 m_ambientColor;
    std::vector<light> m_obj_lights;
    shader m_sh_light;

    // Water
    waterGrid m_obj_water;
    shader m_sh_water;
    glm::vec3 m_waterColor;

    float m_dropProbability = 0.03f;
    float m_maxDropDepth = -0.10f;
    float m_dropDepthTime = 0.05f; // in seconds

    bool m_bDrop = false;
    float m_curretDropDepth;
    glm::vec2 m_dropPos;   

    // Random numbers generators
    std::random_device m_rd;
    std::mt19937 m_gen;
    std::uniform_real_distribution<float> m_uniformZeroToOne;

    // skyBox
    skyBox m_obj_skyBox;
    shader m_sh_skyBox;
    GLuint m_gl_cubeMap;

    // *=*=*=*=*=*=*=*=*=*=
    //         GUI
    // *=*=*=*=*=*=*=*=*=*=

    int m_gui_FrameNumToSumCounter;
    int m_gui_FrameNumToSum;
    float m_gui_FrameNumRenderTimeCounter;
    float m_gui_AvarageFrameNumRenderTime;

    std::string m_gui_renderer;

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

    // 0 - GL_TEXTURE_CUBE_MAP_POSITIVE_X - right
    // 1 - GL_TEXTURE_CUBE_MAP_NEGATIVE_X - left 
    // 2 - GL_TEXTURE_CUBE_MAP_POSITIVE_Y - top
    // 3 - GL_TEXTURE_CUBE_MAP_NEGATIVE_Y - bottom
    // 4 - GL_TEXTURE_CUBE_MAP_POSITIVE_Z - front
    // 5 - GL_TEXTURE_CUBE_MAP_NEGATIVE_Z - back
    void PrepareCubeMapTexture(std::vector<std::string> files);

private:

    void DrawLights(
        const glm::mat4& view,
        const glm::mat4& projection
    );

    void DrawWater(
        const glm::mat4& view,
        const glm::mat4& projection
    );
    void DisturbWater();

    void DrawSkyBox(
        const glm::mat4& view,
        const glm::mat4& projection
    );

private: 

    void RenderGUI();
    void GenGUI_AppStatistics();
    void GenGUI_Light();
    void GenGUI_Materials();
    static void InfoWindowSizeCallback(ImGuiSizeCallbackData* data);

private: 

    void SetUpWindowsCallbacks();
    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void MouseButton_RIGHT_Callback(GLFWwindow* window, int action, int mods);
    static void MouseButton_LEFT_Callback(GLFWwindow* window, int action, int mods);
    static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    
    void EveryFrameCallback();
    static duckWindow* GW(GLFWwindow* window);

private: 

    void SetState(wState newState);
    wState GetState();
};
