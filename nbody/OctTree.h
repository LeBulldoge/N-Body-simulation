#pragma once
#include <bitset>
#include "Body.h"

struct BoundingBox {
	glm::vec3 center, width;
};
typedef struct BoundingBox BoundingBox;

class OctTree {

	BoundingBox region;
	Body* bodies;
	OctTree* children;
	uint8_t activeNodes;
	//OctTree& _parent;

public:
	OctTree();
	OctTree(const BoundingBox box, Body* bods);
	OctTree(const BoundingBox box);
	~OctTree();
	OctTree traverse();
};