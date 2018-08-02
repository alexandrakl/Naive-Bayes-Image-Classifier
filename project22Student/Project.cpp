#include "Project.h"
/**
* @brief default constructor
* @param sim simulator pointer
*/
Project::Project(Simulator &sim, double a, double e) {
    this->simulator = sim;
    this->alpha = a;
    this->epsilon = e;
    this->N = simulator.getHeight()*simulator.getWidth();
    this->numObservations = 5;
    this->numActions = 4;
}
/**
 * @brief get probability of observation in state
 * @param o observation
 * @param c Point2D representing current state state
 * @return probability of observation o in state c
 */
double Project::getProbOfObsInState(int o, Point2D c) {
    ///TODO: Probability of observation in state
    return 1.0/numObservations;
}
/**
 * @brief get probability of transition
 * @param a robot action
 * @param c current state
 * @param n next state
 * @return probability of transitioning from current state to next state under action a
 */
double Project::getProbOfTrans(RobotAction a, Point2D c, Point2D n) {
    ///TODO: Probabililty of transition between states
    return 1.0/N;
}
/**
 * @brief get transition probability distribution
 * @param a action
 * @param curr current state
 * @return distribution (calculuated) over next state P(N| curr, a)
 */
std::vector<double> Project::getTransitionDistribution(RobotAction a, Point2D curr) {
    std::vector<double> dist(this->N);
    for (int i=0; i<this->N; i++) {
        int r = i/simulator.getWidth();
        int c = i%simulator.getWidth();
        dist[i] = getProbOfTrans(a, curr, Point2D(r, c));
    }
    return dist;
}
/**
 * @brief get probability distribution over observations in state
 * @param c current state
 * @return distribution over observations in state c
 */
std::vector<double> Project::getObservationDistribution(Point2D c) {
    std::vector<double> dist(this->numObservations);
    for (int i=0; i<this->numObservations; i++) {dist[i] = getProbOfObsInState(i, c);}
    return dist;
}
/**
 * @brief update counters
 * @param a action
 * @param o observation
 * @param c current state
 * @param n next state
 */
void Project::updateCounters(RobotAction a, int o, Point2D c, Point2D n) {
    ///TODO: update counters for validation
}
/**
 * @brief get sampled transition probability
 * @param a action
 * @param c current state
 * @param n next state
 * @return probability estimated from experimentation P(n|c,a)
 */
double Project::getEstProbOfTrans(RobotAction a, Point2D c, Point2D n) {
    ///TODO: Get estimated probability of transition (in terms of counters)
    return 0.0;
}
/**
 * @brief get sampled transition probability distribution
 * @param a action
 * @param c current state
 * @return distribution (estimate) over next state P(N | c, a)
 */
std::vector<double> Project::getEstimatedTransitionDistribution(RobotAction a, Point2D c) {
    std::vector<double> dist(this->N);
    for (int i=0; i<this->N; i++) {
        int ri = i/simulator.getWidth();
        int ci = i%simulator.getWidth();
        dist[i] = getEstProbOfTrans(a, c, Point2D(ri, ci));
    }
    return dist;
}
/**
 * @brief get estimated probability of observation o in state c
 * @param o observation
 * @param c current state
 * @return probability estimate of observation o in state c
 */
double Project::getEstObsProb(int o, Point2D c) {
    ///TODO: Get estimated observation proability (in terms of counters)
    return 0.0;
}
/**
 * @brief get estimated distribution over observations in state c
 * @param c current state
 * @return distribution (estimate) over observations
 */
std::vector<double> Project::getEstimatedObservationDistribution(Point2D c) {
    std::vector<double> dist(this->numObservations);
    for (int i=0; i<this->numObservations; i++) {dist[i] = getEstObsProb(i, c);}
    return dist;
}
/**
 * @brief validate probability estimates
 */
void Project::validate() {
    std::vector<Point2D> hLocs = simulator.getHiddenObstacleLocations();
    double sqTransError = 0.0;
    double sqObError = 0.0;
    double numSquares = 0;
    for (int i=0; i<this->N; i++) {
        int r = i/simulator.getWidth();
        int c = i%simulator.getWidth();
        if (std::find(hLocs.begin(), hLocs.end(), Point2D(r, c)) == hLocs.end()) {
            std::vector<double> oHat = getEstimatedObservationDistribution(Point2D(r, c));
            std::vector<double> oDist = getObservationDistribution(Point2D(r, c));
            double maxOHat = oHat[std::distance(oHat.begin(), std::max_element(oHat.begin(), oHat.end()))];
            double maxO = oDist[std::distance(oDist.begin(), std::max_element(oDist.begin(), oDist.end()))];
            if (!isnan(maxOHat)) {sqObError += (maxOHat - maxO)*(maxOHat - maxO);}
            numSquares++;
            for (int j=0; j<this->numActions; j++) {
                std::vector<double> distHat = getEstimatedTransitionDistribution((RobotAction)j, Point2D(r, c));
                std::vector<double> dist = getTransitionDistribution((RobotAction)j, Point2D(r, c));
                double maxPHat = distHat[std::distance(distHat.begin(), std::max_element(distHat.begin(), distHat.end()))];
                double maxP = dist[std::distance(dist.begin(), std::max_element(dist.begin(), dist.end()))];
                if (!isnan(maxPHat)) {sqTransError += (maxPHat-maxP)*(maxPHat-maxP);}
            }
        }
    }
    //Average Square error of transitions and observations
    std::cout<<"Average transition error: "<<sqTransError/(numSquares*4)<<std::endl;
    std::cout<<"Average observation error: "<<sqObError/(numSquares)<<std::endl;
}

