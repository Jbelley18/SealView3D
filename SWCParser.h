#ifndef SWCPARSER_H
#define SWCPARSER_H

#include <vector>
#include <string>

// Define a struct for NeuronNode, representing each node in the neuron
struct NeuronNode {
    int id;
    int type;
    float x, y, z, radius;
    int parent;
};

// Define soma type constant
const int SOMA_TYPE = 1;  // Replace 1 with the actual type ID for soma if different

class SWCParser {
public:
    SWCParser(const std::string& filePath);    // Constructor with file path
    std::vector<NeuronNode> parse();           // Method to parse SWC file

private:
    std::string filePath;  // Path to the SWC file
};

#endif // SWCPARSER_H
