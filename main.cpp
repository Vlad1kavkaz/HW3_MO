#include "Graph.h"
#include "GraphNode.h"
#include "Task.h"


int main() {
    Task taskA("a", 3);
    Task taskB("b", 5);
    Task taskC("c", 2);
    Task taskD("d", 4);
    Task taskE("e", 3);
    Task taskF("f", 1);
    Task taskG("g", 4);
    Task taskH("h", 3);
    Task taskI("i", 3);
    Task taskJ("j", 2);
    Task taskK("k", 5);

    GraphNode graphNode1({}, {}, 1);
    GraphNode graphNode2({taskA}, {graphNode1}, 2);
    GraphNode graphNode3({taskC}, {graphNode2}, 3);
    GraphNode graphNode4({taskE, taskD}, {graphNode2, graphNode3}, 4);
    GraphNode graphNode5({taskG}, {graphNode3}, 5);
    GraphNode graphNode6({taskF, taskH, taskJ, taskB}, {graphNode4, graphNode5, graphNode3, graphNode1}, 6);
    GraphNode graphNode7({taskI, taskK}, {graphNode6, graphNode5}, 7);

    vector<GraphNode> graphVector = {graphNode1, graphNode2, graphNode3,
                                     graphNode4, graphNode5,
                                     graphNode6, graphNode7};


    Graph graph(graphVector);
    graph.buildGraph();

    return 0;
}
