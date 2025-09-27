#include "WeChat_yxc.h"

WeChat_yxc::WeChat_yxc(const string& id, const string& pwd, const string& name, time_t birth, const string& loc)
    : User_yxc(id, pwd, name, birth, loc), SocialMediaPost_yxc(id, "moments") {}

WeChat_yxc::WeChat_yxc() : WeChat_yxc("", "", "", 0, "") {}

void WeChat_yxc::setWeChatID(const string& id) {
    wechatID = id;
    cout << "微信ID已设置: " << wechatID << endl;
    saveToFile();
}

bool WeChat_yxc::loginByWeChatID(const string& wechatID, const string& password) {
    if (this->wechatID == wechatID && verifyPassword(password)) {
        cout << "通过微信号登录成功！" << endl;
        return true;
    }
    cout << "微信号或密码错误，登录失败。" << endl;
    return false;
}

void WeChat_yxc::addFriend(const string& friendID, const string& service) {
    User_yxc::addFriend(friendID, service);
}

void WeChat_yxc::removeFriend(const string& friendID, const string& service) {
    User_yxc::removeFriend(friendID, service);
}

void WeChat_yxc::joinGroup(const string& groupID, const string& service) {
    cout << "您只能通过推荐加入微信群组。" << endl;
}

void WeChat_yxc::leaveGroup(const string& groupID, const string& service) {
    User_yxc::leaveGroup(groupID, service);
}

void WeChat_yxc::displayInfo() const {
    User_yxc::displayInfo();
}

void WeChat_yxc::recommendToGroup(const string& groupID, const string& userID) {
    auto it = find_if(wechatFriendsList.begin(), wechatFriendsList.end(), [&](const pair<string, string>& f) {
        return f.first == userID;
        });
    if (it != wechatFriendsList.end()) {
        if (groupMap_yxc.find(groupID) != groupMap_yxc.end()) {
            groupMap_yxc[groupID].addMember(userID);
            notifyUser(userID, "您已被推荐加入微信群组 " + groupID + "。");
            cout << "已推荐用户 " << userID << " 加入群组 " << groupID << endl;
            groupMap_yxc[groupID].saveToFile("WeChat");
        }
        else {
            cout << "群组 " << groupID << " 不存在。" << endl;
        }
    }
    else {
        cout << "用户 " << userID << " 不是您的好友，无法推荐。" << endl;
    }
}

void WeChat_yxc::postMoment() {
    post();
}

void WeChat_yxc::viewMoments() const {
    view();
}

void WeChat_yxc::deleteMoment() {
    deletePost();
}

void WeChat_yxc::likeMoment(const string& friendID, const string& timestamp) {
    likePost(friendID, timestamp);
}

void WeChat_yxc::commentMoment(const string& friendID, const string& timestamp, const string& comment) {
    commentPost(friendID, timestamp, comment);
}

const vector<pair<string, string>>& WeChat_yxc::getFriendsList() const {
    return wechatFriendsList;
}

bool WeChat_yxc::isBlocked(const string& otherUserID) const {
    return blockedUsers.find(otherUserID) != blockedUsers.end();
}
