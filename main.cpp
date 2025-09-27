#include "User_yxc.h"
#include "Utils_yxc.h"
#include "QQ_yxc.h"
#include "WeChat_yxc.h"
#include "Weibo_yxc.h"
#include "Group_yxc.h"
#include "Message_yxc.h"
#include "SocialMediaPost_yxc.h"
#include <functional>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cerrno>
#include <unordered_map>
#include <set>
#include <map>
#include <queue>
#include <memory>
#include <limits>
#include <regex>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <direct.h>
#pragma comment(lib, "ws2_32.lib")
using namespace std;



// 全局群组映射
unordered_map<string, Group_yxc> groupMap_yxc;

// 添加 pause 函数

void GroupManagementMenu(shared_ptr<User_yxc> user, const string& service);
// 显示登录菜单
void displayLoginMenu() {
    cout << "\n========================================" << endl;
    cout << "              登录菜单" << endl;
    cout << "========================================" << endl;
    cout << "请选择要登录的社交媒体平台:" << endl;
    cout << "1. QQ" << endl;
    cout << "2. 微信" << endl;
    cout << "3. 微博" << endl;
    cout << "4. 退出所有登录" << endl;
    cout << "0. 退出程序" << endl;
    cout << "========================================" << endl;
    cout << "请选择 (0-4): ";
}

