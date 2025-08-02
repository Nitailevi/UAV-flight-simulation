#include "Simulation.hpp"

Simulation::Simulation(const SimParams& p, const SimCmds& c) //constructor 
  : params(p), cmds(c), time(0.0), nextCmd(0)
{
    //create n_uav UAVs with initial position, velocity, and target
    for (int i = 0; i < params.n_uav; ++i) {
        uavs.emplace_back(i, params.x0, params.y0, params.z0, //construct the new element directly in place
                          params.v0, params.az, params.r);
    }
}

void Simulation::run() {
    while (time <= params.time_lim) {
        //process commands that are due at the current time
        //nextCmd is the index of the next command to process
        while (nextCmd < cmds.cmds.size() &&
               std::get<0>(cmds.cmds[nextCmd]) <= time)
        {
            auto [t, id, tx, ty] = cmds.cmds[nextCmd++];
            uavs[id].setTarget(tx, ty);
        }
        //update each UAV's position
   
        for (auto& u : uavs) { 
            u.update(params.dt, time);
        }
        time += params.dt;
    }
}

void Simulation::saveOutputs(const std::string& folder) {
    for (auto& u : uavs) {
        u.saveHistory(folder);
    }
}
