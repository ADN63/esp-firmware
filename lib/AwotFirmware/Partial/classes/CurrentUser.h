#pragma once
#ifndef _PARTIAL_CURRENT_USER_H
#define _PARTIAL_CURRENT_USER_H

class CurrentUser
{
private:
    bool isLogged;
    User user;

    /* data */
public:
    CurrentUser(/* args */);
    ~CurrentUser();
    bool IsUserLogged();
    void LoggedIn(User);
    void LoggedOut();
    User GetUser();
};

CurrentUser::CurrentUser(/* args */)
{
    this->isLogged = false;
}

CurrentUser::~CurrentUser()
{
}
inline void CurrentUser::LoggedIn(User _user)
{
    this->user = _user;
    this->isLogged = true;
}
inline void CurrentUser::LoggedOut()
{
    this->isLogged = false;
}

inline User CurrentUser::GetUser()
{
    return this->user;
}

bool CurrentUser::IsUserLogged()
{
    return this->isLogged;
}

#endif