void displayWeChatLoginMenu() {
    cout << "\n========================================" << endl;
    cout << "              微信登录菜单" << endl;
    cout << "========================================" << endl;
    cout << "请选择登录方式:" << endl;
    cout << "1. 通过用户ID登录" << endl;
    cout << "2. 通过微信号登录" << endl;
    cout << "0. 返回上一级菜单" << endl;
    cout << "========================================" << endl;
    cout << "请选择 (0-2): ";
}
// 好友管理菜单
void FriendManagementMenu(shared_ptr<User_yxc> user, const string& service, unordered_map<string, shared_ptr<User_yxc>>& loggedInServices) {
    int choice;
    while (true) {
        cout << "\n----- 好友管理 -----" << endl;
        cout << "1. 查看好友列表" << endl;
        cout << "2. 查询好友信息" << endl;
        cout << "3. 发送好友请求" << endl;
        cout << "4. 处理好友请求" << endl;
        cout << "5. 修改好友备注" << endl;
        cout << "6. 删除好友" << endl;
        cout << "7. 拉黑好友" << endl;
        if (service == "QQ") {
     
        }
        else if (service == "WeChat") {
            cout << "8. 查看与QQ的共同好友" << endl;
        }
        else if (service == "Weibo") {
            cout << "8. 查看与QQ或微信的共同好友" << endl;
        }
        cout << "0. 返回上一级菜单" << endl;
        cout << "请选择 (0-8): ";
        while (!(cin >> choice)) {
            cin.clear();
            cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            cout << "输入无效，请输入数字 (0-8): ";
        }
        cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n'); // 忽略无效输入

        switch (choice) {
        case 1: {
            user->displayFriends(service);
            pause();
            break;
        }
        case 2: {
            string friendID;
            cout << "请输入要查询的好友ID: ";
            getline(cin, friendID);
            user->queryFriend(friendID, service);
            pause();
            break;
        }
        case 3: {
            string friendID, remark;
            cout << "请输入好友ID: ";
            getline(cin, friendID);
            cout << "请输入备注: ";
            getline(cin, remark);
            user->sendFriendRequest(friendID, remark, service);
            pause();
            break;
        }
        case 4: {
            user->processFriendRequests();
            pause();
            break;
        }
        case 5: {
            string friendID, newRemark;
            cout << "请输入要修改备注的好友ID: ";
            getline(cin, friendID);
            cout << "请输入新的备注: ";
            getline(cin, newRemark);
            user->modifyFriendRemark(friendID, newRemark, service);
            pause();
            break;
        }
        case 6: {
            string friendID;
            cout << "请输入要删除的好友ID: ";
            getline(cin, friendID);
            user->removeFriend(friendID, service);
            pause();
            break;
        }
        case 7: {
            string friendID;
            cout << "请输入要拉黑的好友ID: ";
            getline(cin, friendID);
            user->blockUser(friendID);
            pause();
            break;
        }
        case 8: {
            if (service == "QQ") {
                cout << "无效的选择，请重试。" << endl;
                pause();
            }
            else if (service == "WeChat") {
                if (loggedInServices.find("QQ") != loggedInServices.end()) {
                    user->displayMutualFriends(*loggedInServices["QQ"], "WeChat");
                }
                else {
                    cout << "QQ未登录。" << endl;
                }
            }
            else if (service == "Weibo") {
                cout << "请选择要查看共同好友的服务:\n1. QQ\n2. 微信\n请选择 (1-2): ";
                int serviceChoice;
                while (!(cin >> serviceChoice)) {
                    cin.clear();
                    cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
                    cout << "输入无效，请输入数字 (1-2): ";
                }
                cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n'); // 忽略无效输入
                if (serviceChoice == 1 && loggedInServices.find("QQ") != loggedInServices.end()) {
                    user->displayMutualFriends(*loggedInServices["QQ"], "Weibo");
                }
                else if (serviceChoice == 2 && loggedInServices.find("WeChat") != loggedInServices.end()) {
                    user->displayMutualFriends(*loggedInServices["WeChat"], "Weibo");
                }
                else {
                    cout << "所选服务未登录。" << endl;
                }
            }
            pause();
            break;
        }
        case 0:
            return;
        default:
            cout << "无效的选择，请重试。" << endl;
            pause();
        }
    }
}
// 信息管理菜单
void InfoManagementMenu(shared_ptr<User_yxc> user, const string& service) {
    int choice;
    while (true) {
        cout << "\n----- 信息管理 -----" << endl;
        cout << "1. 显示用户信息" << endl;
        cout << "2. 设置隐私" << endl;
        cout << "3. 更新状态" << endl;
        cout << "4. 修改密码" << endl;
        cout << "5. 修改个人信息" << endl;
        if (service == "WeChat") {
            cout << "6. 绑定微信号" << endl;
        }
        cout << "0. 返回上一级菜单" << endl;
        cout << "请选择 (0-6): ";
        while (!(cin >> choice)) {
            cin.clear();
            cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            cout << "输入无效，请输入数字 (0-6): ";
        }
        cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n'); // 忽略无效输入

        switch (choice) {
        case 1: {
            cout << "\n----- 显示用户信息 -----" << endl;
            user->displayInfo();
            pause();
            break;
        }
        case 2: {
            cout << "\n----- 设置隐私 -----" << endl;
            string key, value;
            cout << "可设置的隐私选项键: moments, blogs\n";
            cout << "请输入要设置的选项: ";
            getline(cin, key);

            vector<string> allowedKeys = { "moments", "blogs" };
            if (find(allowedKeys.begin(), allowedKeys.end(), key) == allowedKeys.end()) {
                cout << "无效的隐私选项键，请选择以下选项之一: moments, blogs" << endl;
                pause();
                break;
            }

            cout << "可设置的隐私选项值: public, friends, private\n";
            cout << "请输入权限: ";
            getline(cin, value);

            vector<string> allowedValues = { "public", "friends", "private" };
            if (find(allowedValues.begin(), allowedValues.end(), value) == allowedValues.end()) {
                cout << "无效的隐私选项值，请选择以下选项之一: public, friends, private" << endl;
                pause();
                break;
            }

            user->setPrivacy(key, value);
            pause();
            break;
        }
        case 3: {
            cout << "\n----- 更新状态 -----" << endl;
            string newStatus;
            cout << "请输入新的状态 (在线, 离线, 忙碌, 隐身): ";
            getline(cin, newStatus);

            vector<string> allowedStatuses = { "在线", "离线", "忙碌", "隐身" };
            if (find(allowedStatuses.begin(), allowedStatuses.end(), newStatus) == allowedStatuses.end()) {
                cout << "无效的状态选项，请选择以下选项之一: 在线, 离线, 忙碌, 隐身" << endl;
                pause();
                break;
            }

            user->updateStatus(newStatus);
            pause();
            break;
        }
        case 4: {
            cout << "\n----- 修改密码 -----" << endl;
            string oldPassword, newPassword;
            cout << "请输入旧密码: ";
            getline(cin, oldPassword);
            cout << "请输入新密码: ";
            getline(cin, newPassword);
            user->changePassword(oldPassword, newPassword);
            pause();
            break;
        }
        case 5: {
            cout << "\n----- 修改个人信息 -----" << endl;
            user->modifyUserInfo();
            pause();
            break;
        }
        case 6: {
            if (service == "WeChat") {
                cout << "\n----- 绑定微信号 -----" << endl;
                string wechatID;
                cout << "请输入要绑定的微信号: ";
                getline(cin, wechatID);
                dynamic_pointer_cast<WeChat_yxc>(user)->setWeChatID(wechatID);
                pause();
            }
            break;
        }
        case 0:
            return;
        default:
            cout << "无效的选择，请重试。" << endl;
            pause();
        }
    }
}
// 群组管理菜单
void GroupManagementMenu(shared_ptr<User_yxc> user, const string& service) {
    int choice;
    while (true) {
        cout << "\n======= 群组管理菜单 =======" << endl;
        cout << "1. 群组操作" << endl;
        cout << "2. 群组消息" << endl;
        cout << "3. 成员管理" << endl;
        cout << "4. 管理员管理" << endl;
        cout << "0. 返回上一级菜单" << endl;
        cout << "===========================" << endl;
        cout << "请选择 (0-4): ";
        while (!(cin >> choice)) {
            cin.clear();
            cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            cout << "输入无效，请输入数字 (0-4): ";
        }

        cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n'); // 忽略无效输入

        switch (choice) {
        case 1: { // 群组操作
            int groupOperationChoice;
            while (true) {
                cout << "\n----- 群组操作 -----" << endl;
                if (service != "WeChat") { // 如果不是微信，显示“加入群组”选项
                    cout << "1. 加入群组" << endl;
                }
                cout << "2. 退出群组" << endl;
                cout << "3. 处理群组邀请" << endl;
                cout << "4. 查看群组列表" << endl;
                cout << "5. 创建群组" << endl;
                cout << "6. 设置群组开放状态" << endl;
                cout << "0. 返回上一级菜单" << endl;
                cout << "请选择 (0-6): ";
                while (!(cin >> groupOperationChoice)) {
                    cin.clear();
                    cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
                    cout << "输入无效，请输入数字 (0-6): ";
                }
                cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n'); // 忽略无效输入

                switch (groupOperationChoice) {
                case 1: {
                    if (service == "WeChat") { // 如果是微信，跳过“加入群组”
                        cout << "无效的选择，请重试。" << endl;
                        pause();
                        break;
                    }
                    cout << "\n----- 加入群组 -----" << endl;
                    string groupID;
                    cout << "请输入群组ID: ";
                    getline(cin, groupID);
                    user->joinGroup(groupID, service);
                    pause();
                    break;
                }
                case 2: {
                    cout << "\n----- 退出群组 -----" << endl;
                    string groupID;
                    cout << "请输入要退出的群组ID: ";
                    getline(cin, groupID);
                    user->leaveGroup(groupID, service);
                    pause();
                    break;
                }
                case 3: {
                    cout << "\n----- 处理群组邀请 -----" << endl;
                    user->processGroupInvites();
                    pause();
                    break;
                }
                case 4: {
                    cout << "\n----- 查看群组列表 -----" << endl;
                    user->displayGroups(service);
                    pause();
                    break;
                }
                case 5: {
                    cout << "\n----- 创建群组 -----" << endl;
                    string groupID;
                    cout << "请输入新的群组ID: ";
                    getline(cin, groupID);
                    if (groupMap_yxc.find(groupID) == groupMap_yxc.end()) {
                        groupMap_yxc[groupID] = Group_yxc(groupID, user->getUserID(), service, service == "QQ", service == "WeChat");
                        user->addGroup(groupID);
                        user->saveToFile();
                        groupMap_yxc[groupID].saveToFile(service);
                        cout << "群组 " << groupID << " 创建成功！" << endl;
                    }
                    else {
                        cout << "群组ID已存在，请选择其他ID。" << endl;
                    }
                    pause();
                    break;
                }
                case 6: {
                    cout << "\n----- 设置群组开放状态 -----" << endl;
                    string groupID;
                    cout << "请输入群组ID: ";
                    getline(cin, groupID);
                    if (groupMap_yxc.find(groupID) != groupMap_yxc.end()) {
                        cout << "当前群组 " << groupID << " 的开放状态为: " << (groupMap_yxc[groupID].isGroupOpen() ? "开放" : "关闭") << endl;
                        cout << "是否将群组设置为开放？(y/n): ";
                        char openChoice;
                        cin >> openChoice;
                        cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n'); // 忽略无效输入
                        if (openChoice == 'y' || openChoice == 'Y') {
                            groupMap_yxc[groupID].setGroupOpen(true);
                        }
                        else {
                            groupMap_yxc[groupID].setGroupOpen(false);
                        }
                        groupMap_yxc[groupID].saveToFile(service);
                    }
                    else {
                        cout << "群组 " << groupID << " 不存在。" << endl;
                    }
                    pause();
                    break;
                }
                case 0:
                    goto GroupManagementMenuLabel;
                default:
                    cout << "无效的选择，请重试。" << endl;
                    pause();
                }
            }
        GroupManagementMenuLabel:
            break;
        }
        case 2: { // 群组消息
            int groupMessageChoice;
            while (true) {
                cout << "\n----- 群组消息 -----" << endl;
                cout << "1. 发送群消息" << endl;
                cout << "2. 查看群组消息" << endl;
                cout << "0. 返回上一级菜单" << endl;
                cout << "请选择 (0-2): ";
                while (!(cin >> groupMessageChoice)) {
                    cin.clear();
                    cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
                    cout << "输入无效，请输入数字 (0-2): ";
                }
                cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n'); // 忽略无效输入

                switch (groupMessageChoice) {
                case 1: {
                    cout << "\n----- 发送群消息 -----" << endl;
                    string groupID, content;
                    cout << "请输入群组ID: ";
                    getline(cin, groupID);
                    cout << "请输入消息内容: ";
                    getline(cin, content);
                    user->sendGroupMessage(groupID, content);
                    pause();
                    break;
                }
                case 2: {
                    cout << "\n----- 查看群组消息 -----" << endl;
                    string groupID;
                    cout << "请输入群组ID: ";
                    getline(cin, groupID);
                    user->viewGroupMessages(groupID);
                    pause();
                    break;
                }
                case 0:
                    goto GroupManagementMenuLabel;
                default:
                    cout << "无效的选择，请重试。" << endl;
                    pause();
                }
            }
        }
        case 3: { // 成员管理
            int memberManagementChoice;
            while (true) {
                cout << "\n----- 成员管理 -----" << endl;
                cout << "1. 查询群组成员" << endl;
                cout << "2. 邀请成员" << endl;
                cout << "3. 踢出群成员" << endl;
                cout << "0. 返回上一级菜单" << endl;
                cout << "请选择 (0-3): ";
                while (!(cin >> memberManagementChoice)) {
                    cin.clear();
                    cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
                    cout << "输入无效，请输入数字 (0-3): ";
                }
                cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n'); // 忽略无效输入

                switch (memberManagementChoice) {
                case 1: {
                    cout << "\n----- 查询群组成员 -----" << endl;
                    string groupID;
                    cout << "请输入要查询的群组ID: ";
                    getline(cin, groupID);
                    user->queryGroupMembers(groupID);
                    pause();
                    break;
                }
                case 2: {
                    cout << "\n----- 邀请成员 -----" << endl;
                    string groupID, userID;
                    cout << "请输入群组ID: ";
                    getline(cin, groupID);
                    cout << "请输入要邀请的用户ID: ";
                    getline(cin, userID);
                    groupMap_yxc[groupID].inviteMember(userID);
                    pause();
                    break;
                }
                case 3: {
                    cout << "\n----- 踢出群成员 -----" << endl;
                    string groupID, userID;
                    cout << "请输入群组ID: ";
                    getline(cin, groupID);
                    cout << "请输入要踢出的用户ID: ";
                    getline(cin, userID);
                    user->kickMember(groupID, userID);
                    pause();
                    break;
                }
                case 0:
                    goto GroupManagementMenuLabel;
                default:
                    cout << "无效的选择，请重试。" << endl;
                    pause();
                }
            }
        }
        case 4: { // 管理员管理
            int adminManagementChoice;
            while (true) {
                cout << "\n----- 管理员管理 -----" << endl;
                cout << "1. 查看群组角色" << endl;
                cout << "2. 添加管理员" << endl;
                cout << "3. 移除管理员" << endl;
                cout << "0. 返回上一级菜单" << endl;
                cout << "请选择 (0-3): ";
                while (!(cin >> adminManagementChoice)) {
                    cin.clear();
                    cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
                    cout << "输入无效，请输入数字 (0-3): ";
                }
                cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n'); // 忽略无效输入

                switch (adminManagementChoice) {
                case 1: {
                    cout << "\n----- 查看群组角色 -----" << endl;
                    string groupID;
                    cout << "请输入群组ID: ";
                    getline(cin, groupID);
                    string role = user->getGroupRole(groupID);
                    cout << "您在群组 " << groupID << " 中的角色是: " << role << endl;
                    pause();
                    break;
                }
                case 2: {
                    cout << "\n----- 添加管理员 -----" << endl;
                    string groupID, userID;
                    cout << "请输入群组ID: ";
                    getline(cin, groupID);
                    cout << "请输入要添加为管理员的用户ID: ";
                    getline(cin, userID);
                    user->addAdmin(groupID, userID);
                    pause();
                    break;
                }
                case 3: {
                    cout << "\n----- 移除管理员 -----" << endl;
                    string groupID, userID;
                    cout << "请输入群组ID: ";
                    getline(cin, groupID);
                    cout << "请输入要移除的管理员用户ID: ";
                    getline(cin, userID);
                    user->removeAdmin(groupID, userID);
                    pause();
                    break;
                }
                case 0:
                    goto GroupManagementMenuLabel;
                default:
                    cout << "无效的选择，请重试。" << endl;
                    pause();
                }
            }
        }
        case 0:
            return; // 返回上一级菜单
        default:
            cout << "无效的选择，请重试。" << endl;
            pause();
        }
    }
}
// 黑名单管理菜单
void BlocklistManagementMenu(shared_ptr<User_yxc> user) {
    int choice;
    while (true) {
        cout << "\n----- 黑名单管理 -----" << endl;
        cout << "1. 拉黑用户" << endl;
        cout << "2. 取消拉黑" << endl;
        cout << "0. 返回上一级菜单" << endl;
        cout << "请选择 (0-2): ";
        while (!(cin >> choice)) {
            cin.clear();
            cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            cout << "输入无效，请输入数字 (0-2): ";
        }
        cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n'); // 忽略无效输入

        switch (choice) {
        case 1: {
            string otherUserID;
            cout << "请输入要拉黑的用户ID: ";
            getline(cin, otherUserID);
            user->blockUser(otherUserID);
            pause();
            break;
        }
        case 2: {
            string otherUserID;
            cout << "请输入要取消拉黑的用户ID: ";
            getline(cin, otherUserID);
            user->unblockUser(otherUserID);
            pause();
            break;
        }
        case 0:
            return;
        default:
            cout << "无效的选择，请重试。" << endl;
            pause();
        }
    }
}
// QQ功能菜单
void QQMenu(shared_ptr<User_yxc> user, unordered_map<string, shared_ptr<User_yxc>>& loggedInServices);

