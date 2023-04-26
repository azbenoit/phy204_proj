#include <chrono>
#include <iostream>
#include <omp.h>

using namespace std;
using namespace std::chrono;

#define L 100
#define V 100
#define eps 1e-5

int main(int argc, char *argv[]) {
  float phi[L * L + (L + 1) * L / 2] = {0};
  float err = eps + 1;

  // set boundary

  float slope = (float)V / (float)L;

  for (int x = 0; x < L + 1; ++x) {
    phi[L - x] = x * slope;
  }

  high_resolution_clock::time_point t1 = high_resolution_clock::now();

  int iter = 0;

  while (err > eps) {
    ++iter;
    err = 0;
#pragma omp parallel for shared(phi) schedule(static)
    for (int y = 1; y < L - 1; ++y) {
      int offset = y * L + y * (y + 1) / 2;

      // for (int x = 1; x < 2 * L - 1; ++x) {
      float old = phi[offset];

      phi[offset] = 0.5 * (phi[offset + 1] + phi[offset + L + y + 2]);

      float temp_err = phi[offset] - old;

      if (temp_err > err) {
        err = temp_err;
      }

      for (int x = 1; x < L + 1 + y; ++x) {
        float old = phi[offset + x];

        phi[offset + x] = 0.25 * (phi[offset + x + 1] + phi[offset + x - 1] +
                                  phi[offset + x - (L + y + 1)] +
                                  phi[offset + x + L + y + 2]);

        float temp_err = phi[offset + x] - old;

        if (temp_err > err) {
          err = temp_err;
        }
      }
    }
  }
  // #pragma omp parallel for shared(phi) schedule(static)
  //     for (int x = 1; x < 2 * L - 1; ++x) {
  //       for (int y = 1; y < L - 1; ++y) {
  //         float old = phi[y][x];
  //         phi[y][x] = 0.25 * (phi[y][x + 1] + phi[y][x - 1] + phi[y + 1][x]
  //         +
  //                             phi[y - 1][x]);

  //         for (int x = 0; x < L - 1; x++) {
  //           phi[0][x] = phi[L - 1 - x][L - 1];
  //         }

  //         float temp_err = phi[y][x] - old;

  //         if (temp_err > err) {
  //           err = temp_err;
  //         }
  //       }
  //     }

  //     for (int x = 0; x < L - 1; x++) {
  //       phi[0][x] = phi[L - 1 - x][L - 1];
  //     }
  //   }

  high_resolution_clock::time_point t2 = high_resolution_clock::now();
  duration<double> time_span = duration_cast<duration<double>>(t2 - t1);

  cout << iter << endl;
  cout << err << endl;
  std::cout << "It took me " << time_span.count() << " seconds." << endl;

  return 0;
}
