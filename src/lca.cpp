
#include "lca.hpp"

#include <complex>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <vector>

#include <fstream>

Lca::Lca() {}

#if (defined(_WIN32) || defined(WIN32)) && (_MSC_VER < 1800) // > VS2012
Lca::~Lca() {};
#else
Lca::~Lca() = default;
#endif

size_t* makeDim(size_t x, size_t y, size_t z) {
   size_t *dim = new size_t[3];
   dim[0] = x;
   dim[1] = y;
   dim[2] = z;
   return dim;
}
/*!
 * Xd  - delayed data, dimensions Nx x Ny
 * w   - windows,      dimensions Nw x M
 */
int Lca::run(float* Xd_in, size_t Nx, size_t Ny, size_t M, float* win_in, size_t Nw, float* wrf, size_t wrf_size[2]) {

   try{

      this->Nx = Nx;
      this->Ny = Ny;
      this->M  = M;
      this->Nw = Nw;

      bool y_only_ones;

      this->x_avg = wrf_size[0];
      this->y_avg = wrf_size[1];

      this->y_mean = (y_avg-1)/2;
      this->x_mean = (x_avg-1)/2;


      // The filter must have odd number of rows/columns
      if( y_avg%2==0 or x_avg%2==0 ) {
         std::cout << "The 'wrf' filter must have odd number of rows/columns.";
         return 1;
      }

      // If only range-values are supplied, and these are all 1's, then set the
      // 'y_only_ones' flag which will be used later to simplify computations.
      if( x_avg == 1 ) {
         y_only_ones = true;
         for(int y=0; y<y_avg; ++y) {
            if( wrf[y,0] != 1 ) {
               y_only_ones = false;
            }
         }
      }
      else {
         y_only_ones = false;
      }

   //   std::vector<std::complex<float>> Z;          // 3D
   //   std::vector<std::complex<float>> wrf;        // 2D

//      #define uniqueArrayND(dims, shape, data) { std::unique_ptr<ArrayND>(new ArrayND( dims, makeDim(Ny,1,1), 0 )); }
//
//      std::vector<size_t> dims(3,0);

//      new_y                   = std::unique_ptr<ArrayND>(new ArrayND( 1, makeDim(Ny,1,1), 0 ));
//      new_w                   = std::unique_ptr<ArrayND>(new ArrayND( 1, makeDim(Ny,1,1), 0 ));
//      img_y_segment_abs_sum   = std::unique_ptr<ArrayND>(new ArrayND( 1, makeDim(Nw,1,1), 0 ));
//      w_sum                   = std::unique_ptr<ArrayND>(new ArrayND( 1, makeDim(Nw,1,1), 0 ));
//
      win                     = std::unique_ptr<ArrayND>(new ArrayND( 2, makeDim(Nw,M,1),  (std::complex<float>*)win_in ));
      Xd                      = std::unique_ptr<ArrayND>(new ArrayND( 3, makeDim(Nx,Ny,M), (std::complex<float>*)Xd_in ));

//      img_y_segment_abs       = std::unique_ptr<ArrayND>(new ArrayND( 2, makeDim(2*y_avg,Nw,1), 0 ));
//
//      img_y                   = std::unique_ptr<ArrayND>(new ArrayND( 2, makeDim(Ny+2*y_avg,Nw,1), 0 ));
//      img_y_abs               = std::unique_ptr<ArrayND>(new ArrayND( 2, makeDim(Ny+2*y_avg,Nw,1), 0 ));

      new_img                 = std::unique_ptr<ArrayND>(new ArrayND( 2, makeDim(Nx,Ny,1), 0 ));
//      new_win                 = std::unique_ptr<ArrayND>(new ArrayND( 2, makeDim(Nx,Ny,1), 0 ));
//   //   for(int x=0; x<Nx+2*x_avg; ++x) {
//   //      img_y[x]             = new Data[Ny];
//   //      img_y_abs[x]         = new Data[Ny];
//   //   }
//
//      img_abs                 = std::unique_ptr<ArrayND>(new ArrayND( 3, makeDim(Nx,Ny,Nw), 0 ));
//   //   for(int x=0; x<Nx; ++x) {
//   //      img_abs[x]           = new Data*[Ny];
//   //      img_abs[x]           = new Data*[Ny];
//   //      for(int y=0; y<Ny; ++y) {
//   //         img_abs[x][y]     = new Data[Nw];
//   //      }
//   //   }
//
//      w_sum_min = 0;

   //   new_y                   = std::unique_ptr<std::complex<float>*>(  new std::complex<float>[Ny]);
   //   new_w                   = std::unique_ptr<std::complex<float>*>(  new std::complex<float>[Ny]);
   //   img_y_segment_abs_sum   = std::unique_ptr<std::complex<float>*>(  new std::complex<float>[Nw]);
   //   w_sum                   = std::unique_ptr<std::complex<float>*>(  new std::complex<float>[Nw]);
   //
   //
   //   img_y_segment_abs       = std::unique_ptr<std::complex<float>*[]>( new std::complex<float>*[Nw]);
   //   img_y                   = std::unique_ptr<std::complex<float>*[]>( new std::complex<float>*[Nw]);
   //   img_y_abs               = std::unique_ptr<std::complex<float>*[]>( new std::complex<float>*[Nw]);
   //   img_abs                 = std::unique_ptr<std::complex<float>**[]>(new std::complex<float>*[Nw]);
   //
   //   for(int w=0; w<Nw; ++w) {
   //      img_y_segment_abs[w] = std::unique_ptr<std::complex<float>[]>( new std::complex<float>*[2*y_avg]);
   //      img_y[w]             = std::unique_ptr<std::complex<float>[]>( new std::complex<float>[Ny+2*y_mean]);
   //      img_y_abs[w]         = std::unique_ptr<std::complex<float>[]>( new std::complex<float>[Ny+2*y_mean]);
   //      img_abs[w]           = std::unique_ptr<std::complex<float>*[]>(new std::complex<float>[Nx*Ny]);
   //
   //      for(int x=0; x<Nx; ++x) {
   //         img_abs[x]        = std::unique_ptr<std::complex<float>*[]>(new std::complex<float>[Nx*Ny]);
   //      }
   //   }
   //
   //   w_sum_min = 0;
   //   new_img                 = std::unique_ptr<std::complex<float>*[]>( new std::complex<float>[Nx*Ny]);
   //   new_img_all             = std::unique_ptr<std::complex<float>**[]>(new std::complex<float>[Nx*Ny*Nw]);
   //   new_win                 = std::unique_ptr<std::complex<float>*[]>( new std::complex<float>[Nx*Ny]);

      std::cout << "Nx: " << Nx << "  Ny: " << Ny << "  M: " << M << "  Nw: " << Nw << std::endl;

      // Select the window that yield the least power
      bool first_run;
      Data sum, least_sum;
//      ArrayND *xd = Xd.get();
//
//      std::ofstream file;
//      file.open("data.txt");
//
//      for(size_t x = 0; x < Nx; ++x) {
//         for(size_t y = 0; y < Ny; ++y) {
//            sum = 0;
//            for(size_t m = 0; m < M; ++m) {
//               sum += (*Xd)(x,y,m); //(*win)(w,m); //(*Xd)(x,y,m);// *
//            }
//            (*new_img)(x,y) = sum;
//            file << (*new_img)(x,y).real() << '\t' << (*new_img)(x,y).imag() << '\t';
//            if( y==0 )
//               std::cout << x << " (" << std::abs(sum) << ")(" << sum.real() << "," << sum.imag() << ")" << std::endl;
//         }
//      }


      if( y_avg == 1 && x_avg == 1 && wrf[0,0] ) {
         for(size_t x = 0; x < Nx; ++x) {
            std::cout << x << std::endl;
            for(size_t y = 0; y < Ny; ++y) {
               least_sum = 0;
               first_run = true;
               for(size_t w = 0; w < Nw; ++w) {
                  sum = 0;
                  for(size_t m = 0; m < M; ++m) {
                     sum += (*Xd)(x,y,m) * (*win)(w,m); //(*Xd)(x,y,m);// *
                  }
                  if(std::abs(sum) < std::abs(least_sum) || first_run) {
                     least_sum = sum;
                     first_run = false;
                  }
               }
               (*new_img)(x,y) = least_sum;
               if( y==0 )
                  std::cout << x << " (" << std::abs((*win)(w,0)) << ")(" << sum.real() << "," << sum.imag() << ")" << std::endl;
            }
         }
      //
      //   l,m = np.meshgrid(range(0,Ny),range(0,Nx))
      //   selected_window = np.abs(Z).argmin(2)
      //   return Z[m,l,selected_window], selected_window
      }
      else {
         std::cout << "Huh? Unexpected." << std::endl;
      }
//
//      std::cout << "Finished" << std::endl;
//
//      return 0;

   } catch (const std::exception &e) {
//      auto e = std::current_exception();
      std::cout << "Lca::run(...) caught exception " << e.what() << ". Aborting." << std::endl;
      return 1;
   }


//
//###############################
//# Perform 'window averaging'. #
//###############################
//# A way to make the beamformer estimate a pixel more accurately, the beamformer output
//# may be computed for a 'window' of pixels around the one we wish to image, and the window
//# that gave the overall lowest output power may be applied to the center pixel.
//#
//# It is common that the averaging window is comprised of only ones, and no averaging is
//# required in azimuth. That mode is handled first, and will be less computationally
//# intensive than the next 'else if' which handles arbitrary averaging windows.
//elif Y_ONLY_ONES:
//
//   # Iterate over all azimuth coordinates
//   for x in range (Nx):
//
//      # Compute the power of each range pixel (square them)
//      for y in range(Ny):
//         for w in range(Nw):
//            img_y[w,y] = Z[x,y,w]
//            img_y_abs[w,y] = img_y[w,y].real**2 + img_y[w,y].imag**2
//
//      # Compute the beamformer output for the first y-segment
//      for w in range(Nw):
//         w_sum_min = 0
//         w_sum[w] = 0
//         for y in range(y_avg):
//            w_sum[w] += img_y_abs[w,y]
//
//         # Store the beamformer output for this window in image archive
//         new_img_all[x,y,w]= img_y[w,y_avg]
//
//         # Select the window that yielded the minimum output power of the beamformer
//         if w_sum[w] < w_sum_min or w==0:
//               w_sum_min = w_sum[w]
//               new_win[x,y_avg] = w
//               new_img[x,y_avg] = img_y[w,y_avg]
//
//
//      # Select a range segment:
//      for y in range(1+y_mean,Ny-y_mean):
//         # Compute the beamformer output for each of the windows
//         for w in range(Nw):
//            w_sum[w] += img_y_abs[w,y+y_mean] - img_y_abs[w,y-y_mean-1]
//
//            # Store the beamformer output for this window in image archive
//            new_img_all[x,y,w]= img_y[w,y]
//
//            # Select the window that yielded the minimum output power of the beamformer
//            if w_sum[w] < w_sum_min or w==0:
//               w_sum_min = w_sum[w]
//               new_win[x,y] = w
//               new_img[x,y] = img_y[w,y]
//
//   return new_img, new_win, new_img_all
//
//# Handle arbitrary window functions:
//elif y_avg != 0 and x_avg != 0:
//
//   # Compute the image absolute value
//   for y in range(Ny):
//      for x in range(Nx):
//         for w in range(Nw):
//            img_abs[w,x,y] = Z[x,y,w].real**2 + Z[x,y,w].imag**2
//
//#      for w in range(Nw):
//#         img_abs[w,:,:] = Z[:,:,w]**2
//
//
//   # Select a range segment:
//   for y in range(y_mean,Ny-y_mean):
//      # Select an azimuth segment:
//      for x in range(x_mean,Nx-x_mean):
//         # Compute the accumulated beamformer output for each of the windows
//         for w in range(Nw):
//            w_sum[w] = 0
//            for yy in range(2*y_mean+1):
//               for xx in range(2*x_mean+1):
//                  w_sum[w] += img_abs[w,y+yy-y_mean,x+xx-x_mean]*wrf[yy,xx]
//
//            # Store the beamformer output for this window in image archive
//            new_img_all[x,y,w] = Z[x,y,w]
//
//            # Select the window that yielded the minimum output power of the beamformer
//            if w_sum[w] < w_sum_min or w==0:
//               w_sum_min = w_sum[w]
//               new_win[x,y] = w
//               new_img[x,y] = Z[x,y,w]
//
//   return new_img, new_win, new_img_all

}

float* Lca::getImage() {
   float *test = new float[Nx*Ny*2];
   for(int x=0; x<Nx; ++x) {
      for(int y=0; y<Ny; ++y) {
         test[x*2*Ny+2*y] = (*new_img)(x,y).real();
         test[x*2*Ny+2*y+1] = (*new_img)(x,y).imag();
      }
   }
   return test;
//   return (float*)&(*new_img)(0,0);
}

extern "C" {
   Lca* Lca_new() { return new Lca(); }

   int Lca_run(Lca *lca, float* Xd, size_t Nx, size_t Ny, size_t M, float* win, size_t Nw, float* wrf, size_t *wrf_size) {
      return lca->run(Xd, Nx, Ny, M, win, Nw, wrf, (size_t*)wrf_size);
   }

   float* Lca_getImage(Lca *lca) {
      return lca->getImage();
   }
}
