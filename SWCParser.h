#ifndef SWCPARSER_H
#define SWCPARSER_H

#include <vector>
#include <string>

// Enum for Neuron types (instead of using raw integers)
enum NeuronType {
    SOMA = 1,
    AXON = 2,
    DENDRITE = 3,
    APICAL_DENDRITE = 4,
    CUSTOM_TYPE_5 = 5,
    CUSTOM_TYPE_6 = 6,
    CUSTOM_TYPE_7 = 7,   // Custom type for nodes 4-26
    CUSTOM_TYPE = -1
};


// Define a struct for NeuronNode, representing each node in the neuron
struct NeuronNode {
    int id;
    NeuronType type;  // Use enum for neuron type
    float x, y, z, radius;
    int parent;
};

class SWCParser {
public:
    SWCParser(const std::string& filePath);    // Constructor with file path
    std::vector<NeuronNode> parse();           // Method to parse SWC file

private:
    std::string filePath;  // Path to the SWC file
};

#endif // SWCPARSER_H
