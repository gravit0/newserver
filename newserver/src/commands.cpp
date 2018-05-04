/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   commands.cpp
 * Author: gravit
 *
 * Created on 2 ноября 2017 г., 13:21
 */
#include "main.hpp"
#include "EventManager.hpp"
#include <sstream>
#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>
#include <fcntl.h>
#include <dlfcn.h>
#include <chrono>
#include <thread>

CallTable::CmdResult cmd_unknown(unsigned int, std::string,Client*)
{
    
    return CallTable::CmdResult(message_result::ERROR_CMDINCORRECT);
}
CallTable::CmdResult cmd_stop(unsigned int, std::string,Client*)
{
    gsock->stop();
    return CallTable::CmdResult(message_result::OK);
}
CallTable::CmdResult cmd_su(unsigned int, std::string,Client* client)
{
    event.sendEvent(1,"jhjhjhhj");
    client->adminlevel = 100;
    return CallTable::CmdResult(message_result::OK);
}
CallTable::CmdResult cmd_listen(unsigned int, std::string,Client* client)
{
    EventListener ev;
    ev.client = client;
    ev.event = 1;
    event.addListener(ev);
    return CallTable::CmdResult(message_result::OK);
}
struct _module_version
{
    unsigned int version;
    unsigned int api;
};
struct _module_api
{
    CallTable::CallCell* calltable;
};
_module_api module_api;
bool module_api_init = false;
CallTable::CmdResult cmd_loadmodule(unsigned int, std::string file,Client*)
{
    if(!module_api_init)
    {
        module_api.calltable = gsock->table.table;
        module_api_init = true;
    }
    void* fd = dlopen(file.c_str(), RTLD_LAZY);
    if(fd == NULL) {
        perror("[MODULE ERROR]");
        return CallTable::CmdResult(message_result::ERROR_FILENOTFOUND);
    }
    _module_version (*sp_module_init)(_module_api);
    void (*sp_module_main)();
    sp_module_init = (_module_version (*)(_module_api))dlsym(fd,"sp_module_init");
    _module_version v = sp_module_init(module_api);
    if(v.api != 1)
    {
        std::cerr << "[MODULE] Unsupported API version: " << v.api << std::endl;
        return CallTable::CmdResult(message_result::ERROR_FILENOTFOUND);
    }
    sp_module_main = (void (*)())dlsym(fd,"sp_module_call_main");
    sp_module_main();
    return CallTable::CmdResult(message_result::OK);
};
CallTable::CmdResult cmd_setconfig(unsigned int, std::string cfgdir,Client*)
{
    if (config_parse(cfgdir) == 1)
    {
        return CallTable::CmdResult(message_result::ERROR_FILENOTFOUND);
    }
    return CallTable::CmdResult(message_result::OK);
}
void push_cmds()
{
    gsock->table.add(&cmd_unknown); // 0
    gsock->table.add(&cmd_stop); // 1
    gsock->table.add(&cmd_setconfig); // 2
    gsock->table.add(&cmd_loadmodule);  // 3
    gsock->table.add(&cmd_su);  // 3
    gsock->table.add(&cmd_listen);  // 3
}
