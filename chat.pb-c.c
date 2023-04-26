/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: chat.proto */

/* Do not generate deprecated warnings for self */
#ifndef PROTOBUF_C__NO_DEPRECATED
#define PROTOBUF_C__NO_DEPRECATED
#endif

#include "chat.pb-c.h"
void   chat__message__init
                     (Chat__Message         *message)
{
  static const Chat__Message init_value = CHAT__MESSAGE__INIT;
  *message = init_value;
}
size_t chat__message__get_packed_size
                     (const Chat__Message *message)
{
  assert(message->base.descriptor == &chat__message__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t chat__message__pack
                     (const Chat__Message *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &chat__message__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t chat__message__pack_to_buffer
                     (const Chat__Message *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &chat__message__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Chat__Message *
       chat__message__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Chat__Message *)
     protobuf_c_message_unpack (&chat__message__descriptor,
                                allocator, len, data);
}
void   chat__message__free_unpacked
                     (Chat__Message *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &chat__message__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   chat__user_info__init
                     (Chat__UserInfo         *message)
{
  static const Chat__UserInfo init_value = CHAT__USER_INFO__INIT;
  *message = init_value;
}
size_t chat__user_info__get_packed_size
                     (const Chat__UserInfo *message)
{
  assert(message->base.descriptor == &chat__user_info__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t chat__user_info__pack
                     (const Chat__UserInfo *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &chat__user_info__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t chat__user_info__pack_to_buffer
                     (const Chat__UserInfo *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &chat__user_info__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Chat__UserInfo *
       chat__user_info__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Chat__UserInfo *)
     protobuf_c_message_unpack (&chat__user_info__descriptor,
                                allocator, len, data);
}
void   chat__user_info__free_unpacked
                     (Chat__UserInfo *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &chat__user_info__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   chat__user_server_operation__init
                     (Chat__UserServerOperation         *message)
{
  static const Chat__UserServerOperation init_value = CHAT__USER_SERVER_OPERATION__INIT;
  *message = init_value;
}
size_t chat__user_server_operation__get_packed_size
                     (const Chat__UserServerOperation *message)
{
  assert(message->base.descriptor == &chat__user_server_operation__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t chat__user_server_operation__pack
                     (const Chat__UserServerOperation *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &chat__user_server_operation__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t chat__user_server_operation__pack_to_buffer
                     (const Chat__UserServerOperation *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &chat__user_server_operation__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Chat__UserServerOperation *
       chat__user_server_operation__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Chat__UserServerOperation *)
     protobuf_c_message_unpack (&chat__user_server_operation__descriptor,
                                allocator, len, data);
}
void   chat__user_server_operation__free_unpacked
                     (Chat__UserServerOperation *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &chat__user_server_operation__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
static const ProtobufCFieldDescriptor chat__message__field_descriptors[3] =
{
  {
    "receiver",
    1,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Chat__Message, receiver),
    NULL,
    &protobuf_c_empty_string,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "sender",
    2,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Chat__Message, sender),
    NULL,
    &protobuf_c_empty_string,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "message",
    3,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Chat__Message, message),
    NULL,
    &protobuf_c_empty_string,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned chat__message__field_indices_by_name[] = {
  2,   /* field[2] = message */
  0,   /* field[0] = receiver */
  1,   /* field[1] = sender */
};
static const ProtobufCIntRange chat__message__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 3 }
};
const ProtobufCMessageDescriptor chat__message__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "chat.Message",
  "Message",
  "Chat__Message",
  "chat",
  sizeof(Chat__Message),
  3,
  chat__message__field_descriptors,
  chat__message__field_indices_by_name,
  1,  chat__message__number_ranges,
  (ProtobufCMessageInit) chat__message__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor chat__user_info__field_descriptors[3] =
{
  {
    "username",
    1,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Chat__UserInfo, username),
    NULL,
    &protobuf_c_empty_string,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "ip",
    2,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Chat__UserInfo, ip),
    NULL,
    &protobuf_c_empty_string,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "status",
    3,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_INT32,
    0,   /* quantifier_offset */
    offsetof(Chat__UserInfo, status),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned chat__user_info__field_indices_by_name[] = {
  1,   /* field[1] = ip */
  2,   /* field[2] = status */
  0,   /* field[0] = username */
};
static const ProtobufCIntRange chat__user_info__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 3 }
};
const ProtobufCMessageDescriptor chat__user_info__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "chat.UserInfo",
  "UserInfo",
  "Chat__UserInfo",
  "chat",
  sizeof(Chat__UserInfo),
  3,
  chat__user_info__field_descriptors,
  chat__user_info__field_indices_by_name,
  1,  chat__user_info__number_ranges,
  (ProtobufCMessageInit) chat__user_info__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor chat__user_server_operation__field_descriptors[5] =
{
  {
    "r_type",
    1,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_BOOL,
    0,   /* quantifier_offset */
    offsetof(Chat__UserServerOperation, r_type),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "user_name",
    2,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Chat__UserServerOperation, user_name),
    NULL,
    &protobuf_c_empty_string,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "all_users",
    3,
    PROTOBUF_C_LABEL_REPEATED,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(Chat__UserServerOperation, n_all_users),
    offsetof(Chat__UserServerOperation, all_users),
    &chat__user_info__descriptor,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "new_status",
    4,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_INT32,
    0,   /* quantifier_offset */
    offsetof(Chat__UserServerOperation, new_status),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "server_status",
    5,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_INT32,
    0,   /* quantifier_offset */
    offsetof(Chat__UserServerOperation, server_status),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned chat__user_server_operation__field_indices_by_name[] = {
  2,   /* field[2] = all_users */
  3,   /* field[3] = new_status */
  0,   /* field[0] = r_type */
  4,   /* field[4] = server_status */
  1,   /* field[1] = user_name */
};
static const ProtobufCIntRange chat__user_server_operation__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 5 }
};
const ProtobufCMessageDescriptor chat__user_server_operation__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "chat.UserServerOperation",
  "UserServerOperation",
  "Chat__UserServerOperation",
  "chat",
  sizeof(Chat__UserServerOperation),
  5,
  chat__user_server_operation__field_descriptors,
  chat__user_server_operation__field_indices_by_name,
  1,  chat__user_server_operation__number_ranges,
  (ProtobufCMessageInit) chat__user_server_operation__init,
  NULL,NULL,NULL    /* reserved[123] */
};
