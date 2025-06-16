//======= Copyright (c) 2025, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Test Driven Development - graph
//
// $NoKeywords: $ivs_project_1 $tdd_code.cpp
// $Author:     BORIS NICOLAS DRÁB <xdrabbo00@stud.fit.vutbr.cz>
// $Date:       $2025-02-19
//============================================================================//
/**
 * @file tdd_code.cpp
 * @author Martin Dočekal
 * @author Karel Ondřej
 *
 * @brief Implementace metod tridy reprezentujici graf.
 */

#include "tdd_code.h"


Graph::Graph(){}

Graph::~Graph(){}

std::vector<Node*> Graph::nodes() {
    return AllNodes;
}

std::vector<Edge> Graph::edges() const{
    return AllEdges;
}

Node* Graph::addNode(size_t nodeId) {
    for (unsigned index = 0; index < AllNodes.size(); index++) {
        if (AllNodes[index]->id == nodeId) {
            return nullptr;
        }
    }
    Node* NewNode = new Node(nodeId, 0);
    AllNodes.push_back(NewNode);

    return NewNode;   
}

bool Graph::addEdge(const Edge& edge){
    if (edge.a == edge.b) {
        return false;
    }

    for (unsigned index = 0; index < AllEdges.size(); ++index)  {
        if (AllEdges[index] == edge) {
            return false;
        }

    }

    addNode(edge.a);
    addNode(edge.b);

    AllEdges.push_back(edge);
    
    return true;
}

void Graph::addMultipleEdges(const std::vector<Edge>& edges) {
    for (unsigned index = 0; index < edges.size(); index++) {

        bool exist = false;

        for (unsigned index2 = 0; index2 < AllEdges.size(); index2++) {
            if (AllEdges.at(index2) == edges.at(index)){
                exist = true;
                break;
            }
        }

        if(exist) {
            continue;
        }
        addEdge(edges.at(index));
    }
}

Node* Graph::getNode(size_t nodeId){
    for(size_t index = 0; index < this->AllNodes.size(); index++) {
        if(AllNodes.at(index)->id == nodeId) {
            return AllNodes.at(index);

        }
    }
    return nullptr;
}

bool Graph::containsEdge(const Edge& edge) const{
    for (size_t index = 0; index < this->AllEdges.size(); index++) {
        if (AllEdges.at(index) == edge) {
            return true;

        }
    }
    return false;
}

void Graph::removeNode(size_t nodeId){
    unsigned i;
    Node *RemoveNode = this->getNode(nodeId);

    if (RemoveNode == nullptr) {
        throw std::out_of_range("Node does not exist!");
    }

    for (unsigned index = 0; index < AllEdges.size(); ++index) {
        if (AllEdges[index].a == nodeId || AllEdges[index].b == nodeId) {
            AllEdges.erase(AllEdges.begin() + index);
            --index;
        }
    }

    for (unsigned index= 0; index < AllNodes.size(); ++index) {
        if (AllNodes[index]->id == nodeId) {
            delete AllNodes[index];
            AllNodes.erase(AllNodes.begin() + index);
            break;
        }
    }
}

void Graph::removeEdge(const Edge& edge){
    if (!this->containsEdge(edge)) {
        throw std::out_of_range("Node does not exist!");
    }

    for (size_t index = 0; index < this->AllEdges.size(); ++index)  {
        if (this->AllEdges[index] == edge) {
            for (size_t index2 = index; index2 < this->AllEdges.size()-1; index2++) {
                this->AllEdges[index2] = this->AllEdges[index2 + 1];
            }
            this->AllEdges.pop_back();
            return;
        }
    }
}

size_t Graph::nodeCount() const{
    size_t lenght = this->AllNodes.size();
    return lenght;

}

size_t Graph::edgeCount() const{
    size_t lenght = this->AllEdges.size();
    return lenght;
}

size_t Graph::nodeDegree(size_t nodeId) const{
    unsigned degree = 0;

    bool nodeExist = false;

    for (unsigned index = 0; index < this->AllEdges.size(); index++) {
        if (this->AllEdges.at(index).a == nodeId) {
            nodeExist = true;

            degree++;
        }

        if (this->AllEdges.at(index).b == nodeId) {
            nodeExist = true;

            degree++;
        }
    }

    if (!nodeExist) {
        throw std::out_of_range("Node does not exist!");
    }

    return degree; 
}

size_t Graph::graphDegree() const{
    int tmp = 0;
    int count = 0;

    for (unsigned index = 0; index < AllNodes.size(); index++) {
        for (unsigned index2 = 0; index2 < AllEdges.size(); index2++) {
            if (AllNodes[index]->id == AllEdges[index2].a) {
                count++;
            }

            if (AllNodes[index]->id == AllEdges[index2].b) {
                count++;
            }
        }

        if (tmp < count) {
            tmp = count;
        }

        count=0;
    }
    
    return tmp;

}

void Graph::coloring() {
    for (size_t index = 0; index < AllNodes.size(); index++) {
        int colors[AllNodes.size() + 1]; 

        for (size_t index2 = 0; index2 <= AllNodes.size(); index2++) {
            colors[index2] = 0; 
        }
    
        for (size_t index3 = 0; index3 < AllEdges.size(); index3++) {
            if (AllEdges[index3].a == AllNodes[index]->id) {
                Node* neighbors = getNode(AllEdges[index3].b);

                if (neighbors) {
                        colors[neighbors->color] = 1;
                }

            } else if (AllEdges[index3].b == AllNodes[index]->id) {
                Node* neighbors = getNode(AllEdges[index3].a);

                if (neighbors) {
                    colors[neighbors->color] = 1;
                }
            }
        }
    
        int color = 1;
        while (colors[color]) {
            color++;
        }
        AllNodes[index]->color = color;
    }
    
}

void Graph::clear() {
    bool EdgesEmpty = AllEdges.empty();
    bool NodesEmpty = AllNodes.empty();

    while (!EdgesEmpty) {
        this->removeEdge(AllEdges.back());
        EdgesEmpty = AllEdges.empty();
    }

    while (!NodesEmpty) {
        this->removeNode(AllNodes.back()->id);
        NodesEmpty = AllNodes.empty();
    }
    
}




/*** Konec souboru tdd_code.cpp ***/
