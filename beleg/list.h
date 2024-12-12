/*
Gavrilova Anna
Matr.Nr: 53045
s85499
*/

#include "structs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef LIST_H_
#define LIST_H_

extern char buf[], option[], message[];

#define OK 1
#define FAIL 0

List * createList(void); // creates a new list

int insertConnector(connector*previousConnector, void*itemToInsert); // inserts a connector to the list
int insertTail(List* pointedList, void *pointedItemToInsert); // inserts an item at the end of the list
int insertBefore(List* pointedList, void *pointedItemToInsert); // inserts an item before the current position
int insertHead (List* listOfElements, void *itemToInsert);

void* getFirst(List* pointedList); // gets the first item
void* getNext(List* pointedList); // gets the next item

int removeItem(List* pointedList); // removes an item from the list
int deleteList(List* pointedList); // deletes list
//
//int compare (art* itemFromList, art* newElement); // compares a new item with the ones in the list
//void* addArticleToList (List* listOfElements, art* newElement, int(*compare)(art*,art*)); // adds an item to the list according to its article number
//void* reduceArticle (List* listOfElements, art* newElement, int(*compare)(art*,art*)); // reduces the amount of items

#endif /* LIST_H_ */
