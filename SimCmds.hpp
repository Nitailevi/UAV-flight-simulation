#pragma once
#include <string>
#include <vector>
#include <tuple>

struct SimCmds {
    std::vector<std::tuple<double,int,double,double>> cmds; //time, id, target x, target y
    static SimCmds parse(const std::string& filename); //parse the commands from a file
};

