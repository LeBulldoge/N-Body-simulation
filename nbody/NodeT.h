#pragma once
#include <memory>
#include <vector>
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

class NodeT
{
public:

	NodeT()
	{

	}

	NodeT(glm::vec3 center, float width) : mBox(center, width / 2), pData(nullptr), mLeaf(false)
	{
		mCenterOfMass = { center,  0.f };
	}

	~NodeT()
	{

	}

	void build(const float minWidth)
	{
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

			for (NodeT& child : mChildren)
			{
				child.build(minWidth);
			}
		}
		else
		{
			mLeaf = true;
		}
	}

	bool isInside(const glm::vec3& pos)
	{
		return glm::all(glm::lessThanEqual(glm::abs(pos - mBox.center), glm::vec3(mBox.halfWidth * 2)));
	}

	void assignData(Body& data)
	{
		pData = &data;
	}

	Body* getData()
	{
		return pData;
	}

	void populate(std::vector<Body>& data)
	{
		for (auto& d : data)
		{
			for (auto& child : mChildren)
			{
				if (child.isInside(d.getPos()) && child.mLeaf)
				{
					child.assignData(d);
					break;
				}
				else if(!child.mLeaf)
				{
					child.populate(data);
				}
			}
		}
	}

	void calculateForce(Body& body, float& theta)
	{
		if (body != mCenterOfMass.w)
		{
			if (mLeaf)
			{
				body.addForce(mCenterOfMass);
			}
			else if (mBox.halfWidth*2 / glm::distance(body.getPos(), glm::vec3(mCenterOfMass)) < theta)
			{
				body.addForce(mCenterOfMass);
			}
			else
			{
				for (NodeT& child : mChildren)
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

	void update()
	{
		if (!mLeaf)
		{
			int i = 0;
			mCenterOfMass = { 0.f, 0.f, 0.f, 0.f };
			for (NodeT& child : mChildren)
			{
				if (mActive)
				{
					child.update();
					mCenterOfMass += child.mCenterOfMass;
					i++;
				}
			}
			mCenterOfMass.x /= i;
			mCenterOfMass.y /= i;
			mCenterOfMass.z /= i;
		}
		else
		{
			//if (!isInside(pData->getPos()))
			//{
			//	pData = nullptr;
			//	mActive = false;
			//}
			//else
			//{
				mCenterOfMass = glm::vec4(pData->getPos(), pData->getMass());
			//}
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

private:
	std::vector<NodeT> mChildren;

	glm::vec4 mCenterOfMass;
	Body* pData;
	BoundingBox mBox;
	bool mLeaf;
	bool mActive;
};