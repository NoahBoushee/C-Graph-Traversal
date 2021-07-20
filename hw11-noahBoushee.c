//Noah Boushee
//need to ensure that data file runs with large data sets, 400 and 800

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define node listNode
//structs needed by the function
struct graphNode{
    char name[84];
    int arcCount;
    float probabilities[10];
    struct graphNode *arcs[10];
};

struct listNode{
    char name[84];
    struct listNode *next;
    struct listNode * previous;
    struct graphNode *graph;
};
//initializes nodes to null and dummy float array, intialization things
void initializeGraphNode(struct graphNode** point, char data[]){
    struct graphNode *newNode = (struct graphNode*)malloc(sizeof(struct graphNode));    
    strcpy(newNode->name, data);
    float probs[10] = { 0.0 };
    int i;
    struct graphNode *dummyNodes[10];//may be issues with memcpy, will have to check later
    for (i = 0; i <9;i++){
        dummyNodes[i] = NULL;
    }
    newNode-> arcCount = 0;
    memcpy(newNode->probabilities, probs, sizeof(probs));
    memcpy(newNode->arcs, dummyNodes, sizeof(dummyNodes));
    (*point) = newNode;

}
//adds graph node and links them together
void addGraphNode(struct node**head, char data1[], char data2[], float data3){
    struct node *current = NULL, *index = NULL;
    for(current = (*head); current->next != NULL; current = current->next){    
         if(strcmp(current->name, data1) == 0){
             //Checks if the current name is same as first data 
             current->graph->arcCount +=1;
             current ->graph->probabilities[current->graph->arcCount -1] = data3;
             for(index = (*head); index->next != NULL; index = index->next){    
                    if(strcmp(index->name, data2) == 0){
                           current->graph->arcs[current->graph->arcCount -1] = index->graph; 
                           break;
                    }
             }
            break;
         }  
    }
   
}

void addNode(struct node** head, struct node** tail, char data[], struct graphNode* point){  
     struct node *newNode = (struct node*)malloc(sizeof(struct node));  
     strcpy(newNode->name, data);
     newNode->graph = point;  
     if((*head) == NULL) {    
        (*head) = (*tail) = newNode;   
        (*head)->previous = NULL;    
        (*tail)->next = NULL; 
      }  
      else {  
        (*tail)->next = newNode;  
        newNode->previous = (*tail);  
        (*tail) = newNode;    
        (*tail)->next = NULL;  
      } //this adds nodes, edited from earlier program
}  
 
     
//ascending order sort 
void sortList(struct node** head) {  
    struct node *current = NULL, *index = NULL;
    struct node *temp2 = (struct node*)malloc(sizeof(struct node));
    char temp[84];    
    //Current = head
    for(current = (*head); current->next != NULL; current = current->next){    
        for(index = current->next; index != NULL; index = index->next) {  
             //If current greater than index, swap the datas
             if(strcmp(current->name, index->name)>0){  
                  temp2 =  current;  
                  current = index;  
                  index = temp2;  
              }  
         }  
     }  
} 
//prints stuff, the first one is pretty much uneeded
void print(struct node** head) {   //prints node in order, has dummy node for pointing purposes
    struct node *current = (*head);
    struct node *last; 
    printf("Forwards print of LL sorted\n"); 
    while(current != NULL) {  
        printf("%s data node name %s\n",current->name, current->graph->name);  
        last = current;
        current = current->next;  
     } 
} 

void print2(struct node** head) {   //prints the nodlet things
    struct node *current = (*head);
    struct node *last; 
    int i;
    printf("Forwards print of LL sorted\n"); 
    while(current != NULL) {  
        printf("%s %d\n",current->graph->name, current->graph->arcCount);  
        printf("Probabilities\n");
        for (i = 0; i <current->graph->arcCount; i++){
            printf("%.2f %s\n", current->graph->probabilities[i], current->graph->arcs[i]->name);
        }
        last = current;
        current = current->next;  
     } 
}

float drunkWalk(struct node**head, char start[]){//need to ensure that random function is seeded correctly
    float probTotal = 0.0;
    int totalArcs = 0;//very gross function, cycles through and finds matching nodes
    printf("Starting node %s\n", start);
    struct node *current = NULL, *index = NULL;
    for(current = (*head); current->next != NULL; current = current->next){    
         if(strcmp(current->name, start) == 0){
            int nextMove = rand()%(current->graph->arcCount);
            char nextPlace[84];
            strcpy(nextPlace, current->graph->arcs[nextMove]->name);
            printf("Next place %s\n", nextPlace);
            probTotal += current->graph->probabilities[nextMove];
            totalArcs += 1;
            while(strcmp(nextPlace, "Home") != 0){//goes through while loop to find it
                 for(current = (*head); current->next != NULL; current = current->next){    
                    if(strcmp(current->name, nextPlace) == 0){
                      nextMove = rand()%(current->graph->arcCount);
                      strcpy(nextPlace, current->graph->arcs[nextMove]->name);
                      printf("Next place %s\n", nextPlace);
                      probTotal += current->graph->probabilities[nextMove];
                      totalArcs += 1;
                      break;
                    }
                }
            }
            printf("End node Home\n");
            printf("Number of paths taken %d\n", totalArcs);
            break;
         }
    }//divides probTotal by total arcs
    float percent = probTotal/totalArcs;
    return percent;
}
//frees the linked list as well as the graph and its node thing pointers
void freeDL(struct node** head){
     struct node *current, *temp;
     current = (*head);
     int i;
     while(current != NULL){
        temp = current;
        current = current-> next;
        for (i = 0; i <temp->graph->arcCount-1;i++){
            temp->graph->arcs[i] = NULL;
            free(temp->graph->arcs[i]);//frees all the connected links of the graph
        }
        free(temp->graph);
        free(temp);            
      }     
} 

int main(){
    struct node *head, *tail = NULL;
    struct graphNode *newNode = NULL;  
    FILE *stream;
    stream = fopen("./hw11.data", "r");
    char temp[84]; 
    char temp2[84];
    float temp3;
    while(1){
        fscanf(stream, "%s",temp);
        if(strcmp(temp, "STOP") == 0)break;
        initializeGraphNode(&newNode, temp);
        addNode(&head, &tail,temp, newNode);
    }
    char temp4[] = {'d','u','m','m','y','\0'};
    initializeGraphNode(&newNode, temp4);//adds dummy node to fix issue with NULLS
    addNode(&head, &tail,temp4, newNode);
    sortList(&head);
    while(1){
        fscanf(stream, "%s %s %f", temp, temp2, &temp3);
        if(strcmp(temp, "STOP") == 0)break;
        addGraphNode(&head, temp, temp2, temp3);
    }
    //print2(&head); //prints out nodes and things for testing purposes!!
    char start[84];
    fscanf(stream, "%s",start);
    fclose(stream);
    float percent = drunkWalk(&head, start);
    printf("Probability of getting Chinese Virus: %.2f\n",percent);
    freeDL(&head);
    free(head);
    free(tail);
    return 0;
    //Everything seems to work but need to further test it and with large files
}
