#include <iostream>
#include <sys/time.h>

uint64_t get_time()
{
  timeval tv;
  gettimeofday(&tv, nullptr);
  return tv.tv_sec * 1000000 + tv.tv_usec;
}

#define N 3000
double a[N][N];
double b[N][N];


int main(int argc, char * argv[])
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
	
  return 0;
}
