#pragma once
#include <string>
#include <vector>
using namespace std;
class Task {
public:
	Task();
	Task(vector<string> data);
	vector<string> text;
};