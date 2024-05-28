#include "duckWindow.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/trigonometric.hpp>
#include <stb_image.h>

#include <iostream>
#include <cmath>

duckWindow::duckWindow()
    : m_gen(m_rd()), 
      m_uniformZeroToOne(0.0f, 1.0f)
{}

duckWindow::~duckWindow()
{}

void duckWindow::RunInit()
{
    // GUI 
    // ======================
    m_gui_FrameNumToSumCounter = 0;
    m_gui_FrameNumRenderTimeCounter = 0.0f;
    m_gui_FrameNumToSum = 100;
    m_gui_AvarageFrameNumRenderTime = 0.0f;

    const GLubyte* renderer = glGetString(GL_RENDERER);
    m_gui_renderer = std::string((const char*)renderer);

    // GLFW 
    // ======================

    SetUpWindowsCallbacks();
    m_windowState = wState::DEFALUT;

    // Scene
    // ======================
    
    // camera
    m_camera.UpdateRotation(glm::radians(-45.0f), glm::radians(-30.0f));

    // Lights
    m_obj_lights.reserve(m_maxLightsNum);
    m_ambientColor = glm::vec3(1.0f);
    light l; 
    l.InitGL(); 

    l.m_position = glm::vec3(0.4f, 0.5f, 0.4f);
    l.m_diffuseColor = glm::vec3(0.0f, 0.0f, 1.0f);
    l.m_specularColor = glm::vec3(1.0f, 1.0f, 1.0f);
    m_obj_lights.push_back(l);

    l.m_position = glm::vec3(-0.4f, 0.5f, -0.4f);
    l.m_diffuseColor = glm::vec3(0.0f, 1.0f, 0.0f);
    l.m_specularColor = glm::vec3(1.0f, 1.0f, 1.0f);
    m_obj_lights.push_back(l);

    m_sh_light.Init();
    m_sh_light.AttachShader("shaders/lightBillboard.vert", GL_VERTEX_SHADER);
    m_sh_light.AttachShader("shaders/lightBillboard.frag", GL_FRAGMENT_SHADER);
    m_sh_light.Link();

    // Materials
    material m; 
    m.ka = glm::vec3(0.2f);
    m.kd = glm::vec3(1.0f);
    m.ks = glm::vec3(0.5f);
    m.shininess = 128.0f;
    m_materials.insert(std::make_pair("water", m));

    // Water
    m_obj_water.InitGL();
    m_waterColor = glm::vec3(0.0288, 0.960, 0.882);

    m_sh_water.Init();
    m_sh_water.AttachShader("shaders/water.vert", GL_VERTEX_SHADER);
    m_sh_water.AttachShader("shaders/water.frag", GL_FRAGMENT_SHADER);
    m_sh_water.Link();

    // skyBox
    m_obj_skyBox.InitGL();
    
    m_sh_skyBox.Init();
    m_sh_skyBox.AttachShader("shaders/skyBox.vert", GL_VERTEX_SHADER);
    m_sh_skyBox.AttachShader("shaders/skyBox.frag", GL_FRAGMENT_SHADER);
    m_sh_skyBox.Link();

    // std::vector<std::string> defaultSkyBox = {
    //     "resources/textures/CM_skybox/right.jpg",
    //     "resources/textures/CM_skybox/left.jpg",
    //     "resources/textures/CM_skybox/top.jpg",
    //     "resources/textures/CM_skybox/bottom.jpg",
    //     "resources/textures/CM_skybox/front.jpg",
    //     "resources/textures/CM_skybox/back.jpg"
    // }; 

    std::vector<std::string> cadcam = {
        "resources/textures/CM_310/px.png",
        "resources/textures/CM_310/nx.png",
        "resources/textures/CM_310/py.png",
        "resources/textures/CM_310/ny.png",
        "resources/textures/CM_310/pz.png",
        "resources/textures/CM_310/nz.png"
    }; 

    PrepareCubeMapTexture(cadcam);

    // duck 
    m_obj_duck.InitGLFromFile("resources/meshes/duck/duck.txt");
    
    m_sh_duck.Init();
    m_sh_duck.AttachShader("shaders/duck.vert", GL_VERTEX_SHADER);
    m_sh_duck.AttachShader("shaders/duck.frag", GL_FRAGMENT_SHADER);
    m_sh_duck.Link();

    // OpenGL initial configuration
    // ============================

    glClearColor(0.5f, 0.5f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
}

void duckWindow::PrepareCubeMapTexture(std::vector<std::string> files)
{
    glGenTextures(1, &m_gl_cubeMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_gl_cubeMap);
    
    int width, height, nrChannels;
    for (unsigned int i = 0; i < files.size() && i < 6; i++) {
        unsigned char *pixels = stbi_load(files[i].c_str(), &width, &height, &nrChannels, 0);
        if (pixels) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
        } 
        else {
            std::cout << "Cube map read error: \n\t[file]:" << files[i] << std::endl;
        }
        stbi_image_free(pixels);
    }
    
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);  
}

