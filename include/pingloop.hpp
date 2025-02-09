#pragma once

#include "main.hpp"
#include <string.h>

#include "SerialTransfer.h"


void init_ping(STRUCT_Message& ping_msg);


int ping_func(SerialTransfer& s, const STRUCT_Message& ping_msg);

int ping_res_chk(const STRUCT_Message& msg);

int ping(SerialTransfer& s, STRUCT_Message& msg, const STRUCT_Message& ping_msg);
