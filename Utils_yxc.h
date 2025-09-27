#ifndef UTILS_YXC_H
#define UTILS_YXC_H

#include <string>
#include <ctime>
#include <unordered_map>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include "Message_yxc.h"

using namespace std;

const string SERVICE_DIR = "./user/";
extern unordered_map<string, queue<Message_yxc>> messageQueues;
extern unordered_map<string, mutex> messageQueueMutexes;
extern unordered_map<string, condition_variable> messageQueueCVs;

bool createDirectory(const string& path);
bool fileExists(const string& path);
time_t parseDate(const string& dateStr);
string getCurrentTimestamp();
void pause();

#endif // UTILS_YXC_H