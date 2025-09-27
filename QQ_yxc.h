#ifndef QQ_YXC_H
#define QQ_YXC_H

#include "User_yxc.h"
#include "Utils_yxc.h"
class QQ_yxc : public User_yxc {
public:
    using User_yxc::User_yxc;

    void addFriend(const string& friendID, const string& service = "QQ") ;
    void removeFriend(const string& friendID, const string& service = "QQ") override;
    void joinGroup(const string& groupID, const string& service = "QQ");
    void leaveGroup(const string& groupID, const string& service = "QQ");
    void displayInfo() const override;
    void createTemporaryGroup();
};

#endif // QQ_YXC_H