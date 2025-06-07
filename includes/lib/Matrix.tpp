#include "Matrix.hpp"

namespace backprop
{
template <class T>
Matrix<T>::Matrix(const size_t& rows, const size_t& cols) : _rows(rows), _cols(cols), _buffer(NULL)
{
    if (_rows == 0 || _cols == 0)
        throw std::runtime_error("invalid matrix dimensions");
    AllocateBuffer();
}
template <class T>
Matrix<T>::Matrix(const Matrix& m) : _rows(m._rows), _cols(m._cols), _buffer(NULL)
{
    AllocateBuffer();
    *this = m;
}
template <class T>
Matrix<T>& Matrix<T>::operator=(const Matrix& m)
{
    if (this == &m)
        return *this;
    _rows = m._rows;
    _cols = m._cols;
    if (_buffer_size < _rows * _cols)
        AllocateBuffer();
    std::memcpy(_buffer, m._buffer, sizeof(T) * _buffer_size);
    return *this;
}
template <class T>
Matrix<T>::~Matrix()
{
    delete[] _buffer;
}
template <class T>
void Matrix<T>::AllocateBuffer()
{
    delete[] _buffer;
    _buffer_size = _rows * _cols;
    _buffer      = new T[_buffer_size];
}
template <class T>
Matrix<T>& Matrix<T>::operator*(const Matrix& rhs) const
{
    if (_cols != rhs._rows)
        throw std::runtime_error("Invalid matrix dot product: this->cols dont match rhs.rows");
    Matrix result(_rows, rhs._cols);
    for (size_t row = 0; row < result.rows; row++)
    {
        for (size_t col = 0; col < result._cols; col++)
        {
            T& value = result.MatrixValue(row, col);
            for (size_t i = 0; i < _cols; i++)
                *value += MatrixValue(row, i) * rhs.MatrixValue(i, col);
        }
    }
    return result;
}
template <class T>
Matrix<T>& Matrix<T>::operator+=(const Matrix& rhs)
{
    if (_rows != rhs._rows || _cols != rhs._cols)
        throw std::runtime_error("Invalid matrix sum: matrices have different dimensions");
    for (size_t row = 0; row < _rows; row++)
        for (size_t col = 0; col < _cols; col++)
            MatrixValue(row, col) += rhs.MatrixValue(row, col);
    return *this;
}
template <class T>
T& Matrix<T>::MatrixValue(const size_t& row, const size_t& col)
{
    return _buffer[(row * _cols) + col];
}
template <class T>
const T& Matrix<T>::MatrixValue(const size_t& row, const size_t& col) const
{
    return _buffer[(row * _cols) + col];
}
template <class T>
const size_t& Matrix<T>::GetCols() const
{
    return _cols;
}
template <class T>
const size_t& Matrix<T>::GetRows() const
{
    return _rows;
}
template <class T>
void Matrix<T>::Randomize(T low, T high)
{
    for (size_t row = 0; row < _rows; row++)
        for (size_t col = 0; col < _cols; col++)
            MatrixValue(row, col) = ((T)rand() / RAND_MAX) * (high - low) + low;
}

template <class T>
std::ostream& operator<<(std::ostream& out, const Matrix<T>& m)
{
    const size_t& rows = m.GetRows();
    const size_t& cols = m.GetCols();
    out << "======\n";
    for (size_t row = 0; row < rows; row++)
    {
        out << "#  ";
        for (size_t col = 0; col < cols; col++)
            out << std::setprecision(6) << m.MatrixValue(row, col) << "  ";
        out << "  #\n";
    }
    out << "======" << std::endl;
    return out;
}
} // namespace backprop