#pragma once
#include "Node.h"

class Octree
{
public:

	Octree();
	~Octree();

	void Update();
	void Init();
	void Calculate();
	Body* getBodies();

private:

	BoundingBox mTotalRegion;

	Node mRoot;
	std::vector<Body> pBodies;

};