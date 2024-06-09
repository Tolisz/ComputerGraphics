#include "tessellationWindow.h"

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glm/trigonometric.hpp>
#include <iostream>

std::string tessellationWindow::m_shaderBasePath = "shaders/tessellation/";

tessellationWindow::tessellationWindow()
{}

tessellationWindow::~tessellationWindow()
{}

void tessellationWindow::RunInit()
{
    // GLFW
    // *=*=*=*=*=*=*=*=*=*=
    SetUpWindowsCallbacks();

    // Scene
    // *=*=*=*=*=*=*=*=*=*=
    m_windowState = wState::DEFALUT;
    m_camera.UpdateRotation(0.0f, glm::radians(-45.0f));

    m_obj_quad.InitGL();
    
    m_sh_quad.Init();
    m_sh_quad.AttachShader(shPath("quad.vert"), GL_VERTEX_SHADER);
    m_sh_quad.AttachShader(shPath("quad.tesc"), GL_TESS_CONTROL_SHADER);
    m_sh_quad.AttachShader(shPath("quad.tese"), GL_TESS_EVALUATION_SHADER);
    m_sh_quad.AttachShader(shPath("quad.frag"), GL_FRAGMENT_SHADER);
    m_sh_quad.Link();

    // Keyboard functions
    // *=*=*=*=*=*=*=*=*=*=

    m_keyboardMenager.RegisterKey(GLFW_KEY_W, "grid view")
    .AddState("On", std::bind(&tessellationWindow::SetPolyMode, this, std::placeholders::_1))
    .AddState("Off", std::bind(&tessellationWindow::SetPolyMode, this, std::placeholders::_1));
    SetPolyMode(0);

    m_keyboardMenager.RegisterKey(GLFW_KEY_F, "Bezier control points' shape")
    .AddState("Flat", std::bind(&tessellationWindow::SetBezierPointsShape, this, std::placeholders::_1))
    .AddState("Wave", std::bind(&tessellationWindow::SetBezierPointsShape, this, std::placeholders::_1))
    .AddState("Convex", std::bind(&tessellationWindow::SetBezierPointsShape, this, std::placeholders::_1));
    SetBezierPointsShape(0);

    // GUI
    // *=*=*=*=*=*=*=*=*=*=
    const GLubyte* renderer = glGetString(GL_RENDERER);
    m_gui_renderer = std::string((const char*)renderer);
    
    m_gui_hintColor = ImVec4(0.231f, 0.820f, 0.0984f, 1.0f);

    // OpenGL
    // *=*=*=*=*=*=*=*=*=*=

    // UBOs
    m_MatriciesUBO.CreateUBO(2 * sizeof(glm::mat4));
    m_MatriciesUBO.BindBufferBaseToBindingPoint(0);
    m_ControlPointsUBO.CreateUBO(16 * 3 * sizeof(glm::vec4));
    m_ControlPointsUBO.BindBufferBaseToBindingPoint(1);

    m_sh_quad.BindUniformBlockToBindingPoint("Matrices", 0);
    m_sh_quad.BindUniformBlockToBindingPoint("ControlPoints", 1);

    PopulateBezierPointsUBO();

    // Tessellation params
    m_tessLevelOuter = glm::vec4(10.0f, 10.0f, 10.0f, 10.0f);
    m_tessLevelInner = glm::vec2(10.0f, 10.0f);

    glClearColor(0.5f, 0.5f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
}

void tessellationWindow::RunRenderTick()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Update UBO
    float aspect = static_cast<float>(m_width)/m_height;
    glm::mat4 matrices[2] = {m_camera.GetViewMatrix(), m_camera.GetProjectionMatrix(aspect)};
    m_MatriciesUBO.BindUBO();
    m_MatriciesUBO.SetBufferData(0, matrices, 2 * sizeof(glm::mat4));

    // Draw 
    m_sh_quad.Use();
    m_sh_quad.set4fv("outerLevel", m_tessLevelOuter);
    m_sh_quad.set2fv("innerLevel", m_tessLevelInner);
    m_sh_quad.set1i("bezierShape", m_bezierShape);
    glPatchParameteri(GL_PATCH_VERTICES, 4);
    m_obj_quad.Draw();

    RenderGUI();
}

