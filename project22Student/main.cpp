#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <time.h>
#include <unistd.h>
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#define _USE_MATH_DEFINES
#include <cmath>
#endif
#include <vector>
#include <random>
#include <string>
#include "Vector2D.h"
#include "Robot.h"
#include "Simulator.h"
#include "Project.h"
#define SIZEX 10
#define SIZEY 10
int main(int argc, char **argv)
{
    std::random_device rd;
    std::mt19937 rng;
    int steps;  // the steps before found the target
    int waitCounter = 200; // amount to wait between steps (milliseconds)
    int numHiddenObstacles = 20; // number of hidden obstacles
    bool manualControl = true; //manual control or automatic
    bool display = true; // should display movement
    float alpha = 0.9; // transition probabilities
    float epsilon = 0.95; // observation probabilities
    int numSteps = 20000000; // number of steps to take for probability tests
    printf("\n\n*** CS360 Project Begin *** \n\n");
    int sx = -1; int sy = -1;
    if (argc==3 && (sx=std::stoi(argv[1])) && (sy=std::stoi(argv[2]))) {
        printf("Project environment size = [%d,%d]\n", sx, sy);
    } else {     
    	sx = SIZEX;       // use SIZEX for your environment
    	sy = SIZEY;       // use SIZEY for your environment
    }
    rng = std::mt19937(rd());
    Robot *r = new Robot(1);    // create your robot
    Simulator sim = Simulator(sx, sy, r, alpha, epsilon);    // create your environment
    printf("Simulator area [%d x %d] is created\n", sx, sy);
    sim.createRandomHiddenObstacles(numHiddenObstacles);
    sim.display();
    std::cout<<std::endl<<std::endl;
    steps = 0;
	Project p(sim, alpha, epsilon); // project object
    while (steps < numSteps) {
        steps++;
        std::vector<Point2D> locs = sim.getCurrentLocalObstacleLocations();
        std::vector<RobotAction> validActs {MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT};
        std::uniform_int_distribution<> randActDist(0, validActs.size() - 1);
        if (validActs.size() < 1) {std::cout<<"Invalid environment"<<std::endl; exit(1);}
        RobotAction a;
        std::string action = "";
        if (display) {
            std::cout<<"Current Location: ("<<r->getX()<<","<<r->getY()<<")"<<std::endl;
            std::cout<<"Actual Num Walls: "<<sim.getCurrentLocalObstacleLocations().size()<<std::endl;
            std::cout<<"Sensed Num Walls: "<<sim.getCurrentNumWalls()<<std::endl;
        }
        if (manualControl) {
            std::cout<<"Enter action: ";
            getline(std::cin,action);
            if (action.compare("u") == 0) a = MOVE_UP;
            else if (action.compare("d") == 0) a = MOVE_DOWN;
            else if (action.compare("l") == 0) a = MOVE_LEFT;
            else a = MOVE_RIGHT;
        } else {
            a = validActs[randActDist(rng)];
        }
        Point2D curr = r->getPosition();
        sim.step(a);
        Point2D next = r->getPosition();
        int o = sim.getCurrentNumWalls();
        //Update counters
        p.updateCounters(a, o, curr, next);
        locs = sim.getCurrentLocalObstacleLocations();
        if (display) {
            std::cout<<"Selected Action: ";
            if (a == MOVE_UP) {std::cout<<"UP"<<std::endl;}
            else if (a == MOVE_DOWN) {std::cout<<"DOWN"<<std::endl;}
            else if (a == MOVE_LEFT) {std::cout<<"LEFT"<<std::endl;}
            else {std::cout<<"RIGHT"<<std::endl;}
            sim.display();
            std::cout<<std::endl;
            std::cout<<std::endl;
        }
        if (steps % 1000 == 0) std::cout<<steps<<std::endl;
        if (!manualControl && display) {usleep(waitCounter*1000);}
    }
    //Call validation code
    p.validate();
    delete r;
}

