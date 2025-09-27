#include "QQ_yxc.h"
#include "Utils_yxc.h"

void QQ_yxc::addFriend(const string& friendID, const string& service) {
    User_yxc::addFriend(friendID, service);
}

void QQ_yxc::removeFriend(const string& friendID, const string& service) {
    User_yxc::removeFriend(friendID, service);
}

void QQ_yxc::joinGroup(const string& groupID, const string& service) {
    User_yxc::joinGroup(groupID, service);
}

void QQ_yxc::leaveGroup(const string& groupID, const string& service) {
    User_yxc::leaveGroup(groupID, service);
}

void QQ_yxc::displayInfo() const {
    User_yxc::displayInfo();
}

void QQ_yxc::createTemporaryGroup() {
    string tempGroupID = "temp_" + userID + "_" + getCurrentTimestamp();
    groupMap_yxc[tempGroupID] = Group_yxc(tempGroupID, userID, "QQ", true);
    addGroup(tempGroupID);
    cout << "已创建临时讨论组 " << tempGroupID << "。" << endl;
    groupMap_yxc[tempGroupID].saveToFile("QQ");
}