void duckWindow::RunRenderTick()
{
    // Process FrameCallback
    // =============================
    void EveryFrameCallback();

    // Render Scene
    // =============================

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float aspect = static_cast<float>(m_width)/m_height;

    glm::mat4 view = m_camera.GetViewMatrix();
    glm::mat4 projection = m_camera.GetProjectionMatrix(aspect);

    // m_sh_testCube.Use();
    // m_sh_testCube.setM4fv("model", GL_FALSE, glm::mat4(1.0f));
    // m_sh_testCube.setM4fv("view", GL_FALSE, view);
    // m_sh_testCube.setM4fv("projection", GL_FALSE, projection);
    // m_testCube.Draw();

    DrawWater(view, projection);
    DrawSkyBox(view, projection);
    DrawDuck(view, projection);
    DrawLights(view, projection);

    RenderGUI();
}

void duckWindow::DrawLights(
    const glm::mat4& view,
    const glm::mat4& projection
)
{
    m_sh_light.Use();
    m_sh_light.setM4fv("view", GL_FALSE, view);
    m_sh_light.setM4fv("projection", GL_FALSE, projection);
    m_sh_light.set2i("screenSize", m_width, m_height);

    for (int i = 0; i < m_obj_lights.size(); i++) { 
        m_sh_light.set3fv("colorDiffuse", m_obj_lights[i].m_diffuseColor);
        m_sh_light.set3fv("colorSpecular", m_obj_lights[i].m_specularColor);
        m_sh_light.set3fv("billboardPos", m_obj_lights[i].m_position);

        m_obj_lights[i].Draw();
    }
}

void duckWindow::DrawWater(
    const glm::mat4& view,
    const glm::mat4& projection
)
{   
    // Distribute and simulate water
    // =============================
    DisturbWater();
    m_obj_water.SimulateWater(m_deltaTime);

    // Draw water
    // =============================
    m_sh_water.Use();
    //m_sh_water.setM4fv("model", GL_FALSE, glm::mat4(1.0f));
    m_sh_water.setM4fv("view", GL_FALSE, view);
    m_sh_water.setM4fv("projection", GL_FALSE, projection);
    
    m_sh_water.set1i("numberOfLights", m_obj_lights.size());
    m_sh_water.set3fv("ambientColor", m_ambientColor);
    for (int i = 0; i < m_maxLightsNum && i < m_obj_lights.size(); i++) {
        std::string name = "light["; 
        name += std::to_string(i);
        name += "]";

        m_sh_water.set3fv((name + ".position").c_str(), m_obj_lights[i].m_position);
        m_sh_water.set3fv((name + ".diffuseColor").c_str(), m_obj_lights[i].m_diffuseColor);
        m_sh_water.set3fv((name + ".specularColor").c_str(), m_obj_lights[i].m_specularColor);
    }

    const material& mat = m_materials["water"]; 
    m_sh_water.set3fv("material.ka", mat.ka);
    m_sh_water.set3fv("material.kd", mat.kd);
    m_sh_water.set3fv("material.ks", mat.ks);
    m_sh_water.set1f("material.shininess", mat.shininess);

    m_sh_water.set3fv("cameraPos", m_camera.m_worldPos);
    m_sh_water.set3fv("objectColor", m_waterColor);
    m_sh_water.set1f("a", m_obj_water.GetA());

    glActiveTexture(GL_TEXTURE0); 
    glBindTexture(GL_TEXTURE_2D, m_obj_water.GetNormalTex());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_gl_cubeMap);

    m_obj_water.Draw();
}

