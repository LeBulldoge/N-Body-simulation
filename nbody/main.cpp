#include <iostream>
#include <random>
#include <vector>

#include <glm.hpp>

#include "Constants.h"
#include "Body.h"
#include "Graphics.h"
#include "Camera.h"
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
	
	int fbW = WIDTH;
	int fbH = HEIGHT;

	glm::mat4 model(1.f);

	glm::mat4 projection(1.f);
	projection = glm::perspective(glm::radians(45.f), (float)fbW / (float)fbH, 0.1f, 100.f);

	Camera camera(glm::vec3(0.f, 0.f, 2.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
	camera.update();

	glm::mat4 view = camera.getView();

	glm::mat4 MVP = projection * view;

	initGFX(MVP);
	
	std::vector<Body> bodies(AMOUNT);

	WindowManager wm(1);
	wm.addWindow(400, 100, "Framerate", ImGuiCond_FirstUseEver, true, true, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground);

	wm[0].value()->addDrawables(&showFps);

	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, false);
	ImGui_ImplOpenGL3_Init("#version 330");

	while (!glfwWindowShouldClose(window))
	{
		if (!pause)
		{
			for (Body& body : bodies)
			{
				body.resetG();
				for (Body& other : bodies)
				{
					if (body != other)
					{
						body.addG(other);
					}
				}
			}

			for (Body& body : bodies)
			{
				body.update();
			}

			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
				camera.moveF();
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
				camera.moveB();
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
				camera.moveL();
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
				camera.moveR();

			camera.update();

			view = camera.getView();

			glfwGetFramebufferSize(window, &fbW, &fbH);
			projection = glm::perspective(glm::radians(45.f), (float)fbW / (float)fbH, 0.1f, 100.f);

			MVP = projection * view;
		
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();		

			wm.drawAll();

			ImGui::Render();
			ImGui::EndFrame();

			drawBodies(bodies.data(), MVP);

			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapBuffers(window);

		}

		pause = !glfwGetWindowAttrib(window, GLFW_FOCUSED);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;

}
