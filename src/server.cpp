#include "headers/server.hpp"
#include "ruby/ruby.h"
#include <iostream>

SockServer::SockServer() {}

SockServer::~SockServer() {}

void SockServer::send(char* val){
    ruby_script("server.rb");
}
