#include<bits/stdc++.h>
using namespace std;

/**
 * @brief Structure of a node in linked list
 */
struct node{
    int size;
    bool allocatedOrNot;
    struct node *next;
    struct node *prev;
};


//Global pointer to store data about recently allocated memory
struct node *lastAllocated;


/**
 * @brief Used to create a linked list
 * @param head Pointer
 * @param mem_size Size of memory
 * @param isfull To determine whether or not memory has been allocated
 * @return struct node* 
 */
struct node *createnode(struct node *head, int mem_size, bool isfull)
{
    struct node *temp;
    temp = (struct node *)malloc(sizeof(struct node));
    temp->size = mem_size;
    temp->prev = NULL;
    temp->allocatedOrNot = isfull;
    temp->next = NULL;

    if(head == NULL){
        lastAllocated = temp;
        return temp;
    }
    else{
        struct node *ptr = head;
        while(ptr->next != NULL){
            ptr = ptr->next;
        }
        ptr->next = temp;
        temp->prev = ptr;
        return head;
    }
}


/**
 * @brief For printing linked list. 
 * @param head Starting pointer 
 * @return void
 */
void print(struct node *head){
    if(head==NULL){
        cout<<"Linkedlist is Empty\n";
    }
    struct node *ptr2 = head;
    while(ptr2 != NULL){
        cout<<ptr2->size<<" (" <<ptr2->allocatedOrNot<<") <-> ";
        ptr2 = ptr2->next;
    }
    cout<<"NULL\n\n";
}


/**
 * @brief Used to create a new node
 * @param ptr Pointer
 * @param ptr2 Pointer2
 * @param data Requested memory
 */
void newnode(struct node *ptr, struct node *ptr2, int data){
    ptr->allocatedOrNot = true;
    struct node *temp;
    temp = (struct node *)malloc(sizeof(struct node));
    temp->size = ptr->size - data;

    ptr->size = data;
    temp->allocatedOrNot = false;

    if(ptr->next == NULL){
        ptr->next = temp;
        temp->prev = ptr;
        temp->next = NULL;
    }
    else{
        ptr2 = ptr->next;

        ptr->next = temp;
        ptr2->prev = temp;
        temp->prev = ptr;
        temp->next = ptr2;
    }
    lastAllocated = temp;
}


/**
 * @brief In the first fit, the partition is allocated which is the first sufficient block from the top of Main Memory.
 * @param head Head pointer
 * @param data Requested memory
 * @return struct node* 
 */
struct node *firstFit(struct node *head, int data){
    struct node *ptr = head;
    struct node *ptr2 = NULL;
    while(ptr->next !=NULL){
        if(ptr->allocatedOrNot == false){
            if(ptr->size > data){
                newnode(ptr, ptr2, data);
                cout<<"Memory allocated\n";
                return head;
            }
            else if(ptr->size == data){
                lastAllocated = ptr->next;
                cout<<"Memory allocated\n";
                return head;
            }
        }
        ptr = ptr->next;
    }
    cout<<"Memory not allocated\n";
    cout<<"You ask for "<<data<<" memory units\n";
    return head;
}


/**
 * @brief Next fit is similar to the first fit but it will search for the first sufficient partition from the last allocation point. 
 * @param head Head pointer
 * @param data Requested memory
 * @return struct node* 
 */
struct node *nextFit(struct node *head, int data){
    struct node *ptr = lastAllocated;
    struct node *ptr2 = NULL;
    while(ptr->next != lastAllocated){
        if(ptr->allocatedOrNot == false){
            if(ptr->size > data){
                newnode(ptr, ptr2, data);
                cout<<"Memory allocated\n";
                return head;
            }
            else if(ptr->size == data){
                ptr->allocatedOrNot = true;
                lastAllocated = ptr->next;
                cout<<"Memory allocated\n";
                return head;
            }
        }
        ptr = ptr->next;
        if(ptr==NULL){
            ptr = head;
        }
    }
    cout<<"Memory not allocated\n";
    cout<<"You ask for "<<data<<" memory units\n";
    return head;
}


/**
 * @brief Best Fit Allocate the process to the partition which is the first smallest sufficient partition among the free available partition.
 * @param head Head pointer
 * @param data Requested memory
 * @return struct node* 
 */
