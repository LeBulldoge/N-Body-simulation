#pragma once
#include <memory>
#include <glm.hpp>

struct BoundingBox
{
	glm::vec3 center;
	float halfWidth;

	const glm::vec3 getMin() noexcept
	{
		return glm::vec3(center - halfWidth);
	}

	const glm::vec3 getMax() noexcept
	{
		return glm::vec3(center + halfWidth);
	}
};
typedef struct BoundingBox BoundingBox;

template<typename T>
class NodeT
{
public:
	NodeT();
	~NodeT();
private:
	NodeT mChildren[8];

	std::shared_ptr<T> pData;
	BoundingBox mBox;
	bool mLeaf;
};

