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

int main()
{
	initGLFW();
	initGLEW();

	int fbW = WIDTH;
	int fbH = HEIGHT;

	glm::mat4 model(1.f);

	glm::mat4 projection(1.f);
	projection = glm::perspective(glm::radians(45.f), (float)fbW / (float)fbH, 0.1f, 100.f);

	Camera camera = Camera(glm::vec3(0.f, 0.f, 30.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
	camera.update();

	glm::mat4 MVP = projection * camera.getView() * model;

	Octree tree(AMOUNT);

	initGFXBodies(MVP);
	initGFXBoxes(MVP, tree.getBoxAmount());

	WindowManager wm(2);
	wm.addWindow(400, 100, "Framerate", ImGuiCond_FirstUseEver, true, true, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground);
	wm.addWindow(400, 200, "Controls", ImGuiCond_FirstUseEver);

	wm[0].value()->addDrawables([]()
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
			fps.emplace_back(ImGui::GetIO().Framerate);
		}
		ImGui::Separator();
		ImGui::PlotHistogram("Framerate", fps.data(), fps.size(), 0, NULL, 0.0f, 60.0f, ImVec2(325, 30));
	});

	bool showBoxes = false;
	//???
	wm[1].value()->addDrawables([&tree, &showBoxes]()
	{
		ImGui::SliderFloat("THETA", &tree.getTheta(), 0.f, 2.f, "%.5f");
		ImGui::TextWrapped("Controls the performance/accuracy ratio.\nWARNING: Depending on the amount bodies (%i), setting Theta close to 0.0 will cause instability.", AMOUNT);
		if (ImGui::Button("Calculate"))
		{
			tree.BruteForceCalculate();
		}
		ImGui::SameLine();
		ImGui::TextWrapped("Calculates the gravitational pull between bodies using the brute force algorithm (O(n^2)).");
		ImGui::Checkbox("Draw octree", &showBoxes);
	});

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
			MVP = projection * camera.getView() * model;

			drawBodies(tree.getBodiesData(), MVP);
			if (showBoxes)
			{
				drawBox(tree.getBoxesData(), MVP, tree.getBoxAmount());
			}
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