void tessellationWindow::RunClear()
{
    m_obj_quad.DeInitGL();
}

std::string tessellationWindow::shPath(std::string fileName)
{
    return m_shaderBasePath + fileName;
}

void tessellationWindow::PopulateBezierPointsUBO()
{
    std::vector<glm::vec4> points;
    points.reserve(3 * 16);

    float d = 2.0f / 3;

    // flat
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            points.push_back({
                -1.0f + d * i, 
                0.0f, 
                -1.0f + d * j, 
                1.0f,
            });
        }
    }
    // up 
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            points.push_back({
                -1.0f + d * i, 
                (i == 0 || j == 0 || i == 3 || j == 3) ? 0.0f : 1.0f, 
                -1.0f + d * j, 
                1.0f
            });
        }
    }
    // down 
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            points.push_back({
                -1.0f + d * i, 
                (i == 0 || j == 0 || i == 3 || j == 3) ? 0.0f : -1.0f, 
                -1.0f + d * j, 
                1.0f
            });
        }
    }

    m_ControlPointsUBO.BindUBO();
    m_ControlPointsUBO.SetBufferData(0, points.data(), sizeof(glm::vec4) * points.size());
}

void tessellationWindow::RenderGUI()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    // *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=

    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSizeConstraints(ImVec2(200.0f, -1), ImVec2(FLT_MAX, -1), &tessellationWindow::InfoWindowSizeCallback, (void*)this);
    ImGui::Begin("Project: Tessellation", (bool*)0, ImGuiWindowFlags_NoCollapse);
    GenGUI_AppStatistics();
    GenGUI_Tessellation();
    // ImGui::ShowDemoWindow();
    ImGui::End();

    // *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); 

}

void tessellationWindow::InfoWindowSizeCallback(ImGuiSizeCallbackData* data)
{
    tessellationWindow* win = reinterpret_cast<tessellationWindow*>(data->UserData);
    data->DesiredSize.y = win->m_height;
    win->m_gui_menuWidth = data->CurrentSize.x;
}


