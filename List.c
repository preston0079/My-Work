//-----------------------------------------------------------------------------
//Preston Nguyen
//prhunguy
//pa4
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "List.h"

//like reading a book, left to right
//front is left, back is right
//prev is towards front, next is towards back


// Structs --------------------------------------------------------------------
// private Node type
typedef struct NodeObj* Node;

// private NodeObj type
typedef struct NodeObj{
    ListElement data;
    Node next;
    Node prev;
} NodeObj;


// private ListObj type
typedef struct ListObj{
    Node front;
    Node back;
    int length;

    Node cursor;
    int cursorindex;
} ListObj;




// Constructors-Destructors ---------------------------------------------------

// newNode()
// Returns reference to new Node object. Initializes next and data fields.
Node newNode(void* data){
    Node N = malloc(sizeof(NodeObj));
    assert( N!=NULL );
    N->data = data;
    N->next = N->prev = NULL;
    return(N);

    
}

// freeNode()
// Frees heap memory pointed to by *pN, sets *pN to NULL.
void freeNode(Node* pN){
    if( pN!=NULL && *pN!=NULL ){
        free(*pN);
        *pN = NULL;
    }
}


// Creates and returns a new empty List.
List newList(void){
    List L;
    L = malloc(sizeof(ListObj));
    assert( L!=NULL );
    L->front = L->back = NULL;
    L->length = 0;
    L->cursor = NULL;
    L->cursorindex = -1;
    return(L);


}
// Frees all heap memory associated with *pL, and sets *pL to NULL.
void freeList(List* pL){
    if(pL!=NULL && *pL!=NULL) { 
        clear(*pL);
        free(*pL);
        *pL = NULL;
    }
}



