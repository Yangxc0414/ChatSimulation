#include "Weibo_yxc.h"
#include "Utils_yxc.h"
Weibo_yxc::Weibo_yxc(const string& id, const string& pwd, const string& name, time_t birth, const string& loc)
    : User_yxc(id, pwd, name, birth, loc), SocialMediaPost_yxc(id, "blogs") {}

Weibo_yxc::Weibo_yxc() : Weibo_yxc("", "", "", 0, "") {}

void Weibo_yxc::addFriend(const string& friendID, const string& service) {
    User_yxc::addFriend(friendID, service);
}

void Weibo_yxc::removeFriend(const string& friendID, const string& service) {
    User_yxc::removeFriend(friendID, service);
}

void Weibo_yxc::joinGroup(const string& groupID, const string& service) {
    User_yxc::joinGroup(groupID, service);
}

void Weibo_yxc::displayInfo() const {
    User_yxc::displayInfo();
}

void Weibo_yxc::postBlog() {
    post();
}

void Weibo_yxc::viewBlogs() const {
    view();
}

void Weibo_yxc::deleteBlog() {
    deletePost();
}

void Weibo_yxc::likeBlog(const string& friendID, const string& timestamp) {
    likePost(friendID, timestamp);
}

void Weibo_yxc::commentBlog(const string& friendID, const string& timestamp, const string& comment) {
    commentPost(friendID, timestamp, comment);
}

void Weibo_yxc::forwardBlog(const string& originalAuthorID, const string& originalTimestamp, const string& comment) {
    if (isBlocked(originalAuthorID)) {
        cout << "无法转发，对方在您的黑名单中。" << endl;
        return;
    }

    string friendPath = SERVICE_DIR + originalAuthorID + "/blogs.txt";
    if (!fileExists(friendPath)) {
        cout << "用户 " << originalAuthorID << " 没有微博。" << endl;
        return;
    }

    ifstream infile(friendPath.c_str());
    if (!infile.is_open()) {
        cerr << "无法打开文件: " << friendPath << endl;
        return;
    }

    vector<string> blogs;
    string line;
    string currentBlog;
    while (getline(infile, line)) {
        if (line.find("Timestamp: ") == 0) {
            if (!currentBlog.empty()) {
                blogs.push_back(currentBlog);
            }
            currentBlog = line + "\n";
        }
        else {
            currentBlog += line + "\n";
        }
    }
    infile.close();

    if (!currentBlog.empty()) {
        blogs.push_back(currentBlog);
    }

    if (blogs.empty()) {
        cout << "用户 " << originalAuthorID << " 没有微博。" << endl;
        return;
    }

    cout << "用户 " << originalAuthorID << " 的微博列表：" << endl;
    for (size_t i = 0; i < blogs.size(); ++i) {
        cout << i << ": " << blogs[i];
    }

    cout << "请输入要转发的微博编号（0-" << blogs.size() - 1 << "）：";
    size_t choice;
    while (!(cin >> choice) || choice >= blogs.size()) {
        cin.clear();
        cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
        cout << "输入无效，请输入数字（0-" << blogs.size() - 1 << "）：";
    }
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    string& selectedBlog = blogs[choice];
    size_t timestampPos = selectedBlog.find("Timestamp: ");
    if (timestampPos == string::npos) {
        cout << "无法找到微博的时间戳。" << endl;
        return;
    }

    size_t endLine = selectedBlog.find("\n", timestampPos);
    string timestamp = selectedBlog.substr(timestampPos + 11, endLine - timestampPos - 11);

    string content;
    size_t contentPos = selectedBlog.find("Content: ");
    if (contentPos != string::npos) {
        content = selectedBlog.substr(contentPos + 9);
    }

    string userPath = SERVICE_DIR + User_yxc::userID;
    string path = userPath + "/blogs.txt";
    ofstream file(path.c_str(), ios::app);

    if (file.is_open()) {
        file << "Timestamp: " << getCurrentTimestamp() << endl;
        file << "Forwarded from " << originalAuthorID << " at " << timestamp << endl;
        file << "Content: " << content << endl;
        if (!comment.empty()) {
            file << "Comment: " << comment << endl;
        }
        file << "----" << endl;
        file.close();
        cout << "微博转发成功！" << endl;
    }
    else {
        cerr << "无法打开文件进行写入: " << path << endl;
    }
}

const vector<pair<string, string>>& Weibo_yxc::getFriendsList() const {
    return weiboFriendsList;
}

bool Weibo_yxc::isBlocked(const string& otherUserID) const {
    return blockedUsers.find(otherUserID) != blockedUsers.end();
}
