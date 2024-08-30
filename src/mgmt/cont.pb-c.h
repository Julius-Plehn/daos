/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: cont.proto */

#ifndef PROTOBUF_C_cont_2eproto__INCLUDED
#define PROTOBUF_C_cont_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1003000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1003000 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif


typedef struct _Mgmt__ContSetOwnerReq Mgmt__ContSetOwnerReq;


/* --- enums --- */


/* --- messages --- */

/*
 * ContSetOwnerReq changes the ownership of a container.
 */
struct  _Mgmt__ContSetOwnerReq
{
  ProtobufCMessage base;
  /*
   * DAOS system identifier
   */
  char *sys;
  /*
   * UUID or label of the container
   */
  char *cont_id;
  /*
   * UUID or label of the pool that the container is in
   */
  char *pool_id;
  /*
   * formatted user e.g. "bob@"
   */
  char *owner_user;
  /*
   * formatted group e.g. "builders@"
   */
  char *owner_group;
  /*
   * List of pool service ranks
   */
  size_t n_svc_ranks;
  uint32_t *svc_ranks;
};
#define MGMT__CONT_SET_OWNER_REQ__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&mgmt__cont_set_owner_req__descriptor) \
    , (char *)protobuf_c_empty_string, (char *)protobuf_c_empty_string, (char *)protobuf_c_empty_string, (char *)protobuf_c_empty_string, (char *)protobuf_c_empty_string, 0,NULL }


/* Mgmt__ContSetOwnerReq methods */
void   mgmt__cont_set_owner_req__init
                     (Mgmt__ContSetOwnerReq         *message);
size_t mgmt__cont_set_owner_req__get_packed_size
                     (const Mgmt__ContSetOwnerReq   *message);
size_t mgmt__cont_set_owner_req__pack
                     (const Mgmt__ContSetOwnerReq   *message,
                      uint8_t             *out);
size_t mgmt__cont_set_owner_req__pack_to_buffer
                     (const Mgmt__ContSetOwnerReq   *message,
                      ProtobufCBuffer     *buffer);
Mgmt__ContSetOwnerReq *
       mgmt__cont_set_owner_req__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   mgmt__cont_set_owner_req__free_unpacked
                     (Mgmt__ContSetOwnerReq *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*Mgmt__ContSetOwnerReq_Closure)
                 (const Mgmt__ContSetOwnerReq *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCMessageDescriptor mgmt__cont_set_owner_req__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_cont_2eproto__INCLUDED */
