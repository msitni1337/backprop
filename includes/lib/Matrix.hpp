#include <cstring>
#include <stdexcept>

namespace backprop
{
template <class T>
class Matrix
{
private:
    size_t _rows;
    size_t _cols;
    T*     _buffer;
    size_t _buffer_size;

public:
    Matrix(const size_t& rows, const size_t& cols);
    Matrix(const Matrix& m);
    Matrix& operator=(const Matrix& m);
    ~Matrix();

public:
    Matrix&  operator*(const Matrix& rhs) const;
    T&       MatrixValue(const size_t& row, const size_t& col);
    const T& MatrixValue(const size_t& row, const size_t& col) const;

private:
    void AllocateBuffer();
};
} // namespace backprop