#if WIN32
#define _USE_MATH_DEFINES
#endif

#include <math.h>

#include "matrice.h"
#include "symmatrice.h"
#include "geometry.h"
#include "operateurs.h"

void assemble_matrice( geometry &geo, symmatrice &mat)
{
    int offset=0;

    for(int c=0;c<geo.nb()-1;c++)
    {
        int offset0=offset;
        int offset1=offset+geo.getM(c).nbr_pts();
        int offset2=offset+geo.getM(c).nbr_pts()+geo.getM(c).nbr_trg();
        int offset3=offset+geo.getM(c).nbr_pts()+geo.getM(c).nbr_trg()+geo.getM(c+1).nbr_pts();

        //Computing S block first because it's needed for the corresponding N block
        if(c==0) operateurS(geo,c,c,mat,offset1,offset1);
        operateurS(geo,c+1,c,mat,offset3,offset1);
        operateurS(geo,c+1,c+1,mat,offset3,offset3);

        //Computing N block
        if(c==0) operateurN(geo,c,c,mat,offset0,offset0,offset1,offset1);
        operateurN(geo,c+1,c,mat,offset2,offset0,offset3,offset1);
        operateurN(geo,c+1,c+1,mat,offset2,offset2,offset3,offset3);

        //Computing D block
        if(c==0) operateurD(geo,c,c,mat,offset1,offset0);
        if(c!=geo.nb()-2) operateurD(geo,c+1,c,mat,offset3,offset0);
        operateurD(geo,c,c+1,mat,offset1,offset2);
        if(c!=geo.nb()-2) operateurD(geo,c+1,c+1,mat,offset3,offset2);

        offset=offset2;
    }

    //Blocks multiplications
    //Because only half the matrix is stored, only the lower part of the matrix is treated
    offset=0;
    double K=1.0/(4*M_PI);
    for(int c=0;c<geo.nb()-1;c++)
    {
        int offset0=offset;
        int offset1=offset+geo.getM(c).nbr_pts();
        int offset2=offset+geo.getM(c).nbr_pts()+geo.getM(c).nbr_trg();
        int offset3=offset+geo.getM(c).nbr_pts()+geo.getM(c).nbr_trg()+geo.getM(c+1).nbr_pts();
        int offset4=offset+geo.getM(c).nbr_pts()+geo.getM(c).nbr_trg()+geo.getM(c+1).nbr_pts()+geo.getM(c+1).nbr_trg();

        //Each operator is scaled with the appropriate constant

        //Column 1
        if(c==0) mult(mat,offset0,offset0,offset1,offset1,(geo.sigma_in(c)+geo.sigma_out(c))*K);
        if(c==0) mult(mat,offset1,offset0,offset2,offset1,-2.0*K);
        mult(mat,offset2,offset0,offset3,offset1,(geo.sigma_out(c))*K);
        mult(mat,offset3,offset0,offset4,offset1,-1.0*K);

        //Column 2
        if(c==0) mult(mat,offset1,offset1,offset2,offset2,(1.0/geo.sigma_in(c)+1.0/geo.sigma_out(c))*K);
        mult(mat,offset2,offset1,offset3,offset2,-1.0*K);
        mult(mat,offset3,offset1,offset4,offset2,(1.0/geo.sigma_out(c))*K);

        //Column 3
        mult(mat,offset2,offset2,offset3,offset3,(geo.sigma_in(c+1)+geo.sigma_out(c+1))*K);
        mult(mat,offset3,offset2,offset4,offset3,-2.0*K);

        //Column 4
        mult(mat,offset3,offset3,offset4,offset4,(1.0/geo.sigma_in(c+1)+1.0/geo.sigma_out(c+1))*K);

        offset=offset2;
    }
}



void assemble_EITmatrice( geometry &geo, symmatrice &mat)
{
    // same as assemble_matrice without multiplication of blocks
    int offset=0;

    for(int c=0;c<geo.nb()-1;c++)
    {
        int offset0=offset;
        int offset1=offset+geo.getM(c).nbr_pts();
        int offset2=offset+geo.getM(c).nbr_pts()+geo.getM(c).nbr_trg();
        int offset3=offset+geo.getM(c).nbr_pts()+geo.getM(c).nbr_trg()+geo.getM(c+1).nbr_pts();

        //Computing S block first because it's needed for the corresponding N block
        if(c==0) operateurS(geo,c,c,mat,offset1,offset1);
        operateurS(geo,c+1,c,mat,offset3,offset1);
        operateurS(geo,c+1,c+1,mat,offset3,offset3);

        //Computing N block
        if(c==0) operateurN(geo,c,c,mat,offset0,offset0,offset1,offset1);
        operateurN(geo,c+1,c,mat,offset2,offset0,offset3,offset1);
        operateurN(geo,c+1,c+1,mat,offset2,offset2,offset3,offset3);

        //Computing D block
        if(c==0) operateurD(geo,c,c,mat,offset1,offset0);
        if(c!=geo.nb()-2) operateurD(geo,c+1,c,mat,offset3,offset0);
        operateurD(geo,c,c+1,mat,offset1,offset2);
        if(c!=geo.nb()-2) operateurD(geo,c+1,c+1,mat,offset3,offset2);

        offset=offset2;

    }

}

void deflat(genericMatrix &M, int start, int end, double coef)
{// deflat the matrix 

    for(int i=start;i<=end;i++)
    {
        for(int j=i;j<=end;j++)
        {
            M(i,j)+=coef;
        }
    }
}
