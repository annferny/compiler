/*
Gavrilova Anna
Matr.Nr: 53045
s85499
*/

#include "stack.h"
#include "namelist.h"
#include <stdlib.h>

extern char*  vCode;
extern char*  pCode;
tLabelStackNode *labelStackTop = NULL;

/*
 * Die Funktion `createLabel` erstellt ein neues Label.
 * Sie allokiert Speicher für das Label und speichert den aktuellen Codezeiger als Index.
 * Bei einem Speicherfehler wird eine Fehlermeldung ausgegeben und das Programm beendet.
 */
tLabel* createLabel() {
    tLabel *label = malloc(sizeof(tLabel));
    if (label == NULL) {
        perror("Memory allocation failed for label");
        exit(-1);
    }
    label->indexJump = pCode - vCode;
    return label;
}


/*
 * Die Funktion `pushLabel` fügt ein Label zum Stack hinzu.
 * Sie allokiert Speicher für einen neuen Stack-Knoten und setzt ihn an die Spitze des Stacks.
 * Bei einem Speicherfehler wird eine Fehlermeldung ausgegeben und das Programm beendet.
 */
void pushLabel(tLabel *label) {
    tLabelStackNode *newNode = malloc(sizeof(tLabelStackNode));
    if (newNode == NULL) {
        perror("Memory allocation failed for label stack node");
        exit(-1);
    }
    newNode->label = label;
    newNode->next = labelStackTop;
    labelStackTop = newNode;
}

/*
 * Die Funktion `popLabel` entfernt das oberste Element vom Label-Stack.
 * Wenn der Stack leer ist, gibt sie eine Fehlermeldung aus und gibt NULL zurück.
 * Ansonsten gibt sie das oberste Label zurück und aktualisiert den Stack.
 */
tLabel* popLabel() {
    if (labelStackTop == NULL) {
        perror("popLabel: labalStackTop is NULL\n");
        return NULL;
    }

    tLabelStackNode *temp = labelStackTop;
    tLabel *label = temp->label;
    labelStackTop = labelStackTop->next;
    free(temp);
    return label;
}