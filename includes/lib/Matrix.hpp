#include <cstring>
#include <iomanip>
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
    Matrix& operator*(const Matrix& rhs) const;
    Matrix& operator+=(const Matrix& rhs);

public:
    inline T&            MatrixValue(const size_t& row, const size_t& col);
    inline const T&      MatrixValue(const size_t& row, const size_t& col) const;
    inline const size_t& GetCols() const;
    inline const size_t& GetRows() const;

public:
    void Randomize(T low, T high);

private:
    void AllocateBuffer();
};

template <class T>
std::ostream& operator<<(std::ostream& out, const Matrix<T>& m);
} // namespace backprop