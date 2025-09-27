#ifndef WEIBO_YXC_H
#define WEIBO_YXC_H

#include "User_yxc.h"
#include "SocialMediaPost_yxc.h"

class Weibo_yxc : public User_yxc, public SocialMediaPost_yxc {
public:
    Weibo_yxc(const string& id, const string& pwd, const string& name, time_t birth, const string& loc);
    Weibo_yxc();

    void addFriend(const string& friendID, const string& service = "Weibo") ;
    void removeFriend(const string& friendID, const string& service = "Weibo") override;
    void joinGroup(const string& groupID, const string& service = "Weibo");
    void displayInfo() const override;
    void postBlog();
    void viewBlogs() const;
    void deleteBlog();
    void likeBlog(const string& friendID, const string& timestamp);
    void commentBlog(const string& friendID, const string& timestamp, const string& comment);
    void forwardBlog(const string& originalAuthorID, const string& originalTimestamp, const string& comment);

    const vector<pair<string, string>>& getFriendsList() const override;
    bool isBlocked(const string& otherUserID) const override;
};

#endif // WEIBO_YXC_H