// Graph.h
#ifndef HW3_MO_GRAPH_H
#define HW3_MO_GRAPH_H

#include "GraphNode.h"
#include <vector>
#include <algorithm>
#include <set>
#include <iomanip>

using namespace std;

class Graph {

private:

    vector<GraphNode> _graph; // Граф, представленный в виде вектора узлов

public:

    Graph() = default;
    explicit Graph(const vector<GraphNode>& graph) : _graph(graph) {}

    // Метод для построения графа
    void buildGraph() {
        //Заполнение nextTasks у вершин
        fillNextNode();
        //Вычисление ранних и поздних стартов и финишей
        calculateEarlyStartAndFinish();
        calculateLateStartAndFinish();
        //Вычисление всех резервов
        calculateResrve();
        calculateTotalReserve();
        calculateFreeReserve();
        //Вывод результатов в консоль
        printDoneTasks();
        printDoneNodes();
        //Нахождение критического пути
        makeCriticalPath();
    }

    // Метод для расчета раннего старта и завершения
    void calculateEarlyStartAndFinish() {
        vector<GraphNode> calculatedEarly;
        _graph[0].earlyStart = 0;
        calculatedEarly.push_back(_graph[0]);
        while (!calculatedEarly.empty()) {
            vector<GraphNode> newCalculatedBuffer;
            for (auto& node : calculatedEarly) {
                for (auto& next : node.nextTasks) {
                    int foundIndex = foundIndexPair(next);
                    int newEarlyStart = node.earlyStart + next.second.duration;
                    next.second.earlyFinish = newEarlyStart;
                    if (foundIndex != -1) {
                        if (_graph[foundIndex].earlyStart == 0 || newEarlyStart > _graph[foundIndex].earlyStart) {
                            _graph[foundIndex].earlyStart = newEarlyStart;
                        }
                        newCalculatedBuffer.push_back(_graph[foundIndex]);
                    }
                }
            }
            calculatedEarly.clear();
            for (auto& newEarly : newCalculatedBuffer) {
                calculatedEarly.push_back(newEarly);
            }
        }
    }

    // Метод для расчета позднего старта и завершения
    void calculateLateStartAndFinish() {
        vector<GraphNode> calculatedLate;
        _graph.back().lateFinish = _graph.back().earlyStart;
        calculatedLate.push_back(_graph.back());
        while (!calculatedLate.empty()) {
            vector<GraphNode> newCalculatedBuffer;
            for (auto& node : calculatedLate) {
                for (auto& previous : node.previousTasks) {
                    int foundIndex = foundIndexPair(previous);
                    int newLateFinish = node.lateFinish - previous.second.duration;
                    previous.second.lateStart = newLateFinish;
                    if (foundIndex != -1) {
                        if (_graph[foundIndex].lateFinish == 0 || newLateFinish < _graph[foundIndex].lateFinish) {
                            _graph[foundIndex].lateFinish = newLateFinish;
                            newCalculatedBuffer.push_back(_graph[foundIndex]);
                        }
                    }
                }
            }
            calculatedLate.clear();
            for (auto& newLate : newCalculatedBuffer) {
                calculatedLate.push_back(newLate);
            }
        }
    }

    // Метод для расчета резерва
    void calculateResrve() {
        for (auto& node : _graph) {
            node.reserveNode = node.lateFinish - node.earlyStart;
        }
    }

    // Метод для поиска индекса пары узел-задача в графе
    int foundIndexPair(pair<GraphNode, Task>& pair) {
        for (int i = 0; i < _graph.size(); i++) {
            if (_graph[i].nameNode == pair.first.nameNode) {
                return i;
            }
        }
        return -1;
    }

    // Метод для заполнения следующего узла для каждого узла в графе
    void fillNextNode() {
        for (auto& node : _graph) {
            for (auto& otherNode : _graph) {
                for (auto& previousPair : otherNode.previousTasks) {
                    if (previousPair.first.nameNode == node.nameNode) {
                        node.nextTasks.push_back(make_pair(otherNode, previousPair.second));
                    }
                }
            }
        }
    }

    // Метод для получения узла из графа по заданному узлу
    GraphNode getNode(GraphNode& nodeOther) {
        for (auto& node : _graph) {
            if (node.nameNode == nodeOther.nameNode) {
                return node;
            }
        }
        return {};
    }

    // Метод для расчета общего резерва
    void calculateTotalReserve() {
        for (auto& node : _graph) {
            for (auto& prev : node.previousTasks) {
                GraphNode nodeFromGraph = getNode(prev.first);
                prev.second.totalReserve = node.lateFinish - nodeFromGraph.earlyStart - prev.second.duration;
            }
        }
    }

