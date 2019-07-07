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

void showFps()
{
	static std::vector<float> fps;
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
		1000.0f / ImGui::GetIO().Framerate,
		ImGui::GetIO().Framerate);
	if (fps.size() > 100)
	{
		fps.erase(fps.begin());
	}
	else
	{
		fps.push_back(ImGui::GetIO().Framerate);
	}
	ImGui::Separator();
	ImGui::PlotHistogram("Framerate", fps.data(), fps.size(), 0, NULL, 0.0f, 60.0f, ImVec2(325, 30));
}

int main(void)
{
	initGLFW();
	initGLEW();
	initGFX();
	
	std::vector<Body> bodies(AMOUNT);

	WindowManager wm(5);
	wm.addWindow(400, 100, "Framerate", ImGuiCond_FirstUseEver, true, true, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground);

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

			drawBodies(bodies.data());

			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapBuffers(window);
		}
		pause = !glfwGetWindowAttrib(window, GLFW_FOCUSED);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;

}
