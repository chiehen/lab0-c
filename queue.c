#include <bsd/string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    /* TODO: What if malloc returned NULL? */
    if (!q)
        return NULL;
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    /* TODO: How about freeing the list elements and the strings? */
    /* Free queue structure */
    if (q) {
        while (q->head) {
            list_ele_t *p = q->head;
            q->head = p->next;
            free(p->value);
            free(p);
        }
    }
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    if (!q)
        return false;
    list_ele_t *newh;
    newh = malloc(sizeof(list_ele_t));
    if (!newh)
        return false;
    char *val = malloc(strlen(s) + 1);
    if (!val) {
        free(newh);
        return false;
    }
    strncpy(val, s, strlen(s) + 1);
    newh->value = val;
    newh->next = q->head;
    q->head = newh;
    if (!q->tail)
        q->tail = newh;
    q->size++;
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (!q)
        return false;
    list_ele_t *newt;  // newt means new tail
    newt = malloc(sizeof(list_ele_t));
    if (!newt)
        return false;
    char *val = malloc(strlen(s) + 1);
    if (!val) {
        free(newt);
        return false;
    }
    strncpy(val, s, strlen(s) + 1);
    newt->value = val;
    newt->next = NULL;
    if (q->tail)
        q->tail->next = newt;
    q->tail = newt;
    if (!q->head)
        q->head = newt;
    q->size++;
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || !q->head)
        return false;
    list_ele_t *ele = q->head;
    q->head = q->head->next;  // if head==tail??
    char *val = ele->value;
    if (sp) {
        if (strlen(val) > bufsize - 1) {
            strncpy(sp, val, bufsize - 1);
            sp[bufsize - 1] = '\0';
        } else {
            strncpy(sp, val, strlen(val) + 1);
        }
    }
    free(val);
    free(ele);
    q->size--;
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (q)
        return q->size;
    return 0;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (q && q->head) {
        list_ele_t *pre = q->head;
        list_ele_t *curr = pre->next;
        pre->next = NULL;
        while (curr) {
            list_ele_t *n = curr->next;
            curr->next = pre;
            pre = curr;
            curr = n;
        }
        q->tail = q->head;
        q->head = pre;
    }
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    /* q is NULL or empty*/
    if (!q || !q->head || !q->head->next)
        return;

    q->head = merge_sort(q->head);
    list_ele_t *temp = q->head;
    while (temp->next)
        temp = temp->next;
    q->tail = temp;
}

list_ele_t *merge_sort(list_ele_t *head)
{
    // when no element or one element in list, stop
    if (!head || !head->next)
        return head;
    list_ele_t *fast = head->next;
    list_ele_t *slow = head;

    // find the middle
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    fast = slow->next;
    slow->next = NULL;

    // divide
    list_ele_t *l1 = merge_sort(head);
    list_ele_t *l2 = merge_sort(fast);

    return merge(l1, l2);
}

list_ele_t *merge(list_ele_t *l1, list_ele_t *l2)
{
    // merge with pseudo node
    list_ele_t *result;
    list_ele_t *result_h;
    if (!l2 || (l1 && strcmp(l1->value, l2->value) < 0)) {
        result = l1;
        l1 = l1->next;
    } else {
        result = l2;
        l2 = l2->next;
    }
    result_h = result;
    while (l1 && l2) {
        if (strcmp(l1->value, l2->value) < 0) {
            result->next = l1;
            result = result->next;
            l1 = l1->next;
        } else {
            result->next = l2;
            result = result->next;
            l2 = l2->next;
        }
    }
    if (l1)
        result->next = l1;
    if (l2)
        result->next = l2;
    return result_h;
}
