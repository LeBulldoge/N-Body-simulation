#pragma once
#include <memory>
#include <vector>
#include <algorithm>
#include <ranges>
#include "Body.h"

struct BoundingBox
{
	glm::vec3 center;
	float halfWidth;
	
	BoundingBox() : center(0), halfWidth(0)
	{

	}

	BoundingBox(glm::vec3 c, float w) : center(c), halfWidth(w / 2) 
	{

	}

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

class Node
{
private:
	Node* pRoot;
	std::vector<Node> mChildren;

	glm::vec4 mCenterOfMass;
	Body* pData;
	BoundingBox mBox;
	bool mLeaf;
	bool mActive;

	const bool isInside(const glm::vec3& pos) const
	{
		return glm::all(glm::lessThanEqual(glm::abs(pos - mBox.center), glm::vec3(mBox.halfWidth * 2)));
	}

	void assignData(Body* data)
	{
		pData = data;
		mActive = true;
	}

	void update()
	{
		if (mActive)
		{
			if (!mLeaf)
			{
				int i = 0;
				mCenterOfMass = { 0.f, 0.f, 0.f, 0.f };
				mActive = false;
				for (Node& child : mChildren)
				{
					if (mActive)
					{
						child.update();
						mCenterOfMass += child.mCenterOfMass;
						i++;
					}
					if (child.mActive)
					{
						mActive = true;
					}
				}
				mCenterOfMass.x /= i;
				mCenterOfMass.y /= i;
				mCenterOfMass.z /= i;
			}
			else
			{
				if (!isInside(pData->getPos()))
				{
					pRoot->populate(pData);
					pData = nullptr;
					mActive = false;
				}
				else
				{
					mCenterOfMass = glm::vec4(pData->getPos(), pData->getMass());
				}
			}
		}
	}

public:

	Node()
	{

	}

	Node(glm::vec3 center, float width) : mBox(center, width / 2), pData(nullptr), mLeaf(false), mActive(false)
	{
		mCenterOfMass = { center,  0.f };
	}

	~Node()
	{

	}

	void build(const float minWidth, Node* root)
	{
		pRoot = root;
		if (mBox.halfWidth * 2 > minWidth)
		{
			mChildren.emplace_back(mBox.getMax(), mBox.halfWidth);
			mChildren.emplace_back(glm::vec3(mBox.getMax().x, mBox.getMax().y, mBox.getMin().z), mBox.halfWidth);
			mChildren.emplace_back(glm::vec3(mBox.getMin().x, mBox.getMax().y, mBox.getMin().z), mBox.halfWidth);
			mChildren.emplace_back(glm::vec3(mBox.getMin().x, mBox.getMax().y, mBox.getMax().z), mBox.halfWidth);
			mChildren.emplace_back(mBox.getMin(), mBox.halfWidth);
			mChildren.emplace_back(glm::vec3(mBox.getMin().x, mBox.getMin().y, mBox.getMax().z), mBox.halfWidth);
			mChildren.emplace_back(glm::vec3(mBox.getMax().x, mBox.getMin().y, mBox.getMax().z), mBox.halfWidth);
			mChildren.emplace_back(glm::vec3(mBox.getMax().x, mBox.getMin().y, mBox.getMin().z), mBox.halfWidth);

			for (Node& child : mChildren)
			{
				child.build(minWidth, root);
			}
		}
		else
		{
			mLeaf = true;
		}
	}

	Body* getData()
	{
		return pData;
	}

	void populate(Body* body)
	{
		for (auto &child : mChildren)
		{
			if (child.isInside(body->getPos()))
			{
				if (child.mLeaf)
				{
					child.assignData(body);
					break;
				}
				else
				{
					child.populate(body);
					break;
				}
			}
			if (child.mActive)
			{
				mActive = true;
			}
		}
	}

	void calculateForce(Body* body, float& theta)
	{
		if (body->getMass() != mCenterOfMass.w)
		{
			if (mLeaf)
			{
				body->addForce(mCenterOfMass);
			}
			else if (mBox.halfWidth*2 / glm::distance(body->getPos(), glm::vec3(mCenterOfMass)) < theta)
			{
				body->addForce(mCenterOfMass);
			}
			else
			{
				for (Node& child : mChildren)
				{
					if (!mChildren.empty())
					{
						child.calculateForce(body, theta);
					}
				}
			}
		}
	}

	void updateStart(int num)
	{
		if (num == 1) num = 0;
		if (num == 8) num = 6;
		for (int i = num; i < num + 2; i++)
		{
			mChildren[i].update();
		}
	}

	bool empty()
	{
		if (pData == nullptr)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
};