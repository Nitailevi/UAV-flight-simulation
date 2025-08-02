#include "SimParams.hpp"
#include <fstream>
#include <sstream>
#include <map>
#include <iostream>

// helper function to trim whitespace from a string
static std::string trim(const std::string& s) {
    const char* ws = " \t\n\r\f\v"; // ws contains whitespace characters, 
    size_t b = s.find_first_not_of(ws); // find the first non-whitespace character
    size_t  e = s.find_last_not_of(ws);
    
    if (b==std::string::npos){
        return ""; // if no non-whitespace character is found, return empty string
    } 
    else{
        return s.substr(b, e-b+1); // return the substring from the first to the last non-whitespace character
    }
}

SimParams SimParams::parse(const std::string& filename) { //parse the parameters from a file
    std::ifstream file(filename);
    std::string  line;
    std::map<std::string, double> vals;

    //valid file check
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
        }
    
    //map parameters to the values
    while (std::getline(file, line)) { //read the file line by line
        
        size_t pos = line.find('=');
        if (pos == std::string::npos){  
            continue; //if the line does not contain '='
        }
        std::string key = trim(line.substr(0, pos)); //get the key
        std::string val = trim(line.substr(pos + 1)); //get the value
        vals[key] = std::stod(val); //convert the value to a double and store it in the map
    }
        // Debug output : show parsed values
        // for (auto& p : vals) {
        //     std::cout << "Parsed key: '" << p.first << "' => " << p.second << "\n";
        // }
    
    //required keys check
    const std::vector<std::string> required_keys = {
    "Dt", "N_uav", "R", "X0", "Y0", "Z0", "V0", "Az", "TimeLim"
        };

    //format check
    for (const auto& key : required_keys) {
        if (vals.find(key) == vals.end()) {
         throw std::runtime_error("Missing key in SimParams.ini: " + key);
             }
        }

return SimParams{
    vals["Dt"],
    static_cast<int>(vals["N_uav"]),
    vals["R"],
    vals["X0"],
    vals["Y0"],
    vals["Z0"],
    vals["V0"],
    vals["Az"],
    vals["TimeLim"]
};

}
