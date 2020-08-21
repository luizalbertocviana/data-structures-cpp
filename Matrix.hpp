#ifndef matrix_hpp
#define matrix_hpp

#include <algorithm>
#include <iostream>
#include <vector>

template<typename Type>
class Matrix{
private:
  using Data = std::vector<Type>;
public:
  using size_type = typename Data::size_type;
  using reference = typename Data::reference;
private:
  Data      data_;
  size_type rows_;
  size_type cols_;

  size_type index(size_type i, size_type j) const{
    return cols_ * i + j;
  }
public:
  const size_type& num_rows;
  const size_type& num_cols;
  
  Matrix(size_type rows, size_type cols)
    : data_{}, rows_{rows}, cols_{cols}, num_rows{rows_}, num_cols{cols_}
  {
    data_.resize(rows_ * cols_);
  }

  Matrix(const Matrix& m)
    : data_{m.data_}, rows_{m.rows_}, cols_{m.cols_}, num_rows{rows_}, num_cols{cols_}
  {}

  Matrix& operator=(const Matrix& m){
    data_ = m.data_;
    rows_ = m.rows_;
    cols_ = m.cols_;

    return *this;
  }

  Matrix(Matrix&& m)
    : data_{std::move(m.data_)}, rows_{std::move(m.rows_)}, cols_{std::move(m.cols_)}, num_rows{rows_}, num_cols{cols_}
  {
    m.rows_ = 0;
    m.cols_ = 0;
  }

  reference operator=(Matrix&& m){
    data_ = std::move(m.data_);
    rows_ = std::move(m.rows_);
    cols_ = std::move(m.cols_);

    m.rows_ = 0;
    m.cols_ = 0;

    return *this;
  }

  reference at(size_type i, size_type j){
    return data_[index(i, j)];
  }

  Type const_at(size_type i, size_type j) const{
    return data_[index(i, j)];
  }

  void print(std::ostream& out = std::cout){
    for (size_type i {0}; i < num_rows; i++){
      for (size_type j {0}; j < num_cols; j++){
        out << at(i, j) << ' ';
      }
      out << '\n';
    }
  }

  void transpose(){
    if (num_rows == num_cols){
      for (size_type i {0}; i < num_rows; i++){
        for (size_type j {i + 1}; j < num_cols; j++){
          std::swap(at(i, j), at(j, i));
        }
      }
    }
    else{
      Matrix<Type> t {num_cols, num_rows};

      for (size_type i {0}; i < num_rows; i++){
        for (size_type j {0}; j < num_cols; j++){
          t.at(j, i) = at(i, j);
        }
      }

      std::swap(data_, t.data_);
    }
    
    std::swap(rows_, cols_);
  }

  class Row{
    Matrix<Type>& parent_;
    size_type     row_index_;
  public:
    Row(Matrix<Type>& parent, size_type row_index)
      : parent_{parent}, row_index_{row_index}
    {}

    reference operator[](size_type j){
      return parent_.at(row_index_, j);
    }

    Row& operator=(const Type& e){
      for (size_type j {0}; j < parent_.num_cols; j++){
        (*this)[j] = e;
      }

      return *this;
    }

    Row& operator=(const Row& row){
      for (size_type j {0}; j < parent_.num_cols; j++){
        (*this)[j] = row[j];
      }

      return *this;
    }

    void swap(Row&& row){
      for (size_type j {0}; j < parent_.num_cols; j++){
        std::swap((*this)[j], row[j]);
      }
    }
  };

  class Col{
    Matrix<Type>& parent_;
    size_type     col_index_;
  public:
    Col(Matrix<Type>& parent, size_type col_index)
      : parent_{parent}, col_index_{col_index}
    {}

    reference operator[](size_type i){
      return parent_.at(i, col_index_);
    }

    Col& operator=(const Type& e){
      for (size_type i {0}; i < parent_.num_rows; i++){
        (*this)[i] = e;
      }

      return *this;
    }

    Col& operator=(const Col& col){
      for (size_type i {0}; i < parent_.num_rows; i++){
        (*this)[i] = col[i];
      }

      return *this;
    }

    void swap(Col&& col){
      for (size_type i {0}; i < parent_.num_rows; i++){
        std::swap((*this)[i], col[i]);
      }
    }
  };

  Row row(size_type i){
    return {*this, i};
  }

  Col col(size_type j){
    return {*this, j};
  }

  Matrix& operator=(const Type& e){
    for (size_type i {0}; i < num_rows; i++){
      row(i) = e;
    }

    return *this;
  }
};

template<typename Type>
class SquareMatrix : public Matrix<Type>{
  using Mat = Matrix<Type>;
public:
  using size_type = typename Mat::size_type;
  SquareMatrix(size_type n) : Mat{n, n}
  {}

  SquareMatrix& operator=(const Type& e){
    Mat::operator=(e);

    return *this;
  }
};

template<typename Type>
class UpperTriangularMatrix{
private:
  using ThisType = UpperTriangularMatrix<Type>;

  using Data = Matrix<Type>;
public:
  using size_type = typename Data::size_type;
private:
  size_type n_;

  size_type half_rows_;

  Data data_;

  static size_type set_half_rows_(size_type n){
    if (n % 2 == 0){
      return n / 2;
    }
    else{
      return n / 2 + 1;
    }
  }

  std::pair<size_type, size_type> index_(size_type i, size_type j) const{
    if (i < half_rows_){
      return {i, i + 1 + j};
    }
    else{
      return {(n_ - 1) - i, (n_ - 1) - j};
    }
  }

  template<typename Parent>
  class reference_base{
    static constexpr const Type default_type_value {};

    Parent& parent_;

    size_type row_index_;
    size_type col_index_;
  public:
    reference_base(Parent& parent, size_type row_index, size_type col_index)
      : parent_{parent}, row_index_{row_index}, col_index_{col_index}
    {}

    operator Type() const{
      if (row_index_ <= col_index_){
        auto[triangular_i, triangular_j] {parent_.index_(row_index_, col_index_)};

        return parent_.data_.const_at(triangular_i, triangular_j);
      }
      else{
        return default_type_value;
      }
    }

    reference_base& operator=(Type&& value){
      if (row_index_ <= col_index_){
        auto[triangular_i, triangular_j] {parent_.index_(row_index_, col_index_)};

        parent_.data_.at(triangular_i, triangular_j) = value;
      }

      return *this;
    }

    reference_base& operator=(reference_base&& ref){
      *this = ref;
    }
  };
public:
  const size_type& num_rows;
  const size_type& num_cols;

  UpperTriangularMatrix(size_type n)
    : n_{n}, half_rows_{set_half_rows_(n_)}, data_{half_rows_, n_ + 1}, num_rows{n_}, num_cols{n_}
  {}

  using reference       = reference_base<ThisType>;
  using const_reference = reference_base<const ThisType>;

  reference at(size_type i, size_type j){
    return {*this, i, j};
  }

  const_reference const_at(size_type i, size_type j) const{
    return {*this, i, j};
  }

  UpperTriangularMatrix& operator=(Type&& e){
    data_ = e;

    return *this;
  }
};

template<typename Type>
class LowerTriangularMatrix{
  using Data = UpperTriangularMatrix<Type>;

  Data data_;
public:
  using size_type = typename Data::size_type;
  using reference = typename Data::reference;

  size_type& num_rows;
  size_type& num_cols;

  LowerTriangularMatrix(size_type n) : data_{n}, num_rows{data_.num_rows}, num_cols{data_.num_cols}
  {}

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
