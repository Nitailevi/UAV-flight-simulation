#pragma once
#include "SimParams.hpp"
#include "SimCmds.hpp"
#include "UAV.hpp"
#include <vector>
#include <string>

class Simulation {
public:
    Simulation(const SimParams& params, const SimCmds& cmds); //constructor
    void run();
    void saveOutputs(const std::string& folder = "."); //"." is the current directory

private:
    SimParams params;
    SimCmds cmds;
    double time;
    size_t nextCmd; //marks next command to operate in command list
    std::vector<UAV> uavs;
};
