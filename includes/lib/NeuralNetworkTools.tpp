#include "NeuralNetwork.hpp"
#include <cmath>

namespace backprop
{
template <class T>
std::ostream& operator<<(std::ostream& out, const NeuralNetwork<T>& nn)
{
    out << "====================Neural Network Architect   =================\n";
    const std::vector<size_t>& arch = nn.GetArchitecture();
    out << "{ ";
    for (size_t i = 0; i < arch.size(); i++)
        out << arch[i] << ((i == arch.size() - 1) ? "" : ", ");
    out << " }\n";
    out << "====================Neural Network Structure   =================\n";
    {
        const std::vector<Matrix<T> >& activations = nn.GetActivations();
        const std::vector<Matrix<T> >& weights     = nn.GetWeights();
        const std::vector<Matrix<T> >& biases      = nn.GetBiases();
        out << "** Inputs Matrix **\n";
        out << activations[0];
        for (size_t l = 1; l < activations.size(); l++)
        {
            out << "\n[Layer " << l << "]:\n* Weights Matrix:\n";
            out << weights[l - 1];
            printf("* Biases Matrix:\n");
            out << biases[l - 1];
            if (l == activations.size() - 1)
                out << "** Output Matrix **\n";
            else
                out << "*Activation Matrix:\n";
            out << activations[l];
        }
    }
    out << "====================Neural Network Gradient   =================\n";
    {
        const std::vector<Matrix<T> >& activations_gradient = nn.GetActivationsGradient();
        const std::vector<Matrix<T> >& weights_gradient     = nn.GetWeightsGradient();
        const std::vector<Matrix<T> >& biases_gradient      = nn.GetBiasesGradient();
        for (size_t l = 0; l < activations_gradient.size(); l++)
        {
            out << "\n[Layer " << l << "]:\n* Weights Matrix:\n";
            out << weights_gradient[l];
            printf("* Biases Matrix:\n");
            out << biases_gradient[l];
            out << "*Activation Matrix:\n";
            out << activations_gradient[l];
        }
    }
    out << "================================================================" << std::endl;
    return out;
}

template <class T>
void sigmoid_function(Matrix<T>& m)
{
    for (size_t row = 0; row < m.GetRows(); row++)
        m.MatrixValue(row, 0) = 1.0 / (1.0 + std::exp(-m.MatrixValue(row, 0)));
}

template <class T>
const T sigmoid_function_derivitive(const T& v)
{
    return v * (1 - v);
}
} // namespace backprop