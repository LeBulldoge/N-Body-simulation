#pragma once
#include "NodeT.h"
#include "Constants.h"
#include <thread>
#include <random>
#include <atomic>

class OctreeT
{
public:
	OctreeT()
	{

	}

	OctreeT(int amount) : mRoot(glm::vec3(0.f), 10.f), mAmount(amount)
	{
		mTheta = 0.5f;
		mActive = true;

		std::random_device mSeed;
		std::mt19937 mRng;
		std::uniform_real_distribution<> mDist;
		mRng = std::mt19937(mSeed());
		mDist = std::uniform_real_distribution<>(-1, 1);

		mData.reserve(mAmount);

		for (int i = 0; i < mAmount; i++)
		{
			glm::vec3 pos;
			float t = mDist(mRng) * 2.f * PI;
			float s = mDist(mRng) * PI;
			pos.x = sin(t) * cos(s) * 8.f;
			pos.y = sin(t) * sin(s) * 8.f;
			pos.z = cos(t) * mDist(mRng) * 8.f;
			glm::vec3 vel = glm::cross(glm::vec3(pos), glm::vec3(1, 1, 1));
			float orbital_vel = sqrt(2 * glm::length(vel));
			vel = glm::normalize(vel) * orbital_vel / 20000.f;
			mData.emplace_back(pos, vel, 1.f);
		}
		mRoot.build(1.f);
		mRoot.populate(mData);
	}

	~OctreeT()
	{

	}

	void processBodies(int i, const int max, int num)
	{
		int start;
		while (mActive)
		{
			if (!(mUpdateCount.load() & num))
			{
				start = i;
				//printf("Thread %i start: i = %i\n", std::this_thread::get_id(), i);
				for (i; i < max; i++)
				{
					mData[i].resetForce();
					mRoot.calculateForce(mData[i], mTheta);
				}
				i = start;
				for (i; i < max; i++)
				{
					mData[i].update();
				}
				i = start;
				mUpdateCount |= num;
				//printf("Thread %i finish: i = %i, (%i/%i)\n", std::this_thread::get_id(), i, mUpdateCount.load(), mUpdateCount.load());
			}
			else
			{
				if (mUpdateCount.load() >= 15 && mUpdateCount.load() < 255)
				{
					mRoot.updateStart(num);
					mUpdateCount |= num * 16;
				}
				else if (mUpdateCount == 255)
				{
					mUpdateCount = 0;
				}
				std::this_thread::yield();
			}
		}
	}

	const Body* getBodiesData() noexcept
	{
		return mData.data();
	}

	const int getBodiesAmount() noexcept
	{
		return mAmount;
	}

	float& getTheta() noexcept
	{
		return mTheta;
	}

private:
	NodeT mRoot;

	bool mActive;
	int mAmount;
	std::atomic<int> mUpdateCount;
	float mTheta;

	std::vector<Body> mData;
};

