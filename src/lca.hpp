#pragma once

#include "Array.hpp"
#include <complex>
#include <memory>
#include <vector>

class Lca {

private:
   int   Ny;
   int   Nx;
   int   M;
   int   Nw;
   int   y_avg;
   int   y_mean;
   int   x_avg;
   int   x_mean;
   float w_sum_min;
   int x,y,w,xx,yy;

   typedef std::complex<float>  Data;
   typedef Array<Data> ArrayND;
   typedef std::unique_ptr<ArrayND> ArrayNDptr;
//   typedef std::vector<Array1D> Array2D;
//   typedef std::vector<Array2D> Array3D;


   ArrayNDptr Xd;          // 3D
   ArrayNDptr win;          // 3D
//   std::vector<std::complex<float>> wrf;        // 2D
   ArrayNDptr new_y;      // 1D
   ArrayNDptr new_w;      // 1D
   ArrayNDptr img_y_segment_abs; // 2D
   ArrayNDptr img_y_segment_abs_sum; // 1D
   ArrayNDptr w_sum;      // 1D
   ArrayNDptr img_y;      // 2D
   ArrayNDptr img_y_abs;  // 2D
   ArrayNDptr new_img;    // 2D
   ArrayNDptr new_img_all;// 3D
   ArrayNDptr new_win;    // 2D

   // For XY-averaging:
   ArrayNDptr img_abs;    // 3D

public:

   Lca();

   virtual ~Lca();

   int run(float* Xd, size_t Nx, size_t Ny, size_t M, float* win, size_t Nw, float* wrf, size_t wrf_size[2]);

   float* getImage();

};


// C entry functions that allows the LCA beamformer to be called from e.g. Python using ctypes
extern "C" {
   Lca* Lca_new();

   int Lca_run(Lca *lca, float* Xd, size_t Nx, size_t Ny, size_t M, float* win, size_t Nw, float* wrf, size_t* wrf_size);

   float* Lca_getImage(Lca *lca);
}
