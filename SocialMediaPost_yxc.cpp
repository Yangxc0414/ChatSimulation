#include "SocialMediaPost_yxc.h"
#include "User_yxc.h"
#include "Utils_yxc.h"
SocialMediaPost_yxc::SocialMediaPost_yxc(const string& id, const string& type) : userID(id), postType(type) {}

SocialMediaPost_yxc::~SocialMediaPost_yxc() {}

void SocialMediaPost_yxc::post() {
    string content;
    cout << "请输入要发布的" << postType << "内容（输入 end 结束输入）：" << endl;
    string line;
    while (getline(cin, line)) {
        if (line == "end") break;
        content += line + "\n";
    }

    string timestamp = getCurrentTimestamp();

    string userPath = SERVICE_DIR + userID;
    string path = userPath + "/" + postType + ".txt";
    ofstream file(path.c_str(), ios::app);

    if (file.is_open()) {
        file << "Timestamp: " << timestamp << endl;
        file << "Content: " << endl << content << endl;
        file << "Likes: 0" << endl;
        file << "Comments: 0" << endl;
        file << "----" << endl;
        file.close();
        cout << postType << "发布成功！" << endl;
    }
    else {
        cerr << "无法打开文件进行写入: " << path << endl;
    }
}

void SocialMediaPost_yxc::view() const {
    cout << "\n----- 好友的" << postType << " -----" << endl;
    for (const auto& friendInfo : getFriendsList()) {
        shared_ptr<User_yxc> friendUser = make_shared<User_yxc>();
        friendUser->initializeFromFile(friendInfo.first);
        string privacy = friendUser->getPrivacy(postType);

        if (privacy == "private" && friendInfo.first != userID) {
            cout << "好友 " << friendInfo.first << " 的" << postType << "不可见。" << endl;
            continue;
        }
        else if (privacy == "friends" && find_if(getFriendsList().begin(), getFriendsList().end(), [&](const pair<string, string>& f) {
            return f.first == friendInfo.first;
            }) == getFriendsList().end()) {
            cout << "好友 " << friendInfo.first << " 的" << postType << "仅对好友可见。" << endl;
            continue;
        }

        string friendPath = SERVICE_DIR + friendInfo.first + "/" + postType + ".txt";
        if (fileExists(friendPath)) {
            cout << "好友 " << friendInfo.first << " (" << friendInfo.second << ") 的" << postType << "：" << endl;
            ifstream file(friendPath.c_str());
            if (file.is_open()) {
                string line;
                while (getline(file, line)) {
                    cout << line << endl;
                }
                file.close();
            }
            else {
                cerr << "无法打开文件: " << friendPath << endl;
            }
            cout << "==============================" << endl;
        }
    }

    string userPath = SERVICE_DIR + userID + "/" + postType + ".txt";
    if (fileExists(userPath)) {
        cout << "您的" << postType << "：" << endl;
        ifstream file(userPath.c_str());
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                cout << line << endl;
            }
            file.close();
        }
        else {
            cerr << "无法打开文件: " << userPath << endl;
        }
        cout << "==============================" << endl;
    }
    else {
        cout << "您还没有发布任何" << postType << "。" << endl;
    }

    cout << "查看完毕。" << endl;
}

void SocialMediaPost_yxc::deletePost() {
    string userPath = SERVICE_DIR + userID;
    string path = userPath + "/" + postType + ".txt";

    ifstream infile(path.c_str());
    if (!infile.is_open()) {
        cerr << "无法打开文件: " << path << endl;
        return;
    }

    vector<string> posts;
    string line;
    string currentPost;
    while (getline(infile, line)) {
        if (line.find("Timestamp: ") == 0) {
            if (!currentPost.empty()) {
                posts.push_back(currentPost);
            }
            currentPost = line + "\n";
        }
        else {
            currentPost += line + "\n";
        }
    }
    infile.close();

    if (!currentPost.empty()) {
        posts.push_back(currentPost);
    }

    if (posts.empty()) {
        cout << "您还没有发布任何" << postType << "。" << endl;
        return;
    }

    cout << "您的" << postType << "列表：" << endl;
    for (size_t i = 0; i < posts.size(); ++i) {
        cout << i << ": " << posts[i];
    }

    cout << "请输入要删除的" << postType << "编号（0-" << posts.size() - 1 << "）：";
    size_t choice;
    while (!(cin >> choice) || choice >= posts.size()) {
        cin.clear();
        cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
        cout << "输入无效，请输入数字（0-" << posts.size() - 1 << "）：";
    }
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    posts.erase(posts.begin() + choice);

    ofstream outfile(path.c_str(), ios::trunc);
    if (outfile.is_open()) {
        for (const auto& post : posts) {
            outfile << post;
        }
        outfile.close();
        cout << postType << "已删除。" << endl;
    }
    else {
        cerr << "无法打开文件进行写入: " << path << endl;
    }
}

