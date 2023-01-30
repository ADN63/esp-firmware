#pragma once
#ifndef _PARTIAL_USER_BASE_H
#define _PARTIAL_USER_BASE_H

class User
{
private:
    /* data */

public:
    char username[20];
    char password[20];
    User(/* args */);
    User(const char *, const char *);
    ~User();
};

User::User(const char *username, const char *password)
{
    sprintf_P(this->username, PSTR("%s"), username);
    sprintf_P(this->password, PSTR("%s"), password);
}

User::User(/* args */)
{
}

User::~User()
{
}

#endif
