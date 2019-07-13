#pragma once
#include "Body.h"
#include <algorithm>

struct BoundingBox
{
	glm::vec3 center;
	float width;

	glm::vec3 getMin()
	{
		return glm::vec3(center - width / 2);
	}

	glm::vec3 getMax()
	{
		return glm::vec3(center + width / 2);
	}
};
typedef struct BoundingBox BoundingBox;

class Node
{
public:
	Node();
	Node(glm::vec3 center, float width);
	Node(glm::vec3 center, float width, std::vector<Body> bodies);
	~Node();

	void update();
	bool empty();
	bool active();
	void populate();
	void calculateForce(Body& body, float& theta);
	bool isInside(std::shared_ptr<Body>& body);
	void addBody(std::shared_ptr<Body>& body);
	void moveBody(std::shared_ptr<Body>& body);

private:

	BoundingBox mRegion;
	std::vector<std::shared_ptr<Body>> pBodies;
	glm::vec4 mCenterOfMass;
	std::vector<Node> mChildren;
	bool mLeaf;
	bool mActive;
};

