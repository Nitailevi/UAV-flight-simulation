#include "SimCmds.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>

SimCmds SimCmds::parse(const std::string& filename) {
    SimCmds sc; //create a SimCmds object
    std::ifstream file(filename); //open the file
    std::string line; //empty vessel of string
    
    //valid file check
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
        }

    //read the file line by line
    while (std::getline(file, line)) { //read the file line by line
        if (line.empty()) continue; //skip empty lines
        std::istringstream iss(line); //parse the line
        //format check
        double t, x, y;
        int id;
        if (!(iss >> t >> id >> x >> y)) {
            throw std::runtime_error("Invalid line format in SimCmds.txt: " + line);
        }

        sc.cmds.emplace_back(t, id, x, y);
    }
    std::sort(sc.cmds.begin(), sc.cmds.end(),
              [](std::tuple<double,int,double,double> t1, std::tuple<double,int,double,double> t2){ return std::get<0>(t1) < std::get<0>(t2); });
    return sc;
}
