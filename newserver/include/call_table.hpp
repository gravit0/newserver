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
class CallTable
{
private:
   CallTable( const CallTable& ) = delete;
   void operator=( const CallTable& ) = delete;
public:
    typedef std::pair<void*,size_t> pair;
    typedef std::variant<message_result::results,pair > CmdResult;
    typedef CmdResult (*CallCell)(unsigned int, std::string,Client*);
    CallCell default_cell;
    CallCell* table;
    unsigned int size;
    unsigned int autoincrement;
    CallTable(unsigned int size,CallCell _default);
    unsigned int add(CallCell c);
    inline CmdResult call(unsigned int index,unsigned int arg1, std::string arg2,Client* arg3)
    {
        return table[index](arg1,arg2,arg3);
    }
    bool realloc(unsigned int newsize);
    ~CallTable();
};
#endif
