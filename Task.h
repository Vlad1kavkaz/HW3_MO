// Task.h
#ifndef HW3_MO_TASK_H
#define HW3_MO_TASK_H

#include <iostream>
#include <string>

using namespace std;

class Task {
public:
    string nameTask;
    int duration;
    int earlyFinish;
    int lateStart;
    int totalReserve;
    int freeReserve;

    Task() : duration(0), totalReserve(-1), freeReserve(-1), earlyFinish(0), lateStart(0) {}

    Task(const string& nameTask, int duration) : nameTask(nameTask), duration(duration),
                                                 totalReserve(-1), freeReserve(-1), earlyFinish(0), lateStart(0) {}
};

// Операторы сравнения для Task
bool operator < (const Task& lhs, const Task& rhs) {
    return lhs.nameTask < rhs.nameTask;
}

bool operator == (const Task& task1, const Task& task2) {
    return task1.nameTask == task2.nameTask && task1.duration == task2.duration;
}

#endif //HW3_MO_TASK_H
