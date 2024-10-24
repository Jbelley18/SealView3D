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

        // Assign colors based on the node type
        switch (node.type) {
            case NeuronType::SOMA_TYPE:
                node.color = {0.68f, 0.85f, 0.90f}; // Light blue for soma
            break;
            case NeuronType::AXON_TYPE:
                node.color = {0.68f, 1.0f, 0.68f}; // Light green for axons
            break;
            case NeuronType::DENDRITE_TYPE:
                node.color = {1.0f, 0.68f, 0.68f}; // Light red for dendrites
            break;
            default:
                node.color = {0.5f, 0.5f, 0.5f}; // Gray for unclassified or other types
            break;
        }

        nodes.push_back(node);
    }

    file.close();
    return nodes;
}

// Define the >> operator for NeuronType
std::istream& operator>>(std::istream& is, NeuronType& type) {
    int value;
    is >> value;
    type = static_cast<NeuronType>(value);
    return is;
}
