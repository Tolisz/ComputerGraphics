#include "duckWindow.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <cmath>

void duckWindow::RunInit()
{
    glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
}

void duckWindow::RunRenderTick()
{
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    ImGui::NewFrame();

    bool my_tool_active = true;

    // Create a window called "My First Tool", with a menu bar.
    ImGui::Begin("My First Tool", &my_tool_active, ImGuiWindowFlags_MenuBar);
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
            if (ImGui::MenuItem("Save", "Ctrl+S"))   { /* Do stuff */ }
            if (ImGui::MenuItem("Close", "Ctrl+W"))  { my_tool_active = false; }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    // Edit a color stored as 4 floats
    // ImGui::ColorEdit4("Color", my_color);

    // Generate samples and plot them
    float samples[100];
    for (int n = 0; n < 100; n++)
        samples[n] = std::sinf(n * 0.2f + ImGui::GetTime() * 1.5f);
    ImGui::PlotLines("Samples", samples, 100);

    // Display contents in a scrolling region
    ImGui::TextColored(ImVec4(1,1,0,1), "Important Stuff");
    ImGui::BeginChild("Scrolling");
    for (int n = 0; n < 50; n++)
        ImGui::Text("%04d: Some text", n);
    ImGui::EndChild();
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void duckWindow::RunClear()
{

}


