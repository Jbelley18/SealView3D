#ifndef SWCPARSER_H
#define SWCPARSER_H

#include <array>
#include <vector>
#include <string>
#include <iostream>

// Define the NeuronType enum to represent neuron components
enum class NeuronType {
    SOMA_TYPE = 1,     // Soma type
    AXON_TYPE = 2,     // Axon type
    DENDRITE_TYPE = 3  // Dendrite type
};

// Define a struct for NeuronNode, representing each node in the neuron
struct NeuronNode {
    int id;
    NeuronType type;  // Use NeuronType enum here
    float x, y, z, radius;
    int parent;
    std::array<float, 3> color; // Color attribute for rendering purposes
};

// Forward declaration of the operator>> function for reading NeuronType
std::istream& operator>>(std::istream& is, NeuronType& type);

class SWCParser {
public:
    explicit SWCParser(const std::string& filePath);    // Constructor with file path
    std::vector<NeuronNode> parse();                    // Method to parse SWC file

private:
    std::string filePath;  // Path to the SWC file
};

#endif // SWCPARSER_H
