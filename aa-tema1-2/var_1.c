#include <stdio.h>
#include <stdlib.h>
#include<limits.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#define READ "r+"
#define WRITE "w"
#define IN "dijkstra.in"
#define OUT "dijkstra.out"

typedef struct node{
    int value;
    int cost;
    struct node* next;
    struct node* down;

}node;

typedef struct distance{

    int cost;
    int node;

}distance;

typedef struct graph{
    int m;
    int n;
    int source;
    int destination;
}graph;


void copy(distance* a, distance* b, int n){

    int i;
    for(i = 0; i < n; i++){
        a[i].cost = b[i].cost;
        a[i].node = b[i].node;

    }

}

void initializeMatrix(int** a, int size){

    int i,j;
    for(i = 0; i < size; i++)
        for(j = 0; j < size; j++)
            a[i][j] = -1;


}

void printArray(int* a, int n){
    int i;
    for(i = 0; i < n; i++)
        printf("%d ", a[i]);
    printf("\n");
}


int popFirstElement(int* a, int size, long long int* k){

    int nr = a[0], i;
    *k += 3;    /// 2 atribuiri( si din for) + o accesare

    for(i = 1; i < size; i++){
        a[i-1] = a[i];
        *k+=7;
    }

    a[i-1] = INT_MAX;
    *k+=4; /// for, atribuire si accesare

    return nr;
}

void swap(int* v, int i, int j){

    int aux;
    aux = v[i];
    v[i] = v[j];
    v[j] = aux;

}

void insertSorted(int* v, distance* dist, int cost, int node, int size, long long int* k){

    int i, j;
    *k+=1;
     for(i = 0; i < size - 1; i++){
        *k+=4;
        *k+=2;   //antet if
        if(v[0] == INT_MAX){    // daca vectorul este gol
            v[0] = node;
            *k+=2;
            break;
        }

        *k+=2;
        if(v[i] == INT_MAX){   // daca inserez la final
            v[i] = node;
            *k+=2;
            break;
        }

        *k+=7;
        if(v[i] != INT_MAX && cost < dist[v[i]].cost){
            /**Acum ca am gasit pozitia mut elementele la dreapta*/
            *k+=1;
            for(j = size -1; j > i; j--){
                *k+=3;
                v[j] = v[j-1];
                *k+=4;
            }
            *k+=1;
            break;
        }
     }

     *k+=2;
      v[i] = node;
     *k+=2;
}

int isInArray(int* v, int nr, int size, long long int* k){

    int i;
    *k+=1;
    for(i = 0; i < size; i++){
        *k+=5;
        if(v[i] == nr)
            return 1;
    }
    *k+=1;
    return 0;

}

void deletePosition(int* v, int cost, int node, int size, long long int* k){

    int i,gasit = 0;
    *k+=2;

    for(i = 0; i < size - 1; i++){
        *k+=4;
        *k+=4;
        if(v[i] == node || gasit == 1){
            gasit = 1;
            v[i] = v[i+1];
            *k+=5;
         }
    }
    *k+=2;
    v[i] = INT_MAX;
    *k+=2;
}

void printDist(distance* dist,graph g){
    int k;
    for(k=0;k<g.n;k++)
        printf("%d ",dist[k].cost);
    printf("\n");
}

void readMatrix(int** a, graph g,FILE* fp){

    int i=2, l=0, k = 0, p = 0;

    for(i = 0; i < g.m; i++){

            fscanf(fp,"%d",&k);
            fscanf(fp,"%d",&p);
            fscanf(fp,"%d",&l);
            a[k][p] = l;
    }

    fclose(fp);

}

void printGraphMatrix(int** a, int n){

    int i, j;
    for(i=0;i<n;i++){
        for(j=0;j<n;j++)
            printf("%d ",a[i][j]);
        printf("\n");
    }
}

graph getGraph(FILE **fp,graph g){
    int l = 0;

    fscanf(*fp,"%d",&l);
    g.m = l;

    fscanf(*fp,"%d",&l);
    g.n = l;

    fscanf(*fp,"%d",&l);
    g.source = l;

    fscanf(*fp,"%d",&l);
    g.destination = l;

    return g;
}


