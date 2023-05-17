//-----------------------------------------------------------------------------
//Preston Nguyen
//prhunguy
//pa4
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "Matrix.h"
#include "List.h"

typedef struct EntryObj *Entry;

// private Entry Object type
typedef struct EntryObj {
    int col;
    double val;
} EntryObj;

typedef struct MatrixObj {
    int size;
    int NNZ;
    List *row;
} MatrixObj;

// constructor for entry
Entry newEntry(int column, double value) {
    
    // column can't be less than 0
    if(column < 0){
        printf("Error: newEntry() column less than 0\n");
        exit(EXIT_FAILURE);
    }

    Entry E = malloc(sizeof(EntryObj));
    E->col = column;
    E->val = value;

    return E;
}

void deleteEntry(Entry *pE) {
    
    if( pE!=NULL && *pE!=NULL ){
        free(*pE);
        *pE = NULL;
    }

}

// newMatrix()
// Returns a reference to a new nXn Matrix object in the zero state.
Matrix newMatrix(int n){
    
    if(n < 0){
        printf("Error: newMatrix n less than 0\n");
        exit(EXIT_FAILURE);
    }

    Matrix M = malloc(sizeof(MatrixObj));

    M->size = n;
    M->NNZ = 0;

    M->row = malloc((n+1) * sizeof(List));

    for(int i = 1; i <= n; i++){
        M->row[i] = newList();
    }

    return M;

}


// freeMatrix()
// Frees heap memory associated with *pM, sets *pM to NULL.
void freeMatrix(Matrix* pM){
    
    if (pM != NULL && *pM != NULL){

        makeZero(*pM);

        for (int i = 1; i <= size((*pM)); i++){
            freeList(&((*pM)->row[i]));
            (*pM)->row[i] = NULL;
        }

        free((*pM)->row);
        free(*pM);
        *pM = NULL;
    }

}





// Access functions
// size()
// Return the size of square Matrix M.
int size(Matrix M){
    if( M == NULL ){
        printf("Error: calling size() on NULL matrix reference\n");
        exit(EXIT_FAILURE);
    }
    
    return M->size;
}


// NNZ()
// Return the number of non-zero elements in M.
int NNZ(Matrix M){
    if( M == NULL ){
        printf("Error: calling size() on NULL matrix reference\n");
        exit(EXIT_FAILURE);
    }

    return M->NNZ;
}


// equals()
// Return true (1) if matrices A and B are equal, false (0) otherwise.
int equals(Matrix A, Matrix B){

    if( A==NULL || B==NULL ){
        printf("Error: calling equals() on NULL Matrix reference\n");
        exit(EXIT_FAILURE);
    }    

    Entry x, y;
    List c, d;

    if(size(A) != size(B)){
        return 0;
    }

    
    for(int i = 1; i <= size(A); i++){

        c = A->row[i];
        d = B->row[i];
        moveFront(c);
        moveFront(d);

        // if(length(A->row[i]) == length(B->row[i])){as---------------------------------------TEST 6 NO LONGER ISSUE
        //     continue;
        // }else{
        //     return 0;
        // }

        if(length(A->row[i]) != length(B->row[i])){
            return 0;
        }

        while( (index(c) >= 0 && index(d) >= 0) && (size(A) == size(B))){
            x = (Entry)get(A->row[i]);
            y = (Entry)get(B->row[i]);

            if(x->val != y->val){
                return 0;
            }
            if(x->col != y->col){
                return 0;
            }
            moveNext(A->row[i]);
            moveNext(B->row[i]);
        }
    }
    return 1;
}


// Manipulation procedures
// makeZero()
// Re-sets M to the zero Matrix state.
void makeZero(Matrix M){
    if(M == NULL){
        printf("Error: calling makeZero() on NULL Matrix reference\n");
        exit(EXIT_FAILURE);
    }

    M->NNZ = 0;

    for(int i = 1; i <= size(M); i++){
        for(moveFront(M->row[i]); index(M->row[i]) >= 0; moveNext(M->row[i])){
            Entry E = (Entry)get(M->row[i]);
            deleteEntry(&E);
        }
        clear(M->row[i]);
    }
}


