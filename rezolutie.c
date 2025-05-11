#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <windows.h>
#include <psapi.h>
#include <math.h>
#include <stdint.h>
#include <inttypes.h>
typedef struct
{
    int *literali;
    int total;
    int sumaliterali;
}clauza;
typedef struct
{
    clauza *clauze;
    int n;
    int nrliterali;
    int maxn;
}setclauze;
int clauzecreate;
void printMemoryUsage() 
{
    PROCESS_MEMORY_COUNTERS memInfo;
    GetProcessMemoryInfo(GetCurrentProcess(), &memInfo, sizeof(memInfo));
    SIZE_T virtualMemUsed = memInfo.PagefileUsage;
    SIZE_T physicalMemUsed = memInfo.WorkingSetSize;
    printf("Memory Used (RAM): %.2f MB\n", physicalMemUsed / (1024.0 * 1024.0));
}
int cmp_abs(const void *a, const void *b) 
{
    int x=*(int*)a;
    int y=*(int*)b;
    return abs(x)-abs(y);
}
void setare(char * linie,setclauze *set)
{
    char *temp=malloc(strlen(linie)+1);
    if(temp!=NULL)
    {
        strcpy(temp,linie);
        char *nr=strtok(temp," ");
        nr=strtok(NULL," ");
        nr=strtok(NULL," ");
        set->nrliterali=atoi(nr);
        nr=strtok(NULL," ");
        set->n=atoi(nr);
    }
    else
    {
        printf("Nu sa putut crea temp");
    }
    free(temp);
    set->clauze=malloc(sizeof(clauza)*set->n);
    int b=set->nrliterali*2;
    if(set->clauze==NULL)
    {
        printf("nu sa putut crea spatiu.\n");
    }
}
int getnrliterali(char *linie)
{
    char* por=malloc(strlen(linie)+1);
    strcpy(por,linie);
    char *temp=strtok(por," ");
    int numarlit=0;
    int ok=1;
    int prev=0;
    while (temp && atoi(temp)!=0)
    {
        if(atoi(temp)==-prev)
        {
            numarlit=0;
            break;
        }
        prev=atoi(temp);
        numarlit++;
        temp=strtok(NULL," ");
    }
    free(por);
    return numarlit;
}
int tautologie(clauza a)
{
    for(int i=0;i<a.total-1;i++)
    {
        if(a.literali[i]==-a.literali[i+1])
            return 1;
    }
    return 0;
}
void citire(FILE *fisier,setclauze *set)
{
    char linie[300];
        int numar=0;
        while(fgets(linie,sizeof(linie),fisier))
        {
            if(linie[0]!='0'){
            if(linie[0]!='c')
            {
                if(linie[0]=='p')
                {
                    setare(linie,set);
                }
                else
                {
                    int numarlit=getnrliterali(linie);
                    if(numarlit>0)
                    {
                        
                        char *temp=strtok(linie," ");
                        set->clauze[numar].literali=malloc(sizeof(int)*numarlit);
                        set->clauze[numar].sumaliterali=0;
                        if(numarlit>set->maxn)
                            set->maxn=numarlit;
                        for(int i=0;i<numarlit;i++)
                        {
                            set->clauze[numar].literali[i]=atoi(temp);
                            set->clauze[numar].sumaliterali=set->clauze[numar].sumaliterali+set->clauze[numar].literali[i];
                            temp=strtok(NULL," ");
                        }
                        set->clauze[numar].total=numarlit;
                        if(numarlit!=1)
                            qsort(set->clauze[numar].literali,set->clauze[numar].total, sizeof(int), cmp_abs);
                        numar++;
                    }
                    else
                    {
                        set->n--;
                    }
                }

            }
        }
    }
}
int clauzanoua(setclauze set,clauza a)
{
    if(a.total>set.maxn)
        return 0;
    else
    {
        for(int i=0;i<set.n;i++)
        {
            if(set.clauze[i].total==a.total && set.clauze[i].sumaliterali==a.sumaliterali)
            {
            int egale=1;
            for (int j=0; j<a.total; j++) 
            {
                if (set.clauze[i].literali[j] != a.literali[j]) 
                {
                    egale = 0;
                    break;
                }
            }
            if (egale) 
            {
               return 0; 
            }
            }
        }
    }
    return 1;
}
int literalCom(clauza a,clauza b)
{
    int nr=0;
    for(int i=0;i<a.total;i++)
    {
        for(int j=0;j<b.total;j++)
        {
            if(b.literali[j]==-a.literali[i])
                {
                    nr++;
                    if(nr>1)
                        return 0;
                }
        }
    }
    if(nr==1)
        return 1;
    return 0;
}
clauza CreareClauza(clauza a,clauza b)
{
    clauza c;
    int p=0;
    int i=0,j=0;
    c.literali=malloc(sizeof(int)*(a.total+b.total));
    c.sumaliterali=0;
    while(i<a.total &&j<b.total)
    {
        if(a.literali[i]==-b.literali[j])
        {
            i++;
            j++;
        }
        else
        {    
            if(abs(a.literali[i])<abs(b.literali[j]))
            {
                c.literali[p]=a.literali[i];
                c.sumaliterali+=a.literali[i];
                p++;
                i++;
            }
            else
            {
                if(abs(a.literali[i])>abs(b.literali[j]))
                {
                   c.literali[p]=b.literali[j];
                   c.sumaliterali+=b.literali[j];
                    p++;
                    j++; 
                }
                else
                {
                    c.literali[p]=b.literali[j];
                    c.sumaliterali+=b.literali[j];
                    p++;
                    j++;
                    i++;
                }
            }
        }
    }
    while(i<a.total)
    {
        c.literali[p]=a.literali[i];
        c.sumaliterali+=a.literali[i];
        i++;
        p++;
    }
    while(j<b.total)
    {
        c.literali[p]=b.literali[j];
        c.sumaliterali+=b.literali[j];
        j++;
        p++;
    }
    if (p == 0) 
    {
        free(c.literali);
        c.literali = NULL;
    } 
    else 
    {
        c.literali = realloc(c.literali, sizeof(int) * p);
        if(c.literali==NULL)
        {
            printf("Nu sa putut realoca memorie\n");
            p=0;
            return c;
        }
    }
    c.total=p;
    return c;
}
void afisareclauza(clauza a)
{
    for(int i=0;i<a.total;i++)
    {
        printf("%d;",a.literali[i]);
    }
    printf("\n");
}
int rezolutie(setclauze *set)
{
    int nou=1;
    while(nou)
    {
        nou=0;
        for(int i=0;i<set->n-1;i++)
        {
            int dplit=0;
            for(int j=i+1;j<set->n;j++)
            {
                if(literalCom(set->clauze[i],set->clauze[j]))
                {
                    clauza c=CreareClauza(set->clauze[i],set->clauze[j]);
                    if(c.total!=0)
                    {
                        if(!tautologie(c) && clauzanoua(*set,c))
                        {
                            clauzecreate++;
                            if(clauzecreate%90000==0)
                            {
                                return 1;
                            }
                            clauza *temp=realloc(set->clauze,sizeof(clauza)*(set->n+1));
                            if(temp==NULL)
                            {
                                printf("Eroare la realocarea vectorului de clauze.\n");
                                free(c.literali);
                                continue;
                            }
                            set->clauze=temp;
                            set->clauze[set->n]=c;
                            set->n++;
                            nou=1;
                        }
                        else
                            free(c.literali);
                    }
                    else
                        return 0;
                }
            }
        }
    }
    return 1;
}
int main()
{
    clauzecreate=0;
    LARGE_INTEGER frequency, st, en;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&st);
    clock_t start=clock();
    FILE *fisier=fopen("exemplu.cnf","r");
    setclauze set;
    if(!fisier)
    {
        printf("Fisierul nu a putut fi deschis");
    }
    else
    {
        set.maxn=0;
        citire(fisier,&set);
        fclose(fisier);
        int rez=rezolutie(&set);
        if(rez)
            printf("Satisfiabila!\n");
        else
            printf("Nesatisfiabila!\n");
    }
    clock_t end=clock();
    double TimeUsed=((double)(end-start))/CLOCKS_PER_SEC;
    QueryPerformanceCounter(&en);
    double timp=(double)(en.QuadPart - st.QuadPart) / frequency.QuadPart;
    printf("Nr clauze create: %d \n",clauzecreate);
    printf("Wall-clock time: %.6f seconds\n", timp);
    printf("CPU Time: %.6f seconds\n",TimeUsed);
    printMemoryUsage();
    return 0;
}