#include <Eigen/Sparse>
#include <chrono>
#include <iostream>
#include <omp.h>

using namespace std;
using namespace std::chrono;

#define L 100
#define V 100
#define eps 1e-5
#define O 201       // 1+2*L
#define MAX_I 15050 // L(L+1)/2 + L*L
#define OFFSET(y) ((y) * (y + O) / 2)

typedef Eigen::SparseMatrix<float> Mat;
typedef Eigen::Matrix<float, MAX_I, 1> Vec;
typedef Eigen::Triplet<float> Triplet;

int offset(int y) { return y * (y + O) / 2; }

int main(int argc, char *argv[]) {
  Mat mat(MAX_I, MAX_I);

  high_resolution_clock::time_point t1 = high_resolution_clock::now();
  // set boundary
  Vec b = Vec::Zero();

  float slope = (float)V / (float)L;

  for (int x = 0; x < L + 1; ++x) {
    b[L - x] = x * slope;
  }

  // set matrix

  // for y in range(1, L-1):
  //   i = coord_to_index((y, 0), L)

  //   matrix[i, i] = -2
  //   matrix[i, i+1] = 1
  //   matrix[i, coord_to_index((y+1, 1), L)] = 1

  //   for x in range(1, L+y):
  //       i+=1
  //       matrix[i, i] = -4
  //       matrix[i, i+1] = 1
  //       matrix[i, i-1] = 1
  //       matrix[i, coord_to_index((y-1, x-1), L)] = 1
  //       matrix[i, coord_to_index((y+1, x+1), L)] = 1

  //   write the above code in C++ using Eigen

  int N = 0;
  int insertions = 0;

  // mat.reserve(73059);
  vector<Triplet> triplets;
  triplets.reserve(73059);
  for (int y = 1; y < L - 1; ++y) {
    ++N;
    int i = OFFSET(y);

    triplets.push_back(Triplet(i, i, -2));
    triplets.push_back(Triplet(i, i + 1, 1));
    triplets.push_back(Triplet(i, OFFSET(y + 1) + 1, 1));

    // mat.insert(i, i) = -2;
    // mat.insert(i, i + 1) = 1;
    // mat.insert(i, OFFSET(y + 1) + 1) = 1;

    insertions += 3;

    // #pragma omp parallel for shared(mat) schedule(static)
    for (int x = 1; x < L + y; ++x) {
      ++N;
      // ++i;

      triplets.push_back(Triplet(i + x, i + x, -4));
      triplets.push_back(Triplet(i + x, i + x + 1, 1));
      triplets.push_back(Triplet(i + x, i + x - 1, 1));
      triplets.push_back(Triplet(i + x, OFFSET(y - 1) + x - 1, 1));
      triplets.push_back(Triplet(i + x, OFFSET(y + 1) + x + 1, 1));

      // mat.insert(i, i) = -4;
      // mat.insert(i, i + 1) = 1;
      // mat.insert(i, i - 1) = 1;
      // mat.insert(i, OFFSET(y - 1) + x - 1) = 1;
      // mat.insert(i, OFFSET(y + 1) + x + 1) = 1;

      insertions += 5;
    }
  }

  cout << triplets.size() << endl;

  for (Triplet v : triplets) {
    if (v.row() >= MAX_I || v.col() >= MAX_I || v.row() < 0 || v.col() < 0) {
      cout << "ERROR" << endl;
      cout << v.row() << " " << v.col() << " " << v.value() << endl;
    }
  }

  mat.setFromTriplets(triplets.begin(), triplets.end());

  high_resolution_clock::time_point t2 = high_resolution_clock::now();
  Vec x;
  {
    Eigen::BiCGSTAB<Mat, Eigen::IdentityPreconditioner> bicg;
    bicg.compute(mat);
    x = bicg.solve(b);
    std::cout << "BiCGSTAB: #iterations: " << bicg.iterations()
              << ", estimated error: " << bicg.error() << std::endl;
  }

  high_resolution_clock::time_point t3 = high_resolution_clock::now();
  duration<double> part = duration_cast<duration<double>>(t3 - t2);
  duration<double> whole = duration_cast<duration<double>>(t3 - t1);

  std::cout << "Whole program " << whole.count()*1000 << " ms." << endl;
  std::cout << "Part  " << part.count()*1000 << " ms." << endl;
  std::cout << "N  " << N << endl;
  std::cout << "insertions  " << insertions << endl;

  return 0;
}
