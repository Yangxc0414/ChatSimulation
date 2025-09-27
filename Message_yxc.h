#ifndef MESSAGE_YXC_H
#define MESSAGE_YXC_H

#include <string>
using namespace std;

class Message_yxc {
public:
    string senderID;
    string receiverID;
    string content;
    string timestamp;

    Message_yxc(const string& sender, const string& receiver, const string& msg);
};

#endif // MESSAGE_YXC_H