#pragma once
#include "Node.h"
#include <random>

//Needs rewriting to optimize for multithreading and better tree structure
//Too many allocations every tree build
class Octree
{
public:

	Octree(int amount);
	~Octree();

	void Update();
	void Calculate();
	void BruteForceCalculate();
	const Body* getBodiesData() noexcept;
	const int getBodiesAmount() noexcept;
	const BoundingBox* getBoxesData() noexcept;
	const int getBoxAmount() noexcept;
	float& getTheta() noexcept;
	glm::vec3 randomPos() noexcept;
	glm::vec3 randomVel(glm::vec3 pos);

private:

	BoundingBox mTotalRegion;

	Node mRoot;
	const int mAmount;
	std::vector<Body> mBodies;
	float mTheta;

	std::vector<BoundingBox> pBoxes;

	std::random_device mSeed;
	std::mt19937 mRng;
	std::uniform_real_distribution<> mDist;
	
};