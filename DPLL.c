#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <windows.h>
#include <psapi.h>
#include <math.h>
#include <stdint.h>
#include <inttypes.h>
#define HASH_SIZE 100003
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
    int *singular;
    int m;
    int *pur;
    int *frecventa;
    int maxfrecventa;
    int literalmax;
}setclauze;
int clauzecreate;
int litsingur;
int nesat;
int purlit;
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
    int b=set->nrliterali*2+2;
    set->singular=calloc(b,sizeof(int));
    set->pur=calloc(b,sizeof(int));
    if(set->clauze==NULL)
    {
        printf("nu sa putut crea spatiu la clauze.\n");
    }
    if(set->singular==NULL)
    {
        printf("nu sa putut crea spatiu la vectorul de clauzele cu un singur literal.\n");
    }
    if(set->pur==NULL)
    {
        printf("nu sa putut crea spatiu pentru literalii puri");
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
                    if(set->literalmax==-1)
                    {
                        set->frecventa=calloc(set->nrliterali+1,sizeof(int));
                        if(set->frecventa==NULL)
                        {
                            printf("Nu sa putut crea spatoi pentru vectorul de frecventa.\n");
                        }
                    }
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
                            set->frecventa[abs(set->clauze[numar].literali[i])]++;
                            if(set->frecventa[abs(set->clauze[numar].literali[i])]>set->maxfrecventa)
                            {
                                set->maxfrecventa=set->frecventa[abs(set->clauze[numar].literali[i])];
                                set->literalmax=abs(set->clauze[numar].literali[i]);
                            }
                            if(set->clauze[numar].literali[i]>0)
                            {
                                set->pur[set->nrliterali+set->clauze[numar].literali[i]-1]++;
                            }
                            else
                            {
                                set->pur[abs(set->clauze[numar].literali[i])-1]++;
                            }
                            temp=strtok(NULL," ");
                        }
                        set->clauze[numar].total=numarlit;
                        qsort(set->clauze[numar].literali,set->clauze[numar].total, sizeof(int), cmp_abs);
                        if(numarlit==1)
                        {
                            set->m++;
                            if(set->clauze[numar].literali[0]>0)
                            {
                                set->singular[set->nrliterali+set->clauze[numar].literali[0]-1]++;
                            }
                            else
                            {
                                set->singular[abs(set->clauze[numar].literali[0])-1]++;
                            }
                        }
                        
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
void afisareclauza(clauza a)
{
    for(int i=0;i<a.total;i++)
    {
        printf("%d;",a.literali[i]);
    }
    printf("\n");
}
void singurlit(setclauze *set,int lit)
{
    int i=0;
    while(i<set->n)
    {
        int ok=0,opus=-1;
        for(int j=0;j<set->clauze[i].total;j++)
        {
            if(set->clauze[i].literali[j]==lit)
                ok=1;
            if(set->clauze[i].literali[j]==-lit)
                opus=j;
        }
        if(!ok)
        {
            if(opus>-1)
            {
                if(set->clauze[i].literali[opus]>0)
                    set->pur[set->nrliterali+set->clauze[i].literali[opus]-1]--;
                else
                {
                    set->pur[abs(set->clauze[i].literali[opus])-1]--;
                }
                set->frecventa[abs(set->clauze[i].literali[opus])]--;
                for(int j=opus;j<set->clauze[i].total-1;j++)
                {
                    set->clauze[i].literali[j]=set->clauze[i].literali[j+1];
                }
                set->clauze[i].total--;
                if(set->clauze[i].total==1)
                {
                    set->m++;
                    if(set->clauze[i].literali[0]>0)
                    {
                        set->singular[set->nrliterali+set->clauze[i].literali[0]-1]++;
                    }
                    else
                    {
                        set->singular[abs(set->clauze[i].literali[0])-1]++;
                    }
                }
                if(set->clauze[i].total==0)
                {
                    nesat=1;
                    break;
                }
            }
            i++;
        }
        else
        {
            for(int q=0;q<set->clauze[i].total;q++)
            {
                if(set->clauze[i].literali[q]>0)
                {
                    set->pur[set->nrliterali+set->clauze[i].literali[q]-1]--;      
                }
                else
                {
                    set->pur[abs(set->clauze[i].literali[q])-1]--;
                }
                set->frecventa[abs(set->clauze[i].literali[q])]--;
            }
            free(set->clauze[i].literali);
            for (int q=i;q<set->n-1;q++)
            {
                set->clauze[q]=set->clauze[q+1];
            }
            set->n--;
        }
        if(nesat==1)
            break;
    }
}
setclauze copiere(setclauze *s)
{
    setclauze nou;
    nou.n=s->n;
    nou.nrliterali=s->nrliterali;
    nou.maxn=s->maxn;
    nou.m=s->m;
    nou.clauze=malloc(sizeof(clauza)*s->n);
    nou.maxfrecventa=s->maxfrecventa;
    nou.literalmax=s->literalmax;
    nou.pur=malloc(sizeof(int)*(2*s->nrliterali));
    nou.frecventa=malloc(sizeof(int)*(s->nrliterali+1));
    if(nou.pur==NULL)
    {
       printf("Eroare la creare spatiu pentru copiere\n"); 
    }
    if(nou.clauze==NULL)
    {
        printf("Eroare la creare spatiu pentru copiere\n");
    }
    if(nou.frecventa==NULL)
    {
        printf("Eroare la creare spatiu pentru copiere\n");
    }
    int b=s->nrliterali*2;
    nou.singular=calloc(b,sizeof(int));
    if(nou.singular==NULL)
    {
        printf("Eroare la creare spatiu pentru copiere\n");
    }
    for(int i=0;i<b;i++)
    {
        nou.singular[i]=s->singular[i];
        nou.pur[i]=s->pur[i];
    }
    for(int i=0;i<s->n;i++)
    {
        nou.clauze[i].total=s->clauze[i].total;
        nou.clauze[i].sumaliterali=s->clauze[i].sumaliterali;
        nou.clauze[i].literali=malloc(sizeof(int)*nou.clauze[i].total);
        if(nou.clauze[i].literali== NULL)
        {
            printf("Eroare la copierea clauzei.\n");
        }
        for(int j=0;j<s->clauze[i].total;j++)
        {
            nou.clauze[i].literali[j]=s->clauze[i].literali[j];
        }
    }
    for(int i=0;i<s->nrliterali;i++)
    {
        nou.frecventa[i]=s->frecventa[i];
    }
    return nou;
}
void freeset(setclauze *s)
{
    for(int i=0;i<s->n;i++)
    {
        free(s->clauze[i].literali);
    }
    free(s->clauze);
    free(s->singular);
    free(s->pur);
    free(s->frecventa);
}
int facutsplit;
int clauzavida(setclauze *set)
{
    for(int i=0;i<set->n;i++)
    {
        if(set->clauze[i].total==0)
            return 1;
    }
    return 0;
}
int splitprev[10000];
int cont;
int nuafla(int a)
{
    if(cont==-1)
    {
        cont=0;
        splitprev[cont]=a;
        cont++;
        return 1;
    }
    else
    {
        for(int i=0;i<cont;i++)
        {
            if(splitprev[i]==a)
                return 0;
        }
    }
    splitprev[cont]=a;
    cont++;
    return 1;
}
int dpll(setclauze *set)
{
    if(set->n==1 || set->literalmax==0)
        return 1;
    if(clauzavida(set))
        return 0;
    int nou=1;
    while(nou)
    {
        if(set->m>0)
        {
            for(int i=0;i<set->nrliterali*2;i++)
            {
                if(set->singular[i]>0)
                {
                    if(i>set->nrliterali-1)
                    {    
                        singurlit(set,i-set->nrliterali+1);
                    }
                    else
                    {
                        int q=-(i+1);
                        singurlit(set,q);
                    }
                    set->m--;
                    if(nesat==1)
                    {
                        return 0;
                    }
                    // for(int i=0;i<set->n;i++)
                    // {
                    //     afisareclauza(set->clauze[i]);
                    // }
                }
            }
        }
        else
        {
            for(int i=0;i<set->nrliterali;i++)
            {
                if(set->pur[i] && set->pur[i+set->nrliterali]==0)
                {
                    purlit++;
                    singurlit(set,(-i-1));
                }
                else
                {
                    if(set->pur[i]==0 && set->pur[i+set->nrliterali])
                    {
                        purlit++;
                        singurlit(set,(i-set->nrliterali+1));
                    }
                }
            }
            nou=0;
        }
    }
    int split=set->literalmax;
    if(cont==-1 || nuafla(split))
    {
        nuafla(split);
    setclauze copie1=copiere(set);
    copie1.frecventa[copie1.literalmax]=0;
    copie1.maxfrecventa=0;
    copie1.literalmax=0;
    for(int i=1;i<=copie1.nrliterali;i++)
    {
        if(copie1.frecventa[i]>copie1.maxfrecventa)
        {
            copie1.maxfrecventa=copie1.frecventa[i];
            copie1.literalmax=i+1;
        }
    }
    facutsplit++;
    singurlit(&copie1,split);
    copie1.m=0;
    for(int j=0;j<copie1.n;j++)
    {
        for(int i=0;i<copie1.clauze[j].total;i++)
        {
            if(copie1.clauze[j].literali[i] > 0)
            {
                copie1.pur[copie1.nrliterali + copie1.clauze[j].literali[i] - 1]++;
            }
            else
            {
                copie1.pur[abs(copie1.clauze[j].literali[i]) - 1]++;
            }
            if(copie1.clauze[j].total == 1)
            {
                copie1.m++;
                if(copie1.clauze[j].literali[0] > 0)
                {
                    copie1.singular[copie1.nrliterali + copie1.clauze[j].literali[0] - 1]++;
                }
                else
                {
                    copie1.singular[abs(copie1.clauze[j].literali[0]) - 1]++;
                }
            }
        }
    }
    // for(int i=0;i<copie1.n;i++)
    // {
    //     afisareclauza(copie1.clauze[i]);
    // }
    if(dpll(&copie1))
    {
        freeset(&copie1);
        return 1;
    }
    freeset(&copie1);
    setclauze copie2=copiere(set);
    copie2.frecventa[copie2.literalmax]=0;
    copie2.maxfrecventa=0;
    copie2.literalmax=0;
    for(int i=1;i<=copie2.nrliterali;i++)
    {
         if(copie2.frecventa[i]>copie2.maxfrecventa)
         {
            copie2.maxfrecventa=copie2.frecventa[i];
            copie2.literalmax=i+1;
         }
    }
    facutsplit++;
    singurlit(&copie2,-split);
    copie2.m=0;
    for(int j=0;j<copie2.n;j++)
    {
        for(int i=0;i<copie2.clauze[j].total;i++)
        {
            if(copie2.clauze[j].literali[i] > 0)
            {
                copie2.pur[copie2.nrliterali + copie2.clauze[j].literali[i] - 1]++;
            }
            else
            {
                copie2.pur[abs(copie2.clauze[j].literali[i]) - 1]++;
            }
            if(copie2.clauze[j].total == 1)
            {
                copie2.m++;
                if(copie2.clauze[j].literali[0] > 0)
                {
                    copie2.singular[copie2.nrliterali + copie2.clauze[j].literali[0] - 1]++;
                }
                else
                {
                    copie2.singular[abs(copie2.clauze[j].literali[0]) - 1]++;
                }
            }
        }
    }
    if(dpll(&copie2))
    {
        freeset(&copie2);
        return 1;
    }
    freeset(&copie2);
    }
    return 1;
}
int main()
{
    clauzecreate=0;
    purlit=0;
    nesat=0;
    facutsplit=0;
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
        cont=-1;
        set.maxn=0;
        set.m=0;
        set.literalmax=-1;
        set.maxfrecventa=0;
        set.frecventa=NULL;
        citire(fisier,&set);
        fclose(fisier);
        int rez=dpll(&set);
        if(rez)
            printf("Satisfiabila!\n");
        else
            printf("Nesatisfiabila!\n");
    }
    clock_t end=clock();
    double TimeUsed=((double)(end-start))/CLOCKS_PER_SEC;
    QueryPerformanceCounter(&en);
    double timp=(double)(en.QuadPart - st.QuadPart) / frequency.QuadPart;
    printf("Purliterale create: %d \n",purlit);
    printf("Deapth: %d\n",facutsplit);
    printf("Wall-clock time: %.6f seconds\n", timp);
    printf("CPU Time: %.6f seconds\n",TimeUsed);
    printMemoryUsage();
    return 0;
}