/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   TopoSorter.h
 * Author: ltsach
 *
 * Created on July 11, 2021, 10:21 PM
 */

#ifndef TOPOSORTER_H
#define TOPOSORTER_H
#include "graph/DGraphModel.h"
#include "list/DLinkedList.h"
#include "sorting/DLinkedListSE.h"

template <class T>
class TopoSorter
{
public:
    static int DFS;
    static int BFS;

protected:
    DGraphModel<T> *graph;
    int (*hash_code)(T &, int);

public:
    TopoSorter(DGraphModel<T> *graph, int (*hash_code)(T &, int) = 0)
    {
        // TODO
        this->graph = graph;
        this->hash_code = hash_code;
    }

    DLinkedListSE<T> sort(int mode = 0, bool sorted = true)
    {
        return (mode == DFS) ? dfsSort(sorted) : bfsSort(sorted);
    }

    DLinkedListSE<T> bfsSort(bool sorted = true)
    {
        DLinkedListSE<T> topoOrder;
        xMap<T, int> indegreeMap = vertex2inDegree();
        DLinkedListSE<T> list0Degres = listOfZeroInDegrees();
        if (sorted && list0Degres.size() != 0)
            list0Degres.sort();

        Queue<T> open;
        for (auto it = list0Degres.begin(); it != list0Degres.end(); ++it)
            open.push(*it);

        while (!open.empty())
        {
            T vertex = open.pop();
            topoOrder.add(vertex);
            DLinkedListSE<T> children = this->graph->getOutwardEdges(vertex);
            if (sorted && children.size() != 0)
                children.sort();
            for (auto childIt = children.begin(); childIt != children.end(); ++childIt)
            {
                T child = *childIt;
                if (open.contains(child) || topoOrder.contains(child))
                    continue;

                int new_indeg = indegreeMap.get(child) - 1;
                indegreeMap.put(child, new_indeg);
                if (new_indeg == 0)
                    open.push(child);
            }
        }
        return topoOrder;
    }

    DLinkedListSE<T> dfsSort(bool sorted = true)
    {
        DLinkedListSE<T> topoOrder;
        xMap<T, int> outdegreeMap = vertex2outDegree();
        DLinkedListSE<T> list0Degres = listOfZeroInDegrees();
        if (sorted && list0Degres.size() != 0)
            list0Degres.sort();

        Stack<T> open;
        for (auto it = list0Degres.begin(); it != list0Degres.end(); ++it)
            open.push(*it);

        while (!open.empty())
        {
            T vertex = open.peek();
            int outDegree = outdegreeMap.get(vertex);
            if (outDegree == 0)
            {
                open.pop();
                topoOrder.add(0, vertex);
            }
            else
            {
                DLinkedListSE<T> children = this->graph->getOutwardEdges(vertex);
                if (sorted && children.size() != 0)
                    children.sort();
                for (auto childIt = children.begin(); childIt != children.end(); ++childIt)
                {
                    T child = *childIt;
                    if (topoOrder.contains(child))
                        continue;

                    if (open.contains(child))
                        open.remove(child);
                    open.push(child);
                }
                outdegreeMap.put(vertex, 0);
            }
        }
        return topoOrder;
    }

protected:
    // Helper functions
    xMap<T, int> vertex2inDegree()
    {
        xMap<T, int> map(this->hash_code);
        typename AbstractGraph<T>::Iterator vertexIt = this->graph->begin();
        while (vertexIt != this->graph->end())
        {
            T vertex = *vertexIt;
            int inDegree = this->graph->inDegree(vertex);
            map.put(vertex, inDegree);

            vertexIt++;
        }
        return map;
    }

    xMap<T, int> vertex2outDegree()
    {
        xMap<T, int> map(this->hash_code);
        typename AbstractGraph<T>::Iterator vertexIt = this->graph->begin();
        while (vertexIt != this->graph->end())
        {
            T vertex = *vertexIt;
            int outDegree = this->graph->outDegree(vertex);
            map.put(vertex, outDegree);

            vertexIt++;
        }
        return map;
    }

    DLinkedListSE<T> listOfZeroInDegrees()
    {
        DLinkedListSE<T> list;
        typename AbstractGraph<T>::Iterator vertexIt = this->graph->begin();
        while (vertexIt != this->graph->end())
        {
            T vertex = *vertexIt;
            int inDegree = this->graph->inDegree(vertex);
            if (inDegree == 0)
                list.add(vertex);

            vertexIt++;
        }
        return list;
    }

}; // TopoSorter

template <class T>
int TopoSorter<T>::DFS = 0;
template <class T>
int TopoSorter<T>::BFS = 1;

/////////////////////////////End of TopoSorter//////////////////////////////////

#endif /* TOPOSORTER_H */
