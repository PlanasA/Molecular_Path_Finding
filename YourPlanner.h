#ifndef _YOUR_PLANNER_H_
#define _YOUR_PLANNER_H_

//#include <rl/plan/RrtConCon.h>
//#include <rl/plan/RrtCon.h>
#include <rl/plan/RrtExtCon.h>

using namespace ::rl::plan;

/**
*	The implementation of your planner.
*	modify any of the existing methods to improve planning performance.
*/
//class YourPlanner : public RrtConCon
//class YourPlanner : public RrtCon
class YourPlanner : public RrtExtCon
{
public:
	YourPlanner();
	
	virtual ~YourPlanner();
	
	virtual ::std::string getName() const;

	bool solve();
	
protected:
	void choose(::rl::math::Vector& chosen);

	Vertex connect(Tree& tree, const Neighbor& nearest, const ::rl::math::Vector& chosen);

	Vertex extend(Tree& tree, const Neighbor& nearest, const ::rl::math::Vector& chosen);
	
private:
	
};

#endif // _YOUR_PLANNER_H_
