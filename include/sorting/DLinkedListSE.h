/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   DLinkedListSE.h
 * Author: LTSACH
 *
 * Created on 31 August 2020, 14:13
 */

#ifndef DLINKEDLISTSE_H
#define DLINKEDLISTSE_H
#include "list/DLinkedList.h"
#include "sorting/ISort.h"

template <class T>
class DLinkedListSE : public DLinkedList<T>
{
public:
    DLinkedListSE(
        void (*removeData)(DLinkedList<T> *) = 0,
        bool (*itemEQ)(T &, T &) = 0) : DLinkedList<T>(removeData, itemEQ) {

                                        };

    DLinkedListSE(const DLinkedList<T> &list)
    {
        this->copyFrom(list);
    }

    void sort(int (*comparator)(T &, T &) = 0)
    {
        this->tail->prev->next = nullptr;
        this->tail->prev = nullptr;

        typename DLinkedList<T>::Node *first = this->head->next;
        first->prev = nullptr;
        mergeSort(first, comparator);

        typename DLinkedList<T>::Node *last = this->head;
        while (last->next != nullptr)
            last = last->next;

        last->next = this->tail;
        this->tail->prev = last;

        this->head->next = first;
        this->head->next->prev = this->head;
    }

protected:
    static int compare(T &lhs, T &rhs, int (*comparator)(T &, T &) = 0)
    {
        if (comparator != 0)
            return comparator(lhs, rhs);
        else
        {
            if (lhs < rhs)
                return -1;
            else if (lhs > rhs)
                return +1;
            else
                return 0;
        }
    }

    void mergeSort(typename DLinkedList<T>::Node *&head, int (*comparator)(T &, T &) = nullptr)
    {
        if (head != nullptr && head->next != nullptr)
        {
            typename DLinkedList<T>::Node *second;
            divide(head, second);
            mergeSort(head, comparator);
            mergeSort(second, comparator);
            head = merge(head, second, comparator);
        }
    }

    void divide(typename DLinkedList<T>::Node *&first, typename DLinkedList<T>::Node *&second)
    {
        typename DLinkedList<T>::Node *midpt = first;
        typename DLinkedList<T>::Node *endpt = first->next;
        while (endpt != nullptr && endpt->next != nullptr)
        {
            midpt = midpt->next;
            endpt = endpt->next->next;
        }
        second = midpt->next;
        midpt->next = nullptr;
        if (second != nullptr)
            second->prev = nullptr;
    }

    typename DLinkedList<T>::Node *merge(typename DLinkedList<T>::Node *first, typename DLinkedList<T>::Node *second, int (*comparator)(T &, T &) = nullptr)
    {
        typename DLinkedList<T>::Node dummy;
        typename DLinkedList<T>::Node *ptr = &dummy;

        while (first != nullptr && second != nullptr)
        {
            if (compare(first->data, second->data, comparator) <= 0)
            {
                ptr->next = first;
                first->prev = ptr;
                first = first->next;
            }
            else
            {
                ptr->next = second;
                second->prev = ptr;
                second = second->next;
            }
            ptr = ptr->next;
        }

        if (first != nullptr)
        {
            ptr->next = first;
            first->prev = ptr;
        }
        else
        {
            ptr->next = second;
            if (second != nullptr)
                second->prev = ptr;
        }

        dummy.next->prev = nullptr;
        return dummy.next;
    }
};

#endif /* DLINKEDLISTSE_H */
