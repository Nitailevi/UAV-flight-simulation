#pragma once
#include <string>

struct SimParams {
    double dt; //time step
    int n_uav; //number of UAVs
    double r; //radius
    double x0; //initial x position
    double y0; //initial y position
    double z0; //initial z position
    double v0; //initial velocity
    double az; //initial azimuth
    double time_lim; //time limit

    static SimParams parse(const std::string& filename); //parse the parameters from a file
};
