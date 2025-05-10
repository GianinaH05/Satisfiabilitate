#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void generare(int lit,int cla)
{
    FILE *file=fopen("generat.cnf","w");
    if(!file)
    {
        printf("Nu sa putut deschide fisierul\n");
    }
    else
    {
        fprintf(file,"p cnf %d %d\n",lit,cla);
        srand(time(NULL));
        for(int i=0;i<cla;i++)
        {
            int litcla=(rand()%lit)+1;
            while(litcla<=3)
            {
                litcla=(rand()%lit)+1;
            }
            for(int j=0;j<litcla;j++)
            {
                int var=(rand()%lit)+1;
                int plus=(rand()%2==1)? 1: -1;
                fprintf(file,"%d ",var*plus);
            }
            fprintf(file,"0\n");
        }
        fprintf(file,"0\n");
    }
}
int main()
{
    int literali;
    int clauze;
    printf("Numarul de literali: ");
    scanf("%d",&literali);
    printf("Numar de clauze: ");
    scanf("%d",&clauze);
    generare(literali,clauze);
    printf("S-a generat!");
    return 0;
}