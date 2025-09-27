#include "Group_yxc.h"
#include "Utils_yxc.h"
#include <direct.h>
#include <cerrno>
#include <cstring>


Group_yxc::Group_yxc() : isQQGroup(true), isOpen(true) {}

Group_yxc::Group_yxc(const string& id, const string& ownerID, const string& platform, bool isQQ, bool open)
    : groupID(id), groupOwnerID(ownerID), platform(platform), isQQGroup(isQQ), isOpen(open) {
    members.insert(ownerID);
    admins.insert(ownerID);
}

void Group_yxc::addMember(const string& userID) {
    members.insert(userID);
}

void Group_yxc::removeMember(const string& userID) {
    members.erase(userID);
    admins.erase(userID);
}

bool Group_yxc::hasMember(const string& userID) const {
    return members.find(userID) != members.end();
}

void Group_yxc::displayMembers() const {
    cout << "群组 " << groupID << " 的成员: " << endl;
    for (const auto& member : members) {
        string role = (member == groupOwnerID) ? "群主" : (admins.find(member) != admins.end() ? "管理员" : "成员");
        cout << member << " (" << role << ") ";
    }
    cout << endl;
}

const string& Group_yxc::getGroupID() const {
    return groupID;
}

const string& Group_yxc::getOwnerID() const {
    return groupOwnerID;
}

bool Group_yxc::isQQ() const {
    return isQQGroup;
}

bool Group_yxc::isGroupOpen() const {
    return isOpen;
}

const string& Group_yxc::getPlatform() const {
    return platform;
}

const set<string>& Group_yxc::getMembers() const {
    return members;
}

const set<string>& Group_yxc::getAdmins() const {
    return admins;
}

void Group_yxc::saveToFile(const string& service) const {
    string groupDir;
    if (service == "QQ") {
        groupDir = SERVICE_DIR + "QQ_group/";
    }
    else if (service == "WeChat") {
        groupDir = SERVICE_DIR + "weixin_group/";
    }
    else if (service == "Weibo") {
        groupDir = SERVICE_DIR + "weibo_group/";
    }
    else {
        cerr << "未知的服务类型: " << service << endl;
        return;
    }

    if (!createDirectory(groupDir)) {
        cerr << "无法创建群组目录: " << groupDir << endl;
        return;
    }

    string path = groupDir + groupID + ".txt";
    ofstream file(path.c_str(), ios::out | ios::trunc);

    if (file.is_open()) {
        file << "GroupID: " << groupID << endl;
        file << "OwnerID: " << groupOwnerID << endl;
        file << "Platform: " << platform << endl;
        file << "IsQQGroup: " << isQQGroup << endl;
        file << "IsOpen: " << isOpen << endl;
        file << "Members: " << members.size() << endl;
        for (const auto& member : members) {
            file << member << endl;
        }
        file << "Admins: " << admins.size() << endl;
        for (const auto& admin : admins) {
            file << admin << endl;
        }
        file.close();
    }
    else {
        cerr << "无法打开文件进行写入: " << path << endl;
    }
}

void Group_yxc::initializeFromFile(const string& groupID, const string& service) {
    string groupDir;
    if (service == "QQ") {
        groupDir = SERVICE_DIR + "QQ_group/";
    }
    else if (service == "WeChat") {
        groupDir = SERVICE_DIR + "weixin_group/";
    }
    else if (service == "Weibo") {
        groupDir = SERVICE_DIR + "weibo_group/";
    }
    else {
        cerr << "未知的服务类型: " << service << endl;
        return;
    }

    string path = groupDir + groupID + ".txt";
    ifstream file(path.c_str());
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            if (line.find("GroupID: ") == 0) {
                this->groupID = line.substr(9);
            }
            else if (line.find("OwnerID: ") == 0) {
                groupOwnerID = line.substr(9);
            }
            else if (line.find("Platform: ") == 0) {
                platform = line.substr(10);
            }
            else if (line.find("IsQQGroup: ") == 0) {
                isQQGroup = (line.substr(11) == "1");
            }
            else if (line.find("IsOpen: ") == 0) {
                isOpen = (line.substr(8) == "1");
            }
            else if (line.find("Members: ") == 0) {
                size_t count = stoi(line.substr(9));
                for (size_t i = 0; i < count; ++i) {
                    string memberID;
                    getline(file, memberID);
                    members.insert(memberID);
                }
            }
            else if (line.find("Admins: ") == 0) {
                size_t count = stoi(line.substr(8));
                for (size_t i = 0; i < count; ++i) {
                    string adminID;
                    getline(file, adminID);
                    admins.insert(adminID);
                }
            }
        }
        file.close();
    }
    else {
        cerr << "无法打开群组文件进行初始化: " << path << endl;
    }
}

