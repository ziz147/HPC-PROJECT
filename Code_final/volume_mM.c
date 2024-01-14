#include "volume_mM.h"
#include"matrix.h"
//#include"symmetry_coef.h"
#include"derTopo.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void volume_grad_fun( Vector rho_e, Matrix P_rho, Matrix W, Matrix ELEMENTS , Matrix IND_mask ,ListOfVectors local_support,Matrix BF_supprt, Matrix IND_mask_tot, Vector IND_mask_active, int DERIVATIVES, int DIM ,int NURBS, Vector *grad_v){

    if(DERIVATIVES == 1){
        //sym_coef_temp = sym_coef()
        int sym_coef =1;
        if(DIM == 2){

            if(NURBS ==1){ //NURBS surface
                Matrix der_CP, der_W;
                Vector BF_mask;
                der_NURBS(local_support,BF_supprt,IND_mask_active,IND_mask,IND_mask_tot,rho_e,P_rho,W,DIM,&der_CP,&der_W, &BF_mask) ;

                /* vol_temp*/
            
                Vector ELEMENTS_8 = extractValues(ELEMENTS, &BF_mask.data, 8, BF_mask.length); //ELEMENTS[BF_mask,8]
                Vector ELEMENTS_10= extractValues(ELEMENTS, &BF_mask.data,10, BF_mask.length); //ELEMENTS[BF_mask,10]

                Vector vol_temp = Vec_product(ELEMENTS_8 , ELEMENTS_10); // ELEMENTS[BF_mask,8]*ELEMENTS[BF_mask,10]

                /* grad_v_cp */
                
                //à modifier pour prendre en compte sym_coef
                Matrix grad_v_cp_temp = matrix_vector_multiply(&der_CP, &vol_temp);
                Vector grad_v_cp;
                sum_axis_0(&grad_v_cp_temp, &grad_v_cp); //np.sum(sym_coef_temp*der_CP*vol_temp,axis=0)


                /* grad_v_w */

                //à modifier pour prendre en compte sym_coef
                Matrix grad_v_w_temp = matrix_vector_multiply(&der_W, &vol_temp);
                Vector grad_v_w;
                sum_axis_0(&grad_v_w_temp, &grad_v_w); //np.sum(sym_coef_temp*der_W*vol_temp,axis=0)

                /* grad_ v */

                Vector grad_v = vector_concatenate(&grad_v_cp, &grad_v_w); 
            
            } else{ //BSPLINE Surface
                Matrix BF_support_temp;//Matrix BF_supprt_temp = der_BSPLINE(IND_mask_active,BF_support) à rajouter 
                // même calcul qu'en haut
                Vector ELEMENTS_8 = col(ELEMENTS, 8); //ELEMENTS[:,8]
                Vector ELEMENTS_10= col(ELEMENTS,10); //ELEMENTS[:,10]
                Vector vol_temp = Vec_product(ELEMENTS_8 , ELEMENTS_10); // ELEMENTS[BF_mask,8]*ELEMENTS[BF_mask,10]

                //grad_v = np.sum(sym_coef_temp*BF_support_temp*vol_temp,axis=0)
                Matrix grad_v_temp = matrix_vector_multiply(&BF_support_temp, &vol_temp); 


            }

        } else{ //DIM ==3
            
            if(NURBS == 1){ //NURBS surface
                Matrix der_CP, der_W;
                Vector BF_mask;
                der_NURBS(local_support,BF_supprt,IND_mask_active,IND_mask,IND_mask_tot,rho_e,P_rho,W,DIM,&der_CP,&der_W, &BF_mask) ;

                /* vol_temp */
                
                Vector vol_temp = extractValues(ELEMENTS, &BF_mask.data, 12, BF_mask.length); //ELEMENTS[BF_mask,12]

                /* grad_v_cp */
                
                //à modifier pour prendre en compte sym_coef
                Matrix grad_v_cp_temp = matrix_vector_multiply(&der_CP, &vol_temp);
                Vector grad_v_cp;
                sum_axis_0(&grad_v_cp_temp, &grad_v_cp); //np.sum(sym_coef_temp*der_CP*vol_temp,axis=0)


                /* grad_v_w */

                //à modifier pour prendre en compte sym_coef
                Matrix grad_v_w_temp = matrix_vector_multiply(&der_W, &vol_temp);
                Vector grad_v_w;
                sum_axis_0(&grad_v_w_temp, &grad_v_w); //np.sum(sym_coef_temp*der_W*vol_temp,axis=0)

                /* grad_ v */

                Vector grad_v = vector_concatenate(&grad_v_cp, &grad_v_w);

            } else{ //BSPLINE surface

                Matrix BF_support_temp;//Matrix BF_supprt_temp = der_BSPLINE(IND_mask_active,BF_support) à rajouter 
                // même calcul qu'en haut
                Vector vol_temp = col(ELEMENTS, 12); //ELEMENTS[:,12]

                //grad_v = np.sum(sym_coef_temp*BF_support_temp*vol_temp,axis=0)
                Matrix grad_v_temp = matrix_vector_multiply(&BF_support_temp, &vol_temp); 

            }

        }
    }
}