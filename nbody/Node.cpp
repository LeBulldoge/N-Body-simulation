#include "Node.h"

Node::Node()
{

}

Node::Node(glm::vec3 center, float width)
{
	mRegion.center = center;
	mRegion.width = width;
	mCenterOfMass = { center,  0.f };
}

Node::Node(glm::vec3 center, float width, std::vector<Body> bodies)
{
	mRegion.center = center;
	mRegion.width = width;
	mCenterOfMass = { center,  0.f };
	for (int i = 0; i < bodies.size(); i++)
	{
		pBodies.push_back(std::make_shared<Body>(bodies[i]));
	}
	mActive = true;
}

Node::~Node()
{

}

void Node::populate()
{
	if (pBodies.size() > 1)
	{
		mActive = true;
		mLeaf = false;
		mChildren.reserve(8);
		mChildren.push_back(Node(mRegion.getMax(), mRegion.width / 2.f));
		mChildren.push_back(Node(glm::vec3(mRegion.getMax().x, mRegion.getMax().y, mRegion.getMin().z), mRegion.width / 2.f));
		mChildren.push_back(Node(glm::vec3(mRegion.getMin().x, mRegion.getMax().y, mRegion.getMin().z), mRegion.width / 2.f));
		mChildren.push_back(Node(glm::vec3(mRegion.getMin().x, mRegion.getMax().y, mRegion.getMax().z), mRegion.width / 2.f));
		mChildren.push_back(Node(mRegion.getMin(), mRegion.width / 2.f));
		mChildren.push_back(Node(glm::vec3(mRegion.getMin().x, mRegion.getMin().y, mRegion.getMax().z), mRegion.width / 2.f));
		mChildren.push_back(Node(glm::vec3(mRegion.getMax().x, mRegion.getMin().y, mRegion.getMax().z), mRegion.width / 2.f));
		mChildren.push_back(Node(glm::vec3(mRegion.getMax().x, mRegion.getMin().y, mRegion.getMin().z), mRegion.width / 2.f));

		for (std::shared_ptr<Body>& body : pBodies)
		{
			for (Node& child : mChildren)
			{
				if (child.isInside(body))
				{
					child.addBody(body);
					break;
				}
			}
		}
		
		pBodies.erase(pBodies.begin(), pBodies.end());
		pBodies.shrink_to_fit();

		for (Node& child : mChildren)
		{
			child.populate();
		}
		
	}
	else
	{
		mLeaf = true;
		if (pBodies.empty())
		{
			mActive = false;
		}
		else
		{
			mActive = true;
		}
	}
}

bool Node::empty()
{
	return pBodies.empty();
}

bool Node::active()
{
	return mActive;
}

void Node::update()
{
	if (!mLeaf)
	{
		int i = 0;
		mCenterOfMass = { 0.f, 0.f, 0.f, 0.f };
		for (Node& child : mChildren)
		{
			if (child.active())
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
		/*if (!isInside(pBodies[0]))
		{
			pBodies.pop_back();
		}*/
		mCenterOfMass = glm::vec4(pBodies[0]->Pos(), pBodies[0]->Mass());
	}
}

void Node::calculateForce(Body& body)
{
	if ((mRegion.width) / glm::distance(body.Pos(), glm::vec3(mCenterOfMass)) < THETA || mLeaf)
	{
		body.addG(mCenterOfMass);
	}
	else
	{
		for (Node& child : mChildren)
		{
			if (child.active())
			{
				child.calculateForce(body);
			}
		}
	}
}

bool Node::isInside(std::shared_ptr<Body>& body)
{
	return glm::all(glm::lessThan(glm::abs(body->Pos() - mRegion.center), glm::vec3(mRegion.width)));
}

void Node::addBody(std::shared_ptr<Body>& body)
{
	pBodies.push_back(std::move(body));
}

void Node::moveBody(std::shared_ptr<Body>& body)
{
	if (isInside(body))
	{
		pBodies.push_back(std::move(body));
	}
	else if (!mLeaf)
	{
		for (Node& child : mChildren)
		{
			if (isInside(body))
			{
				pBodies.push_back(std::move(body));
			}
		}
	}
}