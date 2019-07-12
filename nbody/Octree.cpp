#include "Octree.h"
#include <thread>
#include <mutex>

Octree::Octree()
{
	pBodies.reserve(AMOUNT);
	for (int i = 0; i < AMOUNT-1; i++)
	{
		pBodies.emplace_back();
	}
	pBodies.emplace_back(glm::vec3(0.f), glm::vec3(0.f), 3.f);
	Init();
}

Octree::~Octree()
{
	
}

void Octree::Update()
{
	for (Body& body : pBodies)
	{
		body.update();
	}
	pBodies[AMOUNT-1].reset();
	mRoot.update();
}

void Octree::Init()
{
	mRoot = Node(glm::vec3(0.f), 1.f, pBodies);
	mRoot.populate();
	mRoot.update();
}

void Octree::Calculate()
{
	auto calcLambda = [&](int i, const int max)
	{
		//printf("Thread %i start: i = %i\n", std::this_thread::get_id(), i);
		for (i; i < max; i++)
		{
			pBodies[i].resetG();
			mRoot.calculateForce(pBodies[i]);
		}
		//printf("Thread %i finish: i = %i\n", std::this_thread::get_id(), i);
	};

	const int cores = std::thread::hardware_concurrency();

	std::vector<std::thread> threads;

	for (int i = 0; i < cores; i++)
	{
		threads.emplace_back(calcLambda, (int)(AMOUNT * ((float)i / cores)), (int)(AMOUNT * ((float)(i + 1) / cores)));
	}

	for (std::thread& thread : threads)
	{
		thread.join();
	}
}

Body* Octree::getBodies()
{
	return pBodies.data();
}