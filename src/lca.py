import sys, os
# from os.path import dirname
# sys.path.append('./debug/lib')
#os.environ['LD_LIBRARY_PATH'] = './debug/bin'

import ctypes
from ctypes import cdll

import numpy as np
from numpy.ctypeslib import ndpointer
from os.path import dirname

if os.name == 'nt':
   lib = cdll.LoadLibrary(dirname(__file__)+'/../debug/lib/liblca_shared.dll')
elif( os.name == 'posix' ):
   lib = cdll.LoadLibrary(dirname(__file__)+'/../debug/lib/liblca_shared.so')
else:
   print "Yikes. This OS is unsupported!"
   print "Reported OS: %s"%os.name
      

class Lca(object):
   def __init__(self):
      self.obj = lib.Lca_new()    
      
   def run(self, Xd_in, win_in, wrf):
      
      Xd = Xd_in.astype('complex64')
      win = win_in.astype('complex64')
      
      Nx = Xd.shape[0]
      Ny = Xd.shape[1]
      M  = Xd.shape[2]
      
      Nw = win.shape[0]
      
      
      wrf_size = np.array(wrf.shape, dtype=np.uint64)
      
      self.lca_image_size = (Nx, Ny)
          
      olddir = os.getcwd()
      try:
         os.chdir(dirname(__file__))
         lib.Lca_run(self.obj, ctypes.c_void_p(Xd.ctypes.data),
                               Nx, Ny, M,
                               ctypes.c_void_p(win.ctypes.data),
                               Nw,
                               ctypes.c_void_p(wrf.ctypes.data),
                               ctypes.c_void_p(wrf_size.ctypes.data)
                               )
      except:
         os.chdir(olddir)
      
   def getImage(self):
      if hasattr(self, 'lca_image_size'):
         imgsize = self.lca_image_size[0] * self.lca_image_size[1]
           
         # Specify return value
         lib.Lca_getImage.restype = ctypes.POINTER(ctypes.c_float)
           
         # Run function
         ctype_image = lib.Lca_getImage(self.obj)
           
         # Make a copy since we don't own this data
         ctypes_array_pointer = ctypes.cast(ctype_image, ctypes.POINTER(ctypes.c_float*(2*imgsize)))
         image = np.fromiter(ctypes_array_pointer.contents, dtype=np.float, count=2*imgsize)
         
         complex_image = image[::2] + 1j*image[1::2]
#           
         return complex_image.reshape((self.lca_image_size[0], self.lca_image_size[1]))
           
      else:
         print 'ERROR: You need to run the beamformer first!'
      