// Access functions -----------------------------------------------------------
// Returns the number of elements in L.
int length(List L){
    
    if( L==NULL ){
        printf("List Error: calling length() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    return L->length;
}
// Returns index of cursor element if defined, -1 otherwise.
int index(List L){
    
    if( L==NULL ){
        printf("List Error: calling index() on NULL List reference\n");
        exit(EXIT_FAILURE);
    
    }
    return L->cursorindex;
}
// Returns front element of L. Pre: length()>0
void* front(List L){
    
    if( L==NULL ){
        printf("List Error: calling front() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    
    if(length(L) <= 0){
        printf("List Error: calling front() on EMPTY List reference\n");
        exit(EXIT_FAILURE);
    }

    return L->front->data;
}
// Returns back element of L. Pre: length()>0
void* back(List L){
    
    if( L==NULL ){
        printf("List Error: calling back() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    if(length(L) <= 0){
        printf("List Error: calling back() on EMPTY List reference\n");        
        exit(EXIT_FAILURE);
    }

    return L->back->data;

}
// Returns cursor element of L. Pre: length()>0, index()>=0
void* get(List L){
    
    if( L==NULL ){
        printf("List Error: calling get() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    
    if(length(L) <= 0 || index(L) < 0){
        printf("List Error: calling get() on EMPTY List reference\n");
        exit(EXIT_FAILURE);
    }

    return L->cursor->data;
}




// Manipulation procedures ----------------------------------------------------
// Resets L to its original empty state.
void clear(List L){

    if( L==NULL ){
        printf("List Error: calling clear() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    while(L->front != NULL){
        Node temp = L->front->next;
        freeNode(&L->front);
        L->front = temp;
    }

    L->front = L->back = NULL;
    L->length = 0;

    L->cursor = NULL;
    L->cursorindex = -1;


}
// Overwrites the cursor element’s data with x. Pre: length()>0, index()>=0
void set(List L, ListElement x){

    if( L==NULL ){
        printf("List Error: calling set() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    
    if(length(L)<=0 || index(L)<0){
        printf("List Error: calling set() on EMPTY List reference\n");
        exit(EXIT_FAILURE);
    }

    L->cursor->data = x;

}
// If L is non-empty, sets cursor under the front element, otherwise does nothing.
void moveFront(List L){

    if( L==NULL ){
        printf("List Error: calling moveFront() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    // Ff non-empty
    if(length(L) != 0){
        L->cursor = L->front;
        L->cursorindex = 0;
    }
    



}
// If L is non-empty, sets cursor under the back element, otherwise does nothing.
void moveBack(List L){

    if( L==NULL ){
        printf("List Error: calling moveBack() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    // If non-empty
    if(length(L) != 0){
        L->cursorindex = length(L) - 1;
        L->cursor = L->back;
    }


}

// If cursor is defined and not at front, move cursor one
// step toward the front of L; if cursor is defined and at
// front, cursor becomes undefined; if cursor is undefined
// do nothing
void movePrev(List L){
    if( L==NULL ){
        printf("List Error: calling movePrev() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    //if cursor is defined
    if(L->cursorindex != -1){

        //cursor at front
        if(L->cursorindex == 0){

            //undefine cursor
            L->cursorindex = -1;
            L->cursor = NULL;
        }else{
            //take step toward front
            L->cursorindex--;
            L->cursor = L->cursor->prev;
        }
    }


}


// If cursor is defined and not at back, move cursor one
// step toward the back of L; if cursor is defined and at
// back, cursor becomes undefined; if cursor is undefined
// do nothing
void moveNext(List L){
    if( L==NULL ){
        printf("List Error: calling moveNext() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    //if cursor is defined
    if(L->cursorindex != -1){

        //cursor at back
        if(L->cursorindex == length(L) - 1){

            //undefine cursor
            L->cursor = NULL;
            L->cursorindex = -1;
        }else{
            //take step toward back
            L->cursor = L->cursor->next;
            L->cursorindex++;
        }
    }


}

// Insert new element into L. If L is non-empty, insertion takes place before front element.
void prepend(List L, ListElement x){
    
    if( L==NULL ){
        printf("List Error: calling prepend() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    //if empty
    if(length(L) == 0){
        Node N = newNode(x);
        L->front = L->back= N;
        L->length++;
    }else{
        Node N = newNode(x);
        N->next = L->front;
        L->front->prev = N;
        L->front = N;

        L->length++;
        L->cursorindex++;
    }

}
// Insert new element into L. If L is non-empty, insertion takes place after back element.
void append(List L, void* x){
    
    if( L==NULL ){
        printf("List Error: calling append() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    //if empty
    if(length(L) == 0){
        Node N = newNode(x);
        L->front = L->back= N;
        L->length++;
    }else{
        Node N = newNode(x);
        N->prev = L->back;
        L->back->next = N;
        L->back = N;

        L->length++;
    }

}
// Insert new element before cursor. Pre: length()>0, index()>=0
void insertBefore(List L, ListElement x){
    if( L==NULL ){
        printf("List Error: calling insertBefore() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    
    if(length(L)<=0 || index(L)<0){
        printf("List Error: calling insertBefore() on EMPTY List reference\n");
        exit(EXIT_FAILURE);
    }

    
    // If cursor index is 0
    if(index(L) == 0){
        Node N = newNode(x);
        N->next = L->front;
        N->prev = NULL; 

        L->front = N;
        L->cursor->prev = N;

    }else{
        Node N = newNode(x);
        N->prev = L->cursor->prev;
        N->prev->next = N;
        N->next = L->cursor;

        L->cursor->prev = N;
    }

    L->length++;
    L->cursorindex++;
}
// Insert new element after cursor. Pre: length()>0, index()>=0
void insertAfter(List L, ListElement x){
    if( L==NULL ){
        printf("List Error: calling insertAfter() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    
    if(length(L)<=0 || index(L)<0){
        printf("List Error: calling insertAfter() on EMPTY List reference\n");
        exit(EXIT_FAILURE);
    }

    // if cursor index is at end of list
    if(index(L) == (length(L) - 1)){
        Node N = newNode(x);
        N->prev = L->cursor;

        L->back = N;
        L->cursor->next = N;
    }else{
        Node N = newNode(x);
        N->prev = L->cursor;
        N->next = L->cursor->next;
        N->next->prev = N;

        L->cursor->next = N;
    }
    L->length++;

}
// Delete the front element. Pre: length()>0
void deleteFront(List L){
    if( L==NULL ){
        printf("List Error: calling deleteFront() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    
    if(length(L)<=0){
        printf("List Error: calling deleteFront() on EMPTY List reference\n");
        exit(EXIT_FAILURE);
    }

    // If the list is only 1 element long
    if(length(L) == 1){
        freeNode(&L->front);
        L->front = L->back = NULL;
    }else{
        // For list that are composed of more than 1 element
        Node newfront = L->front->next;
        freeNode(&L->front);
        L->front = newfront;
        L->front->prev = NULL;
    }

    L->length--;

    // If cursor index is on first space of list
    if(L->cursorindex == 0){
        L->cursor = NULL;
    }
    //de-increment cursor index by 1 because it is being shifted over due to removal
    L->cursorindex--;

}
// Delete the back element. Pre: length()>0
void deleteBack(List L){
    if( L==NULL ){
        printf("List Error: calling deleteBack() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    
    if(length(L)<=0){
        printf("List Error: calling deleteBack() on EMPTY List reference\n");
        exit(EXIT_FAILURE);
    }

    // if list is 1 element long
    if(length(L) == 1){
        freeNode(&L->back);
        L->front = L->back = NULL;
    }else{
        Node newback = L->back->prev;
        L->back->prev->next = NULL;
        freeNode(&L->back);
        L->back = newback;
        
    }

    //if cursor is at end of list
    if(L->cursorindex == length(L) - 1){
        L->cursorindex = -1;
        L->cursor = NULL;
    }

    L->length--;

}
// Delete cursor element, making cursor undefined. Pre: length()>0, index()>=0
void delete(List L){
    if( L==NULL ){
        printf("List Error: calling delete() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    
    if(length(L)<=0 || index(L)<0){
        printf("List Error: calling delete() on EMPTY List reference\n");
        exit(EXIT_FAILURE);
    }

    if(L->cursorindex == 0){
        deleteFront(L);
    } else if(L->cursorindex == length(L) - 1){
        deleteBack(L);
    } else {
        L->cursor->next->prev = L->cursor->prev;
        L->cursor->prev->next = L->cursor->next;

        //make cursor index undefined
        L->cursorindex = -1;

        freeNode(&L->cursor);
        
        L->length--;
    }

}



// Other operations -----------------------------------------------------------

// Prints to the file pointed to by out, a
// string representation of L consisting
// of a space separated sequence of integers,
// with front on left.
// void printList(FILE* out, List L){
//     Node N = NULL;

//     if( L==NULL ){
//         printf("List Error: calling printList() on NULL List reference\n");
//         exit(EXIT_FAILURE);
//     }

//     for(N = L->front; N != NULL; N = N->next){
//         fprintf(out, "%d ", N->data);
//     }

//     printf("\n");

// }

// Returns a new List representing the same integer
// sequence as L. The cursor in the new list is undefined,
// regardless of the state of the cursor in L. The state
// of L is unchanged.
// List copyList(List L){
//     Node N = NULL;

//     if( L==NULL ){
//         printf("List Error: calling printList() on NULL List reference\n");
//         exit(EXIT_FAILURE);
//     }

//     List clone = newList();
//     for(N = L->front; N != NULL; N = N->next){
//         append(clone, N->data);
//     }
//     return clone;
// }



