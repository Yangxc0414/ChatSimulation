#include "Utils_yxc.h"
#include <direct.h>
#include <fstream>
#include <iostream>
#include <cerrno>
#include <cstring>

unordered_map<string, queue<Message_yxc>> messageQueues;
unordered_map<string, mutex> messageQueueMutexes;
unordered_map<string, condition_variable> messageQueueCVs;

bool createDirectory(const string& path) {
    if (_mkdir(path.c_str()) == 0 || errno == EEXIST) {
        return true;
    }
    else {
        char errorBuffer[256];
        strerror_s(errorBuffer, sizeof(errorBuffer), errno);
        cerr << "创建目录失败: " << errorBuffer << endl;
        return false;
    }
}

bool fileExists(const string& path) {
    ifstream f(path.c_str());
    return f.good();
}

time_t parseDate(const string& dateStr) {
    tm tm_date = {};
    if (sscanf_s(dateStr.c_str(), "%d-%d-%d", &tm_date.tm_year, &tm_date.tm_mon, &tm_date.tm_mday) != 3) {
        cerr << "日期格式无效，期望格式为 YYYY-MM-DD。" << endl;
        return -1;
    }
    tm_date.tm_year -= 1900;
    tm_date.tm_mon -= 1;
    tm_date.tm_sec = 0;
    tm_date.tm_min = 0;
    tm_date.tm_hour = 0;
    tm_date.tm_isdst = -1;
    return mktime(&tm_date);
}

string getCurrentTimestamp() {
    time_t now = time(0);
    tm timeinfo;
    localtime_s(&timeinfo, &now);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", &timeinfo);
    return string(timestamp);
}

void pause() {
    cout << "\n按回车键返回菜单..." << endl;
    cin.get();
}