#pragma once
#include <memory>
#include <glm.hpp>

struct BoundingBox
{
	glm::vec3 center;
	float halfWidth;

	constexpr glm::vec3 getMin() noexcept
	{
		return glm::vec3(center - halfWidth);
	}

	constexpr glm::vec3 getMax() noexcept
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

	bool isInside(const glm::vec3& pos);

private:
	NodeT mChildren[8];

	glm::vec3 mCenterOfMass;
	float mMass;
	std::shared_ptr<T> pData;
	BoundingBox mBox;
	bool mLeaf;
};

template<typename T>
inline NodeT<T>::NodeT()
{
	
}

template<typename T>
inline NodeT<T>::~NodeT()
{

}

template<typename T>
inline bool NodeT<T>::isInside(const glm::vec3& pos)
{
	return glm::all(glm::lessThanEqual(glm::abs(pos - mBox.center), glm::vec3(mBox.width)));
}