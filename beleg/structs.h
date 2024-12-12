/*
Gavrilova Anna
Matr.Nr: 53045
s85499
*/

#ifndef STRUCTS_H_
#define STRUCTS_H_

/*--------------------------------------------------------------------------------------*/

// Struct of one article
struct articles;
typedef struct articles art;

// Struct of one element of the list
struct connectElements;
typedef struct connectElements connector;

// Struct of a circle list
struct structList;
typedef struct structList List;

/*--------------------------------------------------------------------------------------*/

// Struct of one article
typedef struct articles
{
    int articleNumber;
    char* description;
    int amount;
}art;

// Struct of one element of the list
typedef struct connectElements
{
    struct connectElements *next; // pointer on the next element
    struct connectElements *previous; // pointer on the previous element
    void *pointedItem; // pointer on data
}connector;

// Struct of a circle list
typedef struct structList
{
    connector head;
    connector*currentPosition;
} List;

#endif /* STRUCTS_H_ */