// changeEntry()---------------------------------------------------------------------------------------------------------------------------------------------
// Changes the ith row, jth column of M to the value x.
// Pre: 1<=i<=size(M), 1<=j<=size(M)
void changeEntry(Matrix M, int i, int j, double x){
    
    if(M == NULL){
        printf("Error: calling changeEntry() on NULL Matrix reference\n");
        exit(EXIT_FAILURE);
    }
    
    if( (1 > i  &&  i > size(M)) || (1 > j && j > size(M))){
        printf("Error: calling changeEntry() on invalid Matrix reference (precondition error)\n");
        exit(EXIT_FAILURE);
    }

    List L = M->row[i];
    Entry E = NULL;

    if((length(L) == 0)){
        if(x != 0){
            E = newEntry(j, x);
            append(L, E);
            M->NNZ++;
        }        
    }
        
    moveFront(L);

    while(index(L) >= 0){
        
        E = (Entry)get(L);


        if(E->col == j){


            if(x != 0){
                E->val = x;
            }else{
                deleteEntry(&E);
                delete(L);
                M->NNZ--;
            }
            break;

        }else if(E->col > j){
            if(x != 0){
                E = newEntry(j, x);
                insertBefore(L, E);
                M->NNZ++;
            }
            break;
        }


        moveNext(L);
    }


    if(index(L) == -1){
        if(x != 0){
            E = newEntry(j, x);
            append(L, E);
            M->NNZ++;
        }
    }
}


// Matrix Arithmetic operations
// copy()
// Returns a reference to a new Matrix object having the same entries as A.
Matrix copy(Matrix A){

    if(A == NULL){
        printf("Error: calling copy() on NULL Matrix reference\n");
        exit(EXIT_FAILURE);
    }

    Matrix copy = newMatrix(size(A));
    Entry E;
    int i, col;
    double val;

    for(i = 1; i <= size(A); i++){
        moveFront(A->row[i]);
        while(index(A->row[i]) >= 0){
            E = get(A->row[i]);
            col = E->col;
            val = E->val;

            Entry F = newEntry(col, val);
            append(copy->row[i], F);

            copy->NNZ++;

            moveNext(A->row[i]);
        }
    }
    return copy;

}


// transpose()
// Returns a reference to a new Matrix object representing the transpose
// of A.
Matrix transpose(Matrix A){

    if(A == NULL){
        printf("Error: calling transpose() on NULL Matrix reference\n");
        exit(EXIT_FAILURE);
    }

    Matrix T = newMatrix(size(A));
    Entry E;
    int i, col;
    double val;

    T->NNZ = A->NNZ;
    for(i = 1; i <= size(A); i++){
        moveFront(A->row[i]);
        while(index(A->row[i]) >= 0){
            E = get(A->row[i]);
            col = E->col;
            val = E->val;

            E = newEntry(i, val);
            append(T->row[col], E);

            moveNext(A->row[i]);

        }
    }
    return T;

}


// scalarMult()
// Returns a reference to a new Matrix object representing xA.
Matrix scalarMult(double x, Matrix A){
    
    if(A == NULL){
        printf("Error: calling scalarMult() on NULL Matrix reference\n");
        exit(EXIT_FAILURE);
    }     

    Matrix M = newMatrix(size(A)); 
    Entry E;
    int i, col;
    double val;

    if(x == 0 || A->NNZ == 0){
        return M;
    }

    M->NNZ = A->NNZ;

    for(i = 1; i <= size(A); i++){
        moveFront(A->row[i]);
        while(index(A->row[i]) >= 0){
            E = get(A->row[i]);
            
            col = E->col;
            
            val = E->val * x;

            Entry F = newEntry(col, val);

            append(M->row[i], F);

            moveNext(A->row[i]);

        }

    }

   

    return M;

}



// helper function vector sum for functions sum() diff()
// Assuming C is an empty List object, sets C to P+Q (if flag==1) or P-Q 
// (if flag==-1), considered as sparse vectors. 
// Pre: P!=Q, P and Q are rows in different Matrix objects.
void vecSum(List A, List B, List C, int sign, Matrix M) {
    double x, y, z;
    Entry a, b;

    moveFront(A);
    moveFront(B);

    while(index(A) >= 0 && index(B) >= 0){
        a = get(A);
        b = get(B);
        x = a->val;
        y = (sign) * b->val;
        if(a->col == b->col){
            z = x + y;
            if(z != 0){
                Entry E = newEntry(a->col, z);
                append(C, E);
                M->NNZ++;

            }
            moveNext(A);
            moveNext(B);
        }else if(a->col < b->col){
            Entry F = newEntry(a->col, x);
            append(C, F);
            M->NNZ++;
            moveNext(A);
            
        }else{
            Entry G = newEntry(b->col, y);
            append(C, G);
            M->NNZ++;
            moveNext(B);
        }
    }

    while(index(A) >= 0){
        a = get(A);
        Entry H = newEntry(a->col, (a->val));
        append(C, H);
        M->NNZ++;
        moveNext(A);
    }


    while(index(B) >= 0){
        b = get(B);
        Entry I = newEntry(b->col, (sign)*(b->val));
        append(C, I);
        M->NNZ++;
        moveNext(B);
    }
}



