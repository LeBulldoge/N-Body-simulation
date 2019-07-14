#pragma once
#include "Node.h"
#include <random>

class Octree
{
public:

	Octree(int amount);
	~Octree();

	void Update();
	void Calculate();
	void BruteForceCalculate();
	Body* getBodiesData();
	float& getTheta();
	glm::vec3 randomPos();
	glm::vec3 randomVel(glm::vec3 pos);

private:

	BoundingBox mTotalRegion;

	Node mRoot;
	const int mAmount;
	std::vector<Body> mBodies;
	float mTheta;

	std::random_device mSeed;
	std::mt19937 mRng;
	std::uniform_real_distribution<> mDist;
	std::uniform_real_distribution<> mDistMass;
	
};