#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double f (double x){
    return cbrt(x)*10;
    }

void clear_memory ( double **matrix, int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        free (matrix[i]);
    }
    free (matrix);
}

void Bin_write ( FILE *outbin, double x1, unsigned int N, double delta)
{
    unsigned int i;
    double y = f(x1);
    for (i = 0; i < N; i++)
    {
        fwrite(&x1, sizeof(double), 1, outbin);
        fwrite(&y, sizeof(double), 1, outbin);
        x1 += delta;
        y = f (x1);
    }
}

int main()
{
    double x1, x2, delta, tempd;
    unsigned int n, i, tempi=0;
    char group_name[10];
    double **res;

    FILE *input, *output, *outbin;

    printf("\n\t*==========================================*");
    printf("\n\t|                  lab5                    |");
    printf("\n\t*------------------------------------------*");
    printf("\n\t|                File work                 |");
    printf("\n\t*==========================================*\n");

    input = fopen("input.dat", "r");
    if( input == NULL ){
      printf("Cannot open a file!!!\n");
      return 0;
    }

   fgets(group_name, 10, input);
   fscanf(input, "%lf", &x1);
   fscanf(input, "%lf", &x2);
   fscanf(input, "%u", &n);
   fscanf(input, "%lf", &delta);

   printf("\n\t%s", group_name);
   printf("\tx1 = %lf \n\tx2 = %lf \n\tn = %d \n\tdelta = %lf\n", x1, x2, n, delta);

   if (n == 0) {
        n = ((x2-x1)/delta)+1;
   }
   else
   {
        if (n>1) {
            delta = (x2-x1)/(n-1);
        }
        else
        {
            n=1;
            delta=0;
        }
   }

   res = (double **) calloc (n, sizeof(double*)); // Виділення пам'яті під двовимірний масив

   if (res == 0) {
        printf("Memory has not been allocated\n");
        return 0;
   }

   for (i = 0; i < n; i++)
   {
       res[i] = (double *) calloc (2, sizeof (double));
       if (res [i] == NULL){
        printf ("Memory has not been allocated\n");
        return 0;
       }
   }

   for (i = 0; i < n; i++)  // Заповнення массиву
    {
        res [i][0] = x1 + (double) i*delta;
        res [i][1] = f (x1 + (double) i*delta);
    }

   output = fopen("result.txt", "w");
    if( output == NULL ){
      printf("Cannot open a file!!!\n");
      return 0;
    }

    fprintf (output, "\n\t*==========================================*");
    fprintf (output, "\n\t|     N      *       X      *     F(X)     |");
    fprintf (output, "\n\t*==========================================*\n");

    for (i = 0; i < n; i++)     //Заповнення даними із масиву текстового файлу
    {

        fprintf(output, "\t+------------+--------------+--------------+\n");
        fprintf(output, "\t|     %d     | %.8f | %.8f |\n", i+1, res[i][0], res[i][1]);
    }
    fprintf(output, "\t+------------+--------------+--------------+\n");

    outbin = fopen("result.bin", "ab");
    if( outbin == NULL ){
      printf("Cannot open a file!!!\n");
      return 0;
    }

    Bin_write (outbin, x1, n, delta);

    outbin = fopen("result.bin", "rb");
    if( outbin == NULL ){
      printf("Cannot open a file!!!\n");
      return 0;
    }

    printf ("\n\t            Data from .bin file             ");
    printf ("\n\t*==========================================*");
    printf ("\n\t|     N      *       X      *     F(X)     |");
    printf ("\n\t*==========================================*");

    for (i = 0; i < n; i++)
    {
        printf("\n\t+------------+--------------+--------------+\n");
        printf ("\t|     %d     |", i+1);
        fread( &tempd, sizeof(double), 1, outbin);
        printf(" %.8f |", tempd);
        fread( &tempd, sizeof(double), 1, outbin);
        printf(" %.8f |", tempd);
    }

    printf("\n\t+------------+--------------+--------------+\n");

    printf ("\n\n\t\tArray:\n");
    printf("\t x  \t\t f(x)");
    for (i = 0; i < n; i++)
    {
        printf ("\n\t%lf", res [i][0]);
        printf ("\t%lf", res [i][1]);
    }

   clear_memory(res, n);

}
