#pragma once
#include <vector>
#include <tuple>
#include <string>
#include <utility> 
using namespace std;

class UAV {
public:
    UAV(int id, double x0, double y0, double z0, double v0, double az0, double turnR); //constructor
    void setTarget(double x, double y); //set the target position
    void update(double dt, double currentTime); //update the UAV's position 
    void saveHistory(const std::string& folder) const; //save the UAV's history

private:
    int id;
    double x, y, z, v, az, turnRadius; //position, velocity, azimuth, turn radius
    std::pair<double,double> target; //target position
    std::vector<std::tuple<double,double,double,double>> history; //history of: time x cord y cord azimuth
    // State enum to represent the UAV's state
    enum class Mode { FLYING, AWAITING };
    Mode mode = Mode:: FLYING;
    double loopPos = 0.0;
};


