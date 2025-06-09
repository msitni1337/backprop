#pragma once
#include "Matrix.tpp"
#include <iostream>
#include <vector>

namespace backprop
{

template <class T>
struct NeuralNetworkConfig
{
    size_t cost_print_rate                              = 100;
    size_t train_max_iterations                         = 1000;
    double learning_rate                                = 0.001;
    void (*activation_function)(Matrix<T>&)             = NULL;
    const T (*activation_function_derivative)(const T&) = NULL;
};

template <class T>
class NeuralNetwork
{
private:
    std::vector<size_t>    _Architecture; // Each entrie specifies how many neuron in that layer.
    NeuralNetworkConfig<T> _configuration;

private: // State matrices:
    std::vector<Matrix<T> > _Activations;
    std::vector<Matrix<T> > _Weights;
    std::vector<Matrix<T> > _Biases;

private: // Gradient matrices:
    std::vector<Matrix<T> > _ActivationsGradient;
    std::vector<Matrix<T> > _WeightsGradient;
    std::vector<Matrix<T> > _BiasesGradient;

public:
    NeuralNetwork(const std::vector<size_t>& Architecture, const NeuralNetworkConfig<T>& config);
    NeuralNetwork(const NeuralNetwork& NN);
    NeuralNetwork& operator=(const NeuralNetwork& NN);
    ~NeuralNetwork();

public:
    void    Train(const Matrix<T>& input, const Matrix<T>& expected_output);
    void    Predict(const Matrix<T>& input, size_t col);
    void    Predict(const std::vector<T>& input);
    const T CalculateCost(const Matrix<T>& input, const Matrix<T>& expected_output);
    void    Randomize(T low, T high);

public:
    const std::vector<size_t>&     GetArchitecture() const;
    const NeuralNetworkConfig<T>&  GetConfiguration() const;
    const std::vector<Matrix<T> >& GetActivations() const;
    const std::vector<Matrix<T> >& GetWeights() const;
    const std::vector<Matrix<T> >& GetBiases() const;
    const std::vector<Matrix<T> >& GetActivationsGradient() const;
    const std::vector<Matrix<T> >& GetWeightsGradient() const;
    const std::vector<Matrix<T> >& GetBiasesGradient() const;

private:
    const T BackPropagate(const Matrix<T>& input, const Matrix<T>& expected_output);
    void    FeedForward();
    void    Learn();
};

template <class T>
std::ostream& operator<<(std::ostream& out, const NeuralNetwork<T>& nn);

template <class T>
void sigmoid_function(Matrix<T>& m);
template <class T>
const T sigmoid_function_derivitive(const T& v);
} // namespace backprop