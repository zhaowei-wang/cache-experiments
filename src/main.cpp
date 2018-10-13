#include <iostream>
#include <algorithm>
#include <memory>
#include <vector>
#include <initializer_list>
#include <sys/time.h>

#define N 3000

uint64_t get_time()
{
  timeval tv;
  gettimeofday(&tv, nullptr);
  return tv.tv_sec * 1000000 + tv.tv_usec;
}

template <class T>
struct mat
{
  mat(size_t nrow, size_t ncol) : nrow_(nrow), ncol_(ncol)
  {
    vv_ = std::vector<std::vector<T>>(nrow_, std::vector<T>(ncol_, 0));
  }

  mat(std::initializer_list<std::initializer_list<T>> lists)
  {
    for (const auto& l : lists)
      vv_.push_back(l);
    
    nrow_ = vv_.size();
    ncol_ = vv_.at(0).size();
  }

  mat(const mat &m)
  {
    vv_ = m.vv_;
    nrow_ = m.nrow_;
    ncol_ = m.ncol_;
  }

  mat& operator=(mat m)
  {
    std::swap(vv_, m.vv_);
    std::swap(nrow_, m.nrow_);
    std::swap(ncol_, m.ncol_);
    return *this;
  }

  std::vector<T>& operator[](size_t i)
  {
    return vv_[i];
  }

  size_t nrow() { return nrow_; }
  size_t ncol() { return ncol_; }

  using matrix_t = std::vector<std::vector<T>>;  
  typename matrix_t::iterator begin() { return vv_.begin(); }
  typename matrix_t::iterator end()   { return vv_.end();   }
protected:
  std::vector<std::vector<T>> vv_;
  size_t nrow_;
  size_t ncol_;
};

// shows the delta in performance when memory is written non-sequentially
// (bad spatial locality)
template <class T>
void matrix_initialize_test(mat<T> &a, mat<T> &b)
{
  std::cout << "begin allocation" << std::endl;
  
  uint64_t t1 = get_time();
  for (size_t i = 0; i < N; ++i)
    for (size_t j = 0; j < N; ++j)
      a[i][j] = 0;
  uint64_t t2 = get_time();
  std::cout << "Inner loop: col time = " << ((float)(t2-t1)) / 1000000.0 << std::endl;

  t1 = get_time();
  for (size_t i = 0; i < N; ++i)
    for (size_t j = 0; j < N; ++j)
      b[j][i] = 0;
  t2 = get_time();
  std::cout << "Inner loop: row time = " << ((float)(t2-t1)) / 1000000.0 << std::endl;

}

/*
matrix multiplication tests
 */
template <class T>
mat<T> mmult_bad(mat<T> a, mat<T> b)
{
  if (a.ncol() != b.nrow())
    throw std::invalid_argument("mmult : invalid matrix dimensions.");

  mat<T> c(a.nrow(), b.ncol());
  for (size_t i = 0; i < a.nrow(); ++i)
    for (size_t j = 0; j < b.ncol(); ++j)
      for (size_t k = 0; k < a.ncol(); ++k)
	c[i][j] += a[i][k] * b[k][j];

  return c;
}

template <class T>
void print_mat(mat<T> m)
{
  for (const auto& row : m)
  {
    for (const auto& e : row)
      std::cout << e << " ";
    std::cout << std::endl;
  }
}

int main(int argc, char * argv[])
{
  mat<double> m(N,N);
  matrix_initialize_test(m, m);

  mat<int> a = {{1, 2}, {3, 4}};
  mat<int> b = a;
  print_mat(a);
  print_mat(b);
  print_mat(mmult_bad(a, b));
  return 0;
}
