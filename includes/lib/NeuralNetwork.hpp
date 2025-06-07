#include "Matrix.tpp"
#include <vector>

namespace backprop
{
class NeuralNetwork
{
private:
    std::vector<size_t> _Architecture; // Each entrie specifies how many neuron in that layer.
    std::vector<Matrix<double> > _Activations;
    std::vector<Matrix<double> > _Weights;
    std::vector<Matrix<double> > _Biases;
    std::vector<Matrix<double> > _ActivationsGradient;
    std::vector<Matrix<double> > _WeightsGradient;
    std::vector<Matrix<double> > _BiasesGradient;

public:
    NeuralNetwork(const std::vector<size_t>& Architecture);
    NeuralNetwork(const NeuralNetwork& NN);
    NeuralNetwork& operator=(const NeuralNetwork& NN);
    ~NeuralNetwork();
};
} // namespace backprop