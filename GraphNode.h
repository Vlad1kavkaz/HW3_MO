// GraphNode.h
#ifndef HW3_MO_GRAPHNODE_H
#define HW3_MO_GRAPHNODE_H

#include "Task.h"
#include <utility>
#include <vector>

using namespace std;

class GraphNode {
public:
    string nameNode;
    int earlyStart{};
    int lateFinish{};
    int reserveNode{};
    vector<pair<GraphNode, Task>> previousTasks;
    vector<pair<GraphNode, Task>> nextTasks;

    GraphNode() = default;

    // Конструктор с использованием списка инициализации
    GraphNode(const vector<Task>& previousTasks, const vector<GraphNode>& nodeOther, int index)
            : nameNode(to_string(index)), reserveNode(0), earlyStart(0), lateFinish(0) {
        for (size_t i = 0; i < previousTasks.size(); i++) {
            this->previousTasks.push_back(make_pair(nodeOther[i], previousTasks[i]));
        }
    }

    // Конструктор копирования
    GraphNode(const GraphNode& other) {
        nameNode = other.nameNode;
        earlyStart = other.earlyStart;
        lateFinish = other.lateFinish;
        reserveNode = other.reserveNode;
        previousTasks = other.previousTasks; // Глубокое копирование
        nextTasks = other.nextTasks;         // Глубокое копирование
    }

    // Оператор присваивания
    GraphNode& operator = (const GraphNode& other) {
        if (this != &other) {
            nameNode = other.nameNode;
            earlyStart = other.earlyStart;
            lateFinish = other.lateFinish;
            reserveNode = other.reserveNode;
            previousTasks = other.previousTasks; // Глубокое копирование
            nextTasks = other.nextTasks;         // Глубокое копирование
        }
        return *this;
    }
};

// Операторы сравнения для GraphNode
bool operator==(const vector<pair<GraphNode, Task>>& pair1, const vector<pair<GraphNode, Task>>& pair2) {
    return pair1.size() == pair2.size() && equal(pair1.begin(), pair1.end(), pair2.begin(),
                                                 [](const pair<GraphNode, Task>& p1, const pair<GraphNode, Task>& p2) {
                                                     return p1.first.nameNode == p2.first.nameNode &&
                                                            p1.second.nameTask == p2.second.nameTask;
                                                 });
}

// Операторы сравнения для GraphNode
bool operator!=(const vector<pair<GraphNode, Task>>& pair1, const vector<pair<GraphNode, Task>>& pair2) {
    return !(pair1 == pair2);
}

// Операторы сравнения для GraphNode
bool operator==(const GraphNode& node1, const GraphNode& node2) {
    return node1.nextTasks == node2.nextTasks && node1.previousTasks == node2.previousTasks;
}

// Операторы сравнения для GraphNode
bool operator!=(const GraphNode& node1, const GraphNode& node2) {
    return !(node1 == node2);
}

// Оператор сравнения для GraphNode
bool operator < (const GraphNode& node1, const GraphNode& node2) {
    return node1.nameNode < node2.nameNode;
}

#endif //HW3_MO_GRAPHNODE_H
