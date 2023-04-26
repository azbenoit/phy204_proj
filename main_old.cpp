// #include <QGuiApplication>
// #include <QQmlApplicationEngine>

#include <algorithm>
#include <chrono>
#include <ctime>
#include <iostream>
#include <iterator>
#include <omp.h>
#include <ratio>
#include <string>

using namespace std;
using namespace std::chrono;

// #define L 100

// void update_bounds(float **&mat, float *&line) {
//   // Mirror lower side

//   for (int x = 0; x < L - 1; x++) {
//     mat[0][x] = mat[L - 1 - x][L - 1];
//   }
// }

// void save_matrix(float *&matrix) {
//   ofstream myfile;
//   myfile.open("matrix.txt");
//   for (int i = 0; i < L; i++) {
//     for (int j = 0; j < L; j++) {
//       myfile << matrix[i * L + j] << " ";
//     }
//     myfile << endl;
//   }
//   myfile.close();
// }

int main(int argc, char *argv[]) {
  int V = 100;
  int L = 100;
  float eps = 1e-5;

  float phi[L][2 * L];
  // float *phi = calloc(size_of(float)*(L*L+(L+1)*L/2));

  float err = eps + 1;

  // init matrices

  for (int x = 0; x < 2 * L; x++) {
    for (int y = 0; y < L; y++) {
      phi[y][x] = 0;
    }
  }

  // set boundary

  float slope = (float)V / (float)L;

  for (int x = 0; x < L + 1; x++) {
    phi[0][2 * L - 1 - x] = x * slope;
  }

  high_resolution_clock::time_point t1 = high_resolution_clock::now();

  int iter = 0;

  while (err > eps) {
    ++iter;
    err = 0;
// #pragma omp parallel for shared(phi) schedule(static)
    for (int y = 1; y < L - 1; ++y) {
      for (int x = 1; x < 2 * L - 1; ++x) {
        float old = phi[y][x];
        phi[y][x] = 0.25 * (phi[y][x + 1] + phi[y][x - 1] + phi[y + 1][x] +
                            phi[y - 1][x]);

        for (int x = 0; x < L - 1; x++) {
          phi[0][x] = phi[L - 1 - x][L - 1];
        }

        float temp_err = phi[y][x] - old;

        if (temp_err > err) {
          err = temp_err;
        }
      }
    }

    for (int x = 0; x < L - 1; x++) {
      phi[0][x] = phi[L - 1 - x][L - 1];
    }
  }

  high_resolution_clock::time_point t2 = high_resolution_clock::now();
  duration<double> time_span = duration_cast<duration<double>>(t2 - t1);

  cout << iter << endl;
  cout << err << endl;
  std::cout << "It took me " << time_span.count() << " seconds." << endl;

  return 0;
}
