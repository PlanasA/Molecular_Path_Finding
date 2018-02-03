#include "YourPlanner.h"

YourPlanner::YourPlanner() :
//	RrtConCon()
        //RrtCon()
        RrtExtCon()
{
}

YourPlanner::~YourPlanner()
{
}

::std::string
YourPlanner::getName() const
{
	return "Your Planner";
}

void
YourPlanner::choose(::rl::math::Vector& chosen)
{
	//your modifications here
	//RrtConCon::choose(chosen);
	//RrtCon::choose(chosen);
	RrtExtCon::choose(chosen);
}

Rrt::Vertex 
YourPlanner::connect(Tree& tree, const Neighbor& nearest, const ::rl::math::Vector& chosen)
{
	//your modifications here
	return RrtExtCon::connect(tree, nearest, chosen);
	// RrtCon::connect(tree, nearest, chosen);
	//RrtConCon::connect(tree, nearest, chosen);
}
	
Rrt::Vertex 
YourPlanner::extend(Tree& tree, const Neighbor& nearest, const ::rl::math::Vector& chosen)
{
	//your modifications here
	return RrtExtCon::extend(tree, nearest, chosen);
	//RrtConCon::extend(tree, nearest, chosen);
	//RrtCon::extend(tree, nearest, chosen);
}

bool
YourPlanner::solve()
{
	//your modifications here
	return RrtExtCon::solve();
	//RrtConCon::solve();
	//RrtCon::solve();
}

