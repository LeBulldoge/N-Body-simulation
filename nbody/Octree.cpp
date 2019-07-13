#include "Octree.h"
#include <thread>



Octree::Octree()
{
	mTheta = 0.5f;

	mRng = std::mt19937(mSeed());
	mDist = std::uniform_real_distribution<>(-1, 1);
	mDistMass = std::uniform_real_distribution<>(0.1, 1);

	pBodies.reserve(AMOUNT);
	glm::vec3 tempPos;
	for (int i = 0; i < AMOUNT-1; i++)
	{
		tempPos = randomPos();
		pBodies.emplace_back(tempPos, randomVel(tempPos), mDistMass(mRng));
	}
	pBodies.emplace_back(glm::vec3(0.f), glm::vec3(0.f), 3.f);
	
	mRoot = Node(glm::vec3(0.f), 1.f, pBodies);
	mRoot.populate();
	mRoot.update();
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

//TODO:
//Resource race? (std::mutex is too slow)
//Fix typecasting
void Octree::Calculate()
{
	auto calcLambda = [&](int i, const int max)
	{
		//printf("Thread %i start: i = %i\n", std::this_thread::get_id(), i);
		for (i; i < max; i++)
		{
			pBodies[i].resetForce();
			mRoot.calculateForce(pBodies[i], mTheta);
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

Body* Octree::getBodiesData()
{
	return pBodies.data();
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
	pos.x = sin(t)*cos(s);
	pos.y = sin(t)*sin(s);
	pos.z = cos(t)*mDist(mRng);

	return pos;
}

glm::vec3 Octree::randomVel(glm::vec3 pos)
{
	// Initial velocity is 'orbital' velocity from position
	glm::vec3 vel = glm::cross(glm::vec3(pos), glm::vec3(0, 0, 1));
	float orbital_vel = sqrt(2 * glm::length(vel));
	vel = glm::normalize(vel)*orbital_vel / 10000.f;
	return vel;
}