// 微信功能菜单
void WeChatMenu(shared_ptr<User_yxc> user, unordered_map<string, shared_ptr<User_yxc>>& loggedInServices);

// 微博功能菜单
void WeiboMenu(shared_ptr<User_yxc> user, unordered_map<string, shared_ptr<User_yxc>>& loggedInServices);
// QQ功能菜单

void QQMenu(shared_ptr<User_yxc> user, unordered_map<string, shared_ptr<User_yxc>>& loggedInServices) {
    int choice;
    while (true) {
        cout << "\n========= QQ 功能菜单 =========" << endl;
        cout << "1. 好友管理" << endl;
        cout << "2. 群组管理" << endl;
        cout << "3. 消息管理" << endl;
        cout << "4. 信息管理" << endl;
        cout << "5. 黑名单管理" << endl;
        cout << "0. 返回上一级菜单" << endl;
        cout << "=============================" << endl;
        cout << "请选择 (0-5): ";
        while (!(cin >> choice)) {
            cin.clear();
            cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            cout << "输入无效，请输入数字 (0-5): ";
        }

        switch (choice) {
        case 1: {
            FriendManagementMenu(user, "QQ", loggedInServices);
            break;
        }
        case 2: {
            GroupManagementMenu(user, "QQ");
            break;
        }
        case 3: {
            int msgChoice;
            while (true) {
                cout << "\n----- 消息管理 -----" << endl;
                cout << "1. 即时消息" << endl;
                cout << "2. 消息通知" << endl;
                cout << "0. 返回上一级菜单" << endl;
                cout << "请选择 (0-2): ";
                while (!(cin >> msgChoice)) {
                    cin.clear();
                    cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n'); // 忽略无效输入
                    cout << "输入无效，请输入数字 (0-2): ";
                }
                cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n'); // 忽略无效输入
                switch (msgChoice) {
                case 1: {
                    cout << "\n----- 即时消息 -----" << endl;
                    int msgAction;
                    cout << "1. 发送消息\n2. 查看消息\n3. 查看历史消息记录\n请选择 (1-3): ";
                    while (!(cin >> msgAction)) {
                        cin.clear();
                        cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n'); // 忽略无效输入
                        cout << "输入无效，请输入数字 (1-3): ";
                    }
                    cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n'); // 忽略无效输入
                    if (msgAction == 1) {
                        string receiverID, content;
                        cout << "请输入接收者ID: ";
                        getline(cin, receiverID);
                        cout << "请输入消息内容: ";
                        getline(cin, content);
                        user->sendMessage(receiverID, content, "QQ");
                    }
                    else if (msgAction == 2) {
                        user->viewMessages("QQ");
                    }
                    else if (msgAction == 3) {
                        string friendID;
                        cout << "请输入要查看历史消息记录的好友ID: ";
                        getline(cin, friendID);
                        user->viewMessageHistory(friendID, "QQ");
                    }
                    else {
                        cout << "无效的选择。" << endl;
                    }
                    pause();
                    break;
                }
                case 2: {
                    cout << "\n----- 消息通知 -----" << endl;
                    user->viewNotifications();
                    pause();
                    break;
                }
                case 0:
                    goto QQMenuLabel2;
                default:
                    cout << "无效的选择，请重试。" << endl;
                    pause();
                }
            }
        QQMenuLabel2:
            break;
        }
        case 4: {
            InfoManagementMenu(user, "QQ");
            break;
        }
        case 5: {
            BlocklistManagementMenu(user);
            break;
        }
        case 0:
            return; // 返回上一级菜单
        default:
            cout << "无效的选择，请重试。" << endl;
            pause();
        }
    }
}
// 微信功能菜单
void WeChatMenu(shared_ptr<User_yxc> user, unordered_map<string, shared_ptr<User_yxc>>& loggedInServices) {
    int choice;
    while (true) {
        cout << "\n======= 微信 功能菜单 =======" << endl;
        cout << "1. 好友管理" << endl;
        cout << "2. 群组管理" << endl;
        cout << "3. 消息管理" << endl;
        cout << "4. 信息管理" << endl;
        cout << "5. 黑名单管理" << endl;
        cout << "6. 朋友圈" << endl;
        cout << "0. 返回上一级菜单" << endl;
        cout << "=============================" << endl;
        cout << "请选择 (0-6): ";
        while (!(cin >> choice)) {
            cin.clear();
            cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            cout << "输入无效，请输入数字 (0-6): ";
        }

        switch (choice) {
        case 1: {
            FriendManagementMenu(user, "WeChat", loggedInServices);
            break;
        }
        case 2: { // 群组管理
            GroupManagementMenu(user, "WeChat");
            break;
        }
        case 3: { // 消息管理
            int msgChoice;
            while (true) {
                cout << "\n----- 消息管理 -----" << endl;
                cout << "1. 即时消息" << endl;
                cout << "2. 消息通知" << endl;
                cout << "0. 返回上一级菜单" << endl;
                cout << "请选择 (0-2): ";
                while (!(cin >> msgChoice)) {
                    cin.clear();
                    cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
                    cout << "输入无效，请输入数字 (0-2): ";
                }
                cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n'); // 忽略无效输入
                switch (msgChoice) {
                case 1: {
                    cout << "\n----- 即时消息 -----" << endl;
                    int msgAction;
                    cout << "1. 发送消息\n2. 查看消息\n3. 查看历史消息记录\n请选择 (1-3): ";
                    while (!(cin >> msgAction)) {
                        cin.clear();
                        cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
                        cout << "输入无效，请输入数字 (1-3): ";
                    }
                    cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n'); // 忽略无效输入
                    if (msgAction == 1) {
                        string receiverID, content;
                        cout << "请输入接收者ID: ";
                        getline(cin, receiverID);
                        cout << "请输入消息内容: ";
                        getline(cin, content);
                        user->sendMessage(receiverID, content, "WeChat");
                    }
                    else if (msgAction == 2) {
                        user->viewMessages("WeChat");
                    }
                    else if (msgAction == 3) {
                        string friendID;
                        cout << "请输入要查看历史消息记录的好友ID: ";
                        getline(cin, friendID);
                        user->viewMessageHistory(friendID, "WeChat");
                    }
                    else {
                        cout << "无效的选择。" << endl;
                    }
                    pause();
                    break;
                }
                case 2: {
                    cout << "\n----- 消息通知 -----" << endl;
                    user->viewNotifications();
                    pause();
                    break;
                }
                case 0:
                    goto WeChatMenuLabel2;
                default:
                    cout << "无效的选择，请重试。" << endl;
                    pause();
                }
            }
        WeChatMenuLabel2:
            break;
        }
        case 4: {
            InfoManagementMenu(user, "WeChat");
            break;
        }
        case 5: {
            BlocklistManagementMenu(user);
            break;
        }
        case 6: { // 朋友圈
            int momentChoice;
            while (true) {
                cout << "\n----- 朋友圈 -----" << endl;
                cout << "1. 发布朋友圈" << endl;
                cout << "2. 查看朋友圈" << endl;
                cout << "3. 删除朋友圈" << endl;
                cout << "4. 点赞朋友圈" << endl;
                cout << "5. 评论朋友圈" << endl;
                cout << "0. 返回上一级菜单" << endl;
                cout << "请选择 (0-5): ";
                while (!(cin >> momentChoice)) {
                    cin.clear();
                    cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
                    cout << "输入无效，请输入数字 (0-5): ";
                }
                cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n'); // 忽略无效输入

                switch (momentChoice) {
                case 1:
                    dynamic_pointer_cast<WeChat_yxc>(user)->postMoment();
                    pause();
                    break;
                case 2:
                    dynamic_pointer_cast<WeChat_yxc>(user)->viewMoments();
                    pause();
                    break;
                case 3:
                    dynamic_pointer_cast<WeChat_yxc>(user)->deleteMoment();
                    pause();
                    break;
                case 4: {
                    string friendID, timestamp;
                    cout << "请输入好友ID: ";
                    getline(cin, friendID);

                    dynamic_pointer_cast<WeChat_yxc>(user)->likeMoment(friendID, timestamp);
                    pause();
                    break;
                }
                case 5: {
                    string friendID, timestamp, comment;
                    cout << "请输入好友ID: ";
                    getline(cin, friendID);

                    cout << "请输入评论内容: ";
                    getline(cin, comment);
                    dynamic_pointer_cast<WeChat_yxc>(user)->commentMoment(friendID, timestamp, comment);
                    pause();
                    break;
                }
                case 0:
                    goto WeChatMomentMenuLabel;
                default:
                    cout << "无效的选择，请重试。" << endl;
                    pause();
                }
            }
        WeChatMomentMenuLabel:
            break;
        }
        case 0:
            return; // 返回上一级菜单
        default:
            cout << "无效的选择，请重试。" << endl;
            pause();
        }
    }
}
// 微博功能菜单
void WeiboMenu(shared_ptr<User_yxc> user, unordered_map<string, shared_ptr<User_yxc>>& loggedInServices) {
    int choice;
    while (true) {
        cout << "\n======= 微博 功能菜单 =======" << endl;
        cout << "1. 好友管理" << endl;
        cout << "2. 群组管理" << endl;
        cout << "3. 消息管理" << endl;
        cout << "4. 信息管理" << endl;
        cout << "5. 黑名单管理" << endl;
        cout << "6. 微博管理" << endl;
        cout << "0. 返回上一级菜单" << endl;
        cout << "=============================" << endl;
        cout << "请选择 (0-6): ";
        while (!(cin >> choice)) {
            cin.clear();
            cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            cout << "输入无效，请输入数字 (0-6): ";
        }

        switch (choice) {
        case 1: {
            FriendManagementMenu(user, "Weibo", loggedInServices);
            break;
        }
        case 2: { // 群组管理
            GroupManagementMenu(user, "Weibo");
            break;
        }
        case 3: { // 消息管理
            int msgChoice;
            while (true) {
                cout << "\n----- 消息管理 -----" << endl;
                cout << "1. 即时消息" << endl;
                cout << "2. 消息通知" << endl;
                cout << "0. 返回上一级菜单" << endl;
                cout << "请选择 (0-2): ";
                while (!(cin >> msgChoice)) {
                    cin.clear();
                    cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
                    cout << "输入无效，请输入数字 (0-2): ";
                }
                cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n'); // 忽略无效输入
                switch (msgChoice) {
                case 1: {
                    cout << "\n----- 即时消息 -----" << endl;
                    int msgAction;
                    cout << "1. 发送消息\n2. 查看消息\n3. 查看历史消息记录\n请选择 (1-3): ";
                    while (!(cin >> msgAction)) {
                        cin.clear();
                        cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
                        cout << "输入无效，请输入数字 (1-3): ";
                    }
                    cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n'); // 忽略无效输入
                    if (msgAction == 1) {
                        string receiverID, content;
                        cout << "请输入接收者ID: ";
                        getline(cin, receiverID);
                        cout << "请输入消息内容: ";
                        getline(cin, content);
                        user->sendMessage(receiverID, content, "Weibo");
                    }
                    else if (msgAction == 2) {
                        user->viewMessages("Weibo");
                    }
                    else if (msgAction == 3) {
                        string friendID;
                        cout << "请输入要查看历史消息记录的好友ID: ";
                        getline(cin, friendID);
                        user->viewMessageHistory(friendID, "Weibo");
                    }
                    else {
                        cout << "无效的选择。" << endl;
                    }
                    pause();
                    break;
                }
                case 2: {
                    cout << "\n----- 消息通知 -----" << endl;
                    user->viewNotifications();
                    pause();
                    break;
                }
                case 0:
                    goto WeiboMenuLabel2;
                default:
                    cout << "无效的选择，请重试。" << endl;
                    pause();
                }
            }
        WeiboMenuLabel2:
            break;
        }
        case 4: {
            InfoManagementMenu(user, "Weibo");
            break;
        }

        case 5: {
            BlocklistManagementMenu(user);
            break;
        }
        case 6: { // 微博管理
            int blogChoice;
            while (true) {
                cout << "\n----- 微博管理 -----" << endl;
                cout << "1. 发布微博" << endl;
                cout << "2. 查看微博" << endl;
                cout << "3. 删除微博" << endl;
                cout << "4. 点赞微博" << endl;
                cout << "5. 评论微博" << endl;
                cout << "6. 转发微博" << endl; // 转发微博功能整合到微博管理中
                cout << "0. 返回上一级菜单" << endl;
                cout << "请选择 (0-6): ";
                while (!(cin >> blogChoice)) {
                    cin.clear();
                    cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
                    cout << "输入无效，请输入数字 (0-6): ";
                }
                cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n'); // 忽略无效输入
                switch (blogChoice) {
                case 1:
                    dynamic_pointer_cast<Weibo_yxc>(user)->postBlog();
                    pause();
                    break;
                case 2:
                    dynamic_pointer_cast<Weibo_yxc>(user)->viewBlogs();
                    pause();
                    break;
                case 3:
                    dynamic_pointer_cast<Weibo_yxc>(user)->deleteBlog();
                    pause();
                    break;
                case 4: {
                    string friendID, timestamp;
                    cout << "请输入好友ID: ";
                    getline(cin, friendID);

                    dynamic_pointer_cast<Weibo_yxc>(user)->likeBlog(friendID, timestamp);
                    pause();
                    break;
                }
                case 5: {
                    string friendID, timestamp, comment;
                    cout << "请输入好友ID: ";
                    getline(cin, friendID);

                    cout << "请输入评论内容: ";
                    getline(cin, comment);
                    dynamic_pointer_cast<Weibo_yxc>(user)->commentBlog(friendID, timestamp, comment);
                    pause();
                    break;
                }
                case 6: { // 转发微博
                    cout << "\n----- 转发微博 -----" << endl;
                    string originalAuthorID, originalTimestamp, comment;
                    cout << "请输入原微博作者ID: ";
                    getline(cin, originalAuthorID);

                    cout << "请输入转发评论 (可选): ";
                    getline(cin, comment);
                    dynamic_pointer_cast<Weibo_yxc>(user)->forwardBlog(originalAuthorID, originalTimestamp, comment);
                    pause();
                    break;
                }
                case 0:
                    goto WeiboBlogMenuLabel;
                default:
                    cout << "无效的选择，请重试。" << endl;
                    pause();
                }
            }
        WeiboBlogMenuLabel:
            break;
        }
        case 0:
            return; // 返回上一级菜单
        default:
            cout << "无效的选择，请重试。" << endl;
            pause();
        }
    }
}
unordered_map<string, shared_ptr<User_yxc>> loggedInServices; // 已登录的服务映射
shared_ptr<User_yxc> currentUser = nullptr;

