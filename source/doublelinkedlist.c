#include "doublelinkedlist.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

// Given a reference (pointer to pointer) to the head of a DLL and an int, appends a new node at the end
void insertEnd(struct Node** head, uint8_t row, uint8_t col)
{
	// 1. allocate node
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    // Store the head node temporarily
    struct Node* last = *head;

    // 2. put in the data
    newNode->row = row;
    newNode->col = col;

    // 3. This new node is going to be the last node, so make next of it as NULL
    newNode->next = NULL;

    // 4. If the Linked List is empty, then make the new node as head
    if (*head == NULL)
    {
        newNode->prev = NULL;
        *head = newNode;
        return;
    }

    // 5. Else traverse till the last node
    while (last->next != NULL)
        last = last->next;

    // 6. Change the next of last node
    last->next = newNode;

    // 7. Make last node as previous of new node
    newNode->prev = last;
}

// Delete the current head node and update the head pointer
void deleteHead(struct Node** head)
{
    if (*head == NULL)
    {
        return; // No need to delete if the list is empty.
    }

    // Save a reference to the current head node
    struct Node* temp = *head;

    // Update the head pointer to point to the next node
    *head = (*head)->next;

    // If the new head node exists, update its previous pointer
    if (*head != NULL)
    {
        (*head)->prev = NULL;
    }

    // Free the memory of the old head node
    free(temp);
}

// Get a node by index in the doubly linked list
struct Node* getNodeByIndex(struct Node* head, uint8_t index)
{
    if (head == NULL)
    {
        return NULL; // Return NULL if the list is empty or the index is negative.
    }

    struct Node* current = head;
    uint8_t currentIndex = 0;

    while (current != NULL)
    {
        if (currentIndex == index)
        {
            return current; // Return the node when the desired index is reached.
        }

        current = current->next;
        currentIndex++;
    }

    return NULL; // Return NULL if the index is out of bounds.
}

// Get the length of the doubly linked list
uint8_t getLength(struct Node* head)
{
	uint8_t length = 0;
    struct Node* current = head;

    while (current != NULL)
    {
        length++;
        current = current->next;
    }

    return length;
}
