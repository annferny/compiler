/*
Gavrilova Anna
Matr.Nr: 53045
s85499
*/

#include "list.h"


/* Functions for working with list */


// The following function creates a new empty list for further processing of data
List* createList()
{
    List* listOfElements;
    listOfElements = (List*)malloc(sizeof(List));
    if (listOfElements)
    {
        listOfElements->currentPosition = NULL;
        listOfElements->head.next = (connector*)listOfElements;
        listOfElements->head.previous = (connector*)listOfElements;
        listOfElements->head.pointedItem = NULL;
    }
}

// The following function inserts a new connector behind the given one in arguments
int insertConnector(connector*previousConnector, void*itemToInsert)
{
    connector* connectorToInsert = malloc(sizeof(connector));
    if (connectorToInsert)
    {
        connectorToInsert->pointedItem = itemToInsert;
        connectorToInsert->next = previousConnector->next;
        connectorToInsert->previous = previousConnector;
        connectorToInsert->next->previous = connectorToInsert;
        previousConnector->next = connectorToInsert;
        return OK;
    } 
    return FAIL;
}

// The following function inserts data at the end of the list
int insertTail (List* listOfElements, void *itemToInsert)
{
    return insertConnector(listOfElements->head.previous, itemToInsert);
}

// The following function inserts data before the current list item
int insertBefore (List* listOfElements, void *itemToInsert)
{
    return insertConnector(listOfElements->currentPosition->previous, itemToInsert);
}

// The following function gets the first element of the list
void* getFirst (List* listOfElements)
{
    void* tmp = NULL;
    if (listOfElements->head.next)
    {
        tmp = listOfElements->head.next->pointedItem;
        listOfElements->currentPosition = listOfElements->head.next;
    }
    return tmp;
}

// The folliwing function gets the next item from the list
void* getNext (List* listOfElements)
{
    void* tmp = NULL;
    if (listOfElements->currentPosition)
    {
        listOfElements->currentPosition = listOfElements->currentPosition->next;
        if (listOfElements->currentPosition)
        {
            tmp = listOfElements->currentPosition->pointedItem;
        }
    }
    return tmp;
}

// The following function deletes an item on the current position
int removeItem (List* listOfElements)
{
    // Creates a temporary connector for saving the current position
    connector * tmp = listOfElements->currentPosition;

    art* descr = tmp->pointedItem; // pointer to data
    
    // Changes pointers of surrounding connectors so, that they point to each other, and no longer to the current connector
    listOfElements->currentPosition = tmp->next;
    listOfElements->currentPosition->previous = tmp->previous;
    tmp->previous->next = tmp->next;

    if(listOfElements->head.next == tmp) // changes pointer, if the current connector is first in the list
    {
        listOfElements->head.next = tmp->next;
    }
    if(listOfElements->head.previous == tmp) // changes pointer, if the current connector is last in the list
    {
        listOfElements->head.previous = tmp->previous;
    }

    free (descr->description); // free memory for description of article
    // descr->description = NULL;
    // free(descr);
    // descr = NULL;
    free(tmp);
    tmp = NULL;
    return OK;
}

// The following function deletes the whole list
int deleteList (List* listOfElements)
{
    for (art* item = getFirst(listOfElements); item != NULL; item = getNext(listOfElements))
    {
        if(listOfElements->currentPosition->next != listOfElements->currentPosition) // while pointers of current and next connectors are not equal 
        {
            removeItem(listOfElements);
        }
    }
    free(listOfElements);
    return OK;
}

// The following function compares the values of the gotten article number with the one from the list.
// Possible values:
// 1 - article number from the list is larger than the new one
// 0 - article numbers are equal
// -1 - article number from the list is less than the new one

int compare(art* itemFromList, art* newElement)
{
    if (itemFromList->articleNumber == newElement->articleNumber)
    {
        return 0; 
    }
    else if (itemFromList->articleNumber > newElement->articleNumber)
    {
        return 1;
    } else {
        return -1;
    }
}

// The following function adds a new item according to its article number in sorted order. 
// If the gotten article number already exists, then increases its amount.

