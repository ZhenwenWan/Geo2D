#include "geometric_structures.h"

EW_ENTER

namespace geo2d {

double computeDistance(const C_Point& p1, const C_Point& p2) {
    double dx = p1.first - p2.first;
    double dy = p1.second - p2.second;
    return std::sqrt(dx * dx + dy * dy);
}

bool LoopCreator::arePointsEqual(const C_Point& p1, const C_Point& p2) {
    return computeDistance(p1, p2) < MiniDistance;
}

bool LoopCreator::isConnected(const C_Train& a, const C_Train& b) {
    return arePointsEqual(a.back().end, b.front().start);
}

bool LoopCreator::isClosed(const C_Loop& loop) {
    return arePointsEqual(loop.trains.front().front().start, loop.trains.back().back().end);
}

void LoopCreator::findLoops(std::vector<C_Train>& allTrains, C_Loop& currentLoop, std::vector<C_Loop>& loops, std::set<int>& usedTrainIndices) {
    for (size_t i = 0; i < allTrains.size(); ++i) {
        if (usedTrainIndices.find(i) == usedTrainIndices.end() && 
            isConnected(currentLoop.trains.back(), allTrains[i])) {
            currentLoop.trains.push_back(allTrains[i]);
            usedTrainIndices.insert(i);
            
            if (isClosed(currentLoop)) {
                loops.push_back(currentLoop);
                return;
            } else {
                bool Yes = true;
                for(int j = 1; j < currentLoop.trains.size()-1; j++) 
                    if(isConnected(allTrains[i],currentLoop.trains[j])) Yes = false; 
                if(Yes) findLoops(allTrains, currentLoop, loops, usedTrainIndices);
            }
            currentLoop.trains.pop_back();
            usedTrainIndices.erase(i);
        }
    }
}

C_Point LoopCreator::adaptPointToCPoint(const std::pair<double, double>& point) {
    return point; 
}

C_Dash LoopCreator::adaptDashToCDash(Dash& dash) {
	dash.start  = {dash.P1[0], dash.P1[1]};
	dash.end    = {dash.P2[0], dash.P2[1]};
    return {adaptPointToCPoint(dash.start), adaptPointToCPoint(dash.end)};
}

C_Train LoopCreator::adaptTrainToCTrain(dashTrain& train) {
    C_Train c_train;
    train.dashes = train.getDashes();
    for (auto& dash : train.dashes) {
        c_train.push_back(adaptDashToCDash(dash));
    }
    return c_train;
}

C_Loop LoopCreator::adaptLoopToCLoop(dashLoop& loop) {
    C_Loop c_loop;
    loop.trains = loop.getDashTrains();
    for (auto& train : loop.trains) {
        c_loop.trains.push_back(adaptTrainToCTrain(train));
    }
    return c_loop;
}

std::pair<double, double> LoopCreator::adaptCPointToPoint(const C_Point& c_point) {
    return c_point;
}

Dash LoopCreator::adaptCDashToDash(const C_Dash& c_dash) {
    auto start = adaptCPointToPoint(c_dash.start);
    auto end = adaptCPointToPoint(c_dash.end);
    auto P1 = vec2d(start.first,start.second);
    auto P2 = vec2d(end.first,end.second);
    Dash dash(P1,P2);
    dash.start = start;
    dash.end   = end;
    return dash;
}

dashTrain LoopCreator::adaptCTrainToTrain(const C_Train& c_train) {
    dashTrain trainA;
    for (const auto& c_dash : c_train) {
        trainA.dashes.push_back(adaptCDashToDash(c_dash));
    }
    dashTrain trainB(trainA.dashes);
    trainB.dashes = trainA.dashes;
    return trainB;
}

dashLoop LoopCreator::adaptCLoopToLoop(const C_Loop& c_loop) {
    dashLoop loopA;
    for (const auto& c_train : c_loop.trains) {
        loopA.trains.push_back(adaptCTrainToTrain(c_train));
    }
    dashLoop loopB(loopA.trains);
    loopB.trains = loopA.trains;
    return loopB;
}

std::vector<dashLoop> LoopCreator::createLoops(std::vector<dashTrain>& trains) {
    std::vector<C_Train> c_trains;
    for (auto& train : trains) {
        c_trains.push_back(adaptTrainToCTrain(train));
    }

    std::set<int> usedTrainIndices;
    std::vector<C_Loop> c_loops;
    for (size_t i = 0; i < c_trains.size(); ++i) {
        if(usedTrainIndices.find(i)==usedTrainIndices.end()) {
            C_Loop currentLoop;
            usedTrainIndices.insert(i);
            currentLoop.trains.push_back(c_trains[i]);
            findLoops(c_trains, currentLoop, c_loops, usedTrainIndices);
        };
    }

    std::vector<dashLoop> loops;
    for (const auto& c_loop : c_loops) {
        loops.push_back(adaptCLoopToLoop(c_loop));
    }

    return loops;
}

} // namespace geo2d

EW_LEAVE
