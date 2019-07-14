#pragma once
#include <memory>
#include <functional>
#include <vector>
#include <string>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

class WMwindow;
using windowPtr = std::shared_ptr<WMwindow>;

class WMwindow
{
private:

	//using func = std::function<void()>;
	//using funcPtr = std::shared_ptr<func>;

	int mWidth;
	int mHeight;
	std::string mTitle;
	ImGuiCond mCond;
	ImGuiWindowFlags mFlags;
	bool mVisibility;
	bool mPinned;
	std::vector<std::function<void()>> mDrawables;

public:

	WMwindow(int width, int height, std::string title, ImGuiCond cond, bool visibility = true, bool pinned = false, ImGuiWindowFlags flags = 0);
	~WMwindow();

	std::string getTitle();

	template<typename T>
	void addDrawables(T&& f);
	void removeDrawable(int i);

	void draw();

	void show();
	void hide();

	bool getPinned();
	void pin();
	void unpin();

	void toggleVisibility();

	void addFlags(ImGuiWindowFlags flags);
	void removeFlags(ImGuiWindowFlags flags);

};

WMwindow::WMwindow(int width, int height, std::string title, ImGuiCond cond, bool visibility, bool pinned, ImGuiWindowFlags flags)
	:
	mWidth(width),
	mHeight(height),
	mTitle(title),
	mCond(cond),
	mVisibility(visibility),
	mPinned(pinned),
	mFlags(flags)
{
	mFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_MenuBar;
}

WMwindow::~WMwindow()
{

}

std::string WMwindow::getTitle()
{
	return mTitle;
}

template<typename T>
void WMwindow::addDrawables(T&& f)
{
	mDrawables.push_back(f);
}

void WMwindow::removeDrawable(int i)
{
	mDrawables.erase(mDrawables.begin() + i);
}

void WMwindow::draw()
{
	if (mVisibility)
	{
		ImGui::SetNextWindowSize(ImVec2(mWidth, mHeight), mCond);
		ImGui::Begin(mTitle.c_str(), &mVisibility, mFlags);
		ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::MenuItem("Pin", NULL, true, !mPinned))
			{
				pin();
			}
			ImGui::EndMenuBar();
		}
		if (!mDrawables.empty())
		{
			for (std::function<void()>& func : mDrawables)
			{
				std::invoke(func);
			}
		}
		ImGui::End();
		if (mPinned)
		{
			ImGui::SetWindowFocus();
		}
	}
}

void WMwindow::show()
{
	mVisibility = true;
}
void WMwindow::hide()
{
	mVisibility = false;
}
void WMwindow::toggleVisibility()
{
	mVisibility = !mVisibility;
}

bool WMwindow::getPinned()
{
	return mPinned;
}
void WMwindow::pin()
{
	mPinned = true;
	mFlags &= ~ImGuiWindowFlags_NoBringToFrontOnFocus;
}
void WMwindow::unpin()
{
	mPinned = false;
	mFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
}

void WMwindow::addFlags(ImGuiWindowFlags flags)
{
	mFlags |= flags;
}
void WMwindow::removeFlags(ImGuiWindowFlags flags)
{
	mFlags &= ~flags;
}