struct node *bestFit(struct node *head, int data){
    struct node *ptr = head;
    struct node *ptr2 = NULL;
    struct node *bestptr;
    int min = 10000, status = 0;
    while(ptr->next != NULL){
        if(ptr->allocatedOrNot == false){
            if(ptr->size > data){
                if(min > (ptr->size - data)){
                    min = (ptr->size - data);
                    bestptr = ptr;
                    status = 1;
                }
            }
            else if(ptr->size == data){
                ptr->allocatedOrNot = true;
                lastAllocated = ptr->next;
                cout<<"Memory allocated\n";
                return head;
            }
        }
        ptr = ptr->next;
    }
    if(status){
        newnode(bestptr, ptr2, data);
        cout<<"Memory allocated.\n";
        return head;
    }
    cout<<"Memory not allocated.\n";
    cout<<"You asked for "<< data << " memory units.\n";
    return head;
}


/**
 * @brief Used to free up heap memory.
 * @param head Head pointer
 * @param num The number at which we had to free up memory
 * @return struct node* 
 */
struct node *freeMem(struct node *head, int num){
    struct node *ptr = head;
    while(num!=1){
        ptr = ptr->next;
        num--;
    }
    if(ptr == NULL){
        cout<<"There is no memory in the heap.\n";
    }
    else if(ptr->allocatedOrNot == false){
        cout<<"Memory is already free.\n";
    }
    else if(ptr->allocatedOrNot == true){
        if(ptr->prev == NULL && ptr->next->allocatedOrNot == true){
            ptr->allocatedOrNot = false;
            cout<<"Memory freed\n";
        }
        else if(ptr->prev == NULL && ptr->next->allocatedOrNot == false){
            ptr->allocatedOrNot = false;
            ptr->size += ptr->next->size;

            struct node *temp;
            temp = ptr->next;
            ptr->next = ptr->next->next;
            ptr->next->prev = ptr;

            free(temp);
            cout<<"Memory freed\n";
        }
        else if(ptr->next == NULL && ptr->prev->allocatedOrNot == true){
            ptr->allocatedOrNot = false;
            cout<<"Memory freed\n";
        }
        else if(ptr->next == NULL && ptr->prev->allocatedOrNot == false){
            ptr->allocatedOrNot = false;
            ptr->size += ptr->prev->size;

            struct node *temp;
            temp = ptr->prev;
            ptr->prev = ptr->prev->prev;
            ptr->prev->next = ptr;

            free(temp);
            cout<<"Memory freed\n";
        }
        else if(ptr->next->allocatedOrNot == true && ptr->prev->allocatedOrNot == false){
            ptr->prev->size += ptr->size;

            ptr->prev->next = ptr->next;
            ptr->next->prev = ptr->prev;

            free(ptr);
            cout<<"Memory freed\n";
        }
        else if(ptr->next->allocatedOrNot == false && ptr->prev->allocatedOrNot == true){
            ptr->next->size += ptr->size;

            ptr->next->prev = ptr->prev;
            ptr->prev->next = ptr->next;

            free(ptr);
            cout<<"Memory freed\n";
        }
        else if(ptr->next->allocatedOrNot == true && ptr->prev->allocatedOrNot == true){
            ptr->allocatedOrNot = false;
            cout<<"Memory freed\n";
        }
        else if(ptr->next->allocatedOrNot == false && ptr->prev->allocatedOrNot == false){
            ptr->prev->size += ptr->size + ptr->next->size;
    
            ptr->prev->next = ptr->next->next;
            ptr->next->next->prev = ptr->prev;

            free(ptr->next);
            free(ptr);
            cout<<"Memory freed\n";
        }
    }
    return head;
}


int main(int argc, char const *argv[])
{
    struct node* head = NULL;
    int num1, num2;
    bool True = true, False = false;
    for(int i=0;i<6;i++){  
        num1 = rand() % 6 + 3;  
        num2 = rand() % 6 + 3;
        head = createnode(head, pow(2, num1), False);
        head = createnode(head, pow(2, num2), True);
    }
    print(head);

    head = freeMem(head, 6);
    print(head);

    head = firstFit(head, 30);
    print(head);
    
    head = nextFit(head, 60);
    print(head);
    
    head = freeMem(head, 1);
    print(head);

    head = firstFit(head, 80);
    print(head);
    
    head = freeMem(head, 2);
    print(head);

    head = nextFit(head, 20);
    print(head);
    
    head = bestFit(head, 10);
    print(head);
    
    head = freeMem(head, 10);
    print(head);

    head = firstFit(head, 250);
    print(head);
    
    head = nextFit(head, 6);
    print(head);
    
    head = bestFit(head, 40);
    print(head);
    
    return 0;
}
