#ifndef Project_hpp
#define Project_hpp

#include <stdio.h>
#include <vector>
#include <map>
#include "Robot.h"
#include "Vector2D.h"
#include "Simulator.h"
#include <iomanip>
#include <unordered_map>
#include <assert.h>
/**
 * @class Project
 * @brief represents the project to be completed
 */
class Project {
private:
    /**< number of grid cells */
    int N;
    /**< simulator instance */
    Simulator simulator;
    /**< transition probabilities */
    double alpha;
    /**< observation probabilities */
    double epsilon;
    /**< number of observations */
    int numObservations;
    /**< number of actions */
    int numActions;
public:
    /**
     * @brief default constructor
     * @param sim simulator pointer
     */
    Project(Simulator &sim, double a, double e);
    /**
     * @brief get probability of observation in state
     * @param o observation
     * @param c Point2D representing current state state
     * @return probability of observation o in state c
     */
    double getProbOfObsInState(int o, Point2D c);
    /**
     * @brief get probability of transition
     * @param a robot action
     * @param c current state
     * @param n next state
     * @return probability of transitioning from current state to next state under action a
     */
    double getProbOfTrans(RobotAction a, Point2D c, Point2D n);
    /**
     * @brief update counters
     * @param a action
     * @param o observation
     * @param c current state
     * @param n next state
     */
    void updateCounters(RobotAction a, int o, Point2D c, Point2D n);
    /**
     * @brief get transition probability distribution over next state given current state and action a
     * @param a action
     * @param curr current state
     * @return probability P(N | curr, a)
     */
    std::vector<double> getTransitionDistribution(RobotAction a, Point2D curr);
    /**
     * @brief get probability distribution over observations
     * @param c state
     * @return distribution over observations in state c
     */
    std::vector<double> getObservationDistribution(Point2D c);
    /**
     * @brief get sampled transition probability
     * @param a action
     * @param c current state
     * @param n next state
     * @return probability estimated from experimentation P(n|c,a)
     */
    double getEstProbOfTrans(RobotAction a, Point2D c, Point2D n);
    /**
     * @brief get sampled transition probability distribution
     * @param a action
     * @param c current state
     * @return distribution (estimate) over next state
     */
    std::vector<double> getEstimatedTransitionDistribution(RobotAction a, Point2D c);
    /**
     * @brief get estimated distribution over observations in state
     * @param c current state
     * @return distribution (estimate) over observations
     */
    std::vector<double> getEstimatedObservationDistribution(Point2D c);
    /**
     * @brief get esimtated probability of observation in state
     * @param o observation
     * @param c state
     * @return distribution (estimate) over observations
     */
    double getEstObsProb(int o, Point2D c);
    /**
     * @brief validate probability estimates
     */
    void validate();
};
#endif
