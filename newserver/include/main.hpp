#ifndef MAIN_H
#define MAIN_H
#include <vector>
#include <string>
#include <mutex>
#include <list>
#include <map>
#include "config.hpp"
#include "Logger.hpp"
#include "Sock.hpp"
#include <boost/noncopyable.hpp>
std::vector<std::string_view> split(std::string_view cmd, const char splitchar);
extern void cmd_exec(message_head* head, std::string cmd, Client* sock);
extern void push_cmds();
extern Sock* gsock;
extern int config_parse(const std::string& filename);
int main(int argc, char** argv);
#endif
