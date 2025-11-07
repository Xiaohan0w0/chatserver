#ifndef USER_HPP
#define USER_HPP 

#include <string>
using namespace std;

// User表的ORM类
class User 
{ 
public:
    User(int id = -1, string name = "", string password = "", string state = "offline");

    void setId(int id);
    void setName(string name);
    void setPassword(string password);
    void setState(string state);

    int getId();
    string getName();
    string getPassword();
    string getState();

private:
    int id;
    string name;
    string password;
    string state;
};

#endif