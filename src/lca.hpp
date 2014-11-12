
#include <complex>
#include <vector>

class Lca {


   int   Ny;
   int   Nx;
   int   Nw;
   int   y_avg;
   int   y_mean;
   int   x_avg;
   int   x_mean;
   float w_sum_min;
   int x,y,w,xx,yy;

   typedef std::complex<float>  Data;
   typedef std::vector<Data>    Array1D;
   typedef std::vector<Array1D> Array2D;
   typedef std::vector<Array2D> Array3D;


   Array3D Xd;          // 3D
//   std::vector<std::complex<float>> wrf;        // 2D
   Array1D new_y;      // 1D
   Array1D new_w;      // 1D
   Array2D img_y_segment_abs; // 2D
   Array1D img_y_segment_abs_sum; // 1D
   Array1D w_sum;      // 1D
   Array2D img_y;      // 2D
   Array2D img_y_abs;  // 2D
   Array2D new_img;    // 2D
   Array3D new_img_all;// 3D
   Array2D new_win;    // 2D

   // For XY-averaging:
   Array3D img_abs;    // 3D

public:

   Lca();

   virtual ~Lca();

   void run(float* Xd, size_t Nx, size_t Ny, size_t M, float* w, size_t Nw, float* wrf, size_t wrf_size[2]);

};