void tessellationWindow::GenGUI_AppStatistics()
{
    m_gui_fps.Update(m_deltaTime);

    static bool fpsLimit = true;
    bool prevFpsLimit = fpsLimit;

    const char* swapIntervals[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10"};
    static int currentSwapInterval = 0;

    if(ImGui::CollapsingHeader("App Statistics"))
    {
        ImGui::TextColored(m_gui_hintColor, m_gui_renderer.c_str());

        if (ImGui::BeginTable("split", 2))
        {
            ImGui::TableNextColumn(); 
            ImGui::Text("FPS = %.2f", m_gui_fps.GetAvarageFPS());
            ImGui::TableNextColumn(); 
            ImGui::Checkbox("FPS limit", &fpsLimit);
            if (ImGui::BeginItemTooltip())
            {
                ImGui::TextColored(m_gui_hintColor, "With the limit maximum FSP number is bounded by a screen refresh rate.");
                ImGui::TextColored(m_gui_hintColor, "Without the limit maximum GPU and CPU generate as much FPS as it possible");
                ImGui::EndTooltip();
            }
            ImGui::TableNextColumn(); 
            ImGui::Text("MS = %.6f", m_gui_fps.GetRenderTime());

            if (prevFpsLimit != fpsLimit) {
                glfwSwapInterval(fpsLimit ? currentSwapInterval + 1 : 0);
                m_gui_fps.Reset();
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
                        glfwSwapInterval(currentSwapInterval + 1);
                        m_gui_fps.Reset();
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

void tessellationWindow::GenGUI_Tessellation()
{
    if(ImGui::CollapsingHeader("Tessellation"))
    {   
        ImGui::SeparatorText("Parameters");

        ImGui::DragFloat4("outer", reinterpret_cast<float*>(&m_tessLevelOuter), 0.1f, 1.0f, 100.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp);
        ImGui::DragFloat2("inner", reinterpret_cast<float*>(&m_tessLevelInner), 0.1f, 1.0f, 100.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp);

        ImGui::SeparatorText("Keyboard");
        if (ImGui::BeginTable("Tessellation settings", 3, 
                    ImGuiTableFlags_BordersH |
                    ImGuiTableFlags_BordersV)) 
        {
            ImGui::TableSetupColumn("Key", ImGuiTableColumnFlags_WidthFixed, 23.0f);
            ImGui::TableSetupColumn("State", ImGuiTableColumnFlags_WidthFixed, 37.0f);
            ImGui::TableSetupColumn("Description", ImGuiTableColumnFlags_WidthFixed, m_gui_menuWidth - 100.0f);
            ImGui::TableHeadersRow();
            
            for (const auto& key : m_keyboardMenager.GetRegisteredKeys()) {
                auto info = m_keyboardMenager.GetCurrentState(key);
                ImGui::TableNextColumn();
                ImGui::SameLine(15); ImGui::TextWrapped("%c", key);
                ImGui::TableNextColumn();
                ImGui::Text(info.name.c_str());
                ImGui::TableNextColumn();
                ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + m_gui_menuWidth - 100.0f);
                ImGui::Text(m_keyboardMenager.GetKeyHint(key).c_str());
                ImGui::PopTextWrapPos();
            }   

            ImGui::EndTable();
        }
    }
}

void tessellationWindow::SetUpWindowsCallbacks()
{
    glfwSetMouseButtonCallback(m_window, &tessellationWindow::MouseButtonCallback);
    glfwSetKeyCallback(m_window, &tessellationWindow::KeyCallback);
    glfwSetCursorPosCallback(m_window, &tessellationWindow::CursorPosCallback);
}

void tessellationWindow::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // (1) GUI callback handling
    // =========================
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
    if (io.WantCaptureKeyboard) {
        return;
    }

    // (2) App callback handling
    // =========================
    tessellationWindow* win = GW(window);

    if (action == GLFW_PRESS) {
        win->m_keyboardMenager.GoToNextState(key);
    }
}

void tessellationWindow::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
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

void tessellationWindow::MouseButton_RIGHT_Callback(GLFWwindow* window, int action, int mods)
{
    tessellationWindow* win = GW(window);

    if (win->GetState() == wState::DEFALUT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        win->m_mousePrevPos.x = xpos;
        win->m_mousePrevPos.y = ypos;

        win->SetState(wState::CAMERA_MOVE);
    }
    else if (win->GetState() == wState::CAMERA_MOVE && action == GLFW_RELEASE) {
        win->SetState(wState::DEFALUT);
    }
}

void tessellationWindow::MouseButton_LEFT_Callback(GLFWwindow* window, int action, int mods)
{
    tessellationWindow* win = GW(window);

    if (win->GetState() == wState::DEFALUT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        win->m_mousePrevPos.x = xpos;
        win->m_mousePrevPos.y = ypos;

        win->SetState(wState::CAMERA_ROTATE);
    }
    else if (win->GetState() == wState::CAMERA_ROTATE && action == GLFW_RELEASE) {
        win->SetState(wState::DEFALUT);
    }
}

void tessellationWindow::CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
    // (1) GUI callback handling
    // =========================
    ImGuiIO& io = ImGui::GetIO();
    io.AddMousePosEvent(xpos, ypos);
    if (io.WantCaptureMouse)
        return;

    // (2) App callback handling
    // =========================
    tessellationWindow* win = GW(window);

    float deltaX = xpos - win->m_mousePrevPos.x;
    float deltaY = ypos - win->m_mousePrevPos.y;
    win->m_mousePrevPos.x = xpos;
    win->m_mousePrevPos.y = ypos;

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

tessellationWindow* tessellationWindow::GW(GLFWwindow* window)
{
    return reinterpret_cast<tessellationWindow*>(glfwGetWindowUserPointer(window));
}

void tessellationWindow::SetPolyMode(unsigned i)
{
    glPolygonMode(GL_FRONT_AND_BACK, i == 0 ? GL_LINE : GL_FILL);
}

void tessellationWindow::SetBezierPointsShape(unsigned i)
{
    m_bezierShape = i;
}

void tessellationWindow::SetState(wState newState)
{
    m_windowState = newState;   
}

tessellationWindow::wState tessellationWindow::GetState()
{
    return m_windowState;
}
