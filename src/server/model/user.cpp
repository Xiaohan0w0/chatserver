#include "user.hpp"

User::User(int id, string name, string password, string state)
{
    this->id = id;
    this->name = name;
    this->password = password;
    this->state = state;
}

void User::setId(int id)
{
    this->id = id;
}
void User::setName(string name)
{
    this->name = name;
}
void User::setPassword(string password)
{
    this->password = password;
}
void User::setState(string state)
{
    this->state = state;
}
int User::getId() const
{
    return this->id;
}

string User::getName() const
{
    return this->name;
}
string User::getPassword() const
{
    return this->password;
}
string User::getState() const
{
    return this->state;
}