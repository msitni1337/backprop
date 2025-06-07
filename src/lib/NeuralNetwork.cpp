#include "NeuralNetwork.hpp"

namespace backprop
{
NeuralNetwork::NeuralNetwork(const std::vector<size_t>& Architecture) : _Architecture(Architecture)
{
    if (_Architecture.size() < 3)
        throw std::runtime_error("invalid neural network: layers count must be > 2");

    // Constructing Input Layer:
    if (_Architecture[0] == 0)
        throw std::runtime_error(
            "invalid neural network's input layer: input neurons count must be > 0"
        );
    _Activations.emplace_back(_Architecture[0], 1);

    // Constructing Hidden layers, Gradient layers and output layer:
    for (size_t i = 1; i < _Architecture.size(); i++)
    {
        if (_Architecture[i] == 0)
            throw std::runtime_error("invalid neural network's layer: neuron count must be > 0");
        // Hidden Layers:
        _Activations.emplace_back(_Architecture[i], 1);
        _Weights.emplace_back(_Architecture[i], _Architecture[i - 1]);
        _Biases.emplace_back(_Architecture[i], 1);
        // Gradient Layers:
        _ActivationsGradient.emplace_back(_Architecture[i], 1);
        _WeightsGradient.emplace_back(_Architecture[i], _Architecture[i - 1]);
        _BiasesGradient.emplace_back(_Architecture[i], 1);
    }
}
NeuralNetwork::NeuralNetwork(const NeuralNetwork& NN)
{
    *this = NN;
}
NeuralNetwork& NeuralNetwork::operator=(const NeuralNetwork& NN)
{
    if (this == &NN)
        return *this;
    _Architecture        = NN._Architecture;
    _Activations         = NN._Activations;
    _Weights             = NN._Weights;
    _Biases              = NN._Biases;
    _ActivationsGradient = NN._ActivationsGradient;
    _WeightsGradient     = NN._WeightsGradient;
    _BiasesGradient      = NN._BiasesGradient;
    return *this;
}
NeuralNetwork::~NeuralNetwork() {}
} // namespace backprop