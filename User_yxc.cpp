#include "User_yxc.h"
#include "Utils_yxc.h"
#include <sstream>
#include <limits>
#include <regex>

User_yxc::User_yxc() {}

User_yxc::User_yxc(const string& id, const string& pwd, const string& name, time_t birth, const string& loc)
    : userID(id), password(pwd), nickname(name), birthTime(birth), location(loc), firstTime(time(0)) {
    calculateAge();
    privacySettings["moments"] = "friends";
    privacySettings["blogs"] = "public";
}

User_yxc::~User_yxc() {}

void User_yxc::displayInfo() const {
    cout << "==============================" << endl;
    cout << "用户ID: " << userID << endl;
    if (!wechatID.empty()) {
        cout << "微信ID: " << wechatID << endl;
    }
    cout << "昵称: " << nickname << endl;

    char birthDate[11];
    tm birth_tm;
    localtime_s(&birth_tm, &birthTime);
    snprintf(birthDate, sizeof(birthDate), "%d-%02d-%02d",
        birth_tm.tm_year + 1900, birth_tm.tm_mon + 1, birth_tm.tm_mday);
    cout << "生日: " << birthDate << endl;
    cout << "年龄: " << age << " 岁" << endl;
    cout << "位置: " << location << endl;

    time_t currentTime = time(0);
    long long tAge = (currentTime - firstTime) / (60 * 60 * 24);
    cout << "T年龄: " << tAge << " 天" << endl;

    cout << "已激活的服务: ";
    for (const auto& service : services) {
        cout << service << " ";
    }
    cout << endl;

    cout << "当前状态: " << status << endl;
    cout << "==============================" << endl;
}

void User_yxc::calculateAge() {
    time_t currentTime = time(0);
    tm now_tm;
    localtime_s(&now_tm, &currentTime);

    tm birth_tm;
    localtime_s(&birth_tm, &birthTime);

    age = now_tm.tm_year - birth_tm.tm_year;

    if ((now_tm.tm_mon < birth_tm.tm_mon) ||
        (now_tm.tm_mon == birth_tm.tm_mon && now_tm.tm_mday < birth_tm.tm_mday)) {
        age--;
    }
}
// 将用户信息保存到文件的函数
void User_yxc::saveToFile() const {
    // 创建基目录（如果不存在）
    if (!createDirectory(SERVICE_DIR)) {
        cerr << "无法创建基目录: " << SERVICE_DIR << endl;
        return;
    }

    // 构建用户目录路径
    string userPath = SERVICE_DIR + userID;

    // 创建用户目录（如果不存在）
    if (!createDirectory(userPath)) {
        cerr << "无法创建用户目录: " << userPath << endl;
        return;
    }

    // 保存 QQ 群组信息到文件
    string qqGroupPath = userPath + "/qq_groups.txt";
    ofstream qqGroupFile(qqGroupPath.c_str(), ios::out | ios::trunc);
    if (qqGroupFile.is_open()) {
        // 遍历群组列表，将属于 QQ 平台的群组写入文件
        for (const auto& groupID : groupsList) {
            if (groupMap_yxc.find(groupID) != groupMap_yxc.end() && groupMap_yxc.at(groupID).getPlatform() == "QQ") {
                qqGroupFile << groupID << endl;
            }
        }
        qqGroupFile.close();
    }
    else {
        cerr << "无法打开文件进行写入: " << qqGroupPath << endl;
    }

    // 保存微信群组信息到文件
    string wechatGroupPath = userPath + "/wechat_groups.txt";
    ofstream wechatGroupFile(wechatGroupPath.c_str(), ios::out | ios::trunc);
    if (wechatGroupFile.is_open()) {
        // 遍历群组列表，将属于微信平台的群组写入文件
        for (const auto& groupID : groupsList) {
            if (groupMap_yxc.find(groupID) != groupMap_yxc.end() && groupMap_yxc.at(groupID).getPlatform() == "WeChat") {
                wechatGroupFile << groupID << endl;
            }
        }
        wechatGroupFile.close();
    }
    else {
        cerr << "无法打开文件进行写入: " << wechatGroupPath << endl;
    }

    // 保存微博群组信息到文件
    string weiboGroupPath = userPath + "/weibo_groups.txt";
    ofstream weiboGroupFile(weiboGroupPath.c_str(), ios::out | ios::trunc);
    if (weiboGroupFile.is_open()) {
        // 遍历群组列表，将属于微博平台的群组写入文件
        for (const auto& groupID : groupsList) {
            if (groupMap_yxc.find(groupID) != groupMap_yxc.end() && groupMap_yxc.at(groupID).getPlatform() == "Weibo") {
                weiboGroupFile << groupID << endl;
            }
        }
        weiboGroupFile.close();
    }
    else {
        cerr << "无法打开文件进行写入: " << weiboGroupPath << endl;
    }

    // 保存用户基本信息到文件
    string path = userPath + "/info.txt";
    ofstream file(path.c_str(), ios::out | ios::trunc);

    if (file.is_open()) {
        // 写入用户的基本信息
        file << "password: " << password << endl;
        file << "name: " << nickname << endl;
        file << "ID: " << userID << endl;
        file << "WeChatID: " << wechatID << endl;
        file << "birthTime: " << birthTime << endl;
        file << "location: " << location << endl;
        file << "firstTime: " << firstTime << endl;
        file << "status: " << status << endl;

        // 写入隐私设置
        file << "privacySettings: " << privacySettings.size() << endl;
        for (const auto& setting : privacySettings) {
            file << setting.first << " " << setting.second << endl;
        }

        // 写入被拉黑的用户列表
        file << "blockedUsers: " << blockedUsers.size() << endl;
        for (const auto& blocked : blockedUsers) {
            file << blocked << endl;
        }

        // 写入 QQ 好友列表
        file << "qqFriends: " << qqFriendsList.size() << endl;
        for (const auto& friendInfo : qqFriendsList) {
            file << friendInfo.first << " " << friendInfo.second << endl;
        }

        // 写入微信好友列表
        file << "wechatFriends: " << wechatFriendsList.size() << endl;
        for (const auto& friendInfo : wechatFriendsList) {
            file << friendInfo.first << " " << friendInfo.second << endl;
        }

        // 写入微博好友列表
        file << "weiboFriends: " << weiboFriendsList.size() << endl;
        for (const auto& friendInfo : weiboFriendsList) {
            file << friendInfo.first << " " << friendInfo.second << endl;
        }
        // 写入服务列表
        file << "services: " << services.size() << endl;
        for (const auto& service : services) {
            file << service << endl;
        }

        // 写入通知列表
        file << "notifications: " << notifications.size() << endl;
        for (const auto& note : notifications) {
            file << note << endl;
        }

        file.close();
    }
    else {
        cerr << "无法打开文件进行写入: " << path << endl;
    }
}

