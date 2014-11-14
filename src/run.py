
import pylab as pl

from lca import *

def makeKaiser(M, beta, phi):
   wc          = np.kaiser(M, beta)           # Window coefficients
   win         = wc / sum(wc)                   # Normalised window   
   return win


def runLca():
   
   print "On the roll"

   lca = Lca()
   
   # Load Holmengraa
   from scipy import io
   #mat_file = io.loadmat('/home/me/Work/UiO/Phd/Code/data/sonar/real/hugin/hisas1030/run120202_4 h_graa1-1/run120202_4 h_graa1-1.mat')
   #Xd = mat_file['sss_mfdata'].copy()
   
   mat_file = io.loadmat('/home/me/Work/UiO/Phd/Code/data/sonar/simulations/hisas1030/speckle_cube/Specle_1_Cube.mat')
   
   Xd = mat_file['dataCube'].copy()
   
   das = Xd.sum(2)
  
   print Xd.shape
   M = Xd.shape[2]
   
   win = np.zeros((5,M))
   win[0] = makeKaiser(M, 0.2, 0)
   win[1] = makeKaiser(M, 0.5, 0)
   win[2] = makeKaiser(M, 0.8, 0)
   win[3] = makeKaiser(M, 1.0, 0)
   win[4] = makeKaiser(M, 1.2, 0)
#    beta = 0
#    wc          = np.kaiser(M, beta)           # Window coefficients
#    win         = wc / sum(wc)                   # Normalised window

   wrf = np.array([[1]])
   
   lca.run(Xd, win, wrf)

   data = lca.getImage()

   vlim = (-35, 0)
   
   
   
   fn = pl.figure()
   ax1 = fn.add_subplot(211)
   max_value = 20*np.log10(np.abs(das).max())
   ax1.imshow( 20*np.log10(np.abs(das)) - max_value
             , cmap=pl.cm.gray
             , interpolation='nearest'
             , aspect='auto'
             , vmin=vlim[0]
             , vmax=vlim[1]
              )
   
   
   ax2 = fn.add_subplot(212)
   max_value = 20*np.log10(np.abs(data).max())
   #data[data==0] = 1e-6
   ax2.imshow( 20*np.log10(np.abs(data)) - max_value
             , cmap=pl.cm.gray
             , interpolation='nearest'
             , aspect='auto'
             , vmin=vlim[0]
             , vmax=vlim[1]
             )
#              , extent=(0,1 #(lca.sonar_image_x_bounds[::-1]+lca.sonar_image_y_bounds[::-1])

   pl.show()

   print 'hello world'
   
   
# if __name__ == "__main__":
#    runLca()