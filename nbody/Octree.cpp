#include "Octree.h"
#include <thread>

Octree::Octree(const int amount) : mAmount(amount)
{
	mTheta = 0.5f;

	mRng = std::mt19937(mSeed());
	mDist = std::uniform_real_distribution<>(-1, 1);

	mBodies.reserve(mAmount);
	glm::vec3 tempPos;
	for (int i = 0; i < mAmount - 1; i++)
	{
		tempPos = randomPos();
		mBodies.emplace_back(tempPos, randomVel(tempPos), 1.f);
	}
	mBodies.emplace_back(glm::vec3(0.f), glm::vec3(0.f), 5.f);

	mRoot = Node(glm::vec3(0.f), 10.f, mBodies);
	mRoot.populate(pBoxes);
	mRoot.update();
}

Octree::~Octree()
{

}

void Octree::Update()
{
	for (Body& body : mBodies)
	{
		body.update();
	}
	mBodies[mAmount - 1].reset();
	pBoxes.erase(pBoxes.begin(), pBoxes.end());
	mRoot = Node(glm::vec3(0.f), 10.f, mBodies);
	mRoot.populate(pBoxes);
	mRoot.update();
}

//TO DO:
//Resource race? (std::mutex is too slow)
void Octree::Calculate()
{
	auto calcLambda = [&](int i, const int max)
	{
		//printf("Thread %i start: i = %i\n", std::this_thread::get_id(), i);
		for (i; i < max; i++)
		{
			mBodies[i].resetForce();
			mRoot.calculateForce(mBodies[i], mTheta);
		}
		//printf("Thread %i finish: i = %i\n", std::this_thread::get_id(), i);
	};

	const int cores = std::thread::hardware_concurrency();

	std::vector<std::thread> threads;
	threads.reserve(cores);

	for (int i = 0; i < cores; i++)
	{
		threads.emplace_back(calcLambda,
			static_cast<int>(mAmount * (static_cast<float>(i) / cores)),
			static_cast<int>(mAmount * (static_cast<float>(i + 1) / cores)));
	}

	for (std::thread& thread : threads)
	{
		thread.join();
	}
}

void Octree::BruteForceCalculate()
{
	auto calcLambda = [&](int i, const int max)
	{
		//printf("Thread %i start: i = %i\n", std::this_thread::get_id(), i);
		for (i; i < max; i++)
		{
			mBodies[i].resetForce();
			for (Body& other : mBodies)
			{
				if (mBodies[i] != other)
				{
					mBodies[i].addForce({ other.getPos(), other.getMass() });
				}
			}
		}
		//printf("Thread %i finish: i = %i\n", std::this_thread::get_id(), i);
	};

	const int cores = std::thread::hardware_concurrency();

	std::vector<std::thread> threads;
	threads.reserve(cores);

	for (int i = 0; i < cores; i++)
	{
		threads.emplace_back(calcLambda,
			static_cast<int>(mAmount * (static_cast<float>(i) / cores)),
			static_cast<int>(mAmount * (static_cast<float>(i + 1) / cores)));
	}

	for (std::thread& thread : threads)
	{
		thread.join();
	}
}

Body* Octree::getBodiesData()
{
	return mBodies.data();
}

BoundingBox* Octree::getBoxesData()
{
	return pBoxes.data();
}

int Octree::getBoxAmount()
{
	return pBoxes.size();
}

float & Octree::getTheta()
{
	return mTheta;
}

glm::vec3 Octree::randomPos()
{
	glm::vec3 pos;
	float t = mDist(mRng) * 2.f * PI;
	float s = mDist(mRng) * PI;
	pos.x = sin(t)*cos(s)*8.f;
	pos.y = sin(t)*sin(s)*8.f;
	pos.z = cos(t)*mDist(mRng)*8.f;

	return pos;
}

glm::vec3 Octree::randomVel(glm::vec3 pos)
{
	// Initial velocity is 'orbital' velocity from position
	glm::vec3 vel = glm::cross(glm::vec3(pos), glm::vec3(0, 0, 1));
	float orbital_vel = sqrt(2 * glm::length(vel));
	vel = glm::normalize(vel)*orbital_vel / 20000.f;
	return vel;
}