void duckWindow::DrawSkyBox(
    const glm::mat4& view,
    const glm::mat4& projection
)
{
    m_sh_skyBox.Use();
    m_sh_skyBox.setM4fv("view", GL_FALSE, view);
    m_sh_skyBox.setM4fv("projection", GL_FALSE, projection);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_gl_cubeMap);

    glEnable(GL_CULL_FACE);
    m_obj_skyBox.Draw();
    glDisable(GL_CULL_FACE);
}

void duckWindow::DrawDuck(
    const glm::mat4& view,
    const glm::mat4& projection
)
{
    m_sh_duck.Use();
    m_sh_duck.setM4fv("model", GL_FALSE, glm::scale(glm::mat4x4(1.0f), glm::vec3(0.001f)));
    m_sh_duck.setM4fv("view", GL_FALSE, view);
    m_sh_duck.setM4fv("projection", GL_FALSE, projection);

    m_obj_duck.Draw();
}

void duckWindow::DisturbWater()
{
    float a = m_obj_water.GetA();

    if (!m_bDrop && m_uniformZeroToOne(m_gen) <= m_dropProbability) {
        m_bDrop = true;
        m_dropPos = glm::vec2(m_uniformZeroToOne(m_gen), m_uniformZeroToOne(m_gen));
        m_dropPos *= a;
        m_dropPos -= a/2;
        m_curretDropDepth = 0.0f;
    }    

    if (m_bDrop) {
        m_curretDropDepth -= (-m_maxDropDepth / m_dropDepthTime) * m_deltaTime;
        m_obj_water.DisturbWaterAt(m_dropPos, m_curretDropDepth);
        if (m_curretDropDepth <= m_maxDropDepth) {
            m_bDrop = false;
        }
    }
}


void duckWindow::RunClear()
{
    if (m_obj_lights.size() != 0) {
        m_obj_lights[0].DeInitGL();
    }

    m_obj_water.DeInitGL();

}

