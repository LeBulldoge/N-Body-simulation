#include "OctTree.h"

OctTree::OctTree()
{
	bodies = NULL;
}

OctTree::OctTree(const BoundingBox box, Body* bods)
{
	region = box;
	bodies = bods;
}

OctTree::OctTree(const BoundingBox box)
{
	region = box;
	bodies = NULL;
}
OctTree::~OctTree()
{
	delete children;
}