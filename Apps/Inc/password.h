#ifndef POCKETTRUSTEE_PASSWORD_H
#define POCKETTRUSTEE_PASSWORD_H

class PasswordFile
{
public:
    enum {PWD_NAME_MAX=32, PWD_PWD_MAX=64, PWD_ACCOUNT_MAX=64};
    static constexpr char pwd_dir[] = "./passwords/";
    static constexpr char pwd_suffix[] = ".pwd";

    int load(const char* pwd_name);
    int save();
    static int remove(const char* name);
    int remove();

    char name[PWD_NAME_MAX] {};
    char account[PWD_ACCOUNT_MAX] {};
    char pwd[PWD_PWD_MAX] {};
};

#endif