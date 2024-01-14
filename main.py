import numpy as np
from local_Support_mM import local_support_fun
import time

# Chargement des données
ELEMENTS = np.loadtxt('ELEMENTS.dat')
IND_mask = np.loadtxt('IND_mask.dat', dtype=int)
IND_mask_tot = np.loadtxt('IND_mask_tot.dat', dtype=int)
U1 = np.loadtxt('U1.dat')
U2 = np.loadtxt('U2.dat')
U3 = np.loadtxt('U3.dat')

temps_debut = time.time()
    
# Appel de la fonction
local_support, BF_support, IND_mask_active = local_support_fun(ELEMENTS, IND_mask, IND_mask_tot, U1, U2, U3, 'standard')

temps_fin = time.time()
temps_total = temps_fin - temps_debut

print(f"Temps pris par la fonction : {temps_total} secondes")


# Enregistrement des sorties dans des fichiers texte
with open('local_support_py.txt', 'w') as file:
    for sublist in local_support:
        file.write(' '.join(map(str, sublist)) + '\n')
#np.savetxt('BF_support.txt', BF_support, fmt='%f')  # Adapt to the desired format
np.savetxt('IND_mask_active_py.txt', IND_mask_active, fmt='%f')  # Adapt to the desired format
