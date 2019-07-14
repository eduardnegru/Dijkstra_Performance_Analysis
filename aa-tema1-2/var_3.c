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

void initializeMatrix(int** a, int size){

    int i,j;
    for(i = 0; i < size; i++)
        for(j = 0; j < size; j++)
            a[i][j] = -1;


}

void insertMinHeap(int* v, distance* dist, int cost, int node, int* dim, long long int* k){

    int temp;

    *dim = *dim+1;
    v[*dim] = node;
    int i = *dim;

    *k+=5;

    while(i>1 && dist[v[i/2]].cost > cost){  /// cat timp parintele > ce vreau sa introduc

            *k+=6;
            temp=v[i];
            v[i]=v[i/2];
            v[i/2]=temp;
            i=i/2;
            *k+=11;

    }

}

int getCostAt(node* head, int m, int n){

    int i = 0;
    node* aux = head;
    for(i = 0; i < m; i++)
        aux = aux->down;

    aux = aux->next;
    while(aux->next!=NULL){
        if((aux->value -1) == n)
           return aux->cost;
        aux = aux->next;
    }

    return aux->value == n ? aux->cost : 0;

}


void swap(int* v, int i, int j){

    int aux;
    aux = v[i];
    v[i] = v[j];
    v[j] = aux;

}


int getMinimumElement(int* v,distance* dist, int size, int* dim, long long int* k){

    int i = 1, l, r, nr = v[1];
    v[i] = v[*dim];
    *dim = *dim - 1;

    *k+=8;

    while(2*i <=(*dim) && 2*i+1 <=(*dim)){
        l=2*i;
        r=2*i+1;
        *k+=11;

        *k+=15;
        if(dist[v[l]].cost >= dist[v[i]].cost && dist[v[r]].cost >= dist[v[i]].cost)
            return nr;

        *k+=15;
        if(dist[v[l]].cost < dist[v[i]].cost || dist[v[r]].cost < dist[v[i]].cost){    /// inca nu e heap
            *k+=7;
            if(dist[v[l]].cost < dist[v[r]].cost){

                swap(v,l,i);
                i=l;
                *k+=8;
            }
            else if(dist[v[l]].cost > dist[v[r]].cost){

                 swap(v,r,i);
                 i=r;
                 *k+=8;
            }
            else if(dist[v[l]].cost == dist[v[r]].cost){
                *k+=7;
                *k+=3;
                 if(v[l] < v[r]){

                    swap(v,l,i);
                    i=l;
                    *k+=8;
                 }
                 else if(v[l] > v[r]){

                    swap(v,r,i);
                    i=r;
                    *k+=8;
                 }
            }
        }
    }

    return nr;
}


