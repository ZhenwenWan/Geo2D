#ifndef GEOMETRIC_STRUCTURES_H
#define GEOMETRIC_STRUCTURES_H

#include <vector>
#include <set>
#include <cmath>
#include <utility>
#include "dashLoop.h"

using namespace ew;
using namespace ew::geo2d;

EW_ENTER

namespace geo2d {

using C_Point = std::pair<double, double>;

struct C_Dash {
    C_Point start;
    C_Point end;
};

using C_Train = std::vector<C_Dash>;

struct C_Loop {
    std::vector<C_Train> trains;
};

class LoopCreator {
public:
    bool arePointsEqual(const C_Point& p1, const C_Point& p2);
    bool isConnected(const C_Train& a, const C_Train& b);
    bool isClosed(const C_Loop& loop);
    void findLoops(std::vector<C_Train>& allTrains, 
    C_Loop& currentLoop, std::vector<C_Loop>& loops, std::set<int>& usedTrainIndices);

    static C_Point adaptPointToCPoint(const std::pair<double, double>& point);
    static C_Dash adaptDashToCDash(Dash& dash);
    static C_Train adaptTrainToCTrain(dashTrain& train);
    static C_Loop adaptLoopToCLoop(dashLoop& loop);
    
    static std::pair<double, double> adaptCPointToPoint(const C_Point& c_point);
    static Dash adaptCDashToDash(const C_Dash& c_dash);
    static dashTrain adaptCTrainToTrain(const C_Train& c_train);
    static dashLoop adaptCLoopToLoop(const C_Loop& c_loop);

    std::vector<dashLoop> createLoops(std::vector<dashTrain>& trains);
};

} // namespace geo2d

EW_LEAVE

#endif // GEOMETRIC_STRUCTURES_H

