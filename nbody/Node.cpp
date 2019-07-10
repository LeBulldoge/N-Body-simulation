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
}

Node::~Node()
{

}

void Node::populate()
{
	if (pBodies.size() > 1)
	{
		mChildren.erase(mChildren.begin(), mChildren.end());
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
		for (Node& child : mChildren)
		{
			child.populate();
		}
		
	}
	else
	{
		mLeaf = true;
	}
	//auto remove = std::remove_if(mChildren.begin(), mChildren.end(), [](Node& child) { return child.empty(); });
	//mChildren.erase(remove, mChildren.end());
	
	if (!pBodies.empty())
	{
		update();
	}	
}

bool Node::empty()
{
	return pBodies.empty();
}

void Node::update()
{
	if (!mLeaf)
	{
		int i = 0;
		mCenterOfMass = { 0.f, 0.f, 0.f, 0.f };
		for (Node& child : mChildren)
		{
			if (!child.empty())
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
		mCenterOfMass = glm::vec4(pBodies[0]->Pos(), pBodies[0]->Mass());
	}
}

void Node::calculateForce(Body& body)
{
	if (glm::all(glm::lessThan((mRegion.width / 2) / glm::abs(body.Pos() - glm::vec3(mCenterOfMass)), glm::vec3(THETA))) || mLeaf)
	{
		body.addG(mCenterOfMass);
	}
	else
	{
		for (Node& child : mChildren)
		{
			if (!child.empty())
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