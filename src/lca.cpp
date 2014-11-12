
#include "lca.hpp"

#include <complex>
#include <iostream>
#include <vector>


/*!
 * Xd  - delayed data, dimensions Nx x Ny
 * w   - windows,      dimensions Nw x M
 */
void Lca::run(float* Xd_in, size_t Nx, size_t Ny, size_t M, float* w, size_t Nw, float* wrf, size_t wrf_size[2]) {

   bool y_only_ones;

   x_avg = wrf_size[0];
   y_avg = wrf_size[1];

   y_mean = (y_avg-1)/2;
   x_mean = (x_avg-1)/2;


   // The filter must have odd number of rows/columns
   if( y_avg%2==0 or x_avg%2==0 ) {
      std::cout << "The 'wrf' filter must have odd number of rows/columns.";
      return;
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

   Xd                      = Array3D( Xd_in, Xd_in+Nx*Ny*M );

   new_y                   = Array1D( Ny, 0 );
   new_w                   = Array1D( Ny, 0 );
   img_y_segment_abs_sum   = Array1D( Nw, 0 );
   w_sum                   = Array1D( Nw, 0 );


   img_y_segment_abs       = Array2D( 2*y_avg, Array1D(Nw, 0));
//   for(int y=0; y<2*y_avg; ++y) {
//      img_y_segment_abs[y] = new Data[Nw];
//   }

   img_y                   = Array2D( Ny+2*y_avg, Array1D(Nw, 0));
   img_y_abs               = Array2D( Ny+2*y_avg, Array1D(Nw, 0));
//   for(int y=0; y<Ny+2*y_avg; ++y) {
//      img_y[y]             = new Data[Nw];
//      img_y_abs[y]         = new Data[Nw];
//   }

   new_img                 = Array2D( Nx, Array1D(Ny, 0));
   new_win                 = Array2D( Nx, Array1D(Ny, 0));
//   for(int x=0; x<Nx+2*x_avg; ++x) {
//      img_y[x]             = new Data[Ny];
//      img_y_abs[x]         = new Data[Ny];
//   }

   img_abs                 = Array3D( Nx, Array2D( Nx, Array1D(Ny, 0)));
//   for(int x=0; x<Nx; ++x) {
//      img_abs[x]           = new Data*[Ny];
//      img_abs[x]           = new Data*[Ny];
//      for(int y=0; y<Ny; ++y) {
//         img_abs[x][y]     = new Data[Nw];
//      }
//   }

   w_sum_min = 0;

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


// Select the window that yield the least power
bool first_run;
Data sum, least_sum;
if( y_avg == 1 && x_avg == 1 && wrf[0,0] ) {
   least_sum = 0;
   for(size_t x = 0; x < Xd.size(); ++x) {
      for(size_t y = 0; y < Xd[0].size(); ++y) {
         sum = 0;
         for(size_t m = 0; m < Xd[0][0].size(); ++m) {
            sum += Xd[x][y][m];
         }
         if(std::abs(sum) < std::abs(least_sum) || first_run) {
            least_sum = sum;
            first_run = false;
         }
      }
   }
//
//   l,m = np.meshgrid(range(0,Ny),range(0,Nx))
//   selected_window = np.abs(Z).argmin(2)
//   return Z[m,l,selected_window], selected_window
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
