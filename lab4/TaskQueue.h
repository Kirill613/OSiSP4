#pragma once
#include <string>;
#include <queue>
#include "Task.h"
using namespace std;
class TaskQueue {
public:
	TaskQueue();
	void AddTask(Task task);
	Task RemoveTask();
	int Size();
private:
	CRITICAL_SECTION criticalSection;
	queue<Task> tasks;
};