/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Sock.hpp
 * Author: gravit
 *
 * Created on 28 сентября 2017 г., 16:45
 */

#ifndef SOCK_HPP
#define SOCK_HPP
#include "config.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <exception>
#include <boost/noncopyable.hpp>
#include <sys/epoll.h>
#include <memory>
#include "call_table.hpp"
class Sock;
class Client : public boost::noncopyable {
protected:
    int sock;
    Client(int sock);
    friend Sock;
    char buf[SOCK_BUF_SIZE];
    unsigned int bytes;
public:
    typedef std::shared_ptr<Client> ptr;
    bool isListener = false;
    bool isAutoClosable = false;
    int write(std::string str);
    int write(std::pair<void*,size_t> data);
    int send_ok();
    int send_error(unsigned int errorcode);
    int read();
    virtual ~Client();
};
#include "call_table.hpp"
//ВАЖНО
namespace cmds
{
enum : unsigned char
{
    stop = 1,
    setconfig = 2,
    loadmodule = 3,
    MAX_COMMANDS = 4
};
}
namespace flags
{
    enum : unsigned short{
        multiparams = 1 >> 0,
        old_command = 1 >> 1
    };
}
namespace cmdflags{
}

struct message_head
{
    unsigned char version;
    unsigned char cmd;
    unsigned short flag;
    unsigned int cmdflags;
    unsigned int size;
};
//////
class Sock : public boost::noncopyable {
private:
    struct sockaddr_in srvr_name;
    int sock_;
    const char* filename_c;
    bool loopEnable;
    int epollsock;
    int max_connect;
    epoll_event* events;
public:
    enum class multithread_loop
    {
        MASTER,
        SLAVE
    };
    CallTable table;
    Sock(std::string host,int port, int max_connect);
    void loop();
    void loop_impl(multithread_loop l);
    int exec(char* data, unsigned int size,Client* t);
    int deattach();
    void stop();
    int wait(int timeout);
    virtual ~Sock();
private:

};

class socket_exception : public std::exception {
public:

    enum Errors {
        SocketError,
        BindError,
        AcceptError,
        RecvError
    };
    Errors thiserr;
    socket_exception(Errors err);
    virtual const char* what() const noexcept;
};
#endif /* SOCK_HPP */

