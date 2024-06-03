#include "tessellationWindow.h"

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

tessellationWindow::tessellationWindow()
{}

tessellationWindow::~tessellationWindow()
{}

void tessellationWindow::RunInit()
{
    // GLFW
    // *=*=*=*=*=*=*=*=*=*=
    
    // Scene
    // *=*=*=*=*=*=*=*=*=*=

    // GUI
    // *=*=*=*=*=*=*=*=*=*=
    const GLubyte* renderer = glGetString(GL_RENDERER);
    m_gui_renderer = std::string((const char*)renderer);
    
    m_gui_hintColor = ImVec4(0.231f, 0.820f, 0.0984f, 1.0f);

    // OpenGL
    // *=*=*=*=*=*=*=*=*=*=

    glClearColor(0.5f, 0.5f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
}

void tessellationWindow::RunRenderTick()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    RenderGUI();
}

void tessellationWindow::RunClear()
{

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
    //ImGui::ShowDemoWindow();
    ImGui::End();

    // *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); 

}

void tessellationWindow::InfoWindowSizeCallback(ImGuiSizeCallbackData* data)
{
    tessellationWindow* win = reinterpret_cast<tessellationWindow*>(data->UserData);
    data->DesiredSize.y = win->m_height;    
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