    // Метод для расчета свободного резерва
    void calculateFreeReserve() {
        for (auto& node : _graph) {
            for (auto& prev : node.previousTasks) {
                GraphNode nodeFromGraph = getNode(prev.first);
                prev.second.freeReserve = node.earlyStart - nodeFromGraph.earlyStart - prev.second.duration;
            }
        }
    }

    // Метод для построения критического пути
    void makeCriticalPath() {
        set<Task> nullReserveTasks;
        for (const auto& node : _graph) {
            for (const auto& taskPair : node.previousTasks) {
                const Task& task = taskPair.second;
                if (task.freeReserve == 0 && task.totalReserve == 0) {
                    nullReserveTasks.insert(task);
                }
            }
        }

        set<GraphNode> nullReserveNodes;
        for (auto& node : _graph) {
            if (node.reserveNode == 0) {
                if (isInSet(node.previousTasks, nullReserveTasks)) {
                    nullReserveNodes.insert(node);
                }
                if (isInSet(node.nextTasks, nullReserveTasks)) {
                    nullReserveNodes.insert(node);
                }
            }
        }

        auto lastNodeIterator = nullReserveNodes.end();
        --lastNodeIterator;

        const GraphNode& lastNode = *lastNodeIterator;

        cout << "Critical Path:\n";
        for (const auto& startNode : nullReserveNodes) {
            cout << "[" << startNode.nameNode << "] ";
            if (startNode != lastNode) {
                cout << "- ";
            }
            findCriticalPath(startNode, nullReserveTasks);
        }
        cout << endl << "Critical Path Length: " << lastNode.earlyStart << endl;
    }

    // Метод для поиска критического пути, начиная с текущего узла
    void findCriticalPath(const GraphNode& currentNode, const set<Task>& nullReserveTasks) {
        bool firstNode = true;
        for (const auto& nextNode : currentNode.nextTasks) {
            if (isInSet(nullReserveTasks, nextNode.second)) {
                if (!firstNode) {
                    cout << " -> ";
                }
                cout << "(" << nextNode.second.nameTask << ") -> ";
                findCriticalPath(nextNode.first, nullReserveTasks);
                firstNode = false;
            }
        }
    }

    // Метод для проверки наличия задачи в множестве
    bool isInSet(const set<Task>& tasks, const Task& task) {
        return tasks.find(task) != tasks.end();
    }

    // Метод для проверки наличия задачи в векторе пар узел-задача
    bool isInSet(vector<pair<GraphNode, Task>>& tasks, set<Task>& set) {
        bool flag = false;
        for (auto& task : tasks) {
            for (auto &elem: set) {
                if (task.second == elem) {
                    flag = true;
                    break;
                }
            }
        }
        return flag;
    }

    GraphNode getNodeFromTask(const Task& task) {
        for (auto& node : _graph) {
            for (auto& nextTask : node.nextTasks) {
                if (nextTask.second == task) {
                    return getNode(nextTask.first);
                }
            }
        }
        return {};
    }

    void printDoneTasks() {
        set<Task> setTasks;
        for (const auto& node : _graph) {
            for (const auto& taskPair : node.previousTasks) {
                setTasks.insert(taskPair.second);
            }
        }
        cout << "------------------------------------------------------------------------------------------" << endl;
        cout << "|  Name  |  Duration  |  Early Finish  |  Late Start  |  Total Reserve  |  Free Reserve  |" << endl;
        cout << "------------------------------------------------------------------------------------------" << endl;
        for (const auto& task : setTasks) {
            GraphNode nextNode = getNodeFromTask(task);
            cout << "|   " << setw(2) << task.nameTask << "   |   " <<
                 setw(4) << task.duration << "     |    " <<
                 setw(6) << nextNode.earlyStart << "      |   " <<
                 setw(6) << nextNode.lateFinish << "     |        " <<
                 setw(2) << task.totalReserve << "       |       " <<
                 setw(2) << task.freeReserve << "       |" << endl;
        }
        cout << "------------------------------------------------------------------------------------------" << endl;
    }



    void printDoneNodes() {
        cout << "------------------------------------------------------" << endl;
        cout << "|  Name  |  Early Start  |  Late Finish  |  Reserve  |" << endl;
        cout << "------------------------------------------------------" << endl;
        for (const auto& node : _graph) {
            cout << "|   " << setw(2) << node.nameNode << "   |     " <<
                 setw(4) << node.earlyStart << "      |    " <<
                 setw(6) << node.lateFinish << "     |   " <<
                 setw(4) << node.reserveNode << "    |" << endl;
        }
        cout << "------------------------------------------------------" << endl;
    }

};

#endif //HW3_MO_GRAPH_H