void duckWindow::RenderGUI()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    // *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse;

    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSizeConstraints(ImVec2(200.0f, -1), ImVec2(FLT_MAX, -1), &duckWindow::InfoWindowSizeCallback, (void*)this);
    ImGui::Begin("Project: Duck", (bool*)0, flags);
    GenGUI_AppStatistics();
    GenGUI_Light();
    GenGUI_Materials();
    //ImGui::ShowDemoWindow();
    ImGui::End();

    // *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void duckWindow::GenGUI_AppStatistics()
{
    if (m_gui_FrameNumToSumCounter >= m_gui_FrameNumToSum) {
        m_gui_FrameNumToSumCounter = 0;
        m_gui_AvarageFrameNumRenderTime = m_gui_FrameNumRenderTimeCounter / m_gui_FrameNumToSum;
        m_gui_FrameNumRenderTimeCounter = 0.0f;
        m_gui_FrameNumToSum = static_cast<int>(std::max(std::ceilf(0.5f / m_gui_AvarageFrameNumRenderTime), 1.0f));
    }
    else {
        m_gui_FrameNumToSumCounter++;
        m_gui_FrameNumRenderTimeCounter += m_deltaTime;
    }

    auto resetFPSCounter = [&](int swapValue) {
        glfwSwapInterval(swapValue);
        m_gui_FrameNumToSumCounter = 1;
        m_gui_FrameNumToSum = 1;
    };

    static bool fpsLimit = true;
    bool prevFpsLimit = fpsLimit;

    const char* swapIntervals[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10"};
    static int currentSwapInterval = 0;

    if(ImGui::CollapsingHeader("App Statistics"))
    {
        ImGui::TextColored(ImVec4(0.231f, 0.820f, 0.0984f, 1.0f), m_gui_renderer.c_str());

        if (ImGui::BeginTable("split", 2))
        {
            ImGui::TableNextColumn(); 
            ImGui::Text("FPS = %.2f", 1.0f / m_gui_AvarageFrameNumRenderTime);
            ImGui::TableNextColumn(); 
            ImGui::Checkbox("FPS limit", &fpsLimit);
            if (ImGui::BeginItemTooltip())
            {
                ImGui::TextColored(ImVec4(0.231f, 0.820f, 0.0984f, 1.0f), "With the limit maximum FSP number is bounded by a screen refresh rate.");
                ImGui::TextColored(ImVec4(0.231f, 0.820f, 0.0984f, 1.0f), "Without the limit maximum GPU and CPU generate as much FPS as it possible");
                ImGui::EndTooltip();
            }
            ImGui::TableNextColumn(); 
            ImGui::Text("MS = %.6f", m_gui_AvarageFrameNumRenderTime);

            if (prevFpsLimit != fpsLimit) {
                resetFPSCounter(fpsLimit ? currentSwapInterval + 1 : 0);
            }

            ImGui::TableNextColumn(); 
            ImGui::BeginDisabled(!fpsLimit);
            if (ImGui::BeginCombo("swap", swapIntervals[currentSwapInterval])) 
            {
                for (int i = 0; i < IM_ARRAYSIZE(swapIntervals); i++) {
                    const bool is_selected = (i == currentSwapInterval);
                    if (ImGui::Selectable(swapIntervals[i], is_selected)) 
                    {
                        currentSwapInterval = i;
                        resetFPSCounter(currentSwapInterval + 1);
                    }

                    if (is_selected) ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            } 
            ImGui::EndDisabled();
            
            ImGui::EndTable();
        }
    }
}

void duckWindow::GenGUI_Light()
{
    if(ImGui::CollapsingHeader("Lights")) 
    {
        ImGui::ColorEdit3("ambient color", (float*)&m_ambientColor, ImGuiColorEditFlags_NoInputs);
        for (int i = 0; i < m_obj_lights.size() && i < m_maxLightsNum; i++) {
            std::string separatorName = "light ";
            separatorName += std::to_string(i);
            
            ImGui::PushID(i);
            ImGui::SeparatorText(separatorName.c_str());
            if (ImGui::BeginTable("split", 2))
            {
                ImGui::TableNextColumn();
                ImGui::ColorEdit3("diffuse", (float*)&m_obj_lights[i].m_diffuseColor, ImGuiColorEditFlags_NoInputs);
                ImGui::TableNextColumn();
                ImGui::ColorEdit3("specular", (float*)&m_obj_lights[i].m_specularColor, ImGuiColorEditFlags_NoInputs);
                ImGui::EndTable();
            }
            ImGui::DragFloat3("position", (float*)&m_obj_lights[i].m_position, 0.1f);
            ImGui::PopID();
        }
    }
}

void duckWindow::GenGUI_Materials()
{
    if(ImGui::CollapsingHeader("Materials")) 
    {   
        int i = 0;
        for (auto& [key, value] : m_materials) {
            ImGui::SeparatorText(key.c_str());
            if (ImGui::BeginTable("split", 3)) 
            {
                ImGui::TableNextColumn();
                ImGui::ColorEdit3("ka", (float*)&value.ka, ImGuiColorEditFlags_NoInputs);
                ImGui::TableNextColumn();
                ImGui::ColorEdit3("kd", (float*)&value.kd, ImGuiColorEditFlags_NoInputs);
                ImGui::TableNextColumn();
                ImGui::ColorEdit3("ks", (float*)&value.ks, ImGuiColorEditFlags_NoInputs);
                ImGui::EndTable();
            }
            ImGui::DragFloat("shinness", &value.shininess, 1.0f);
            
            i++;
        }
    }
}

void duckWindow::InfoWindowSizeCallback(ImGuiSizeCallbackData* data)
{
    duckWindow* win = reinterpret_cast<duckWindow*>(data->UserData);
    data->DesiredSize.y = win->m_height;    
}

void duckWindow::SetUpWindowsCallbacks()
{
    glfwSetFramebufferSizeCallback(m_window, &duckWindow::FramebufferSizeCallback);
    glfwSetMouseButtonCallback(m_window, &duckWindow::MouseButtonCallback);
    glfwSetScrollCallback(m_window, &duckWindow::ScrollCallback);
    glfwSetCursorPosCallback(m_window, &duckWindow::CursorPosCallback);
}   

void duckWindow::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    duckWindow* win = GW(window);

    win->m_width = static_cast<float>(width);
    win->m_height = static_cast<float>(height);
    
    glViewport(0, 0, width, height);
}

void duckWindow::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    // (1) GUI callback handling
    // =========================
    ImGuiIO& io = ImGui::GetIO();
    io.AddMouseButtonEvent(button, action == GLFW_PRESS);
    if (io.WantCaptureMouse)
        return;

    // (2) App callback handling
    // =========================
    switch (button)
    {
    case GLFW_MOUSE_BUTTON_RIGHT:
        MouseButton_RIGHT_Callback(window, action, mods);    
        break;
    case GLFW_MOUSE_BUTTON_LEFT:
        MouseButton_LEFT_Callback(window, action, mods);
        break;

    default:
        break;
    }
}

