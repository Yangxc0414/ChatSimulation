#ifndef USER_YXC_H
#define USER_YXC_H

#include <string>
#include <vector>
#include <map>
#include <set>
#include <ctime>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <memory>
#include "Group_yxc.h"
#include "Utils_yxc.h"
using namespace std;

class User_yxc {
protected:
    string userID;
    string password;
    string wechatID;
    string nickname;
    time_t birthTime;
    int age;
    string location;
    vector<pair<string, string>> qqFriendsList;
    vector<pair<string, string>> wechatFriendsList;
    vector<pair<string, string>> weiboFriendsList;
    vector<string> groupsList;
    time_t firstTime;
    vector<string> services;
    vector<string> notifications;
    map<string, string> privacySettings;
    set<string> blockedUsers;
    string status;

public:
    User_yxc();
    User_yxc(const string& id, const string& pwd, const string& name, time_t birth, const string& loc);
    virtual ~User_yxc();

    virtual void displayInfo() const;
    void calculateAge();
    void saveToFile() const;
    static bool findUser(const string& username);
    void initializeFromFile(const string& username);
    void addService(const string& service);
    bool hasService(const string& service) const;
    void modifyFriendRemark(const string& friendID, const string& newRemark, const string& service);
    void queryFriend(const string& friendID, const string& service) const;
    void displayMutualFriends(const User_yxc& otherUser, const string& service) const;
    const string& getUserID() const;
    const string& getWeChatID() const;
    bool isBlocked(const string& otherUserID) const;
    virtual void sendFriendRequest(const string& friendID, const string& remark = "", const string& service = "QQ");
    void processFriendRequests();
    virtual void addFriend(const string& friendID, const string& remark = "", const string& service = "QQ", bool isProcessingRequest = false);
    virtual void removeFriend(const string& friendID, const string& service = "QQ");
    void joinGroup(const string& groupID, const string& service);
    void leaveGroup(const string& groupID, const string& service);
    void addGroup(const string& groupID);
    void queryGroupMembers(const string& groupID) const;
    void beKickedFromGroup(const string& groupID, const string& service);
    void displayFriends(const string& service) const;
    void displayGroups(const string& service) const;
    void addNotification(const string& message);
    void viewNotifications();
    void notifyUser(const string& otherUserID, const string& message);
    void setPrivacy(const string& key, const string& value);
    string getPrivacy(const string& key) const;
    void updateStatus(const string& newStatus);
    void blockUser(const string& otherUserID);
    void unblockUser(const string& otherUserID);
    void sendMessage(const string& receiverID, const string& content, const string& service);
    void viewMessages(const string& service);
    void saveMessageToFile(const string& receiverID, const string& content, const string& service);
    void viewMessageHistory(const string& friendID, const string& service) const;
    void sendGroupMessage(const string& groupID, const string& content);
    void viewGroupMessages(const string& groupID) const;
    string getGroupRole(const string& groupID) const;
    void addAdmin(const string& groupID, const string& userID);
    void removeAdmin(const string& groupID, const string& userID);
    void kickMember(const string& groupID, const string& userID);
    void inviteMember(const string& groupID, const string& userID);
    vector<pair<string, string>>* getFriendsList(const string& service);
    const vector<pair<string, string>>* getFriendsList(const string& service) const;
    bool verifyPassword(const string& pwd) const;
    void changePassword(const string& oldPassword, const string& newPassword);
    void processGroupInvites();
    void displayFriendInfo(const string& friendID) const;
    void modifyUserInfo();
};

#endif // USER_YXC_H