void Group_yxc::sendGroupMessage(const string& senderID, const string& content, const string& service) {
    string groupDir;
    if (service == "QQ") {
        groupDir = SERVICE_DIR + "QQ_group/";
    }
    else if (service == "WeChat") {
        groupDir = SERVICE_DIR + "weixin_group/";
    }
    else if (service == "Weibo") {
        groupDir = SERVICE_DIR + "weibo_group/";
    }
    else {
        cerr << "未知的服务类型: " << service << endl;
        return;
    }
    if (!createDirectory(groupDir)) {
        cerr << "无法创建群组目录: " << groupDir << endl;
        return;
    }
    string path = groupDir + groupID + "_messages.txt";
    ofstream file(path.c_str(), ios::app);
    if (file.is_open()) {
        file << "[" << getCurrentTimestamp() << "] " << senderID << ": " << content << endl;
        file.close();
        cout << "群消息已发送。" << endl;
    }
    else {
        cerr << "无法打开文件进行写入: " << path << endl;
    }
}

void Group_yxc::viewGroupMessages(const string& service) const {
    string groupDir;
    if (service == "QQ") {
        groupDir = SERVICE_DIR + "QQ_group/";
    }
    else if (service == "WeChat") {
        groupDir = SERVICE_DIR + "weixin_group/";
    }
    else if (service == "Weibo") {
        groupDir = SERVICE_DIR + "weibo_group/";
    }
    else {
        cerr << "未知的服务类型: " << service << endl;
        return;
    }

    string path = groupDir + groupID + "_messages.txt";
    ifstream file(path.c_str());

    if (file.is_open()) {
        cout << "群组 " << groupID << " 的消息记录:" << endl;
        string line;
        while (getline(file, line)) {
            cout << line << endl;
        }
        file.close();
    }
    else {
        cerr << "无法打开文件进行读取: " << path << endl;
    }
}

void Group_yxc::addAdmin(const string& userID) {
    if (members.find(userID) != members.end()) {
        admins.insert(userID);
        cout << "用户 " << userID << " 已成为管理员。" << endl;
    }
    else {
        cout << "用户 " << userID << " 不是群成员，无法成为管理员。" << endl;
    }
}

void Group_yxc::removeAdmin(const string& userID) {
    if (admins.find(userID) != admins.end()) {
        admins.erase(userID);
        cout << "用户 " << userID << " 已不再是管理员。" << endl;
    }
    else {
        cout << "用户 " << userID << " 不是管理员。" << endl;
    }
}

void Group_yxc::kickMember(const string& userID) {
    if (members.find(userID) != members.end()) {
        members.erase(userID);
        admins.erase(userID);
        cout << "用户 " << userID << " 已被踢出群组。" << endl;
    }
    else {
        cout << "用户 " << userID << " 不是群成员。" << endl;
    }
}

void Group_yxc::inviteMember(const string& userID) {
    if (isOpen) {
        string inviteRequestPath = SERVICE_DIR + userID + "/group_invites.txt";
        ofstream outFile(inviteRequestPath, ios::app);
        if (outFile.is_open()) {
            outFile << groupID << " " << getCurrentTimestamp() << endl;
            outFile.close();
            cout << "用户 " << userID << " 已被邀请加入群组 " << groupID << "。" << endl;
        }
        else {
            cerr << "无法打开文件: " << inviteRequestPath << endl;
        }
    }
    else {
        cout << "群组已关闭，无法邀请新成员。" << endl;
    }
}

void Group_yxc::setGroupOpen(bool open) {
    isOpen = open;
    cout << "群组 " << groupID << " 的开放状态已设置为: " << (isOpen ? "开放" : "关闭") << endl;
}