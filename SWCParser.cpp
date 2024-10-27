#include "SWCParser.h"
#include <fstream>
#include <sstream>
#include <iostream>

SWCParser::SWCParser(const std::string& filePath) : filePath(filePath) {}

std::vector<NeuronNode> SWCParser::parse() {
    std::vector<NeuronNode> nodes;
    std::ifstream file(filePath);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filePath << std::endl;
        return nodes;
    }

    // Read each line from the SWC file with the Structure of n T x y z R P
    while (std::getline(file, line)) {
        if (line[0] == '#') continue;  // Skip comments

        std::istringstream ss(line);
        NeuronNode node;
        int typeValue;  // Temporary variable to read type as integer
        ss >> node.id >> typeValue >> node.x >> node.y >> node.z >> node.radius >> node.parent;

        // Convert integer type to NeuronType enum
        switch (typeValue) {
            case 1: node.type = SOMA; break;
            case 2: node.type = AXON; break;
            case 3: node.type = DENDRITE; break;
            case 4: node.type = APICAL_DENDRITE; break;
            case 5: node.type = CUSTOM_TYPE_5; break;
            case 6: node.type = CUSTOM_TYPE_6; break;
            case 7: node.type = CUSTOM_TYPE_7; break;
            default:
                std::cerr << "Warning: Unknown neuron type " << typeValue << " for node ID " << node.id << std::endl;
            node.type = CUSTOM_TYPE;  // Fallback for unknown types
            break;
        }


        nodes.push_back(node);
    }

    file.close();
    return nodes;
}
