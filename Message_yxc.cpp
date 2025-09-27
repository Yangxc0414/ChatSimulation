#include "Message_yxc.h"
#include "Utils_yxc.h"
#include <ctime>
#include <sstream>


Message_yxc::Message_yxc(const string& sender, const string& receiver, const string& msg)
    : senderID(sender), receiverID(receiver), content(msg), timestamp(getCurrentTimestamp()) {}