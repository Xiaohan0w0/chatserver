#ifndef FRIENDMODEL_HPP
#define FRIENDMODEL_HPP 
#include <vector>
#include "user.hpp"

class FriendModel
{
public:
    // 添加好友关系
    void insert(int userid, int friendid);
    // 返回用户好友列表 friendid
    vector<User> query(int userid);
};

#endif