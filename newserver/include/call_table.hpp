#ifndef CALLTABLE_HPP
#define CALLTABLE_HPP
#include <string>
#include <vector>
#include <variant>
class Client;
struct message_result
{
    unsigned char version;
    unsigned char code;
    unsigned short flag; //Зарезервировано
    unsigned int size;
    enum results : unsigned char{
        OK = 0,
        ERROR_FILENOTFOUND = 1,
        ERROR_CMDINCORRECT = 2,
        ERROR_PKGALREADYLOADED = 3,
        EVENT = 4
    };
    enum flags : unsigned short
    {
        FLAG_EVENT = 1
    };
};
struct message_error : public message_result
{
    unsigned int errorcode;
};
namespace CallTable {
typedef std::pair<void*,size_t> pair;
typedef std::variant<message_result::results,pair > CmdResult;
typedef CmdResult (*CallCell)(unsigned int, std::string,Client*);
struct CommandFunction
{
    CallCell cell;
    long long int uuid;
};
class CallTable : public std::vector<CommandFunction>
{
public:
    CommandFunction default_cell;
    CallTable(unsigned int size,CommandFunction _default);
    inline CmdResult call(size_t index,unsigned int arg1,const std::string& arg2,Client* arg3)
    {
        return this->operator[](index).cell(arg1,arg2,arg3);
    }
    ~CallTable();
};
};
#endif
