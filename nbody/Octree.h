#pragma once
#include "Node.h"

class Octree
{
public:

	Octree();
	~Octree();

	void Update();
	void Calculate();
	Body* getBodies();

private:

	BoundingBox mTotalRegion;

	Node mRoot;
	std::vector<Body> pBodies;

};