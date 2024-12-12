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
        DLinkedListSE<T> topologicalOrder;
        xMap<T, int> indegreeMap = vertex2inDegree();
        DLinkedListSE<T> list0Degres = listOfZeroInDegrees();
        if (sorted && !list0Degres.empty())
            list0Degres.sort();

        Queue<T> open;
        for (const auto &vertex : list0Degres)
            open.push(vertex);

        while (!open.empty())
        {
            T vertex = open.pop();
            topologicalOrder.add(vertex);
            DLinkedListSE<T> children = graph->getOutwardEdges(vertex);
            if (sorted && !children.empty())
                children.sort();

            for (const auto &child : children)
            {
                if (open.contains(child) || topologicalOrder.contains(child))
                    continue;

                int new_indeg = indegreeMap.get(child) - 1;
                indegreeMap.put(child, new_indeg);
                if (new_indeg == 0)
                    open.push(child);
            }
        }
        return topologicalOrder;
    }

    DLinkedListSE<T> dfsSort(bool sorted = true)
    {
        DLinkedListSE<T> topologicalOrder;
        xMap<T, int> outdegreeMap = vertex2outDegree();
        DLinkedListSE<T> list0Degres = listOfZeroInDegrees();
        if (sorted && !list0Degres.empty())
            list0Degres.sort();

        Stack<T> open;
        for (const auto &vertex : list0Degres)
            open.push(vertex);

        while (!open.empty())
        {
            T vertex = open.peek();
            int outDegree = outdegreeMap.get(vertex);
            if (outDegree == 0)
            {
                open.pop();
                topologicalOrder.add(0, vertex);
            }
            else
            {
                DLinkedListSE<T> children = graph->getOutwardEdges(vertex);
                if (sorted && !children.empty())
                    children.sort();

                for (const auto &child : children)
                {
                    if (topologicalOrder.contains(child))
                        continue;

                    if (open.contains(child))
                        open.remove(child);
                    open.push(child);
                }
                outdegreeMap.put(vertex, 0);
            }
        }
        return topologicalOrder;
    }

protected:
    // Helper functions
    // TODO
    xMap<T, int> vertex2inDegree()
    {
        xMap<T, int> map(this->hash_code);
        for (auto vertexIt = this->graph->begin(); vertexIt != this->graph->end(); ++vertexIt)
        {
            T vertex = *vertexIt;
            int inDegree = this->graph->inDegree(vertex);
            map.put(vertex, inDegree);
        }
        return map;
    }

    xMap<T, int> vertex2outDegree()
    {
        xMap<T, int> map(this->hash_code);
        for (auto vertexIt = this->graph->begin(); vertexIt != this->graph->end(); ++vertexIt)
        {
            T vertex = *vertexIt;
            int outDegree = this->graph->outDegree(vertex);
            map.put(vertex, outDegree);
        }
        return map;
    }

    DLinkedListSE<T> listOfZeroInDegrees()
    {
        DLinkedListSE<T> list;
        for (auto vertexIt = this->graph->begin(); vertexIt != this->graph->end(); ++vertexIt)
        {
            T vertex = *vertexIt;
            int inDegree = this->graph->inDegree(vertex);
            if (inDegree == 0)
                list.add(vertex);
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
