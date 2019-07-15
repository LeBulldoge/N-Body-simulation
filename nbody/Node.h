#pragma once
#include "Body.h"

#include <vector>
#include <memory>
#include <algorithm>

#include "Constants.h"

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
	Node(glm::vec3 center, float width, const std::vector<Body>& bodies);
	~Node();

	void update();
	bool empty() const;
	bool active() const;
	void populate(std::vector<BoundingBox>& boxes);
	void calculateForce(Body& body, float& theta);
	bool isInside(const std::shared_ptr<Body>& body) const;
	void addBody(std::shared_ptr<Body>& body);
	BoundingBox getRegion() noexcept;
	glm::vec4 getCenterOfMass() noexcept;


private:

	BoundingBox mRegion;
	std::vector<std::shared_ptr<Body>> pBodies;
	glm::vec4 mCenterOfMass;
	std::vector<Node> mChildren;
	bool mLeaf;
	bool mActive;
};

