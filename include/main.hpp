#pragma once

#ifdef arm180nanoatmega328new
char TYPE_CODE[] = "a1nn";
#endif


typedef struct __attribute__((packed)){
  int32_t  msg_type;
  char     data[4];
} STRUCT_Message;
