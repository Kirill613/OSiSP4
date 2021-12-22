#include "windows.h"
#include "TaskQueue.h"
#include <fstream>
#include <iostream>
#define THREAD_COUNT 10

TaskQueue* taskQueue;
TaskQueue* resultQueue;

DWORD WINAPI ProcessTask() {
	Task task = taskQueue->RemoveTask();
	if (!task.text.empty()) {
		sort(task.text.begin(), task.text.end());
		resultQueue->AddTask(task);
	}
	ExitThread(0);
}
void CreateThreadPool(int threadCount) {
	HANDLE* threads = (HANDLE*)malloc(sizeof(HANDLE) * threadCount);
	for (int i = 0; i < threadCount; i++)
	{
		threads[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ProcessTask, NULL, 0, NULL);
	}
	WaitForMultipleObjects(threadCount, threads, TRUE, INFINITE);
	for (int i = 0; i < threadCount; i++)
	{
		if (threads[i] != NULL)
		{
			threads[i] = NULL;
		}
	}
	free(threads);
}
vector<string> ReadFile() {
	ifstream file("input.txt");
	string word;
	vector<string> data;
	while (file >> word) {
		data.push_back(word);
	}
	return data;
}
void WriteResult(const vector<string>& data) {
	ofstream file("output.txt");
	for (const auto& line : data) {
		file << line << endl;
	}
}
void CreateTasks(vector<string>& data) {
	int taskSize = ceil((double)data.size() / THREAD_COUNT);
	for (int i = 0; i < THREAD_COUNT; i++) {
		Task task(vector<string>{data.begin() + i * taskSize, 
			data.size() < (i + 1) * taskSize ? 
			data.end() : data.begin() + (i + 1) * taskSize});
		taskQueue->AddTask(task);
	}
}
vector<string> MergeSort(vector<string> v1, vector<string> v2)
{
	int i = 0, j = 0;
	vector<string> res{};
	while (i < v1.size() && j < v2.size()) {
		if (v1[i] < v2[j])
			res.push_back(v1[i++]);
		else
			res.push_back(v2[j++]);
	}
	while (i < v1.size())
		res.push_back(v1[i++]);
	while (j < v2.size())
		res.push_back(v2[j++]);
	return res;
}
int main()
{
	std::cout << "Data from input.txt" << endl;
	taskQueue = new TaskQueue();
	resultQueue = new TaskQueue();
	auto data = ReadFile();
	CreateTasks(data);
	CreateThreadPool(THREAD_COUNT);
	vector<string> result;
	if (resultQueue->Size() > 0) {
		result = taskQueue->RemoveTask().text;
		while (resultQueue->Size() > 0) {
			data = resultQueue->RemoveTask().text;
			result = MergeSort(result, data);
		}
		WriteResult(result);
	}
	free(taskQueue);
	free(resultQueue);
	std::cout << "Sorted result in  output.txt" << endl;
	return 0;
}