// 提取公共逻辑的函数
void handleLoginAndRegister(const string& service, unordered_map<string, shared_ptr<User_yxc>>& loggedInServices,
    function<void(shared_ptr<User_yxc>, unordered_map<string, shared_ptr<User_yxc>>&)> menuFunction) {
    shared_ptr<User_yxc> currentUser = nullptr;

    // 检查是否已经登录
    if (loggedInServices.find(service) != loggedInServices.end()) {
        currentUser = loggedInServices[service];
        cout << "您已登录" << service << "。" << endl;
        menuFunction(currentUser, loggedInServices); // 进入相应的功能菜单
        return;
    }

    string userID, password;
    cout << "\n----- " << service << " 登录 -----" << endl;
    cout << "请输入" << service << "账号: ";
    getline(cin, userID);
    cout << "请输入密码: ";
    getline(cin, password);

    if (userID.empty()) {
        cout << "用户ID不能为空，请重新输入。" << endl;
        pause();
        return;
    }

    if (User_yxc::findUser(userID)) {
        if (service == "QQ") {
            currentUser = make_shared<QQ_yxc>();
        }
        else if (service == "WeChat") {
            currentUser = make_shared<WeChat_yxc>();
        }
        else if (service == "Weibo") {
            currentUser = make_shared<Weibo_yxc>();
        }
        currentUser->initializeFromFile(userID); // 从文件初始化用户
        if (currentUser->verifyPassword(password)) {
            loggedInServices[service] = currentUser;
            cout << "登录成功！" << endl;

            // 更新用户状态为“在线”
            currentUser->updateStatus("在线");
            currentUser->saveToFile(); // 保存状态到文件
        }
        else {
            cout << "密码错误，登录失败。" << endl;
            currentUser = nullptr; // 清除当前用户对象
            pause();
            return; // 返回主界面
        }
    }
    else {
        cout << "用户不存在。正在注册新用户..." << endl;
        string nickname;
        cout << "请输入昵称: ";
        getline(cin, nickname);

        if (nickname.empty()) {
            cout << "昵称不能为空，请重新输入。" << endl;
            pause();
            return;
        }

        cout << "请输入生日 (YYYY-MM-DD): ";
        string birthDateStr;
        getline(cin, birthDateStr);
        time_t birthTime = parseDate(birthDateStr);
        if (birthTime == -1) {
            cerr << "解析生日失败，使用当前日期。" << endl;
            birthTime = time(0);
        }

        cout << "请输入位置: ";
        string location;
        getline(cin, location);

        if (location.empty()) {
            cout << "位置不能为空，请重新输入。" << endl;
            pause();
            return;
        }

        if (service == "QQ") {
            currentUser = make_shared<QQ_yxc>(userID, password, nickname, birthTime, location);
        }
        else if (service == "WeChat") {
            currentUser = make_shared<WeChat_yxc>(userID, password, nickname, birthTime, location);
        }
        else if (service == "Weibo") {
            currentUser = make_shared<Weibo_yxc>(userID, password, nickname, birthTime, location);
        }

        currentUser->addService(service); // 添加当前服务

        // 询问用户是否激活其他服务
        cout << "是否要激活微信服务? (y/n): ";
        char activateWeChat;
        cin >> activateWeChat;
        cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n'); // 忽略无效输入
        if (activateWeChat == 'y' || activateWeChat == 'Y') {
            currentUser->addService("WeChat");
            cout << "微信服务已激活。" << endl;
        }

        cout << "是否要激活微博服务? (y/n): ";
        char activateWeibo;
        cin >> activateWeibo;
        cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n'); // 忽略无效输入
        if (activateWeibo == 'y' || activateWeibo == 'Y') {
            currentUser->addService("Weibo");
            cout << "微博服务已激活。" << endl;
        }

        currentUser->saveToFile(); // 保存新用户信息
        loggedInServices[service] = currentUser;
        cout << "注册成功！" << endl;

        // 更新用户状态为“在线”
        currentUser->updateStatus("在线");
        currentUser->saveToFile(); // 保存状态到文件
    }

    // 自动登录其他服务
    if (currentUser->hasService("WeChat") && loggedInServices.find("WeChat") == loggedInServices.end()) {
        shared_ptr<User_yxc> weChatUser = make_shared<WeChat_yxc>();
        weChatUser->initializeFromFile(userID);
        if (weChatUser->verifyPassword(password)) {
            loggedInServices["WeChat"] = weChatUser;
            cout << "微信登录成功！" << endl;

            // 更新用户状态为“在线”
            weChatUser->updateStatus("在线");
            weChatUser->saveToFile(); // 保存状态到文件
        }
    }

    if (currentUser->hasService("Weibo") && loggedInServices.find("Weibo") == loggedInServices.end()) {
        shared_ptr<User_yxc> weiboUser = make_shared<Weibo_yxc>();
        weiboUser->initializeFromFile(userID);
        if (weiboUser->verifyPassword(password)) {
            loggedInServices["Weibo"] = weiboUser;
            cout << "微博登录成功！" << endl;

            // 更新用户状态为“在线”
            weiboUser->updateStatus("在线");
            weiboUser->saveToFile(); // 保存状态到文件
        }
    }

    menuFunction(currentUser, loggedInServices); // 进入相应的功能菜单
}

