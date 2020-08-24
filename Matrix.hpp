// another way to achieve what #pragma once does
#ifndef matrix_hpp
#define matrix_hpp
// we are going to use swap
#include <algorithm>
// for defining how to print a matrix
#include <iostream>
// we are going to store our matrices using vector
#include <vector>
// class to represent a general matrix
template<typename Type>
class Matrix{
private:
  // alias to our data type
  using Data = std::vector<Type>;
public:
  // we are going to use the same size_type as Data
  using size_type = typename Data::size_type;
  // the same goes for reference
  using reference = typename Data::reference;
private:
  // private data: elements stored linewise, number of rows and number
  // of colors
  Data      data_;
  size_type rows_;
  size_type cols_;
  // converts rectangular to linear index
  size_type index(size_type i, size_type j) const{
    return cols_ * i + j;
  }
public:
  // const references to number of rows and number of columns
  const size_type& num_rows;
  const size_type& num_cols;
  // builds a matrix with a certain number of rows and columns
  Matrix(size_type rows, size_type cols)
    : data_{}, rows_{rows}, cols_{cols}, num_rows{rows_}, num_cols{cols_}
  {
    // allocates data to the elements
    data_.resize(rows_ * cols_);
  }
  // copy constructor
  Matrix(const Matrix& m)
    : data_{m.data_}, rows_{m.rows_}, cols_{m.cols_}, num_rows{rows_}, num_cols{cols_}
  {}
  // copy assignment
  Matrix& operator=(const Matrix& m){
    data_ = m.data_;
    rows_ = m.rows_;
    cols_ = m.cols_;

    return *this;
  }
  // move constructor
  Matrix(Matrix&& m)
    : data_{std::move(m.data_)}, rows_{std::move(m.rows_)}, cols_{std::move(m.cols_)}, num_rows{rows_}, num_cols{cols_}
  {
    m.rows_ = 0;
    m.cols_ = 0;
  }
  // move assignment
  reference operator=(Matrix&& m){
    data_ = std::move(m.data_);
    rows_ = std::move(m.rows_);
    cols_ = std::move(m.cols_);

    m.rows_ = 0;
    m.cols_ = 0;

    return *this;
  }
  // provides access to element at position (i, j)
  reference at(size_type i, size_type j){
    return data_[index(i, j)];
  }
  // returns a copy of element at position (i, j)
  Type const_at(size_type i, size_type j) const{
    return data_[index(i, j)];
  }
  // prints matrix
  void print(std::ostream& out = std::cout){
    for (size_type i {0}; i < num_rows; i++){
      for (size_type j {0}; j < num_cols; j++){
        out << at(i, j) << ' ';
      }
      out << '\n';
    }
  }
  // transforms matrix into its transpose. This destructive operation
  // is done inplace
  void transpose(){
    // if matrix is square, no extra memory is needed to build its transpose
    if (num_rows == num_cols){
      for (size_type i {0}; i < num_rows; i++){
        for (size_type j {i + 1}; j < num_cols; j++){
          std::swap(at(i, j), at(j, i));
        }
      }
    }
    // if matrix is not square, some extra memory is needed to build
    // its transpose
    else{
      // auxiliar memory to temporarily hold the transpose
      Matrix<Type> t {num_cols, num_rows};
      // builds transpose
      for (size_type i {0}; i < num_rows; i++){
        for (size_type j {0}; j < num_cols; j++){
          t.at(j, i) = at(i, j);
        }
      }
      // exchanges matrix data with t data, which is about to die
      std::swap(data_, t.data_);
    }
    // exchanges number of rows and columns
    std::swap(rows_, cols_);
  }
  // nested class to represent a row of matrix
  class Row{
    // matrix reference and row index
    Matrix<Type>& parent_;
    size_type     row_index_;
  public:
    // simple constructor
    Row(Matrix<Type>& parent, size_type row_index)
      : parent_{parent}, row_index_{row_index}
    {}
    // access to position j of row using [] notation
    reference operator[](size_type j){
      return parent_.at(row_index_, j);
    }
    // assigns e to each row position
    Row& operator=(const Type& e){
      for (size_type j {0}; j < parent_.num_cols; j++){
        (*this)[j] = e;
      }

      return *this;
    }
    // position-wise assignment between rows
    Row& operator=(const Row& row){
      for (size_type j {0}; j < parent_.num_cols; j++){
        (*this)[j] = row[j];
      }

      return *this;
    }
    // exchanges row contents
    void swap(Row&& row){
      for (size_type j {0}; j < parent_.num_cols; j++){
        std::swap((*this)[j], row[j]);
      }
    }
  };
  // class to represent a column of a matrix
  class Col{
    // matrix reference and column index
    Matrix<Type>& parent_;
    size_type     col_index_;
  public:
    // simple constructor
    Col(Matrix<Type>& parent, size_type col_index)
      : parent_{parent}, col_index_{col_index}
    {}
    // access to position i of column using [] notation
    reference operator[](size_type i){
      return parent_.at(i, col_index_);
    }
    // assigns e to each column position
    Col& operator=(const Type& e){
      for (size_type i {0}; i < parent_.num_rows; i++){
        (*this)[i] = e;
      }

      return *this;
    }
    // position-wise assignment between columns
    Col& operator=(const Col& col){
      for (size_type i {0}; i < parent_.num_rows; i++){
        (*this)[i] = col[i];
      }

      return *this;
    }
    // exchanges column contents
    void swap(Col&& col){
      for (size_type i {0}; i < parent_.num_rows; i++){
        std::swap((*this)[i], col[i]);
      }
    }
  };
  // returns representation of row i
  Row row(size_type i){
    return {*this, i};
  }
  // returns representation of column j
  Col col(size_type j){
    return {*this, j};
  }
  // assigns e to each position of matrix
  Matrix& operator=(const Type& e){
    for (size_type i {0}; i < num_rows; i++){
      row(i) = e;
    }

    return *this;
  }
};
// class to represent a square matrix
template<typename Type>
class SquareMatrix : public Matrix<Type>{
  // alias to superclass
  using Mat = Matrix<Type>;
public:
  // we are going to use the same size_type as superclass
  using size_type = typename Mat::size_type;
  SquareMatrix(size_type n) : Mat{n, n}
  {}
  // assigns e to each position of matrix
  SquareMatrix& operator=(const Type& e){
    Mat::operator=(e);

    return *this;
  }
};
// class to represent an square upper triangular matrix
template<typename Type>
class UpperTriangularMatrix{
private:
  // type alias to this class type itself. This will save us some
  // typing
  using ThisType = UpperTriangularMatrix<Type>;
  // we use a matrix to hold our elements in a compact way, that is,
  // we are not going to represent the lower triangle
  using Data = Matrix<Type>;
public:
  // we use the same size_type as Data
  using size_type = typename Data::size_type;
private:
  // dimension of matrix
  size_type n_;
  // half of number rows rounded up
  size_type half_rows_;
  // this will hold our elements
  Data data_;
  // auxiliar function to determine value of half_rows_
  static size_type set_half_rows_(size_type n){
    if (n % 2 == 0){
      return n / 2;
    }
    else{
      return n / 2 + 1;
    }
  }
  // converts rectangular index related to some position not below
  // main diagonal to rectangular index in our
  // representation. Basically, we break the upper triangle in two
  // (linewise) halves: the upper half and the lower half. Then we
  // store these halves side-by-side: the lower half to the left
  // (upside down) and the upper half to the right
  std::pair<size_type, size_type> index_(size_type i, size_type j) const{
    if (i < half_rows_){
      return {i, i + 1 + j};
    }
    else{
      return {(n_ - 1) - i, (n_ - 1) - j};
    }
  }
  // every access to a lower triangle position will return this value
  static constexpr const Type default_type_value {};
  // class to represent a reference to a position in our matrix. We
  // declare it as a template to get mutable and const references
  template<typename Parent>
  class reference_base{
    // matrix which this refers to
    Parent& parent_;
    // position of reference
    size_type row_index_;
    size_type col_index_;
  public:
    // simple constructor
    reference_base(Parent& parent, size_type row_index, size_type col_index)
      : parent_{parent}, row_index_{row_index}, col_index_{col_index}
    {}
    // implicitly converts reference to Type
    operator Type() const{
      // if this refers to a upper triangle position
      if (row_index_ <= col_index_){
        // gets rectangular index in our representation
        auto[triangular_i, triangular_j] {parent_.index_(row_index_, col_index_)};
        // return content of that position
        return parent_.data_.const_at(triangular_i, triangular_j);
      }
      // if it refers to a lower triangle position
      else{
        return default_type_value;
      }
    }
    // assigns value to reference position in parent matrix
    reference_base& operator=(const Type& value){
      // if position is on upper triangle, it is mutable, so we assign
      // to it
      if (row_index_ <= col_index_){
        // get rectangular index in our representation
        auto[triangular_i, triangular_j] {parent_.index_(row_index_, col_index_)};
        // assigns to that position
        parent_.data_.at(triangular_i, triangular_j) = value;
      }
      // returns this reference
      return *this;
    }

