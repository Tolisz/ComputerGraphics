#include "duckWindow.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

duckWindow::duckWindow()
{}

duckWindow::~duckWindow()
{}

void duckWindow::RunInit()
{
    SetUpWindowsCallbacks();
    m_windowState = wState::DEFALUT;

    m_testCube.Init();
    m_sh_testCube.VertexShader("shaders/test.vert");
    m_sh_testCube.FragmentShader("shaders/test.frag");
    m_sh_testCube.LinkProgram();

    glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
}

void duckWindow::RunRenderTick()
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    float aspect = static_cast<float>(m_width)/m_height;

    m_sh_testCube.Use();

    m_sh_testCube.setM4fv("model", GL_FALSE, glm::mat4(1.0f));
    m_sh_testCube.setM4fv("view", GL_FALSE, m_camera.GetViewMatrix());
    m_sh_testCube.setM4fv("projection", GL_FALSE, m_camera.GetProjectionMatrix(aspect));

    m_testCube.Draw();


    RenderGUI();
}


void duckWindow::RunClear()
{

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
    ImGui::Begin("Project: Duck", (bool*)1, flags);
    // ImGui::ShowDemoWindow((bool*)1);
    ImGui::End();

    // *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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
}

void duckWindow::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    
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

}

void duckWindow::CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
    duckWindow* win = GW(window);

    float deltaX = xpos - win->m_mouse_prevPos.x;
    float deltaY = ypos - win->m_mouse_prevPos.y;
    win->m_mouse_prevPos.x = xpos;
    win->m_mouse_prevPos.y = ypos;

    switch (win->GetState())
    {
    case wState::CAMERA_MOVE:
        win->m_camera.UpdatePosition(deltaY * win->m_deltaTime);
        break;
    
    case wState::CAMERA_ROTATE:
        win->m_camera.UpdateRotation(-deltaX * win->m_deltaTime, deltaY * win->m_deltaTime);
        break;
        
    default:
        break;
    }
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