#ifndef DOUBLELINKEDLIST_H_
#define DOUBLELINKEDLIST_H
#include <stdint.h>

struct Node {
 uint8_t row;
 uint8_t col;
 struct Node* next;
 struct Node* prev;
};

void insertEnd(struct Node** head, uint8_t row, uint8_t col);
void deleteHead(struct Node** head);
struct Node* getNodeByIndex(struct Node* head, uint8_t index);
uint8_t getLength(struct Node* head);

#endif
