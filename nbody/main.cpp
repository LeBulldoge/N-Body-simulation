#include <iostream>
#include <random>
#include <vector>

#include <glm.hpp>

#include "Constants.h"
#include "Body.h"
#include "Graphics.h"
#include "WindowManager.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

extern bool pause = false;
static WindowManager wm(5);
static std::vector<float> frames;

void showFps()
{
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
		1000.0f / ImGui::GetIO().Framerate,
		ImGui::GetIO().Framerate);
	float fps = ImGui::GetIO().Framerate;
	if (frames.size() > 100) //Max seconds to show
	{
		for (size_t i = 1; i < frames.size(); i++)
		{
			frames[i - 1] = frames[i];
		}
		frames[frames.size() - 1] = fps;
	}
	else
	{
		frames.push_back(fps);
	}
	ImGui::Separator();
	ImGui::PlotHistogram("Framerate", &frames[0], frames.size(), 0, NULL, 0.0f, 100.0f);
}

int main(void)
{
	initGLFW();
	initGLEW();
	initGFX();
	
	std::vector<Body> bodies(AMOUNT);

	wm.addWindow(400, 100, "Framerate", ImGuiCond_FirstUseEver);

	wm[0].value()->addDrawables(&showFps);

	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, false);
	ImGui_ImplOpenGL3_Init("#version 330");

	while (!glfwWindowShouldClose(window))
	{
		if (!pause)
		{
			for (auto& body : bodies)
			{
				body.resetG();
				for (auto& other : bodies)
				{
					if (body != other)
					{
						body.addG(other);
					}
				}
			}

			for (auto& body : bodies)
			{
				body.update();
			}
			
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();		

			wm.drawAll();

			ImGui::Render();
			ImGui::EndFrame();

			drawBodies((GLfloat*)&bodies[0]);

			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapBuffers(window);
		}

		glfwPollEvents();
	}

	glfwTerminate();

	return 0;

}
