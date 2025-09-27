#ifndef SOCIALMEDIAPOST_YXC_H
#define SOCIALMEDIAPOST_YXC_H

#include <string>
#include <vector>
#include <map>
#include <set>
#include <ctime>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <memory>

using namespace std;

class SocialMediaPost_yxc {
protected:
    string userID;
    string postType;

public:
    SocialMediaPost_yxc(const string& id, const string& type);
    virtual ~SocialMediaPost_yxc();

    void post();
    void view() const;
    void deletePost();
    void likePost(const string& friendID, const string& timestamp);
    void commentPost(const string& friendID, const string& timestamp, const string& comment);

    virtual const vector<pair<string, string>>& getFriendsList() const = 0;
    virtual bool isBlocked(const string& otherUserID) const = 0;
};

#endif // SOCIALMEDIAPOST_YXC_H