// sum()
// Returns a reference to a new Matrix object representing A+B.
// pre: size(A)==size(B)
Matrix sum(Matrix A, Matrix B){
    
    if(A == NULL){
        printf("Error: calling sum() on NULL Matrix A reference\n");
        exit(EXIT_FAILURE);
    }

    if(B == NULL){
        printf("Error: calling sum() on NULL Matrix B reference\n");
        exit(EXIT_FAILURE);
    }

    if(size(A) != size(B)){
        printf("Error: calling sum() Pre failure\n");
        exit(EXIT_FAILURE);
    }
    Matrix Add;

    if(equals(A, B)){
        return scalarMult(2, A);
    }else{
        Add = newMatrix(size(A));
        for(int i = 1; i <= size(Add); i++){
            vecSum(A->row[i], B->row[i], Add->row[i], 1, Add);
        }
    }
    return Add;

}


// diff()
// Returns a reference to a new Matrix object representing A-B.
// pre: size(A)==size(B)
Matrix diff(Matrix A, Matrix B){
    
    if(A == NULL){
        printf("Error: calling diff() on NULL Matrix A reference\n");
        exit(EXIT_FAILURE);
    }

    if(B == NULL){
        printf("Error: calling diff() on NULL Matrix B reference\n");
        exit(EXIT_FAILURE);
    }

    if(size(A) != size(B)){
        printf("Error: calling diff() Pre failure\n");
        exit(EXIT_FAILURE);
    }

    Matrix Diff = newMatrix(size(A));

    if(equals(A, B)){
        makeZero(Diff);
        return Diff;
    }else{
        
        for(int i = 1; i <= size(Diff); i++){
            vecSum(A->row[i], B->row[i], Diff->row[i], (-1), Diff);
        }
    }

    // if(equals(A, B)){
    //     makeZero(Diff);
    //     return Diff;
    // }
    // for(int i = 1; i <= Diff->size; i++){
    //     vecSum(A->row[i], B->row[i], Diff->row[i], -1, Diff);
    // }

    return Diff; 

}



// vectorDot()
// Returns the dot product of Lists P and Q considered as sparse vectors.
// Pre: P!=Q, P and Q are rows in different Matrix objects.
double vecDot(List A, List B) {

    if( A==NULL ){
        printf("List Error: calling vecDot() on NULL List reference A\n");
        exit(EXIT_FAILURE);
    }

    if( B==NULL ){
        printf("List Error: calling vecDot() on NULL List reference B\n");
        exit(EXIT_FAILURE);
    }

    double dp = 0.0;
    Entry a, b;

    moveFront(A);
    moveFront(B);

    while(index(A) >= 0 && index(B) >= 0){
        a = get(A);
        b = get(B);

        if(a->col == b->col){
            dp += (a->val * b->val);
            moveNext(A);
            moveNext(B);
        }else if(a->col < b->col){
            moveNext(A);
        }else{
            moveNext(B);
        }
    }
    return dp;
}




// product()
// Returns a reference to a new Matrix object representing AB
// pre: size(A)==size(B)
Matrix product(Matrix A, Matrix B){
    
    if(A == NULL){
        printf("Error: calling product() on NULL Matrix A reference\n");
        exit(EXIT_FAILURE);
    }

    if(B == NULL){
        printf("Error: calling product() on NULL Matrix B reference\n");
        exit(EXIT_FAILURE);
    }

    if(size(A) != size(B)){
        printf("Error: calling product() Pre failure\n");
        exit(EXIT_FAILURE);
    }

    Matrix M = newMatrix(size(A));

    Matrix T = transpose(B);

    int i, j;
    double x;

    for(i = 1; i <= size(A); i++){
        for(j = 1; j <= size(A); j++){
            
            if(length(A->row[i]) == 0){
                break;
            }

            x = vecDot(A->row[i], T->row[j]);
            if(x != 0.0){
                Entry E = newEntry(j, x);
                append(M->row[i], E);
                M->NNZ++;
            }
        }
    }
    freeMatrix(&T);
    return M;
    
}



// printMatrix()
// Prints a string representation of Matrix M to filestream out. Zero rows
// are not printed. Each non-zero row is represented as one line consisting
// of the row number, followed by a colon, a space, then a space separated
// list of pairs "(col, val)" giving the column numbers and non-zero values
// in that row. The double val will be rounded to 1 decimal point.
void printMatrix(FILE* out, Matrix M){
    Entry E;
    int cont = 0;
    for (int i = 1; i <= size(M); i++) {
        cont = 0;
        moveFront((M->row[i]));
        if (index(M->row[i]) >= 0) {
            E = (Entry) get(M->row[i]);
            if (E) {
                fprintf(out, "%d: ", i);
                cont = 1;
            }
        }
        while (index((M->row[i])) >= 0) {
            E = (Entry) get(M->row[i]);
            if (E) {
                fprintf(out, "(%d, %.1f) ", E->col, E->val);
            }
            moveNext(M->row[i]);
        }
        if (cont == 1) {
            fprintf(out, "\n");
        }
    }
    //free(&E);
}