      *this = ref;
    // assigns value of ref to reference position in parent matrix
    reference_base& operator=(const reference_base& ref){
    }
  };
public:
  // const references to number of rows and columns
  const size_type& num_rows;
  const size_type& num_cols;
  // simple constructor
  UpperTriangularMatrix(size_type n)
    : n_{n}, half_rows_{set_half_rows_(n_)}, data_{half_rows_, n_ + 1}, num_rows{n_}, num_cols{n_}
  {}
  // aliases to mutable and const references
  using reference       = reference_base<ThisType>;
  using const_reference = reference_base<const ThisType>;
  // returns mutable reference to position (i, j)
  reference at(size_type i, size_type j){
    return {*this, i, j};
  }
  // returns immutable reference to position (i, j)
  const_reference const_at(size_type i, size_type j) const{
    return {*this, i, j};
  }
  // assigns e to every upper triangle position of this matrix
  UpperTriangularMatrix& operator=(Type&& e){
    data_ = e;

    return *this;
  }
};
// a class to represent a lower triangular matrix
template<typename Type>
class LowerTriangularMatrix{
  // we use an upper triangular matrix to hold our data, and access it
  // transposing row and column positions
  using Data = UpperTriangularMatrix<Type>;
  // our data
  Data data_;
public:
  // we use the same size_type and reference as Data
  using size_type = typename Data::size_type;
  using reference = typename Data::reference;
  // const references to number of rows and columns
  size_type& num_rows;
  size_type& num_cols;
  // simple constructor
  LowerTriangularMatrix(size_type n) : data_{n}, num_rows{data_.num_rows}, num_cols{data_.num_cols}
  {}
  // returns reference to position (i, j)
  reference at(size_type i, size_type j){
    if (i >= j){
      return data_.at(j, i);
    }
    else{
      return 0;
    }
  }
};

#endif
