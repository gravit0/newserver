/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   EventManager.cpp
 * Author: gravit
 * 
 * Created on 31 октября 2017 г., 23:13
 */

#include "EventManager.hpp"
#include <memory.h>
EventManager event;

EventManager::EventManager() {
}

EventManager::~EventManager() {
}
void EventManager::addListener(EventListener ev) {
    ev.client->isListener = true;
    list.push_back(ev);
}

void EventManager::sendEvent(int event, std::string data) {
    const char* c_str = data.c_str();
    int size = data.size();
    message_result* result;
    if(size > 0)
        result = (message_result*) new char[sizeof(message_result) + size];
    else
        result = new message_result();
    result->version = 0;
    result->code = event;
    result->flag = message_result::FLAG_EVENT;
    result->size = data.size();
    if(size > 0)
    memcpy((char*)result + sizeof(struct message_result),c_str,size);
    for (auto &i : list) {
        if (i.event & event) {
            i.client->write((char*) result,(size_t) sizeof(message_result) + size);
        }
    }
    if(size > 0)
        delete[] (char*)result;
    else
        delete result;
}

void EventManager::removeListener(Client* client) {
    list.remove_if([&client](EventListener ev) { if(ev.client == client) return true; });
}
