#include <iostream>
#include <random>
#include <vector>
#include <thread>

#include <glm.hpp>

#include "Constants.h"
#include "Body.h"
#include "Graphics.h"
#include "Camera.h"
#include "WindowManager.h"
//#include "Octree.h"
#include "OctreeT.h"


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

	//Octree tree(AMOUNT);

	OctreeT tTree(AMOUNT);

	initGFXBodies(MVP, tTree.getBodiesAmount());
	//initGFXBoxes(MVP, tree.getBoxAmount());

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
	wm[1].value()->addDrawables([&tTree, &showBoxes]()
	{
		ImGui::SliderFloat("THETA", &tTree.getTheta(), 0.f, 2.f, "%.5f");
		ImGui::TextWrapped("Controls the performance/accuracy ratio.\nWARNING: Depending on the amount bodies (%i), setting Theta close to 0.0 will cause instability.", tTree.getBodiesAmount());
		ImGui::Checkbox("Draw octree", &showBoxes);
	});

	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, false);
	ImGui_ImplOpenGL3_Init("#version 330");

	const int cores = std::thread::hardware_concurrency();

	std::vector<std::thread> threads;
	threads.reserve(cores);

	const auto calcLambda = [&tTree](int i, int max, int num)
	{
		tTree.processBodies(i, max, num);
	};

	int bitFlag = 1;

	for (int i = 0; i < cores; i++)
	{
		threads.emplace_back(calcLambda,
			static_cast<int>(tTree.getBodiesAmount() * (static_cast<float>(i) / cores)),
			static_cast<int>(tTree.getBodiesAmount() * (static_cast<float>(i + 1) / cores)),
			bitFlag);
		bitFlag *= 2;
	}

	while (!glfwWindowShouldClose(window))
	{
		if (!pause)
		{
			//tree.Update();
			
			camera.input(window);
			camera.update();

			glfwGetFramebufferSize(window, &fbW, &fbH);
			projection = glm::perspective(glm::radians(45.f), (float)fbW / fbH, 0.1f, 100.f);
			MVP = projection * camera.getView() * model;

			drawBodies(tTree.getBodiesData(), MVP, tTree.getBodiesAmount());
			if (showBoxes)
			{
				//drawBox(tree.getBoxesData(), MVP, tree.getBoxAmount());
			}
			wm.drawAll();
			glfwSwapBuffers(window);
			
		}
		//pause = !glfwGetWindowAttrib(window, GLFW_FOCUSED);
		glfwPollEvents();
		if (glfwWindowShouldClose(window))
		{
			//tTree.isWorking = false;
		}
	}

	for (std::thread& thread : threads)
	{
		thread.join();
	}

	ImGui::DestroyContext();
	glfwTerminate();

	return 0;

}
