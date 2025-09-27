#ifndef GROUP_YXC_H
#define GROUP_YXC_H

#include <string>
#include <set>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "Message_yxc.h"

using namespace std;

class Group_yxc {
private:
    string groupID;
    set<string> members;
    set<string> admins;
    string groupOwnerID;
    bool isQQGroup;
    bool isOpen;
    string platform;

public:
    Group_yxc();
    Group_yxc(const string& id, const string& ownerID, const string& platform, bool isQQ = true, bool open = true);

    void addMember(const string& userID);
    void removeMember(const string& userID);
    bool hasMember(const string& userID) const;
    void displayMembers() const;
    const string& getGroupID() const;
    const string& getOwnerID() const;
    bool isQQ() const;
    bool isGroupOpen() const;
    const string& getPlatform() const;
    const set<string>& getMembers() const;
    const set<string>& getAdmins() const;
    void saveToFile(const string& service) const;
    void initializeFromFile(const string& groupID, const string& service);
    void sendGroupMessage(const string& senderID, const string& content, const string& service);
    void viewGroupMessages(const string& service) const;
    void addAdmin(const string& userID);
    void removeAdmin(const string& userID);
    void kickMember(const string& userID);
    void inviteMember(const string& userID);
    void setGroupOpen(bool open);
};

extern unordered_map<string, Group_yxc> groupMap_yxc;

#endif // GROUP_YXC_H