bool User_yxc::findUser(const string& username) {
    string path = SERVICE_DIR + username + "/info.txt";
    return fileExists(path);
}

void User_yxc::initializeFromFile(const string& username) {
    qqFriendsList.clear();
    wechatFriendsList.clear();
    weiboFriendsList.clear();
    groupsList.clear();
    services.clear();
    notifications.clear();
    privacySettings.clear();
    blockedUsers.clear();

    string userPath = SERVICE_DIR + username;

    string qqGroupPath = userPath + "/qq_groups.txt";
    ifstream qqGroupFile(qqGroupPath.c_str());
    if (qqGroupFile.is_open()) {
        string groupID;
        while (getline(qqGroupFile, groupID)) {
            groupsList.push_back(groupID);
        }
        qqGroupFile.close();
    }

    string wechatGroupPath = userPath + "/wechat_groups.txt";
    ifstream wechatGroupFile(wechatGroupPath.c_str());
    if (wechatGroupFile.is_open()) {
        string groupID;
        while (getline(wechatGroupFile, groupID)) {
            groupsList.push_back(groupID);
        }
        wechatGroupFile.close();
    }

    string weiboGroupPath = userPath + "/weibo_groups.txt";
    ifstream weiboGroupFile(weiboGroupPath.c_str());
    if (weiboGroupFile.is_open()) {
        string groupID;
        while (getline(weiboGroupFile, groupID)) {
            groupsList.push_back(groupID);
        }
        weiboGroupFile.close();
    }

    string path = userPath + "/info.txt";
    ifstream file(path.c_str());
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            if (line.find("password: ") == 0) {
                password = line.substr(10);
            }
            else if (line.find("name: ") == 0) {
                nickname = line.substr(6);
            }
            else if (line.find("ID: ") == 0) {
                userID = line.substr(4);
            }
            else if (line.find("WeChatID: ") == 0) {
                wechatID = line.substr(10);
            }
            else if (line.find("birthTime: ") == 0) {
                birthTime = static_cast<time_t>(stoll(line.substr(11)));
                calculateAge();
            }
            else if (line.find("location: ") == 0) {
                location = line.substr(10);
            }
            else if (line.find("firstTime: ") == 0) {
                firstTime = static_cast<time_t>(stoll(line.substr(11)));
            }
            else if (line.find("status: ") == 0) {
                status = line.substr(8);
            }
            else if (line.find("privacySettings: ") == 0) {
                size_t count = stoi(line.substr(17));
                for (size_t i = 0; i < count; ++i) {
                    string key, value;
                    file >> key >> value;
                    privacySettings[key] = value;
                    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
            }
            else if (line.find("blockedUsers: ") == 0) {
                size_t count = stoi(line.substr(14));
                for (size_t i = 0; i < count; ++i) {
                    string blockedID;
                    getline(file, blockedID);
                    blockedUsers.insert(blockedID);
                }
            }
            else if (line.find("qqFriends: ") == 0) {
                size_t count = stoi(line.substr(11));
                for (size_t i = 0; i < count; ++i) {
                    string friendID, remark;
                    file >> friendID >> remark;
                    qqFriendsList.push_back({ friendID, remark });
                    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
            }
            else if (line.find("wechatFriends: ") == 0) {
                size_t count = stoi(line.substr(15));
                for (size_t i = 0; i < count; ++i) {
                    string friendID, remark;
                    file >> friendID >> remark;
                    wechatFriendsList.push_back({ friendID, remark });
                    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
            }
            else if (line.find("weiboFriends: ") == 0) {
                size_t count = stoi(line.substr(14));
                for (size_t i = 0; i < count; ++i) {
                    string friendID, remark;
                    file >> friendID >> remark;
                    weiboFriendsList.push_back({ friendID, remark });
                    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
            }
            else if (line.find("services: ") == 0) {
                size_t count = stoi(line.substr(10));
                for (size_t i = 0; i < count; ++i) {
                    string service;
                    getline(file, service);
                    services.push_back(service);
                }
            }
            else if (line.find("notifications: ") == 0) {
                size_t count = stoi(line.substr(14));
                for (size_t i = 0; i < count; ++i) {
                    string note;
                    getline(file, note);
                    notifications.push_back(note);
                }
            }
        }
        file.close();
    }
    else {
        cerr << "无法打开用户文件进行初始化: " << path << endl;
    }
}

void User_yxc::addService(const string& service) {
    if (find(services.begin(), services.end(), service) == services.end()) {
        services.push_back(service);
        cout << "已激活服务: " << service << endl;
        saveToFile();
    }
    else {
        cout << "服务 " << service << " 已经激活。" << endl;
    }
}

bool User_yxc::hasService(const string& service) const {
    return find(services.begin(), services.end(), service) != services.end();
}

// 修改好友备注的函数
// 参数:
//   friendID: 目标好友的用户ID
//   newRemark: 新的备注信息
//   service: 服务类型
void User_yxc::modifyFriendRemark(const string& friendID, const string& newRemark, const string& service) {
    // 获取当前用户在指定服务下的好友列表
    vector<pair<string, string>>* friendsList = getFriendsList(service);

    // 如果好友列表存在
    if (friendsList) {
        // 查找目标好友是否在好友列表中
        auto it = find_if(friendsList->begin(), friendsList->end(), [&](const pair<string, string>& f) {
            return f.first == friendID;
            });

        // 如果找到目标好友
        if (it != friendsList->end()) {
            // 修改好友的备注信息
            it->second = newRemark;

            // 提示用户备注已修改
            cout << "好友 " << friendID << " 的备注已修改为 " << newRemark << endl;

            // 保存当前用户的用户信息到文件
            saveToFile();
        }
        else {
            // 如果未找到目标好友，提示用户
            cout << "好友 " << friendID << " 未找到。" << endl;
        }
    }
}
void User_yxc::queryFriend(const string& friendID, const string& service) const {
    const vector<pair<string, string>>* friendsList = getFriendsList(service);
    if (friendsList) {
        auto it = find_if(friendsList->begin(), friendsList->end(), [&](const pair<string, string>& f) {
            return f.first == friendID;
            });
        if (it != friendsList->end()) {
            cout << "好友 " << friendID << " 在您的好友列表中，备注为: " << it->second << endl;
        }
        else {
            cout << "好友 " << friendID << " 不在您的好友列表中。" << endl;
        }
    }
}
void User_yxc::displayMutualFriends(const User_yxc& otherUser, const string& service) const {
    // 获取当前用户在指定服务下的好友列表
    const vector<pair<string, string>>* friendsList1 = getFriendsList(service);
    // 获取另一个用户在指定服务下的好友列表
    const vector<pair<string, string>>* friendsList2 = otherUser.getFriendsList(service);
    // 如果两个好友列表都存在
    if (friendsList1 && friendsList2) {
        // 用于存储共同好友的容器
        vector<string> mutualFriends;
        // 遍历当前用户的好友列表
        for (const auto& friendInfo : *friendsList1) {
            // 检查当前好友是否在另一个用户的好友列表中
            if (find_if(friendsList2->begin(), friendsList2->end(), [&](const pair<string, string>& f) {
                return f.first == friendInfo.first; // 条件：好友ID匹配
                }) != friendsList2->end()) {
                // 如果找到共同好友，将其添加到 mutualFriends 中
                mutualFriends.push_back(friendInfo.first);
            }
        }
        // 如果没有共同好友
        if (mutualFriends.empty()) {
            cout << "没有共同好友。" << endl;
        }
        else {
            // 输出共同好友列表
            cout << "共同好友: ";
            for (const auto& friendID : mutualFriends) {
                cout << friendID << " ";
            }
            cout << endl;
        }
    }
    else {
        // 如果无法获取好友列表，提示用户
        cout << "无法获取好友列表。" << endl;
    }
}

const string& User_yxc::getUserID() const {
    return userID;
}

const string& User_yxc::getWeChatID() const {
    return wechatID;
}

bool User_yxc::isBlocked(const string& otherUserID) const {
    return blockedUsers.find(otherUserID) != blockedUsers.end();
}

// 发送好友请求的函数

void User_yxc::sendFriendRequest(const string& friendID, const string& remark, const string& service) {
    // 检查是否试图添加自己为好友
    if (friendID == userID) {
        cout << "不能添加自己为好友。" << endl;
        return;
    }
    // 获取当前用户在指定服务下的好友列表
    vector<pair<string, string>>* friendsList = getFriendsList(service);
    // 如果好友列表存在
    if (friendsList) {
        // 检查目标好友是否已经在好友列表中
        auto it = find_if(friendsList->begin(), friendsList->end(), [&](const pair<string, string>& f) {
            return f.first == friendID;
            });

        // 如果目标好友已在好友列表中，提示并返回
        if (it != friendsList->end()) {
            cout << "好友 " << friendID << " 已在您的好友列表中。" << endl;
            return;
        }
    }
    // 检查目标用户是否存在
    if (findUser(friendID)) {
        // 创建目标用户的智能指针
        shared_ptr<User_yxc> otherUser = make_shared<User_yxc>();
        // 从文件初始化目标用户对象
        otherUser->initializeFromFile(friendID);
        // 检查当前用户是否被目标用户拉黑
        if (otherUser->isBlocked(userID)) {
            cout << "对方已将您拉黑，无法发送好友请求。" << endl;
            return;
        }
        // 构建好友请求文件的路径
        string friendRequestsPath = SERVICE_DIR + friendID + "/friend_requests.txt";
        // 打开目标用户的好友请求文件，追加写入
        ofstream outFile(friendRequestsPath, ios::app);
        // 检查文件是否成功打开
        if (outFile.is_open()) {
            // 写入当前用户ID、备注信息和服务类型到文件
            outFile << userID << " " << remark << " " << service << endl;
            outFile.close();
            // 提示好友请求发送成功
            cout << "好友请求已发送给 " << friendID << "。" << endl;
        }
        else {
            // 如果文件无法打开，输出错误信息
            cerr << "无法打开文件: " << friendRequestsPath << endl;
        }
    }
    else {
        // 如果目标用户不存在，提示并返回
        cout << "用户 " << friendID << " 不存在，无法发送好友请求。" << endl;
    }
}
// 处理好友请求的函数
void User_yxc::processFriendRequests() {
    // 构建好友请求文件的路径
    string friendRequestsPath = SERVICE_DIR + userID + "/friend_requests.txt";
    // 打开好友请求文件进行读取
    ifstream inFile(friendRequestsPath);
    // 如果文件无法打开，说明没有好友请求
    if (!inFile.is_open()) {
        cout << "暂无好友请求。" << endl;
        return;
    }
    // 用于存储待处理的好友请求
    vector<tuple<string, string, string>> pendingRequests;
    // 逐行读取文件内容
    string line;
    while (getline(inFile, line)) {
        string senderID, remark, service;
        istringstream iss(line);
        iss >> senderID >> remark >> service;//依次提取三个字符串，分别赋值给 senderID、remark 和 service。
        // 将每条好友请求存储到 pendingRequests 中
        pendingRequests.push_back({ senderID, remark, service });
    }
    // 关闭文件
    inFile.close();
    // 如果没有待处理的好友请求，提示并返回
    if (pendingRequests.empty()) {
        cout << "暂无好友请求。" << endl;
        return;
    }
    // 清空好友请求文件，以便重新写入未处理的好友请求
    ofstream outFile(friendRequestsPath, ios::trunc);
    outFile.close();
    // 遍历所有待处理的好友请求
    for (const auto& request : pendingRequests) {
        // 获取请求的发送者ID、备注和服务类型
        string senderID = get<0>(request);
        string remark = get<1>(request);
        string service = get<2>(request);
        // 提示用户处理好友请求
        cout << "收到来自 " << senderID << " 的好友请求。备注: " << remark << " 是否接受？(y/n): ";
        // 获取用户的选择
        char choice;
        cin >> choice;
        // 忽略输入缓冲区中的剩余字符
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        // 如果用户选择接受好友请求
        if (choice == 'y' || choice == 'Y') {
            // 调用 addFriend 函数将对方添加为好友
            addFriend(senderID, remark, service, true);
            cout << "已添加对方" << endl;
            // 通知对方好友请求已被接受
            notifyUser(senderID, "您的好友请求已被 " + userID + " 接受。");
            // 创建对方的用户对象
            shared_ptr<User_yxc> otherUser = make_shared<User_yxc>();
            otherUser->initializeFromFile(senderID);
            // 提示用户对方也已添加自己为好友
            cout << "对方";
            // 调用对方的 addFriend 函数将自己添加为好友
            otherUser->addFriend(userID, "新好友", service, true);
            // 保存对方的用户信息到文件
            otherUser->saveToFile();
        }
        else {
            // 如果用户选择拒绝好友请求，通知对方请求被拒绝
            notifyUser(senderID, "您的好友请求被 " + userID + " 拒绝。");
            cout << "已拒绝好友请求。" << endl;
        }
    }
}

// 添加好友的函数
//   isProcessingRequest: 是否正在处理好友请求（用于区分主动添加和被动接受）
void User_yxc::addFriend(const string& friendID, const string& remark, const string& service, bool isProcessingRequest) {
    // 检查是否试图添加自己为好友
    if (friendID == userID) {
        cout << "不能添加自己为好友。" << endl;
        return;
    }

    // 获取当前用户在指定服务下的好友列表
    vector<pair<string, string>>* friendsList = getFriendsList(service);

    // 如果好友列表存在
    if (friendsList) {
        // 检查目标好友是否已在好友列表中
        auto it = find_if(friendsList->begin(), friendsList->end(), [&](const pair<string, string>& f) {
            return f.first == friendID;
            });

        // 如果目标好友不在好友列表中
        if (it == friendsList->end()) {
            // 检查目标用户是否存在
            if (findUser(friendID)) {
                // 检查目标用户是否在当前用户的黑名单中
                if (isBlocked(friendID)) {
                    // 提示用户是否取消拉黑并添加为好友
                    cout << "用户 " << friendID << " 在您的黑名单中，是否取消拉黑并添加为好友？(y/n): ";
                    char choice;
                    cin >> choice;
                    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                    // 如果用户选择取消拉黑
                    if (choice == 'y' || choice == 'Y') {
                        unblockUser(friendID); // 取消拉黑
                    }
                    else {
                        // 如果用户选择不取消拉黑，提示未添加好友
                        cout << "未添加好友。" << endl;
                        return;
                    }
                }

                // 将目标用户添加到好友列表中
                friendsList->push_back({ friendID, remark });
                cout << "好友已添加: " << friendID << " 备注: " << remark << endl;

                // 如果不是在处理好友请求（即主动添加好友）
                if (!isProcessingRequest) {
                    // 通知对方用户已被添加为好友
                    notifyUser(friendID, "用户 " + userID + " 添加了您为好友。");

                    // 创建对方的用户对象
                    shared_ptr<User_yxc> otherUser = make_shared<User_yxc>();
                    otherUser->initializeFromFile(friendID);

                    // 调用对方的 addFriend 函数将自己添加为好友
                    otherUser->addFriend(userID, "默认", service, true);

                    // 保存对方的用户信息到文件
                    otherUser->saveToFile();
                }

                // 保存当前用户的用户信息到文件
                saveToFile();
            }
            else {
                // 如果目标用户不存在，提示无法添加好友
                cout << "用户 " << friendID << " 不存在，无法添加好友。" << endl;
            }
        }
        else {
            // 如果目标好友已在好友列表中，提示已存在
            cout << "好友 " << friendID << " 已在您的好友列表中。" << endl;
        }
    }
}
// 删除好友的函数
void User_yxc::removeFriend(const string& friendID, const string& service) {
    // 获取当前用户在指定服务下的好友列表
    vector<pair<string, string>>* friendsList = getFriendsList(service);

    // 如果好友列表存在
    if (friendsList) {
        // 使用 std::remove_if 查找并移除目标好友
        auto it = std::remove_if(friendsList->begin(), friendsList->end(), [&](const pair<string, string>& f) {
            return f.first == friendID; // 条件：好友ID匹配
            });

        // 如果找到目标好友
        if (it != friendsList->end()) {
            // 从好友列表中删除目标好友
            friendsList->erase(it, friendsList->end());

            // 提示用户已删除对方
            cout << "删除了对方" << endl;

            // 保存当前用户的用户信息到文件
            saveToFile();

            // 检查目标用户是否存在
            if (User_yxc::findUser(friendID)) {
                // 创建目标用户的智能指针
                shared_ptr<User_yxc> otherUser = make_shared<User_yxc>();

                // 从文件初始化目标用户对象
                otherUser->initializeFromFile(friendID);

                // 调用目标用户的 removeFriend 函数，删除当前用户
                otherUser->removeFriend(userID, service);

                // 保存目标用户的用户信息到文件
                otherUser->saveToFile();
            }
        }
        else {
            // 如果未找到目标好友，提示用户
            cout << "未找到好友: " << friendID << endl;
        }
    }
}
void User_yxc::joinGroup(const string& groupID, const string& service) {
    if (find(groupsList.begin(), groupsList.end(), groupID) == groupsList.end()) {
        if (groupMap_yxc.find(groupID) == groupMap_yxc.end()) {
            cout << "群组 " << groupID << " 不存在。" << endl;
            return;
        }
        if (groupMap_yxc[groupID].getPlatform() != service) {
            cout << "群组 " << groupID << " 不属于 " << service << " 平台。" << endl;
            return;
        }
        groupsList.push_back(groupID);
        cout << "已加入 " << service << " 群组: " << groupID << endl;
        groupMap_yxc[groupID].addMember(userID);

        string ownerID = groupMap_yxc[groupID].getOwnerID();
        if (ownerID != userID) {
            notifyUser(ownerID, "用户 " + userID + " 加入了您的群组 " + groupID + "。");
        }

        saveToFile();
        groupMap_yxc[groupID].saveToFile(service);
    }
    else {
        cout << "您已经在群组: " << groupID << " 中。" << endl;
    }
}

void User_yxc::leaveGroup(const string& groupID, const string& service) {
    auto it = std::remove(groupsList.begin(), groupsList.end(), groupID);
    if (it != groupsList.end()) {
        groupsList.erase(it, groupsList.end());
        cout << "已退出 " << service << " 群组: " << groupID << endl;
        groupMap_yxc[groupID].removeMember(userID);

        string ownerID = groupMap_yxc[groupID].getOwnerID();
        if (ownerID != userID) {
            notifyUser(ownerID, "用户 " + userID + " 退出了您的群组 " + groupID + "。");
        }

        saveToFile();
        groupMap_yxc[groupID].saveToFile(service);
    }
    else {
        cout << "未找到群组: " << groupID << endl;
    }
}

void User_yxc::addGroup(const string& groupID) {
    groupsList.push_back(groupID);
    saveToFile();
}

void User_yxc::queryGroupMembers(const string& groupID) const {
    auto it = groupMap_yxc.find(groupID);
    if (it != groupMap_yxc.end()) {
        it->second.displayMembers();
    }
    else {
        cout << "群组 " << groupID << " 未找到。" << endl;
    }
}

void User_yxc::beKickedFromGroup(const string& groupID, const string& service) {
    leaveGroup(groupID, service);
    cout << "您已被群组 " << groupID << " 踢出。" << endl;
    addNotification("您已被群组 " + groupID + " 踢出。");
}

void User_yxc::displayFriends(const string& service) const {
    const vector<pair<string, string>>* friendsList = getFriendsList(service);
    if (friendsList) {
        cout << service << " 好友列表: ";
        for (const auto& friendInfo : *friendsList) {
            cout << friendInfo.first << " (" << friendInfo.second << ") ";
        }
        cout << endl;
    }
}

void User_yxc::displayGroups(const string& service) const {
    cout << "群组列表 (" << service << "): ";
    for (const auto& groupID : groupsList) {
        if (groupMap_yxc.find(groupID) != groupMap_yxc.end() && groupMap_yxc.at(groupID).getPlatform() == service) {
            cout << groupID << " ";
        }
    }
    cout << endl;
}

void User_yxc::addNotification(const string& message) {
    notifications.push_back(message);
    saveToFile();
}

void User_yxc::viewNotifications() {
    if (notifications.empty()) {
        cout << "暂无新通知。" << endl;
    }
    else {
        cout << "\n----- 消息通知 -----" << endl;
        for (const auto& note : notifications) {
            cout << note << endl;
        }
        notifications.clear();
        saveToFile();
    }
}

void User_yxc::notifyUser(const string& otherUserID, const string& message) {
    string otherUserPath = SERVICE_DIR + otherUserID + "/info.txt";
    if (fileExists(otherUserPath)) {
        shared_ptr<User_yxc> otherUser = make_shared<User_yxc>();
        otherUser->initializeFromFile(otherUserID);
        otherUser->addNotification(message);
        otherUser->saveToFile();
    }
}

void User_yxc::setPrivacy(const string& key, const string& value) {
    vector<string> allowedKeys = { "moments", "blogs" };
    vector<string> allowedValues = { "public", "friends", "private" };

    if (find(allowedKeys.begin(), allowedKeys.end(), key) == allowedKeys.end()) {
        cout << "无效的隐私选项键，请选择以下选项之一: moments, blogs" << endl;
        return;
    }

    if (find(allowedValues.begin(), allowedValues.end(), value) == allowedValues.end()) {
        cout << "无效的隐私选项值，请选择以下选项之一: public, friends, private" << endl;
        return;
    }

    privacySettings[key] = value;
    cout << "隐私设置已更新。" << endl;
    saveToFile();
}

string User_yxc::getPrivacy(const string& key) const {
    auto it = privacySettings.find(key);
    if (it != privacySettings.end()) {
        return it->second;
    }
    return "public";
}

void User_yxc::updateStatus(const string& newStatus) {
    vector<string> allowedStatuses = { "在线", "离线", "忙碌", "隐身" };

    if (find(allowedStatuses.begin(), allowedStatuses.end(), newStatus) != allowedStatuses.end()) {
        status = newStatus;
        cout << "状态已更新为: " << status << endl;
        saveToFile();
    }
    else {
        cout << "无效的状态选项，请选择以下选项之一: 在线, 离线, 忙碌, 隐身" << endl;
    }
}

void User_yxc::blockUser(const string& otherUserID) {
    auto it = find_if(qqFriendsList.begin(), qqFriendsList.end(), [&](const pair<string, string>& f) {
        return f.first == otherUserID;
        });
    // 如果找到目标用户
    if (it != qqFriendsList.end()) {
        blockedUsers.insert(otherUserID);
        cout << "已将用户 " << otherUserID << " 加入黑名单。" << endl;
        saveToFile();
    }
    else {
        // 如果未找到目标用户，提示用户无法拉黑

        cout << "用户 " << otherUserID << " 不是您的好友，无法拉黑。" << endl;
    }
}

void User_yxc::unblockUser(const string& otherUserID) {
    // 从黑名单中移除目标用户
    blockedUsers.erase(otherUserID);
    cout << "已将用户 " << otherUserID << " 移出黑名单。" << endl;
    saveToFile();
}

void User_yxc::sendMessage(const string& receiverID, const string& content, const string& service) {
    if (blockedUsers.find(receiverID) != blockedUsers.end()) {
        cout << "无法发送消息，对方在您的黑名单中。" << endl;
        return;
    }

    auto it = find_if(qqFriendsList.begin(), qqFriendsList.end(), [&](const pair<string, string>& f) {
        return f.first == receiverID;
        });
    if (it == qqFriendsList.end()) {
        cout << "对方不是您的好友，无法发送消息。" << endl;
        return;
    }

    shared_ptr<User_yxc> receiverUser = make_shared<User_yxc>();
    receiverUser->initializeFromFile(receiverID);
    if (receiverUser->isBlocked(userID)) {
        cout << "你已被对方拉黑，无法发送消息。" << endl;
        return;
    }

    {
        lock_guard<mutex> lock(messageQueueMutexes[service]);
        messageQueues[service].push(Message_yxc(userID, receiverID, content));
    }
    messageQueueCVs[service].notify_one();
    cout << "消息已发送给 " << receiverID << "。" << endl;

    saveMessageToFile(receiverID, content, service);
}

void User_yxc::viewMessages(const string& service) {
    queue<Message_yxc> tempQueue;
    bool hasMessage = false;

    {
        lock_guard<mutex> lock(messageQueueMutexes[service]);
        while (!messageQueues[service].empty()) {
            Message_yxc msg = messageQueues[service].front();
            messageQueues[service].pop();

            if (blockedUsers.find(msg.senderID) == blockedUsers.end()) {
                if (msg.receiverID == userID) {
                    hasMessage = true;
                    cout << "[" << msg.timestamp << "] 来自 " << msg.senderID << " 的消息: " << msg.content << endl;
                }
                else {
                    tempQueue.push(msg);
                }
            }
        }

        messageQueues[service] = tempQueue;
    }

    if (!hasMessage) {
        cout << "暂无新消息。" << endl;
    }
}

void User_yxc::saveMessageToFile(const string& receiverID, const string& content, const string& service) {
    string userPath = SERVICE_DIR + userID;
    string receiverPath = SERVICE_DIR + receiverID;

    if (!createDirectory(userPath)) {
        cerr << "无法创建用户目录: " << userPath << endl;
        return;
    }
    if (!createDirectory(receiverPath)) {
        cerr << "无法创建用户目录: " << receiverPath << endl;
        return;
    }
    string userMessagePath = userPath + "/messages_" + receiverID + "_" + service + ".txt";
    string receiverMessagePath = receiverPath + "/messages_" + userID + "_" + service + ".txt";
    shared_ptr<User_yxc> receiverUser = make_shared<User_yxc>();
    receiverUser->initializeFromFile(receiverID);
    if (receiverUser->isBlocked(userID)) {
        cout << "你已被对方拉黑，消息不会被保存。" << endl;
        return;
    }
    ofstream userFile(userMessagePath.c_str(), ios::app);
    ofstream receiverFile(receiverMessagePath.c_str(), ios::app);
    if (userFile.is_open() && receiverFile.is_open()) {
        string timestamp = getCurrentTimestamp();
        userFile << "[" << timestamp << "] " << userID << ": " << content << endl;
        receiverFile << "[" << timestamp << "] " << userID << ": " << content << endl;
        userFile.close();
        receiverFile.close();
    }
    else {
        cerr << "无法打开消息文件进行写入。" << endl;
    }
}

void User_yxc::viewMessageHistory(const string& friendID, const string& service) const {
    string userPath = SERVICE_DIR + userID;
    string friendPath = SERVICE_DIR + friendID;

    string userMessagePath = userPath + "/messages_" + friendID + "_" + service + ".txt";
    string friendMessagePath = friendPath + "/messages_" + userID + "_" + service + ".txt";

    ifstream userFile(userMessagePath.c_str());
    ifstream friendFile(friendMessagePath.c_str());

    if (userFile.is_open() && friendFile.is_open()) {
        cout << "----- 与 " << friendID << " 的历史消息记录 -----" << endl;
        string line;
        set<string> seenMessages;

        while (getline(userFile, line)) {
            if (seenMessages.find(line) == seenMessages.end()) {
                cout << line << endl;
                seenMessages.insert(line);
            }
        }
        userFile.close();

        while (getline(friendFile, line)) {
            if (seenMessages.find(line) == seenMessages.end()) {
                cout << line << endl;
                seenMessages.insert(line);
            }
        }
        friendFile.close();
    }
    else {
        cerr << "无法打开消息文件进行读取。" << endl;
    }
}

void User_yxc::sendGroupMessage(const string& groupID, const string& content) {
    if (find(groupsList.begin(), groupsList.end(), groupID) == groupsList.end()) {
        cout << "您不在群组 " << groupID << " 中，无法发送消息。" << endl;
        return;
    }

    if (groupMap_yxc[groupID].getPlatform() != "QQ") {
        cout << "群组 " << groupID << " 不属于 QQ 平台。" << endl;
        return;
    }

    groupMap_yxc[groupID].sendGroupMessage(userID, content, "QQ");
}

void User_yxc::viewGroupMessages(const string& groupID) const {
    if (find(groupsList.begin(), groupsList.end(), groupID) == groupsList.end()) {
        cout << "您不在群组 " << groupID << " 中，无法查看消息。" << endl;
        return;
    }

    if (groupMap_yxc.at(groupID).getPlatform() != "QQ") {
        cout << "群组 " << groupID << " 不属于 QQ 平台。" << endl;
        return;
    }
    groupMap_yxc.at(groupID).viewGroupMessages("QQ");
}

string User_yxc::getGroupRole(const string& groupID) const {
    if (groupMap_yxc.find(groupID) == groupMap_yxc.end()) {
        return "NotMember";
    }
    const Group_yxc& group = groupMap_yxc.at(groupID);
    if (group.getOwnerID() == userID) {
        return "Owner";
    }
    if (group.getAdmins().find(userID) != group.getAdmins().end()) {
        return "Admin";
    }
    if (group.hasMember(userID)) {
        return "Member";
    }
    return "NotMember";
}

void User_yxc::addAdmin(const string& groupID, const string& userID) {
    if (getGroupRole(groupID) == "Owner") {
        groupMap_yxc[groupID].addAdmin(userID);
        groupMap_yxc[groupID].saveToFile("QQ");
    }
    else {
        cout << "您不是群主，无法添加管理员。" << endl;
    }
}

void User_yxc::removeAdmin(const string& groupID, const string& userID) {
    if (getGroupRole(groupID) == "Owner") {
        groupMap_yxc[groupID].removeAdmin(userID);
        groupMap_yxc[groupID].saveToFile("QQ");
    }
    else {
        cout << "您不是群主，无法移除管理员。" << endl;
    }
}

void User_yxc::kickMember(const string& groupID, const string& userID) {
    string role = getGroupRole(groupID);
    if (role == "Owner" || role == "Admin") {
        groupMap_yxc[groupID].kickMember(userID);
        groupMap_yxc[groupID].saveToFile("QQ");
        notifyUser(userID, "您已被踢出群组 " + groupID + "。");
    }
    else {
        cout << "您不是群主或管理员，无法踢出成员。" << endl;
    }
}

void User_yxc::inviteMember(const string& groupID, const string& userID) {
    if (getGroupRole(groupID) == "Owner" || getGroupRole(groupID) == "Admin" || getGroupRole(groupID) == "Member") {
        if (groupMap_yxc[groupID].isGroupOpen()) {
            groupMap_yxc[groupID].inviteMember(userID);
            groupMap_yxc[groupID].saveToFile("QQ");
            notifyUser(userID, "您已被邀请加入群组 " + groupID + "。");
        }
        else {
            cout << "群组已关闭，无法邀请新成员。" << endl;
        }
    }
    else {
        cout << "您不是群组成员，无法邀请新成员。" << endl;
    }
}

vector<pair<string, string>>* User_yxc::getFriendsList(const string& service) {
    if (service == "QQ") {
        return &qqFriendsList;
    }
    else if (service == "WeChat") {
        return &wechatFriendsList;
    }
    else if (service == "Weibo") {
        return &weiboFriendsList;
    }
    else {
        cerr << "未知的服务类型: " << service << endl;
        return nullptr;
    }
}

const vector<pair<string, string>>* User_yxc::getFriendsList(const string& service) const {
    if (service == "QQ") {
        return &qqFriendsList;
    }
    else if (service == "WeChat") {
        return &wechatFriendsList;
    }
    else if (service == "Weibo") {
        return &weiboFriendsList;
    }
    else {
        cerr << "未知的服务类型: " << service << endl;
        return nullptr;
    }
}

bool User_yxc::verifyPassword(const string& pwd) const {
    return password == pwd;
}

void User_yxc::changePassword(const string& oldPassword, const string& newPassword) {
    if (verifyPassword(oldPassword)) {
        password = newPassword;
        cout << "密码修改成功。" << endl;
        saveToFile();
    }
    else {
        cout << "旧密码不正确，无法修改密码。" << endl;
    }
}

void User_yxc::processGroupInvites() {
    string groupInvitesPath = SERVICE_DIR + userID + "/group_invites.txt";
    ifstream inFile(groupInvitesPath);
    if (!inFile.is_open()) {
        cout << "暂无群组邀请。" << endl;
        return;
    }

    vector<string> pendingInvites;
    string line;
    while (getline(inFile, line)) {
        pendingInvites.push_back(line);
    }
    inFile.close();

    ofstream outFile(groupInvitesPath, ios::trunc);
    outFile.close();

    if (pendingInvites.empty()) {
        cout << "暂无群组邀请。" << endl;
        return;
    }

    for (const auto& invite : pendingInvites) {
        string groupID = invite.substr(0, invite.find(' '));
        cout << "收到来自群组 " << groupID << " 的邀请。是否接受？(y/n): ";
        char choice;
        cin >> choice;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (choice == 'y' || choice == 'Y') {
            joinGroup(groupID, "QQ");
            cout << "已加入群组 " << groupID << "。" << endl;
        }
        else {
            cout << "已拒绝群组 " << groupID << " 的邀请。" << endl;
        }
    }
}

void User_yxc::displayFriendInfo(const string& friendID) const {
    if (findUser(friendID)) {
        shared_ptr<User_yxc> friendUser = make_shared<User_yxc>();
        friendUser->initializeFromFile(friendID);
        friendUser->displayInfo();
    }
    else {
        cout << "用户 " << friendID << " 不存在。" << endl;
    }
}

void User_yxc::modifyUserInfo() {
    cout << "\n----- 修改个人信息 -----" << endl;
    cout << "当前昵称: " << nickname << endl;
    cout << "请输入新的昵称: ";
    getline(cin, nickname);
    cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n'); // 忽略无效输入
    cout << "当前生日: ";
    char birthDate[11];
    tm birth_tm;
    localtime_s(&birth_tm, &birthTime);
    snprintf(birthDate, sizeof(birthDate), "%d-%02d-%02d",
        birth_tm.tm_year + 1900, birth_tm.tm_mon + 1, birth_tm.tm_mday);
    cout << birthDate << endl;
    cout << "请输入新的生日 (YYYY-MM-DD): ";
    string birthDateStr;
    getline(cin, birthDateStr);
    time_t newBirthTime = parseDate(birthDateStr);
    if (newBirthTime != -1) {
        birthTime = newBirthTime;
    }
    else {
        cout << "日期格式无效，生日未修改。" << endl;
    }

    cout << "当前位置: " << location << endl;
    cout << "请输入新的位置: ";
    getline(cin, location);

    saveToFile();
    cout << "个人信息已更新。" << endl;
}