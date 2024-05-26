#include "duckWindow.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>
#include <cmath>

duckWindow::duckWindow()
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

    // GLFW 
    // ======================

    SetUpWindowsCallbacks();
    m_windowState = wState::DEFALUT;

    // Scene
    // ======================
    
    // Lights
    m_obj_lights.reserve(m_maxLightsNum);
    m_ambientColor = glm::vec3(1.0f);
    light l; 
    l.InitGL(); 

    l.m_position = glm::vec3(0.9f, 0.0f, 0.9f);
    l.m_diffuseColor = glm::vec3(0.0f, 0.0f, 1.0f);
    l.m_specularColor = glm::vec3(1.0f, 1.0f, 1.0f);
    m_obj_lights.push_back(l);

    l.m_position = glm::vec3(-0.9f, 0.0f, -0.9f);
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

    // water

    // m_testCube.Init();

    // m_sh_testCube.Init();
    // m_sh_testCube.AttachShader("shaders/test.vert", GL_VERTEX_SHADER);
    // m_sh_testCube.AttachShader("shaders/test.frag", GL_FRAGMENT_SHADER);
    // m_sh_testCube.Link();

    // Water
    m_obj_water.InitGL();
    m_waterColor = glm::vec3(0.5f, 1.0f, 0.8f);

    m_sh_water.Init();
    m_sh_water.AttachShader("shaders/water.vert", GL_VERTEX_SHADER);
    m_sh_water.AttachShader("shaders/water.frag", GL_FRAGMENT_SHADER);
    m_sh_water.Link();

    //m_textTexture = texture::CreateTexture2D(GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);

    // OpenGL initial configuration
    // ============================

    glClearColor(0.5f, 0.5f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
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
    m_obj_water.SimulateWater();

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

    m_obj_water.Draw();
}



void duckWindow::RunClear()
{
    if (m_obj_lights.size() != 0) {
        m_obj_lights[0].DeInitGL();
    }


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
    ImGui::ShowDemoWindow();
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
        m_gui_FrameNumToSum = static_cast<int>(std::ceilf(0.5f / m_gui_AvarageFrameNumRenderTime));
    }
    else {
        m_gui_FrameNumToSumCounter++;
        m_gui_FrameNumRenderTimeCounter += m_deltaTime;
    }

    static bool fpsLimit = true;
    bool prevFpsLimit = fpsLimit;

    if(ImGui::CollapsingHeader("App Statistics"))
    {
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

            ImGui::EndTable();

            if (prevFpsLimit != fpsLimit) {
                if (fpsLimit) {
                    glfwSwapInterval(1);
                    m_gui_FrameNumToSumCounter = 1;
                    m_gui_FrameNumToSum = 1;
                } 
                else {
                    glfwSwapInterval(0);
                } 
            }
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
        win->m_camera.UpdateRotation(-deltaX * win->m_camera.m_cameraSpeed, deltaY * win->m_camera.m_cameraSpeed);
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