#include "NeuralNetwork.hpp"

namespace backprop
{
template <class T>
NeuralNetwork<T>::NeuralNetwork(
    const std::vector<size_t>& Architecture, const NeuralNetworkConfig<T>& config
)
    : _Architecture(Architecture), _configuration(config)
{
    if (_Architecture.size() < 3)
        throw std::runtime_error("invalid neural network architecture: layers count must be > 2");
    if (_configuration.activation_function == NULL)
        throw std::runtime_error("invalid neural network activation function.");
    if (_configuration.activation_function_derivative == NULL)
        throw std::runtime_error("invalid neural network activation function derivative.");

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
template <class T>
NeuralNetwork<T>::NeuralNetwork(const NeuralNetwork<T>& NN)
{
    *this = NN;
}
template <class T>
NeuralNetwork<T>& NeuralNetwork<T>::operator=(const NeuralNetwork<T>& NN)
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
template <class T>
NeuralNetwork<T>::~NeuralNetwork()
{
}
template <class T>
void NeuralNetwork<T>::Train(const Matrix<T>& input, const Matrix<T>& expected_output)
{
    if (input.GetRows() != _Activations[0].GetRows())
        throw std::runtime_error("Training input rows don't match Neural Network input.");
    if (expected_output.GetRows() != _Activations[_Activations.size() - 1].GetRows())
        throw std::runtime_error(
            "Training expected_output rows don't match Neural Network output."
        );
    if (input.GetCols() != expected_output.GetCols())
        throw std::runtime_error("Each training input column must have an expected output column.");
    std::cout << "==[Initial Cost]: " << CalculateCost(input, expected_output) << std::endl;
    for (size_t i = 1; i <= _configuration.train_max_iterations; i++)
    {
        const T c = BackPropagate(input, expected_output);
        if (i % _configuration.cost_print_rate == 0)
            std::cout << "Training round [" << i << "/" << _configuration.train_max_iterations
                      << "] | Cost = " << c << " Learn rate = " << _configuration.learning_rate
                      << std::endl;
        Learn();
    }
    std::cout << "==[Final Cost]: " << CalculateCost(input, expected_output) << std::endl;
}
template <class T>
void NeuralNetwork<T>::Predict(const Matrix<T>& input, size_t col)
{
    if (input.GetRows() != _Activations[0].GetRows())
        throw std::runtime_error("Predict(): Invalid input matrix.");
    for (size_t i = 0; i < input.GetRows(); i++)
        _Activations[0].MatrixValue(i, 0) = input.MatrixValue(i, col);
    std::cout << "Input matrix:\n" << _Activations[0];
    FeedForward();
    std::cout << "Output matrix:\n" << _Activations[_Activations.size() - 1];
}
template <class T>
void NeuralNetwork<T>::Predict(const std::vector<T>& input)
{
    if (input.size() != _Activations[0].GetRows())
        throw std::runtime_error("Predict(): Invalid input vector.");
    for (size_t i = 0; i < input.size(); i++)
        _Activations[0].MatrixValue(i, 0) = input[i];
    std::cout << "Input matrix:\n" << _Activations[0];
    FeedForward();
    std::cout << "Output matrix:\n" << _Activations[_Activations.size() - 1];
}
template <class T>
const T NeuralNetwork<T>::BackPropagate(const Matrix<T>& input, const Matrix<T>& expected_output)
{
    (void) input;
    (void) expected_output;
    throw std::runtime_error("BackPropagate(): not implemented");
}
template <class T>
void NeuralNetwork<T>::Learn()
{
    for (size_t l = 0; l < _Activations.size() - 1; l++)
    {
        for (size_t row = 0; row < _Weights[l].GetRows(); row++)
        {
            for (size_t col = 0; col < _Weights[l].GetCols(); col++)
                _Weights[l].MatrixValue(row, col) -=
                    _configuration.learning_rate * _WeightsGradient[l].MatrixValue(row, col);
            _Biases[l].MatrixValue(row, 0) -=
                _configuration.learning_rate * _BiasesGradient[l].MatrixValue(row, 0);
        }
    }
}
template <class T>
const T NeuralNetwork<T>::CalculateCost(const Matrix<T>& input, const Matrix<T>& expected_output)
{
    T            total_cost   = 0;
    const size_t train_count  = input.GetCols();
    const size_t input_count  = input.GetRows();
    const size_t output_count = expected_output.GetRows();

    for (size_t n = 0; n < train_count; n++)
    {
        for (size_t i = 0; i < input_count; i++)
            _Activations[0].MatrixValue(i, 0) = input.MatrixValue(i, n);
        T cost = 0;
        FeedForward();
        for (size_t i = 0; i < output_count; i++)
        {
            const T a = _Activations[_Activations.size() - 1].MatrixValue(i, 0) -
                        expected_output.MatrixValue(i, n);
            cost += a * a;
        }
        cost /= output_count;
        total_cost += cost;
    }

    return total_cost / train_count;
}
template <class T>
void NeuralNetwork<T>::FeedForward()
{
    for (size_t i = 0; i < _Weights.size(); i++)
    {
        _Activations[i + 1] = _Weights[i] * _Activations[i];
        _Activations[i + 1] += _Biases[i];
        _configuration.activation_function(_Activations[i + 1]);
    }
}
template <class T>
void NeuralNetwork<T>::Randomize(T low, T high)
{
    for (size_t i = 0; i < _Weights.size(); i++)
    {
        _Weights[i].Randomize(low, high);
        _Biases[i].Randomize(low, high);
    }
}
template <class T>
const std::vector<size_t>& NeuralNetwork<T>::GetArchitecture() const
{
    return _Architecture;
}
template <class T>
const NeuralNetworkConfig<T>& NeuralNetwork<T>::GetConfiguration() const
{
    return _configuration;
}
template <class T>
const std::vector<Matrix<T> >& NeuralNetwork<T>::GetActivations() const
{
    return _Activations;
}
template <class T>
const std::vector<Matrix<T> >& NeuralNetwork<T>::GetWeights() const
{
    return _Weights;
}
template <class T>
const std::vector<Matrix<T> >& NeuralNetwork<T>::GetBiases() const
{
    return _Biases;
}
template <class T>
const std::vector<Matrix<T> >& NeuralNetwork<T>::GetActivationsGradient() const
{
    return _ActivationsGradient;
}
template <class T>
const std::vector<Matrix<T> >& NeuralNetwork<T>::GetWeightsGradient() const
{
    return _WeightsGradient;
}
template <class T>
const std::vector<Matrix<T> >& NeuralNetwork<T>::GetBiasesGradient() const
{
    return _BiasesGradient;
}
} // namespace backprop