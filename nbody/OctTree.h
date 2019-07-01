#include <list>
#include <bitset>
#include "Body.h"

struct BoundingBox {
	glm::vec3 center, halfWidth;
};
typedef struct BoundingBox BoundingBox;

class OctTree {

	BoundingBox bBox;
	Body* objects;
	OctTree* childNodes;
	uint8_t activeNodes;
	OctTree& _parent;

public:
	OctTree();
	OctTree(const glm::vec3& center, const glm::vec3& halfWidth, Body* objects);
	~OctTree();
	OctTree traverse();
};