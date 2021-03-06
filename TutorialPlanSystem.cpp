
#include <fstream>
#include <QDateTime>
#include "TutorialPlanSystem.h"
#include "rl/math/Unit.h"
#include "rl/math/Rotation.h"
#include "rl/plan/UniformSampler.h"

TutorialPlanSystem::TutorialPlanSystem()
{
//  Loading the scene from an predefined xml file which contains the convex model of the robot as well as the sourroundings
//  Here's the collision scene where the puma 560 is loaded.
    rl::sg::bullet::Scene* scene = new rl::sg::bullet::Scene();
    scene->load("xml/rlsg/1enu_scene.xml");
    rl::sg::bullet::Model* sceneModel = static_cast< rl::sg::bullet::Model* > (scene->getModel(0));

//  Loading the kinematics of the puma 560 from a predefined xml file
    rl::kin::Kinematics* kinematics = rl::kin::Kinematics::create("xml/rlkin/1enu_kinematics.xml");

//  Adding the robot kinematics and the scene to our internal model
    this->model.kin   = kinematics;
    this->model.model = sceneModel;
    this->model.scene = scene;

//  Setting the start, goal and current position
    this->start.resize(kinematics->getDof());
    this->goal.resize(kinematics->getDof());
    this->q.resize(kinematics->getDof());

//  Start (a little displacement because the orgonal configuration collides):
    this->start <<  -0.4,0.1,0,0,0,0;

//  Goal:
    this->goal <<   20,
                    20,
					20,
                    0  * rl::math::DEG2RAD,
                    0 * rl::math::DEG2RAD,
		    		0  * rl::math::DEG2RAD;

//  Current position is at the start
    this->q = this->start;

//  Set the model of the sampler to the system model
    this->sampler.model = &this->model;

//  --- Parametrize the planner ---
//  Delta defines the configuration step width of a connect attempt.
//  Here delta is set to 1° => 1° steps are checked during a connect.
    this->planner.delta = 1 * rl::math::DEG2RAD;

//  Epsilon defines the distance between two configurations at which they are
//  just identified as being identical.
    this->planner.epsilon = 1.0e-8f;

//  If kd is set to true, the rrt alogrithm will use a kd-tree for the nearest neighbor
//   search instead of brute-force.
    this->planner.kd = true;

//  duration defines the time interval in which the planner tries to solve the problem.
//  Here the planner stops after 300 seconds of finding no solution.
    this->planner.duration = 300; //[s]

//  Setting the start and the goal position of the planner.
    this->planner.goal = &this->goal;
    this->planner.start = &this->start;

//  Set the sampler and the model of the planner.
    this->planner.sampler = &this->sampler;
    this->planner.model = &this->model;    
}

TutorialPlanSystem::~TutorialPlanSystem()
{
    //Free used memory
    delete this->model.kin;
    delete this->model.model;
    delete this->model.scene;
}

void TutorialPlanSystem::getRandomConfiguration(rl::math::Vector & config)
{
//  By calling generate the sampler returns a random configuration
//  of the robot in our system model
    sampler.generate(config);
}

void TutorialPlanSystem::getRandomFreeConfiguration(rl::math::Vector & config)
{
    //  By calling generateCollisionFree the sampler returns a random configuration
    //  of the robot in our system model where the robot does not collide with it's .
    //  surroundings
    sampler.generateCollisionFree(config);
}

bool TutorialPlanSystem::plan(rl::plan::VectorList & path)
{
    rl::util::Timer timer;

    //Verifies that the model, the start and the goal position are all correct
    if (!this->planner.verify())
    {
            std::cout << "start or goal invalid" << std::endl;
            return false;
    }

    //Call the planner to solve the current problem.
    std::cout << "solve() ... " << std::endl;;
    timer.start();
    bool solved = this->planner.solve();
    timer.stop();
    std::cout << "solve() " << (solved ? "true" : "false") << " " << timer.elapsed() * 1000.0f << " ms" << std::endl;
    
    std::ofstream benchmark;
    benchmark.open("benchmark.csv", std::ios::app);
    benchmark << QDateTime::currentDateTime().toString("yyyy-MM-dd,HH:mm:ss.zzz").toStdString();
    benchmark << ",";
    benchmark << (solved ? "true" : "false");
	benchmark << ",";
	benchmark << this->planner.getName();
	benchmark << ",";
	benchmark << this->planner.getNumVertices();
	benchmark << ",";
	benchmark << this->model.getTotalQueries();
	benchmark << ",";
	benchmark << this->model.getFreeQueries();
	benchmark << ",";
	benchmark << timer.elapsed();
	benchmark << std::endl;
    if(solved)
    {
		this->planner.getPath(path);
	}

    return solved;
}

void TutorialPlanSystem::reset()
{
    //Reset the planner and the model
    this->planner.reset();
    this->model.reset();
}

