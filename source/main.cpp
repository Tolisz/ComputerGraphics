#include <iostream>

#include "duckWindow.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>


#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define WINDOW_TITLE "Duck"

int main() 
{   
    // Init GLFW
    if (!glfwInit())
    {
        std::cout << "GLFW library initialisation failed" << std::endl;
        return 0;
    }

    // Init ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = NULL;
    io.LogFilename = NULL;
    ImGui::StyleColorsLight();

    {
        duckWindow app;
        if (!app.InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE))
        {
            glfwTerminate();
            return 0;
        }

        glfwMakeContextCurrent(app.GetWindowPointer());
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "GLAD initialization failed" << std::endl;
            glfwTerminate();
            return 0;
        }

        ImGui_ImplGlfw_InitForOpenGL(app.GetWindowPointer(), true);
        ImGui_ImplOpenGL3_Init("#version 460");

        try
        {
            app.RunApp();
        }
        catch(const std::exception& ex)
        {
            std::cout << "An exception was thrown: \n\t[what]: " << ex.what() << std::endl;
        }
        catch(...)
        {
            std::cout << "An undefined exception was thrown" << std::endl;
        }
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}