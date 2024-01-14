from compliance_mM import *
from volume_mM import *
import numpy as np
from local_Support_mM import *

op_dir = os.path.join(WD, OP, problem_name)
### Reading files ##
ELEMENTS = np.loadtxt('ELEMENTS.dat')

### Not the most efficient to read rho_e, but ensures it has the right dimension for brodcasting operations (number of elements,1)

rho_temp = np.loadtxt('rho_e_test.dat')
rho_e = np.zeros((len(rho_temp),1))
rho_e[:,0] = rho_temp

scale = 'standard'
IND_mask = np.loadtxt("IND_mask_test.dat",dtype=int)
IND_mask_tot = np.loadtxt('IND_mask_tot_test.dat',dtype=int)
U1 = np.loadtxt('U1.dat')
U2 = np.loadtxt('U2.dat')
U3 = np.loadtxt('U3.dat')

### P_rho and W are 3D arrays that were saved as 2D arrays, we thus must reshape them after reading
P_rho = np.loadtxt('P_rho_test.dat').reshape(28,28,28)
W = np.loadtxt('W_test.dat').reshape(28,28,28)

#It's faster to compute BF_support than read it
local_support, BF_support, IND_mask_active = local_support_fun(ELEMENTS, IND_mask, IND_mask_tot, U1, U2, U3, scale)
DIM=2
der_CP, der_W, BF_mask=der_NURBS(local_support,BF_support,IND_mask_active,IND_mask,IND_mask_tot,P_rho,W,rho_e)