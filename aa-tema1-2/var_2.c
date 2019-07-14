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

void printArray(int* a, int n){
    int i;
    for(i = 0; i < n; i++)
        printf("%d ", a[i]);
    printf("\n");
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


int popFirstElement(int* a, int size, long long int* k){

    int nr = a[0], i;
    *k += 3;

    for(i = 1; i < size; i++){
        a[i-1] = a[i];
        *k+=7;
    }

    a[i-1] = INT_MAX;
    *k+=4;

    return nr;
}


void insertSorted(int* v, distance* dist, int cost, int node, int size, long long int* k){

    int i, j;
    *k+=1;

    for(i = 0; i < size - 1; i++){
        *k+=4;
        *k+=1;   //antet if
        if(v[0] == INT_MAX){    // daca vectorul este gol
            v[0] = node;
            *k+=2;
            break;
        }

        *k+=1;
        if(v[i] == INT_MAX){   // daca inserez la final
            v[i] = node;
            *k+=2;
            break;
        }

        *k+=7;
        if(v[i] != INT_MAX && cost < dist[v[i]].cost){
            /**Acum ca am gasit pozitia mut elementele la dreapta*/
            *k+=2;
            for(j = size -1; j > i; j--){
                *k+=4;
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
        *k+=3;
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

node* insertNode(node* head,int p1, int p2, int cost){

    node* aux = (node*)malloc(sizeof(node));
    aux->next = NULL;
    aux->cost = cost;
    aux->value = p2;
    node* temp = head;

    if(temp->value == -1){
         temp->value = p1;
         goto label;
    }

    while(temp->value != p1 && temp != NULL && temp->value != -1){
        temp = temp->down;
    }

     temp->value = p1;

label:


    while(temp->next != NULL)
        temp = temp->next;

    temp->next = aux;
    aux->next = NULL;

    return head;
}

node* readList(node* head, graph g, FILE* fp){


    int i, l = 0, k = 0, p = 0;


    for(i = 0; i < g.m; i++){

        fscanf(fp,"%d",&k);
        fscanf(fp,"%d",&p);
        fscanf(fp,"%d",&l);
        insertNode(head,k,p,l);

    }

    fclose(fp);
    return head;
}

void printList(node* head){

    node* aux = head->next;
    while(aux!=NULL){
        printf("%d ",aux->value);
        aux = aux->next;
    }
    printf("\n");

}

void printGraphList(node* head){
    node* aux = head;
    while(aux!=NULL){
        printList(aux);
        aux = aux->down;
    }
}

void swap(int* v, int i, int j){

    int aux;
    aux = v[i];
    v[i] = v[j];
    v[j] = aux;

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

    k+=8;   /// 7 operatii elementare are swapul + 1 op de la for
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

node* allocateHeads(node* head, graph g){
    int i = 0;
    node* aux = head;
    aux->value = -1;
    while(i < g.n - 1){
        aux->down = (node*)calloc(1,sizeof(node));
        aux=aux->down;
        aux->value = -1;
        aux->next=NULL;
        aux->down=NULL;
        i++;
    }

    return head;
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

int noNeighbour(int**a, int j, int size){

    int i;
    for(i=0; i < size; i++)
        if(a[j][i] != -1)
            return 0;

    return 1;

}

long long int dijkstraForList(graph g, distance* dist, int* parent,node* head, int* v)
{
    int i, m, size = 1, prevCost, prevNode;
    long long int k = 0;
    node* aux = head;
    int* visited = (int*)calloc(g.n, sizeof(int));

    k+=7; // alocare(4), atribuire(3)
    k+=1;

    for(i = 0; i < g.n; i++){
        k+=4;
        dist[i].cost = INT_MAX;
        dist[i].node = i;
        v[i] = INT_MAX;
        parent[i] = g.source;
        k+=11;
    }
    k+=2;

    dist[g.source].cost = 0;
    dist[g.source].node = g.source;
    v[0] = g.source;

    k+=13;

    for(i = 0; i < g.n; i++){
        k+=7; // antet for, asignare, antet if, visited
        node* temp = aux;

        if(size != 0){
            m = popFirstElement(v,g.n,&k);
            size--;
            k+=3;
        }

        visited[m] = 1;

        while(temp->value != m && temp != NULL && temp->value != -1){
          temp = temp->down;
          k+=9;
        }

        k+=1;

        temp = temp->next;

        while(temp!=NULL){
            k+=17;  // antet if + while

            if(visited[temp->value] == 0  && dist[m].cost != INT_MAX && dist[m].cost + temp->cost < dist[temp->value].cost){

                prevCost = dist[temp->value].cost;
                prevNode = dist[temp->value].node;
                dist[temp->value].cost = dist[m].cost + temp->cost;
                parent[temp->value] = m;
                k+=20;
                if(isInArray(v,dist[temp->value].node,g.n,&k) == 1){
                    deletePosition(v,prevCost,prevNode,g.n,&k);
                    insertSorted(v,dist,dist[temp->value].cost,dist[temp->value].node,g.n,&k);
                }
                else{
                    insertSorted(v,dist,dist[temp->value].cost,dist[temp->value].node,g.n,&k);
                    size++;
                }

            }

            temp = temp->next;
            k++;
        }

    }

    k+=2;
    return k;
}

int main(int argc,char** args)
{

    int *parent, *v;
    FILE* fp = fopen(IN,READ);
    graph g;
    distance* dist;

    g = getGraph(&fp,g);

    node* head = (node*)calloc(1,sizeof(node));
    head->value = 0;
    head->next = NULL;

    /** Allocating memory **/


    v = (int*)calloc(g.n,sizeof(int));
    dist = (distance*)calloc(g.n,sizeof(distance));
    parent = (int*)calloc(g.n,sizeof(int));

	/** Creating the lists*/

    head = allocateHeads(head,g);

	head = readList(head,g,fp);

	FILE* fp1 = fopen(OUT, WRITE);
	long long int count = dijkstraForList(g,dist, parent, head,v);

	/**Printing*/

	fprintf(fp,"%d\n",dist[g.destination].cost);
	printPath(parent,g,g.destination,fp1);
	printf("2: %d -->",dist[g.destination].cost);
	printPathOnScreen(parent,g,g.destination);
	fprintf(fp1,"%lli\n",count);
	printf("%lli\n",count);
	fclose(fp1);




    return 0;
}
