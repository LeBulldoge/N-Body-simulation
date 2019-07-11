#include "Octree.h"
#include <thread>

Octree::Octree()
{
	glm::vec3 center(0.f);
	pBodies.reserve(AMOUNT);
	for (int i = 0; i < AMOUNT; i++)
	{
		pBodies.push_back(Body());
		//center += pBodies[i].Pos();
	}
	center /= pBodies.size();
	mRoot = Node(center, 1.f, pBodies);
	mRoot.populate();
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
	mRoot.update();
}

void Octree::Calculate()
{
	std::thread calc0([&]
	{
		for (int i = 0; i < AMOUNT / 2; i++)
		{
			pBodies[i].resetG();
			mRoot.calculateForce(pBodies[i]);
		}
	});
	std::thread calc1([&]
	{
		for (int i = AMOUNT / 2; i < AMOUNT; i++)
		{
			pBodies[i].resetG();
			mRoot.calculateForce(pBodies[i]);
		}
	});
	calc0.join();
	calc1.join();
}

Body* Octree::getBodies()
{
	return pBodies.data();
}