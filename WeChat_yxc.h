#ifndef WECHAT_YXC_H
#define WECHAT_YXC_H

#include "User_yxc.h"
#include "SocialMediaPost_yxc.h"

class WeChat_yxc : public User_yxc, public SocialMediaPost_yxc {
public:
    WeChat_yxc(const string& id, const string& pwd, const string& name, time_t birth, const string& loc);
    WeChat_yxc();

    void setWeChatID(const string& id);
    bool loginByWeChatID(const string& wechatID, const string& password);
    void addFriend(const string& friendID, const string& service = "WeChat") ;
    void removeFriend(const string& friendID, const string& service = "WeChat") override;
    void joinGroup(const string& groupID, const string& service = "WeChat");
    void leaveGroup(const string& groupID, const string& service = "WeChat");
    void displayInfo() const override;
    void recommendToGroup(const string& groupID, const string& userID);
    void postMoment();
    void viewMoments() const;
    void deleteMoment();
    void likeMoment(const string& friendID, const string& timestamp);
    void commentMoment(const string& friendID, const string& timestamp, const string& comment);

    const vector<pair<string, string>>& getFriendsList() const override;
    bool isBlocked(const string& otherUserID) const override;
};

#endif // WECHAT_YXC_H