#include "SimParams.hpp"
#include "SimCmds.hpp"
#include "Simulation.hpp"
#include <iostream>

int main(int argc, char* argv[]) { 
    // Check for the correct number of command line arguments
    if (argc < 3 || argc > 4) {
        std::cerr << "use: " << argv[0]
                  << " SimParams.ini SimCmds.txt [output_folder]\n";
        return 1;
    }

    const std::string paramsFile = argv[1];
    const std::string cmdsFile   = argv[2];
    const std::string outFolder  = (argc == 4 ? argv[3] : ".");

    try { // Parse simulation parameters and commands
        SimParams params = SimParams::parse(paramsFile);
        SimCmds   cmds   = SimCmds::parse(cmdsFile);

        Simulation sim(params, cmds);
        sim.run();
        sim.saveOutputs(outFolder);

    } catch (const std::exception& ex) { // Catch any exceptions thrown during parsing or simulation
        std::cerr << "Error during simulation: " << ex.what() << "\n";
        return 1;
    }

    std::cout << "Simulation completed successfully.\n";
    return 0;
}