void* addArticleToList (List* listOfElements, art* newElement, int(*compare)(art*,art*))
{
    art* itemFromList;
    int i = 0;

    // Looping the list untill the gotten item is larger than the item from the list
    for(itemFromList = getFirst(listOfElements); itemFromList != NULL && compare(itemFromList, newElement) < 0; itemFromList = getNext(listOfElements));
    
    if (itemFromList != NULL) // if an item was found, that is larger or equal to the gotten item
    {
        if (compare(itemFromList, newElement) == 0)
        {
            // Checks, if descriptions differ
            if (strcmp(itemFromList->description, newElement->description) != 0)
            {
                sprintf(message, "Fehler: Artikel #%d ist schon mit der folgenden Bezeichnung eingetragen: %s", itemFromList->articleNumber, itemFromList->description);
                return NULL;
            }

            // Increasing amount
            i = itemFromList->amount + newElement->amount;
            if (i > 999999) 
            {
                sprintf(message, "Fehler: Anzahl der Artikel kann nicht mehr als 999 999 sein");
                return NULL;
            }
            itemFromList->amount += newElement->amount;
            sprintf (message, "Die Anzahl von Artikel #%d mit Bezeichnung \"%s\" wurde erfolgreich um %d erh&ouml;ht", itemFromList->articleNumber, itemFromList->description, newElement->amount);
        } else {
            if (insertBefore(listOfElements, newElement) == FAIL)
            {
                // if an error occured during inserting an article to the list
                sprintf(message, "Unerwarteter Fehler beim Eintragen des Artikels");
                return NULL;
            }
            sprintf (message, "Artikel #%d mit Bezeichnung \"%s\" und Anzahl %d wurde erfolgreich eingetragen", newElement->articleNumber, newElement->description, newElement->amount);
        }
    } else { // if all items in the list are less than the gotten item
        {
            if (insertTail(listOfElements, newElement) == FAIL)
            {
                // if an error occured during inserting an article to the list
                sprintf(message, "Unerwarteter Fehler beim Eintragen des Artikels");
                return NULL;
            }
            sprintf (message, "Artikel #%d mit Bezeichnung \"%s\" und Anzahl %d wurde erfolgreich eingetragen", newElement->articleNumber, newElement->description, newElement->amount);
        }
    }
}

// The following function reduces the amount of articles in the list.
void* reduceArticle (List* listOfElements, art* newElement, int(*compare)(art*,art*))
{
    art* itemFromList;

    // Looping the list untill the gotten item is larger than the item from the list
    for(itemFromList = getFirst(listOfElements); itemFromList != NULL && compare(itemFromList, newElement) != 0; itemFromList = getNext(listOfElements));

    if (itemFromList != NULL) // if an item was found, that is equal to the gotten item
    {
        // Checks, if descriptions differ
        if (strcmp(itemFromList->description, newElement->description) != 0)
        {
            sprintf(message, "Fehler: Artikel #%d ist schon mit der folgenden Bezeichnung eingetragen: %s", itemFromList->articleNumber, itemFromList->description);
            return NULL;
        }
        if (itemFromList->amount < newElement->amount) // checks, if the amount of items in the list is less, than the amount to delete
        {
            sprintf(message,"Es gibt nur %d Waren f&uuml;r Artikel #%d mit Bezeichnung \"%s\" auf dem Lager. Bitte geben Sie die Anzahl %d ein, um alle zu l&ouml;schen", 
                    itemFromList->amount, itemFromList->articleNumber, itemFromList->description, itemFromList->amount);
            return NULL;
        }

        if (itemFromList->amount == newElement->amount) // checks, if the amount of items in the list is equal to the amount to delete
        {
            sprintf(message,"Alle Waren f&uuml;r Artikel #%d mit Bezeichnung \"%s\" wurden erfolgreich ausgeliefert und der Eintrag wurde gel&ouml;scht", itemFromList->articleNumber, itemFromList->description);
            removeItem(listOfElements); // delete the whole line
        } else {                                            // if the amount of items in the list is more, than the amount to delete
            itemFromList->amount -= newElement->amount;     // then the function reduces its number 
            sprintf(message,"Artikel #%d mit Bezeichnung \"%s\" wurde erfolgreich um %d reduziert", newElement->articleNumber, itemFromList->description, newElement->amount);
        }
    } else { // if the gotten item is not in the list
        {
            sprintf(message, "Fehler: Artikel #%d mit Bezeichnung \"%s\" existiert nicht\n", newElement->articleNumber, newElement->description);
            return NULL;
        }
    }
}