void duckWindow::MouseButton_RIGHT_Callback(GLFWwindow* window, int action, int mods)
{
    duckWindow* win = GW(window);

    if (win->GetState() == wState::DEFALUT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        win->m_mouse_prevPos.x = xpos;
        win->m_mouse_prevPos.y = ypos;

        win->SetState(wState::CAMERA_MOVE);
    }
    else if (win->GetState() == wState::CAMERA_MOVE && action == GLFW_RELEASE) {
        win->SetState(wState::DEFALUT);
    }
}

void duckWindow::MouseButton_LEFT_Callback(GLFWwindow* window, int action, int mods)
{
    duckWindow* win = GW(window);

    if (win->GetState() == wState::DEFALUT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        win->m_mouse_prevPos.x = xpos;
        win->m_mouse_prevPos.y = ypos;

        win->SetState(wState::CAMERA_ROTATE);
    }
    else if (win->GetState() == wState::CAMERA_ROTATE && action == GLFW_RELEASE) {
        win->SetState(wState::DEFALUT);
    }
}


void duckWindow::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    // (1) GUI callback handling
    // =========================
    ImGuiIO& io = ImGui::GetIO();
    io.AddMouseWheelEvent(xoffset, yoffset);
    if (io.WantCaptureMouse)
        return;

    // (2) App callback handling
    // =========================

}

void duckWindow::CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
    // (1) GUI callback handling
    // =========================
    ImGuiIO& io = ImGui::GetIO();
    io.AddMousePosEvent(xpos, ypos);
    if (io.WantCaptureMouse)
        return;

    // (2) App callback handling
    // =========================
    duckWindow* win = GW(window);

    float deltaX = xpos - win->m_mouse_prevPos.x;
    float deltaY = ypos - win->m_mouse_prevPos.y;
    win->m_mouse_prevPos.x = xpos;
    win->m_mouse_prevPos.y = ypos;

    switch (win->GetState())
    {
    case wState::CAMERA_MOVE:
        {
            win->m_camera.UpdatePosition(deltaY * win->m_camera.m_cameraSpeed);
            glm::vec3 v = win->m_camera.m_worldPos;
        }
        break;
    
    case wState::CAMERA_ROTATE:
        win->m_camera.UpdateRotation(-deltaX * win->m_camera.m_cameraSpeed, -deltaY * win->m_camera.m_cameraSpeed);
        break;

    default:
        break;
    }
}

void duckWindow::EveryFrameCallback()
{

}

duckWindow* duckWindow::GW(GLFWwindow* window)
{
    return reinterpret_cast<duckWindow*>(glfwGetWindowUserPointer(window));
}

void duckWindow::SetState(wState newState)
{
    m_windowState = newState;   
}

duckWindow::wState duckWindow::GetState()
{
    return m_windowState;
}