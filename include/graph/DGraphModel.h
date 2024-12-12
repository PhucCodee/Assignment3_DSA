/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   DGraphModel.h
 * Author: LTSACH
 *
 * Created on 23 August 2020, 19:36
 */

#ifndef DGRAPHMODEL_H
#define DGRAPHMODEL_H
#include "graph/AbstractGraph.h"
#include "stacknqueue/Queue.h"
#include "stacknqueue/Stack.h"
#include "hash/xMap.h"
// #include "stacknqueue/PriorityQueue.h"
#include "sorting/DLinkedListSE.h"

//////////////////////////////////////////////////////////////////////
///////////// GraphModel: Directed Graph Model    ////////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
class DGraphModel : public AbstractGraph<T>
{
private:
public:
    DGraphModel(
        bool (*vertexEQ)(T &, T &),
        string (*vertex2str)(T &)) : AbstractGraph<T>(vertexEQ, vertex2str)
    {
    }

    void connect(T from, T to, float weight = 0)
    {
        // TODO
        auto fromVertex = this->getVertexNode(from);
        auto toVertex = this->getVertexNode(to);
        if (!fromVertex)
            throw VertexNotFoundException(this->vertex2Str(*fromVertex));
        if (!toVertex)
            throw VertexNotFoundException(this->vertex2Str(*toVertex));

        fromVertex->connect(toVertex, weight);
    }

    void disconnect(T from, T to)
    {
        // TODO
        auto fromVertex = this->getVertexNode(from);
        auto toVertex = this->getVertexNode(to);
        if (!fromVertex)
            throw VertexNotFoundException(this->vertex2Str(*fromVertex));
        if (!toVertex)
            throw VertexNotFoundException(this->vertex2Str(*toVertex));

        fromVertex->removeTo(toVertex);
    }

    void remove(T vertex)
    {
        // TODO
        auto deleteVertex = this->getVertexNode(vertex);
        if (!deleteVertex)
            throw VertexNotFoundException(this->vertex2Str(*deleteVertex));

        auto inwardEdges = this->getInwardEdges(vertex);
        for (auto it = inwardEdges.begin(); it != inwardEdges.end(); ++it)
        {
            auto node = this->getVertexNode(*it);
            node->removeTo(deleteVertex);
        }

        auto outwardEdges = deleteVertex->getOutwardEdges();
        for (auto it = outwardEdges.begin(); it != outwardEdges.end(); ++it)
        {
            auto node = this->getVertexNode(*it);
            deleteVertex->removeTo(node);
        }

        this->nodeList.removeItem(deleteVertex);
    }

    static DGraphModel<T> *create(
        T *vertices, int nvertices, Edge<T> *edges, int nedges,
        bool (*vertexEQ)(T &, T &),
        string (*vertex2str)(T &))
    {
        // TODO
        auto newModel = new DGraphModel<T>(vertexEQ, vertex2str);

        for (int i = 0; i < nvertices; ++i)
            newModel->add(vertices[i]);
        for (int i = 0; i < nedges; ++i)
            newModel->connect(edges[i].from, edges[i].to, edges[i].weight);

        return newModel;
    }
};

#endif /* DGRAPHMODEL_H */