void SocialMediaPost_yxc::likePost(const string& friendID, const string& timestamp) {
    if (isBlocked(friendID)) {
        cout << "无法点赞，对方在您的黑名单中。" << endl;
        return;
    }

    shared_ptr<User_yxc> friendUser = make_shared<User_yxc>();
    friendUser->initializeFromFile(friendID);
    string privacy = friendUser->getPrivacy(postType);

    if (privacy == "private") {
        cout << "好友 " << friendID << " 的" << postType << "不可见，无法点赞。" << endl;
        return;
    }
    else if (privacy == "friends" && find_if(getFriendsList().begin(), getFriendsList().end(), [&](const pair<string, string>& f) {
        return f.first == friendID;
        }) == getFriendsList().end()) {
        cout << "好友 " << friendID << " 的" << postType << "仅对好友可见，无法点赞。" << endl;
        return;
    }

    string friendPath = SERVICE_DIR + friendID + "/" + postType + ".txt";
    if (!fileExists(friendPath)) {
        cout << "好友 " << friendID << " 没有" << postType << "。" << endl;
        return;
    }

    ifstream infile(friendPath.c_str());
    if (!infile.is_open()) {
        cerr << "无法打开文件: " << friendPath << endl;
        return;
    }

    vector<string> posts;
    string line;
    string currentPost;
    while (getline(infile, line)) {
        if (line.find("Timestamp: ") == 0) {
            if (!currentPost.empty()) {
                posts.push_back(currentPost);
            }
            currentPost = line + "\n";
        }
        else {
            currentPost += line + "\n";
        }
    }
    infile.close();

    if (!currentPost.empty()) {
        posts.push_back(currentPost);
    }

    if (posts.empty()) {
        cout << "好友 " << friendID << " 没有" << postType << "。" << endl;
        return;
    }

    cout << "好友 " << friendID << " 的" << postType << "列表：" << endl;
    for (size_t i = 0; i < posts.size(); ++i) {
        cout << i << ": " << posts[i];
    }

    cout << "请输入要点赞的" << postType << "编号（0-" << posts.size() - 1 << "）：";
    size_t choice;
    while (!(cin >> choice) || choice >= posts.size()) {
        cin.clear();
        cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
        cout << "输入无效，请输入数字（0-" << posts.size() - 1 << "）：";
    }
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    string& selectedPost = posts[choice];
    size_t likesPos = selectedPost.find("Likes: ");
    if (likesPos != string::npos) {
        size_t endLine = selectedPost.find("\n", likesPos);
        int likes = stoi(selectedPost.substr(likesPos + 7, endLine - likesPos - 7));
        likes++;
        selectedPost.replace(likesPos, endLine - likesPos, "Likes: " + to_string(likes));
    }

    ofstream outfile(friendPath.c_str(), ios::trunc);
    if (outfile.is_open()) {
        for (const auto& post : posts) {
            outfile << post;
        }
        outfile.close();
        cout << "已点赞好友的" << postType << "。" << endl;
    }
    else {
        cerr << "无法打开文件进行写入: " << friendPath << endl;
    }
}

void SocialMediaPost_yxc::commentPost(const string& friendID, const string& timestamp, const string& comment) {
    if (isBlocked(friendID)) {
        cout << "无法评论，对方在您的黑名单中。" << endl;
        return;
    }

    shared_ptr<User_yxc> friendUser = make_shared<User_yxc>();
    friendUser->initializeFromFile(friendID);
    string privacy = friendUser->getPrivacy(postType);

    if (privacy == "private") {
        cout << "好友 " << friendID << " 的" << postType << "不可见，无法评论。" << endl;
        return;
    }
    else if (privacy == "friends" && find_if(getFriendsList().begin(), getFriendsList().end(), [&](const pair<string, string>& f) {
        return f.first == friendID;
        }) == getFriendsList().end()) {
        cout << "好友 " << friendID << " 的" << postType << "仅对好友可见，无法评论。" << endl;
        return;
    }

    string friendPath = SERVICE_DIR + friendID + "/" + postType + ".txt";
    if (!fileExists(friendPath)) {
        cout << "好友 " << friendID << " 没有" << postType << "。" << endl;
        return;
    }

    ifstream infile(friendPath.c_str());
    if (!infile.is_open()) {
        cerr << "无法打开文件: " << friendPath << endl;
        return;
    }

    vector<string> posts;
    string line;
    string currentPost;
    while (getline(infile, line)) {
        if (line.find("Timestamp: ") == 0) {
            if (!currentPost.empty()) {
                posts.push_back(currentPost);
            }
            currentPost = line + "\n";
        }
        else {
            currentPost += line + "\n";
        }
    }
    infile.close();

    if (!currentPost.empty()) {
        posts.push_back(currentPost);
    }

    if (posts.empty()) {
        cout << "好友 " << friendID << " 没有" << postType << "。" << endl;
        return;
    }

    cout << "好友 " << friendID << " 的" << postType << "列表：" << endl;
    for (size_t i = 0; i < posts.size(); ++i) {
        cout << i << ": " << posts[i];
    }

    cout << "请输入要评论的" << postType << "编号（0-" << posts.size() - 1 << "）：";
    size_t choice;
    while (!(cin >> choice) || choice >= posts.size()) {
        cin.clear();
        cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
        cout << "输入无效，请输入数字（0-" << posts.size() - 1 << "）：";
    }
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    string& selectedPost = posts[choice];
    size_t commentsPos = selectedPost.find("Comments: ");
    if (commentsPos != string::npos) {
        size_t endLine = selectedPost.find("\n", commentsPos);
        int comments = stoi(selectedPost.substr(commentsPos + 10, endLine - commentsPos - 10));
        comments++;
        selectedPost.replace(commentsPos, endLine - commentsPos, "Comments: " + to_string(comments));

        size_t separatorPos = selectedPost.find("----", commentsPos);
        if (separatorPos != string::npos) {
            selectedPost.insert(separatorPos, "[" + userID + "]: " + comment + "\n");
        }
    }

    ofstream outfile(friendPath.c_str(), ios::trunc);
    if (outfile.is_open()) {
        for (const auto& post : posts) {
            outfile << post;
        }
        outfile.close();
        cout << "已评论好友的" << postType << "。" << endl;
    }
    else {
        cerr << "无法打开文件进行写入: " << friendPath << endl;
    }
}