void handleWeChatLogin(unordered_map<string, shared_ptr<User_yxc>>& loggedInServices) {
    // 检查是否已经登录了 QQ
    if (loggedInServices.find("QQ") != loggedInServices.end()) {
        // 如果 QQ 已经登录，直接进入微信的界面
        if (loggedInServices.find("WeChat") != loggedInServices.end()) {
            cout << "您已登录微信。" << endl;
            WeChatMenu(loggedInServices["WeChat"], loggedInServices);
            return;
        }
        else {
            cout << "检测到您已登录 QQ，是否直接进入微信？(y/n): ";
            char choice;
            cin >> choice;
            cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n'); // 忽略无效输入
            if (choice == 'y' || choice == 'Y') {
                // 使用 QQ 的用户信息登录微信
                shared_ptr<User_yxc> qqUser = loggedInServices["QQ"];
                shared_ptr<WeChat_yxc> weChatUser = make_shared<WeChat_yxc>();
                weChatUser->initializeFromFile(qqUser->getUserID());
                loggedInServices["WeChat"] = weChatUser;
                cout << "已自动登录微信。" << endl;
                WeChatMenu(weChatUser, loggedInServices);
                return;
            }
        }
    }

    // 如果 QQ 未登录，显示微信登录子菜单
    int choice;
    while (true) {
        displayWeChatLoginMenu(); // 显示微信登录子菜单
        while (!(cin >> choice)) {
            cin.clear();
            cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            cout << "输入无效，请输入数字 (0-2): ";
        }
        cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n'); // 忽略无效输入

        switch (choice) {
        case 1: { // 通过用户ID登录
            handleLoginAndRegister("WeChat", loggedInServices, WeChatMenu);
            return;
        }
        case 2: { // 通过微信号登录
            string wechatID, password;
            cout << "\n----- 通过微信号登录 -----" << endl;
            cout << "请输入微信号: ";
            getline(cin, wechatID);
            cout << "请输入密码: ";
            getline(cin, password);

            // 查找用户
            for (const auto& service : loggedInServices) {
                if (service.first == "WeChat") {
                    shared_ptr<WeChat_yxc> weChatUser = dynamic_pointer_cast<WeChat_yxc>(service.second);
                    if (weChatUser->loginByWeChatID(wechatID, password)) {
                        loggedInServices["WeChat"] = weChatUser;
                        cout << "通过微信号登录成功！" << endl;
                        WeChatMenu(weChatUser, loggedInServices); // 进入微信功能菜单
                        return;
                    }
                }
            }

            cout << "微信号或密码错误，登录失败。" << endl;
            pause();
            break;
        }
        case 0: // 返回上一级菜单
            return;
        default:
            cout << "无效的选择，请重试。" << endl;
            pause();
        }
    }
}

