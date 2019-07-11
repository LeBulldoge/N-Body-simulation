#include <iostream>
#include <random>
#include <vector>

#include <glm.hpp>

#include "Constants.h"
#include "Body.h"
#include "Graphics.h"
#include "Camera.h"
#include "WindowManager.h"
#include "Octree.h"

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

int main()
{
	initGLFW();
	initGLEW();

	int fbW = WIDTH;
	int fbH = HEIGHT;

	glm::mat4 projection(1.f);
	projection = glm::perspective(glm::radians(45.f), (float)fbW / (float)fbH, 0.1f, 100.f);

	Camera camera = Camera(glm::vec3(0.f, 0.f, 2.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
	camera.update();

	glm::mat4 MVP = projection * camera.getView();

	initGFX(MVP);

	Octree tree;

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
			tree.Calculate();
			tree.Update();

			camera.input(window);
			camera.update();

			glfwGetFramebufferSize(window, &fbW, &fbH);
			projection = glm::perspective(glm::radians(45.f), (float)fbW / fbH, 0.1f, 100.f);
			MVP = projection * camera.getView();

			drawBodies(tree.getBodies(), MVP);

			wm.drawAll();
			glfwSwapBuffers(window);
		}
		pause = !glfwGetWindowAttrib(window, GLFW_FOCUSED);
		glfwPollEvents();
	}

	ImGui::DestroyContext();
	glfwTerminate();

	return 0;

}
