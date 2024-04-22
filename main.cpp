#include <stdio.h>
#include <math.h>
#include <iostream>
#define N 10000
#include <omp.h>



const int num_threads = 8;
const int iXmax = N;
const int iYmax = N;

const double CxMin=-2.5;
const double CxMax=1.5;
const double CyMin=-2.0;
const double CyMax=2.0;
/* */
double PixelWidth=(CxMax-CxMin)/iXmax;
double PixelHeight=(CyMax-CyMin)/iYmax;
const int IterationMax=200;
const double EscapeRadius=2;
double ER2=EscapeRadius*EscapeRadius;
const int MaxColorComponentValue=255;


unsigned char color[iYmax][iXmax][3];


int main()
{
    double start, stop;
    int iX,iY;
    int Iteration;
    double Cx,Cy;
    double Zx, Zy;
    double Zx2, Zy2;
    int id, i;

    FILE * fp;
    char *filename="new1.ppm";
    char *comment="# ";/* comment should start with # */

    /* bail-out value , radius of circle ;  */

    /*create new file,give it a name and open it in binary mode  */
    fp= fopen(filename,"wb"); /* b -  binary mode */
    /*write ASCII header to the file*/
    fprintf(fp,"P6\n %s\n %d\n %d\n %d\n",comment,iXmax,iYmax,MaxColorComponentValue);

    omp_set_num_threads(num_threads);
    start = omp_get_wtime();

#pragma omp parallel private(id)
    {
        id = omp_get_thread_num();
        std::cout << id << std::endl;
#pragma omp for private(iX, iY, Cx, Cy, Zx, Zy, Zx2, Zy2, Iteration) schedule(static) // Liczba iteracji wątków

        for(iY=0;iY<iYmax;iY++) // Zrównoleglić
        {
            Cy=CyMin + iY*PixelHeight;
            if (fabs(Cy)< PixelHeight/2) Cy=0.0; /* Main antenna */
            for(iX=0;iX<iXmax;iX++)
            {
                Cx=CxMin + iX*PixelWidth;
                /* initial value of orbit = critical point Z= 0 */
                Zx=0.0;
                Zy=0.0;
                Zx2=Zx*Zx;
                Zy2=Zy*Zy;
                /* */
                for (Iteration=0;Iteration<IterationMax && ((Zx2+Zy2)<ER2);Iteration++)
                {
                    Zy=2*Zx*Zy + Cy;
                    Zx=Zx2-Zy2 +Cx;
                    Zx2=Zx*Zx;
                    Zy2=Zy*Zy;
                };
                /* compute  pixel color (24 bit = 3 bytes) */
                if (Iteration==IterationMax)
                { /*  interior of Mandelbrot set = black */
                    color[iY][iX][0]=0;
                    color[iY][iX][1]=0;
                    color[iY][iX][2]=0;
                }
                else
                { /* exterior of Mandelbrot set = white. Pokolorowanie tła według wątka */
                    color[iY][iX][0]=32*id; /* Red*/
                    color[iY][iX][1]=32*id;  /* Green */
                    color[iY][iX][2]=255;/* Blue */
                };
                /*write color to the file*/
                // Zapis do pliku w pętli
            }
        }
    }

    stop = omp_get_wtime();

//    std::cout<<"Launched from the main\n";


    std::cout << "Time: " << stop - start << std::endl;


    fwrite(color,1,3*iYmax*iXmax,fp);
    fclose(fp);
    return 0;
}