#include "Octree.h"

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
	for (Body& body : pBodies)
	{
		body.resetG();
		mRoot.calculateForce(body);
	}
}

Body* Octree::getBodies()
{
	return pBodies.data();
}