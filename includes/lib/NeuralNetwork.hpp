#include "Matrix.tpp"
#include <iostream>
#include <vector>

namespace backprop
{

template <class T>
struct NeuralNetworkConfig
{
    size_t cost_print_rate      = 100;
    size_t train_max_iterations = 1000;
    double learning_rate        = 0.001;
    void (*activation_function)(Matrix<T>&);
};

class NeuralNetwork
{
private:
    std::vector<size_t> _Architecture; // Each entrie specifies how many neuron in that layer.
    NeuralNetworkConfig<double> _configuration;

private: // State matrices:
    std::vector<Matrix<double> > _Activations;
    std::vector<Matrix<double> > _Weights;
    std::vector<Matrix<double> > _Biases;

private: // Gradient matrices:
    std::vector<Matrix<double> > _ActivationsGradient;
    std::vector<Matrix<double> > _WeightsGradient;
    std::vector<Matrix<double> > _BiasesGradient;

public:
    NeuralNetwork(
        const std::vector<size_t>& Architecture, const NeuralNetworkConfig<double>& config
    );
    NeuralNetwork(const NeuralNetwork& NN);
    NeuralNetwork& operator=(const NeuralNetwork& NN);
    ~NeuralNetwork();

public:
    void   Train(const Matrix<double>& input, const Matrix<double>& expected_output);
    double CalculateCost(const Matrix<double>& input, const Matrix<double>& expected_output);
    void   FeedForward();
    void   Randomize(double low, double high);
};
} // namespace backprop