// 初始化群组数据
void initializeGroups() {
    // 确保群组目录存在
    string groupDirs[] = { SERVICE_DIR + "QQ_group/", SERVICE_DIR + "weixin_group/", SERVICE_DIR + "weibo_group/" };
    for (const auto& groupDir : groupDirs) {
        if (!createDirectory(groupDir)) {
            cerr << "无法创建群组目录: " << groupDir << endl;
            return;
        }
    }
    // 从群组目录读取所有群组文件
    for (const auto& groupDir : groupDirs) {
        WIN32_FIND_DATAA findFileData;
        HANDLE hFind = FindFirstFileA((groupDir + "*.txt").c_str(), &findFileData);

        if (hFind != INVALID_HANDLE_VALUE) {
            do {
                string filename = findFileData.cFileName;
                string groupID = filename.substr(0, filename.find('.'));
                Group_yxc group;
                group.initializeFromFile(groupID, groupDir == SERVICE_DIR + "QQ_group/" ? "QQ" : groupDir == SERVICE_DIR + "weixin_group/" ? "WeChat" : "Weibo");
                groupMap_yxc[groupID] = group;
            } while (FindNextFileA(hFind, &findFileData));
            FindClose(hFind);
        }
        else {
            cerr << "没有找到任何群组文件，可能是第一次运行程序。" << endl;
        }
    }
}
void messageProcessingThread(const string& service) {
    while (true) {
        unique_lock<mutex> lock(messageQueueMutexes[service]);
        messageQueueCVs[service].wait(lock, [&service] { return !messageQueues[service].empty(); });

        Message_yxc msg = messageQueues[service].front();
        messageQueues[service].pop();
        lock.unlock();

        // 处理消息
        cout << "处理消息: " << msg.senderID << " -> " << msg.receiverID << ": " << msg.content << endl;
    }
}
int main() {
    // 初始化群组数据
    initializeGroups();

    // 启动消息处理线程
    thread qqMessageThread(messageProcessingThread, "QQ");
    thread wechatMessageThread(messageProcessingThread, "WeChat");
    thread weiboMessageThread(messageProcessingThread, "Weibo");

    // 主程序逻辑
    while (true) {
        displayLoginMenu();
        int choice;
        cin >> choice;
        cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
        switch (choice) {
        case 1: {
            handleLoginAndRegister("QQ", loggedInServices, QQMenu);
            break;
        }
        case 2: {
            handleWeChatLogin(loggedInServices);
            break;
        }
        case 3: {
            handleLoginAndRegister("Weibo", loggedInServices, WeiboMenu);
            break;
        }
        case 4: {
            cout << "退出所有登录..." << endl;
            loggedInServices.clear();
            currentUser = nullptr;
            cout << "所有登录已退出。" << endl;
            pause();
            break;
        }
        case 0: {
            cout << "感谢使用本程序。再见！" << endl;
            loggedInServices.clear();
            return 0;
        }
        default: {
            cout << "无效的选择，请重试。" << endl;
        }
        }
    }

    qqMessageThread.join();
    wechatMessageThread.join();
    weiboMessageThread.join();

    return 0;
}