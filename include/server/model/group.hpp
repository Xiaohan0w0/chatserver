#ifndef GROUP_HPP
#define GROUP_HPP 

#include <string>
#include <vector>
#include "groupuser.hpp"


class Group {
public:
    Group(int id = -1, string name = "", string desc = "");
    void setId(int id);
    void setName(string name);
    void setDesc(string desc);

    int getId();
    string getName();
    string getDesc();
    vector<GroupUser> &getUsers();

private:
    int id;
    string name;
    string desc;
    vector<GroupUser> users;
};
#endif