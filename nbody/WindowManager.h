#pragma once
#include "WMwindow.h"
#include <vector>
#include <algorithm>
#include <optional>

class WindowManager
{
private:

	std::vector<windowPtr> pWindows;
	windowPtr pPinned;

public:

	WindowManager();
	WindowManager(int size);
	~WindowManager();

	template<typename... T>
	void addWindow(T&&... t);
	void destroyWindow(windowPtr ptr);
	void drawAll();
	void move(int a, int b);
	void resetPinnedStatus();
	std::optional<windowPtr> operator[](const unsigned int i);
	std::optional<windowPtr> operator[](std::string title);
	size_t size();
	bool empty();
};

WindowManager::WindowManager()
{
	ImGui::StyleColorsDark();
}

WindowManager::WindowManager(int size)
{
	pWindows.reserve(size);
}

WindowManager::~WindowManager()
{

}

template<typename... T>
void WindowManager::addWindow(T&&... t)
{
	pWindows.emplace_back(std::make_shared<WMwindow>(std::forward<T>(t)...));
}

void WindowManager::destroyWindow(windowPtr ptr)
{
	pWindows.erase(std::remove(pWindows.begin(), pWindows.end(), ptr), pWindows.end());
}

void WindowManager::drawAll()
{
	resetPinnedStatus();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	for (windowPtr ptr : pWindows)
	{
		ptr->draw();
	}

	ImGui::Render();
	ImGui::EndFrame();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void WindowManager::move(const int a, const int b)
{
	std::swap(pWindows[a], pWindows[b]);
}

void WindowManager::resetPinnedStatus()
{
	for (windowPtr ptr : pWindows)
	{
		if (ptr->getPinned() && ptr != pPinned)
		{
			if (pPinned != nullptr) pPinned->unpin();
			pPinned = ptr;
			ImGui::SetWindowFocus(ptr->getTitle().c_str());
		}
	}
}

std::optional<windowPtr> WindowManager::operator[](const unsigned int i)
{
	if (pWindows.size() > i)
	{
		return pWindows[i];
	}
	return std::nullopt;
}
std::optional<windowPtr> WindowManager::operator[](std::string title)
{
	for (windowPtr ptr : pWindows)
	{
		if (ptr->getTitle() == title) return ptr;
	}
	return std::nullopt;
}

size_t WindowManager::size()
{
	return pWindows.size();
}
bool WindowManager::empty()
{
	return pWindows.empty();
}