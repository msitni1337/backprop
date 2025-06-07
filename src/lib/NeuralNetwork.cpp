#include "NeuralNetwork.hpp"

namespace backprop
{
NeuralNetwork::NeuralNetwork(
    const std::vector<size_t>& Architecture, const NeuralNetworkConfig<double>& config
)
    : _Architecture(Architecture), _configuration(config)
{
    if (_Architecture.size() < 3)
        throw std::runtime_error("invalid neural network architecture: layers count must be > 2");
    if (_configuration.activation_function == NULL)
        throw std::runtime_error("invalid neural network activation function.");

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
void NeuralNetwork::Train(const Matrix<double>& input, const Matrix<double>& expected_output)
{
    throw std::runtime_error("Not fully implemented");
    if (input.GetRows() != _Activations[0].GetRows())
        throw std::runtime_error("Training input rows don't match Neural Network input.");
    if (expected_output.GetRows() != _Activations[_Activations.size() - 1].GetRows())
        throw std::runtime_error(
            "Training expected_output rows don't match Neural Network output."
        );
    std::cout << "==[Initial Cost]: " << CalculateCost(input, expected_output);
    // Loop through data: do back prop to calculate gradient and apply it using the learn rate.
    std::cout << "==[Final Cost]: " << CalculateCost(input, expected_output);
}
double NeuralNetwork::CalculateCost(
    const Matrix<double>& input, const Matrix<double>& expected_output
)
{
    double       total_cost   = 0;
    const size_t train_count  = input.GetCols();
    const size_t input_count  = input.GetRows();
    const size_t output_count = expected_output.GetRows();

    for (size_t n = 0; n < train_count; n++)
    {
        for (size_t i = 0; i < input_count; i++)
            _Activations[0].MatrixValue(i, 0) = input.MatrixValue(i, n);
        double cost = 0;
        FeedForward();
        for (size_t i = 0; i < output_count; i++)
        {
            double a = _Activations[_Activations.size() - 1].MatrixValue(i, 0) -
                       expected_output.MatrixValue(i, n);
            cost += a * a;
        }
        cost /= output_count;
        total_cost += cost;
    }

    return total_cost / train_count;
}
void NeuralNetwork::FeedForward()
{
    for (size_t i = 0; i < _Weights.size(); i++)
    {
        _Activations[i + 1] = _Weights[i] * _Activations[i];
        _Activations[i + 1] += _Biases[i];
        _configuration.activation_function(_Activations[i + 1]);
    }
}
void NeuralNetwork::Randomize(double low, double high)
{
    for (size_t i = 0; i < _Weights.size(); i++)
    {
        _Weights[i].Randomize(low, high);
        _Biases[i].Randomize(low, high);
    }
}
} // namespace backprop