void printPathOnScreen(int* parent, graph g, int n){

    if(n == g.source){
        printf("%d ",n);
        return;
    }

    printPathOnScreen(parent,g,parent[n]);

    if(n != g.destination)
         printf("%d ",n);
    else
        printf("%d\n",n);
}


void printPath(int* parent, graph g, int n, FILE* fp){

    if(n == g.source){
        fprintf(fp,"%d ",n);
        return;
    }

    printPath(parent,g,parent[n],fp);

    if(n != g.destination)
         fprintf(fp,"%d ",n);
    else
        fprintf(fp,"%d\n",n);

}


long long int dijkstra(int** a, graph g, distance* dist, int* parent, int* v)
{
    int i, m, prevCost, prevNode, j, size = 1;
    int* visited = (int*)calloc(g.n, sizeof(int));
    long long int k = 0;

    k+=6; // 2 initializari si o alocare
    k+=1; //(i=0)

    for(i = 0; i < g.n; i++){
        k+=4;   // antet for( egal comparatie accesare incrementare
        dist[i].cost = INT_MAX;
        dist[i].node = i;
        v[i] = INT_MAX;
        parent[i] = g.source;
        k+=11; // accesare vector accesare structura egal
    }
    k+=2; // antet for

    dist[g.source].cost = 0; // 4
    dist[g.source].node = g.source; //5
    v[0] = g.source; //3
    k+=14;

    for(i = 0; i < g.n; i++){

        k+=7; // antet for, antet if, atribuire + accesare ( la visited )

        if(size > 0){
            m = popFirstElement(v,g.n,&k);
            size--;
            k+=3;
        }

        visited[m] = 1;

        k+=2;
        for(j = 0; j < g.n; j++){
            k+=23;   // antet if + antet for

            if(visited[j] == 0 && a[m][j] != -1 && dist[m].cost != INT_MAX && dist[m].cost + a[m][j] < dist[j].cost){

                prevCost = dist[j].cost;
                prevNode = dist[j].node;
                dist[j].cost = dist[m].cost + a[m][j];
                parent[j] = m;

                k+=17;  // ce este sus + antet if

                if(isInArray(v,dist[j].node,g.n,&k) == 1){
                    deletePosition(v,prevCost,prevNode,g.n,&k);
                    insertSorted(v,dist,dist[j].cost,dist[j].node,g.n,&k);
                 }
                else{
                    insertSorted(v,dist,dist[j].cost,dist[j].node,g.n,&k);
                    size++;
                    k+=1;
                }
            }
        }
        k+=2;

    }
    k+=2;
    return k;
}

int main(int argc,char** args)
{

    int **a, *parent, i, *v;
    FILE* fp = fopen(IN,READ);
    graph g;
    distance* dist;

    g = getGraph(&fp,g);

    node* head = (node*)calloc(1,sizeof(node));
    head->value = 0;
    head->next = NULL;

    /** Allocating memory **/

    a = (int**)calloc(g.n,sizeof(int*));
    for(i=0; i < g.n; i++){
        a[i] = (int*)calloc(g.n,sizeof(int));
    }
    initializeMatrix(a,g.n);

    v = (int*)calloc(g.n,sizeof(int));


    dist = (distance*)calloc(g.n,sizeof(distance));
    parent = (int*)calloc(g.n,sizeof(int));


    /** Reading matrix */

    readMatrix(a,g,fp);

    FILE* fp1 = fopen(OUT, WRITE);
    long long int count = dijkstra(a,g,dist, parent,v);

    /**Printing the output*/

    fprintf(fp,"%d\n",dist[g.destination].cost);
    printPath(parent,g,g.destination,fp1);
    printf("1: %d -->",dist[g.destination].cost);
    printPathOnScreen(parent,g,g.destination);
    fprintf(fp1,"%lli\n",count);
    printf("%lli\n",count);
    fclose(fp1);



    return 0;
}
