/*
 * This is a simple proof of concept that the architecture could learn
 * from data by training it to output out1 = (in1 XOR in2) and .
 */

#include "NeuralNetwork.tpp"
#include "NeuralNetworkTools.tpp"

int main()
{
    // Generating Training data:
    backprop::Matrix<double> input(2, 4);
    backprop::Matrix<double> expected_output(2, 4);
    for (size_t a = 0; a < 2; a++)
    {
        for (size_t b = 0; b < 2; b++)
        {
            input.MatrixValue(0, a * 2 + b)           = a;
            input.MatrixValue(1, a * 2 + b)           = b;
            expected_output.MatrixValue(0, a * 2 + b) = a ^ b;
            expected_output.MatrixValue(1, a * 2 + b) = a & b;
        }
    }

    std::vector<size_t>                   Architecture({2, 4, 2, 7, 1});
    backprop::NeuralNetworkConfig<double> configuration;
    configuration.learning_rate                  = 0.1;
    configuration.cost_print_rate                = 1000;
    configuration.train_max_iterations           = 500000;
    configuration.activation_function            = backprop::sigmoid_function<double>;
    configuration.activation_function_derivative = backprop::sigmoid_function_derivitive<double>;
    backprop::NeuralNetwork nn(Architecture, configuration);
    nn.Randomize(-1, 1);
    std::cout << "Training Input Matrix:\n" << input;
    std::cout << "Training Expected Output Matrix:\n" << expected_output;
    std::cout << nn;
    // nn.Train(input, expected_output);
    std::cout << "Neural network prediction:" << std::endl;
    for (size_t col = 0; col < input.GetCols(); col++)
        nn.Predict(input, col);
}
