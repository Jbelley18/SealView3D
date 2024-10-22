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

    // Read each line from the SWC file
    while (std::getline(file, line)) {
        if (line[0] == '#') continue;  // Skip comments

        std::istringstream ss(line);
        NeuronNode node;
        ss >> node.id >> node.type >> node.x >> node.y >> node.z >> node.radius >> node.parent;
        nodes.push_back(node);
    }

    file.close();
    return nodes;
}
