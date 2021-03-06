#ifndef CONFIG_H
#define CONFIG_H
#include <string>

#include "Logger.hpp"
#define COPY_BUF_SIZE 8192
#define SOCK_BUF_SIZE 1024
#define SYMLINK_BUF_SIZE 256
struct _SecurityInfo {
    int pid;
    int ppid;
    int uid, euid;
    int gid, egid;
};

struct Configuration {
    std::string host = "127.0.0.1";
    std::string pidfile = "/run/ns.pid";
    _SecurityInfo security;
    int epoll_timeout = 1000;
    int max_connect = 10;
    int port = 8264;
    Logger::level log_level;

    enum DAEMON_TYPE {
        CFG_DAEMON_SIMPLE,
        CFG_DAEMON_FORKING
    };

    enum SETUID_MODES {
        CFG_SETUID_NONE,
        CFG_SETUID_SUID,
        CFG_SETUID_CHANGE,
        CFG_SETUID_USERMODE
    };
    SETUID_MODES setuid_mode = CFG_SETUID_NONE;
    DAEMON_TYPE daemon_type = CFG_DAEMON_SIMPLE;
    bool isSetHost = false;
    bool isSetPort = false;
    bool isDaemon = true;
    bool isAllowWarning = true;
    bool isIgnoreLowException = false;
    bool isPrintVersion = false;
};
extern Configuration cfg;
#endif
