#pragma once
#include "TDNLIB.h"
#include "AI\Graph\Graph.h"


class Action_Node : public NodeBase
{
public:
	//Goal* goal;
	//Action_Node(Goal* goal) : NodeBase(), goal(goal) {}
	Action_Node() : NodeBase() {}
};


class Action_Edge : public EdgeBase
{
public:
	Action_Edge() :EdgeBase() {}
};