int isInHeap(int* v,int node,int* heapSize, long long int* k){

    int i;
    *k+=1;
    for(i=1; i <= *heapSize; i++){
        *k+=3;
        if(v[i] == node)
            return 1;
    }
    *k+=1;
    return 0;
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



int partition(int* a, int start, int end, long long int* k){

    int pindex = start, i, pivot = a[end];
    *k+=4;

    for(i = start; i < end; i++){
        *k+=4;
        if(a[i] <= pivot){
            swap(a,i,pindex);
            pindex++;
            *k+=10; // 7 de la swap
        }
    }

    k+=8;
    swap(a,pindex,end);
    return pindex;
}

void quicksort(int* a, int start, int end, long long int* k){

    *k+=1;
    if(start < end){
        int pivot = partition(a, start, end, k);
        quicksort(a,start,pivot-1,k);
        quicksort(a,pivot+1,end,k);
        *k+=4;
    }

}

void sortTopHeap(int* v, int heapSize, distance* dist, long long int* k){

    int i, j;
    int min = dist[v[1]].cost;
    *k+=5;
     for(i = 1; i <= heapSize; i++){
       *k+=3;
        if(dist[v[i]].cost != min){
            *k+=4;
            break;
        }
    }
    *k+=5;  // for + if + alocare

    // i este pozitia pana unde exista noduri cu acelasi cost

    if(i == 2)  // exista doar un nod
        return;

    int* p = (int*)calloc(i,sizeof(int));
    *k+=1;
    for(j = 1; j < i; j++){
      *k+=6;
      p[j] = v[j];
    }
    *k+=1;

    quicksort(p, 1, i - 1, k);
    *k+=1;
    for(j = 1; j < i; j++){
      *k+=6;
      v[j] = p[j];
    }
    *k+=1;

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

void printHeap(int* heap, int n){
    int i;
    for(i = 1; i <= n; i++)
        printf("%d ", heap[i]);
    printf("\n");
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

void insertNeighbour(int* v, int cost, int node, distance* dist, int* visited, graph g, int* heapSize,  long long int* k){

    int i;
    *k+=4;

     if(visited[node] == 0 && isInHeap(v,node,heapSize,k)==0){

        insertMinHeap(v,dist,cost,node,heapSize,k);

    }
    else if(visited[node] == 0 && isInHeap(v,node,heapSize,k)==1){

        int* b = (int*)calloc(*heapSize+1,sizeof(int));

        *k+=9;  // antet else if + alocare(4)
        *k+=1;
        for(i=1; i <= *heapSize; i++){
            *k+=5;
            b[i] = v[i];
            v[i] = 0;
        }
        *k+=3;

        int aux = *heapSize;
        *heapSize = 0;
        *k+=1;
        for(i=1; i<=aux; i++){
            *k+=4;
            insertMinHeap(v,dist,dist[b[i]].cost,b[i],heapSize,k);
        }
        *k+=1;

    }

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


long long int dijkstraWithMinheapGraph(int** a, graph g, distance* dist, int* parent, int* v)
{
    int i,m,j;
    int heapSize = 0;
    long long int k = 0;
    int* visited = (int*)calloc(g.n, sizeof(int));

    k+=7;

    for(i = 0; i < g.n; i++){
        k+=4;
        dist[i].cost = INT_MAX;
        dist[i].node = i;
        v[i] = -1;                  // am adaugat dupa /??
        parent[i] = g.source;
        k+=11;
    }

    k+=2;

    dist[g.source].cost = 0;
    dist[g.source].node = g.source;
    v[1] = g.source;
    heapSize++;

    k+=14;
    k+=1;
    for(i = 0; i < g.n; i++){

        k+=7; //  if, antet for si visited

        /**Sortez heapul astef incat nodul minim sa fie primul (daca am cost egal)"*/

        sortTopHeap(v, heapSize, dist, &k);

        if(heapSize != 0){
            m = getMinimumElement(v,dist, g.n, &heapSize,&k);
            k+=1;
        }

        visited[m] = 1;

        for(j = 0; j < g.n; j++){
            k+=23;

            if(visited[j] == 0 && a[m][j] != -1 && dist[m].cost != INT_MAX && dist[m].cost + a[m][j] < dist[j].cost){
                dist[j].cost = dist[m].cost + a[m][j];
                parent[j] = m;
                k+=10;
                insertNeighbour(v,dist[j].cost,dist[j].node,dist,visited,g,&heapSize,&k);

            }
        }
        k+=2;
    }
    k+=2;
    return k;
}


int main(int argc,char** args)
{

    int **a, *parent, i, *heap;
    FILE* fp = fopen(IN,READ);
    graph g;
    distance* dist;

    g = getGraph(&fp,g);


    /** Allocating memory **/

    a = (int**)calloc(g.n,sizeof(int*));
    for(i=0; i < g.n; i++){
        a[i] = (int*)calloc(g.n,sizeof(int));
    }
    initializeMatrix(a,g.n);

    heap = (int*)calloc(g.n + 1,sizeof(int));
    dist = (distance*)calloc(g.n,sizeof(distance));
    parent = (int*)calloc(g.n,sizeof(int));


    readMatrix(a,g,fp);
    FILE* fp1 = fopen(OUT, WRITE);
    long long int count = dijkstraWithMinheapGraph(a,g,dist, parent,heap);
    fprintf(fp,"%d\n",dist[g.destination].cost);
    printPath(parent,g,g.destination,fp1);
    printf("3: %d -->",dist[g.destination].cost);
    printPathOnScreen(parent,g,g.destination);
    fprintf(fp1,"%lli\n",count);
    printf("%lli\n",count);
    fclose(fp1);




    return 0;
}
