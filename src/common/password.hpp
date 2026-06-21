#ifndef POCKETTRUSTEE_PASSWORD_H
#define POCKETTRUSTEE_PASSWORD_H

#include "bsp_core.h"

class PasswordFile
{
public:
    enum {PWD_NAME_MAX=32, PWD_PWD_MAX=64, PWD_ACCOUNT_MAX=64, PWD_FILE_SIZE_MAX=256};
    static constexpr char pwd_dir[] = "./passwords/";
    static constexpr char pwd_suffix[] = ".pwd";

    PKT_ERR load(const char* pwd_name);
    PKT_ERR save();
    static PKT_ERR remove(const char* name);
    PKT_ERR remove()
    {
        return remove(name);
    }

    char name[PWD_NAME_MAX] {};
    char account[PWD_ACCOUNT_MAX] {};
    char pwd[PWD_PWD_MAX] {};
};

#endif