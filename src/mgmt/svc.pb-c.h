/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: svc.proto */

#ifndef PROTOBUF_C_svc_2eproto__INCLUDED
#define PROTOBUF_C_svc_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1003000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1003000 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif


typedef struct _Mgmt__DaosResp Mgmt__DaosResp;
typedef struct _Mgmt__GroupUpdateReq Mgmt__GroupUpdateReq;
typedef struct _Mgmt__GroupUpdateReq__Engine Mgmt__GroupUpdateReq__Engine;
typedef struct _Mgmt__GroupUpdateResp Mgmt__GroupUpdateResp;
typedef struct _Mgmt__JoinReq Mgmt__JoinReq;
typedef struct _Mgmt__JoinResp Mgmt__JoinResp;
typedef struct _Mgmt__LeaderQueryReq Mgmt__LeaderQueryReq;
typedef struct _Mgmt__LeaderQueryResp Mgmt__LeaderQueryResp;
typedef struct _Mgmt__GetAttachInfoReq Mgmt__GetAttachInfoReq;
typedef struct _Mgmt__ClientNetHint Mgmt__ClientNetHint;
typedef struct _Mgmt__GetAttachInfoResp Mgmt__GetAttachInfoResp;
typedef struct _Mgmt__GetAttachInfoResp__RankUri Mgmt__GetAttachInfoResp__RankUri;
typedef struct _Mgmt__PrepShutdownReq Mgmt__PrepShutdownReq;
typedef struct _Mgmt__PingRankReq Mgmt__PingRankReq;
typedef struct _Mgmt__SetRankReq Mgmt__SetRankReq;
typedef struct _Mgmt__PoolMonitorReq Mgmt__PoolMonitorReq;


/* --- enums --- */

typedef enum _Mgmt__JoinResp__State {
  /*
   * Server in the system.
   */
  MGMT__JOIN_RESP__STATE__IN = 0,
  /*
   * Server excluded from the system.
   */
  MGMT__JOIN_RESP__STATE__OUT = 1,
  /*
   * Server should start in checker mode.
   */
  MGMT__JOIN_RESP__STATE__CHECK = 2
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(MGMT__JOIN_RESP__STATE)
} Mgmt__JoinResp__State;

/* --- messages --- */

/*
 * Generic response just containing DER from I/O Engine.
 */
struct  _Mgmt__DaosResp
{
  ProtobufCMessage base;
  /*
   * DAOS error code.
   */
  int32_t status;
};
#define MGMT__DAOS_RESP__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&mgmt__daos_resp__descriptor) \
    , 0 }


struct  _Mgmt__GroupUpdateReq__Engine
{
  ProtobufCMessage base;
  uint32_t rank;
  /*
   * primary URI is the only one group update is concerned with
   */
  char *uri;
  uint64_t incarnation;
};
#define MGMT__GROUP_UPDATE_REQ__ENGINE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&mgmt__group_update_req__engine__descriptor) \
    , 0, (char *)protobuf_c_empty_string, 0 }


struct  _Mgmt__GroupUpdateReq
{
  ProtobufCMessage base;
  uint32_t map_version;
  size_t n_engines;
  Mgmt__GroupUpdateReq__Engine **engines;
};
#define MGMT__GROUP_UPDATE_REQ__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&mgmt__group_update_req__descriptor) \
    , 0, 0,NULL }


struct  _Mgmt__GroupUpdateResp
{
  ProtobufCMessage base;
  int32_t status;
};
#define MGMT__GROUP_UPDATE_RESP__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&mgmt__group_update_resp__descriptor) \
    , 0 }


struct  _Mgmt__JoinReq
{
  ProtobufCMessage base;
  /*
   * DAOS system name.
   */
  char *sys;
  /*
   * Server UUID.
   */
  char *uuid;
  /*
   * Server rank desired, if not MAX_UINT32.
   */
  uint32_t rank;
  /*
   * Server CaRT primary provider URI (i.e., for context 0).
   */
  char *uri;
  /*
   * Server CaRT context count.
   */
  uint32_t nctxs;
  /*
   * Server management address.
   */
  char *addr;
  /*
   * Fault domain for this instance's server
   */
  char *srvfaultdomain;
  /*
   * Instance index on server node.
   */
  uint32_t idx;
  /*
   * rank incarnation
   */
  uint64_t incarnation;
  /*
   * URIs for any secondary providers
   */
  size_t n_secondary_uris;
  char **secondary_uris;
  /*
   * CaRT context count for each secondary provider
   */
  size_t n_secondary_nctxs;
  uint32_t *secondary_nctxs;
  /*
   * rank started in check mode
   */
  protobuf_c_boolean check_mode;
};
#define MGMT__JOIN_REQ__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&mgmt__join_req__descriptor) \
    , (char *)protobuf_c_empty_string, (char *)protobuf_c_empty_string, 0, (char *)protobuf_c_empty_string, 0, (char *)protobuf_c_empty_string, (char *)protobuf_c_empty_string, 0, 0, 0,NULL, 0,NULL, 0 }


struct  _Mgmt__JoinResp
{
  ProtobufCMessage base;
  /*
   * DAOS error code
   */
  int32_t status;
  /*
   * Server rank assigned.
   */
  uint32_t rank;
  /*
   * Server state in the system map.
   */
  Mgmt__JoinResp__State state;
  /*
   * Fault domain for the instance
   */
  char *faultdomain;
  /*
   * Join processed locally.
   */
  protobuf_c_boolean localjoin;
  /*
   * Join processed in this version of the system map.
   */
  uint32_t map_version;
};
#define MGMT__JOIN_RESP__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&mgmt__join_resp__descriptor) \
    , 0, 0, MGMT__JOIN_RESP__STATE__IN, (char *)protobuf_c_empty_string, 0, 0 }


struct  _Mgmt__LeaderQueryReq
{
  ProtobufCMessage base;
  /*
   * System name.
   */
  char *sys;
  /*
   * hostset to query
   */
  char *hosts;
};
#define MGMT__LEADER_QUERY_REQ__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&mgmt__leader_query_req__descriptor) \
    , (char *)protobuf_c_empty_string, (char *)protobuf_c_empty_string }


struct  _Mgmt__LeaderQueryResp
{
  ProtobufCMessage base;
  char *current_leader;
  size_t n_replicas;
  char **replicas;
  size_t n_downreplicas;
  char **downreplicas;
};
#define MGMT__LEADER_QUERY_RESP__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&mgmt__leader_query_resp__descriptor) \
    , (char *)protobuf_c_empty_string, 0,NULL, 0,NULL }


struct  _Mgmt__GetAttachInfoReq
{
  ProtobufCMessage base;
  /*
   * System name. For daos_agent only.
   */
  char *sys;
  /*
   * Return Rank URIs for all ranks.
   */
  protobuf_c_boolean all_ranks;
  /*
   * Preferred fabric interface.
   */
  char *interface;
  /*
   * Preferred fabric domain.
   */
  char *domain;
};
#define MGMT__GET_ATTACH_INFO_REQ__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&mgmt__get_attach_info_req__descriptor) \
    , (char *)protobuf_c_empty_string, 0, (char *)protobuf_c_empty_string, (char *)protobuf_c_empty_string }


struct  _Mgmt__ClientNetHint
{
  ProtobufCMessage base;
  /*
   * CaRT provider
   */
  char *provider;
  /*
   * CaRT D_INTERFACE
   */
  char *interface;
  /*
   * CaRT D_DOMAIN for given D_INTERFACE
   */
  char *domain;
  /*
   * CaRT CRT_CTX_SHARE_ADDR
   */
  uint32_t crt_ctx_share_addr;
  /*
   * CaRT CRT_TIMEOUT
   */
  uint32_t crt_timeout;
  /*
   * ARP protocol hardware identifier of the
   */
  uint32_t net_dev_class;
  /*
   * I/O Engine network interface
   */
  /*
   * Server SRX setting (-1, 0, 1; -1 == unset)
   */
  int32_t srv_srx_set;
  /*
   * Client-side environment variables to set
   */
  size_t n_env_vars;
  char **env_vars;
  /*
   * Provider index - anything > 0 is a secondary provider
   */
  uint32_t provider_idx;
};
#define MGMT__CLIENT_NET_HINT__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&mgmt__client_net_hint__descriptor) \
    , (char *)protobuf_c_empty_string, (char *)protobuf_c_empty_string, (char *)protobuf_c_empty_string, 0, 0, 0, 0, 0,NULL, 0 }


struct  _Mgmt__GetAttachInfoResp__RankUri
{
  ProtobufCMessage base;
  uint32_t rank;
  char *uri;
  uint32_t provider_idx;
  uint32_t num_ctxs;
};
#define MGMT__GET_ATTACH_INFO_RESP__RANK_URI__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&mgmt__get_attach_info_resp__rank_uri__descriptor) \
    , 0, (char *)protobuf_c_empty_string, 0, 0 }


struct  _Mgmt__GetAttachInfoResp
{
  ProtobufCMessage base;
  /*
   * DAOS error code
   */
  int32_t status;
  /*
   * Rank URIs for the primary provider
   */
  size_t n_rank_uris;
  Mgmt__GetAttachInfoResp__RankUri **rank_uris;
  /*
   * These CaRT settings are shared with the
   * libdaos client to aid in CaRT initialization.
   */
  /*
   * Ranks local to MS replicas
   */
  size_t n_ms_ranks;
  uint32_t *ms_ranks;
  /*
   * Primary provider hint
   */
  Mgmt__ClientNetHint *client_net_hint;
  /*
   * Version of the system database.
   */
  uint64_t data_version;
  /*
   * Name of the DAOS system
   */
  char *sys;
  /*
   * Rank URIs for additional providers
   */
  size_t n_secondary_rank_uris;
  Mgmt__GetAttachInfoResp__RankUri **secondary_rank_uris;
  /*
   * Hints for additional providers
   */
  size_t n_secondary_client_net_hints;
  Mgmt__ClientNetHint **secondary_client_net_hints;
};
#define MGMT__GET_ATTACH_INFO_RESP__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&mgmt__get_attach_info_resp__descriptor) \
    , 0, 0,NULL, 0,NULL, NULL, 0, (char *)protobuf_c_empty_string, 0,NULL, 0,NULL }


struct  _Mgmt__PrepShutdownReq
{
  ProtobufCMessage base;
  /*
   * DAOS I/O Engine unique identifier.
   */
  uint32_t rank;
};
#define MGMT__PREP_SHUTDOWN_REQ__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&mgmt__prep_shutdown_req__descriptor) \
    , 0 }


struct  _Mgmt__PingRankReq
{
  ProtobufCMessage base;
  /*
   * DAOS I/O Engine unique identifier.
   */
  uint32_t rank;
};
#define MGMT__PING_RANK_REQ__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&mgmt__ping_rank_req__descriptor) \
    , 0 }


struct  _Mgmt__SetRankReq
{
  ProtobufCMessage base;
  /*
   * DAOS I/O Engine unique identifier.
   */
  uint32_t rank;
  /*
   * System map version in which the rank joined the system.
   */
  uint32_t map_version;
};
#define MGMT__SET_RANK_REQ__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&mgmt__set_rank_req__descriptor) \
    , 0, 0 }


struct  _Mgmt__PoolMonitorReq
{
  ProtobufCMessage base;
  /*
   * DAOS system identifier
   */
  char *sys;
  /*
   * Pool UUID associated with the Pool Handle
   */
  char *pooluuid;
  /*
   * Pool Handle UUID for the connection
   */
  char *poolhandleuuid;
  /*
   * Job ID to associate instance with.
   */
  char *jobid;
};
#define MGMT__POOL_MONITOR_REQ__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&mgmt__pool_monitor_req__descriptor) \
    , (char *)protobuf_c_empty_string, (char *)protobuf_c_empty_string, (char *)protobuf_c_empty_string, (char *)protobuf_c_empty_string }


/* Mgmt__DaosResp methods */
void   mgmt__daos_resp__init
                     (Mgmt__DaosResp         *message);
size_t mgmt__daos_resp__get_packed_size
                     (const Mgmt__DaosResp   *message);
size_t mgmt__daos_resp__pack
                     (const Mgmt__DaosResp   *message,
                      uint8_t             *out);
size_t mgmt__daos_resp__pack_to_buffer
                     (const Mgmt__DaosResp   *message,
                      ProtobufCBuffer     *buffer);
Mgmt__DaosResp *
       mgmt__daos_resp__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   mgmt__daos_resp__free_unpacked
                     (Mgmt__DaosResp *message,
                      ProtobufCAllocator *allocator);
/* Mgmt__GroupUpdateReq__Engine methods */
void   mgmt__group_update_req__engine__init
                     (Mgmt__GroupUpdateReq__Engine         *message);
/* Mgmt__GroupUpdateReq methods */
void   mgmt__group_update_req__init
                     (Mgmt__GroupUpdateReq         *message);
size_t mgmt__group_update_req__get_packed_size
                     (const Mgmt__GroupUpdateReq   *message);
size_t mgmt__group_update_req__pack
                     (const Mgmt__GroupUpdateReq   *message,
                      uint8_t             *out);
size_t mgmt__group_update_req__pack_to_buffer
                     (const Mgmt__GroupUpdateReq   *message,
                      ProtobufCBuffer     *buffer);
Mgmt__GroupUpdateReq *
       mgmt__group_update_req__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   mgmt__group_update_req__free_unpacked
                     (Mgmt__GroupUpdateReq *message,
                      ProtobufCAllocator *allocator);
/* Mgmt__GroupUpdateResp methods */
void   mgmt__group_update_resp__init
                     (Mgmt__GroupUpdateResp         *message);
size_t mgmt__group_update_resp__get_packed_size
                     (const Mgmt__GroupUpdateResp   *message);
size_t mgmt__group_update_resp__pack
                     (const Mgmt__GroupUpdateResp   *message,
                      uint8_t             *out);
size_t mgmt__group_update_resp__pack_to_buffer
                     (const Mgmt__GroupUpdateResp   *message,
                      ProtobufCBuffer     *buffer);
Mgmt__GroupUpdateResp *
       mgmt__group_update_resp__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   mgmt__group_update_resp__free_unpacked
                     (Mgmt__GroupUpdateResp *message,
                      ProtobufCAllocator *allocator);
/* Mgmt__JoinReq methods */
void   mgmt__join_req__init
                     (Mgmt__JoinReq         *message);
size_t mgmt__join_req__get_packed_size
                     (const Mgmt__JoinReq   *message);
size_t mgmt__join_req__pack
                     (const Mgmt__JoinReq   *message,
                      uint8_t             *out);
size_t mgmt__join_req__pack_to_buffer
                     (const Mgmt__JoinReq   *message,
                      ProtobufCBuffer     *buffer);
Mgmt__JoinReq *
       mgmt__join_req__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   mgmt__join_req__free_unpacked
                     (Mgmt__JoinReq *message,
                      ProtobufCAllocator *allocator);
/* Mgmt__JoinResp methods */
void   mgmt__join_resp__init
                     (Mgmt__JoinResp         *message);
size_t mgmt__join_resp__get_packed_size
                     (const Mgmt__JoinResp   *message);
size_t mgmt__join_resp__pack
                     (const Mgmt__JoinResp   *message,
                      uint8_t             *out);
size_t mgmt__join_resp__pack_to_buffer
                     (const Mgmt__JoinResp   *message,
                      ProtobufCBuffer     *buffer);
Mgmt__JoinResp *
       mgmt__join_resp__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   mgmt__join_resp__free_unpacked
                     (Mgmt__JoinResp *message,
                      ProtobufCAllocator *allocator);
/* Mgmt__LeaderQueryReq methods */
void   mgmt__leader_query_req__init
                     (Mgmt__LeaderQueryReq         *message);
size_t mgmt__leader_query_req__get_packed_size
                     (const Mgmt__LeaderQueryReq   *message);
size_t mgmt__leader_query_req__pack
                     (const Mgmt__LeaderQueryReq   *message,
                      uint8_t             *out);
size_t mgmt__leader_query_req__pack_to_buffer
                     (const Mgmt__LeaderQueryReq   *message,
                      ProtobufCBuffer     *buffer);
Mgmt__LeaderQueryReq *
       mgmt__leader_query_req__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   mgmt__leader_query_req__free_unpacked
                     (Mgmt__LeaderQueryReq *message,
                      ProtobufCAllocator *allocator);
/* Mgmt__LeaderQueryResp methods */
void   mgmt__leader_query_resp__init
                     (Mgmt__LeaderQueryResp         *message);
size_t mgmt__leader_query_resp__get_packed_size
                     (const Mgmt__LeaderQueryResp   *message);
size_t mgmt__leader_query_resp__pack
                     (const Mgmt__LeaderQueryResp   *message,
                      uint8_t             *out);
size_t mgmt__leader_query_resp__pack_to_buffer
                     (const Mgmt__LeaderQueryResp   *message,
                      ProtobufCBuffer     *buffer);
Mgmt__LeaderQueryResp *
       mgmt__leader_query_resp__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   mgmt__leader_query_resp__free_unpacked
                     (Mgmt__LeaderQueryResp *message,
                      ProtobufCAllocator *allocator);
/* Mgmt__GetAttachInfoReq methods */
void   mgmt__get_attach_info_req__init
                     (Mgmt__GetAttachInfoReq         *message);
size_t mgmt__get_attach_info_req__get_packed_size
                     (const Mgmt__GetAttachInfoReq   *message);
size_t mgmt__get_attach_info_req__pack
                     (const Mgmt__GetAttachInfoReq   *message,
                      uint8_t             *out);
size_t mgmt__get_attach_info_req__pack_to_buffer
                     (const Mgmt__GetAttachInfoReq   *message,
                      ProtobufCBuffer     *buffer);
Mgmt__GetAttachInfoReq *
       mgmt__get_attach_info_req__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   mgmt__get_attach_info_req__free_unpacked
                     (Mgmt__GetAttachInfoReq *message,
                      ProtobufCAllocator *allocator);
/* Mgmt__ClientNetHint methods */
void   mgmt__client_net_hint__init
                     (Mgmt__ClientNetHint         *message);
size_t mgmt__client_net_hint__get_packed_size
                     (const Mgmt__ClientNetHint   *message);
size_t mgmt__client_net_hint__pack
                     (const Mgmt__ClientNetHint   *message,
                      uint8_t             *out);
size_t mgmt__client_net_hint__pack_to_buffer
                     (const Mgmt__ClientNetHint   *message,
                      ProtobufCBuffer     *buffer);
Mgmt__ClientNetHint *
       mgmt__client_net_hint__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   mgmt__client_net_hint__free_unpacked
                     (Mgmt__ClientNetHint *message,
                      ProtobufCAllocator *allocator);
/* Mgmt__GetAttachInfoResp__RankUri methods */
void   mgmt__get_attach_info_resp__rank_uri__init
                     (Mgmt__GetAttachInfoResp__RankUri         *message);
/* Mgmt__GetAttachInfoResp methods */
void   mgmt__get_attach_info_resp__init
                     (Mgmt__GetAttachInfoResp         *message);
size_t mgmt__get_attach_info_resp__get_packed_size
                     (const Mgmt__GetAttachInfoResp   *message);
size_t mgmt__get_attach_info_resp__pack
                     (const Mgmt__GetAttachInfoResp   *message,
                      uint8_t             *out);
size_t mgmt__get_attach_info_resp__pack_to_buffer
                     (const Mgmt__GetAttachInfoResp   *message,
                      ProtobufCBuffer     *buffer);
Mgmt__GetAttachInfoResp *
       mgmt__get_attach_info_resp__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   mgmt__get_attach_info_resp__free_unpacked
                     (Mgmt__GetAttachInfoResp *message,
                      ProtobufCAllocator *allocator);
/* Mgmt__PrepShutdownReq methods */
void   mgmt__prep_shutdown_req__init
                     (Mgmt__PrepShutdownReq         *message);
size_t mgmt__prep_shutdown_req__get_packed_size
                     (const Mgmt__PrepShutdownReq   *message);
size_t mgmt__prep_shutdown_req__pack
                     (const Mgmt__PrepShutdownReq   *message,
                      uint8_t             *out);
size_t mgmt__prep_shutdown_req__pack_to_buffer
                     (const Mgmt__PrepShutdownReq   *message,
                      ProtobufCBuffer     *buffer);
Mgmt__PrepShutdownReq *
       mgmt__prep_shutdown_req__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   mgmt__prep_shutdown_req__free_unpacked
                     (Mgmt__PrepShutdownReq *message,
                      ProtobufCAllocator *allocator);
/* Mgmt__PingRankReq methods */
void   mgmt__ping_rank_req__init
                     (Mgmt__PingRankReq         *message);
size_t mgmt__ping_rank_req__get_packed_size
                     (const Mgmt__PingRankReq   *message);
size_t mgmt__ping_rank_req__pack
                     (const Mgmt__PingRankReq   *message,
                      uint8_t             *out);
size_t mgmt__ping_rank_req__pack_to_buffer
                     (const Mgmt__PingRankReq   *message,
                      ProtobufCBuffer     *buffer);
Mgmt__PingRankReq *
       mgmt__ping_rank_req__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   mgmt__ping_rank_req__free_unpacked
                     (Mgmt__PingRankReq *message,
                      ProtobufCAllocator *allocator);
/* Mgmt__SetRankReq methods */
void   mgmt__set_rank_req__init
                     (Mgmt__SetRankReq         *message);
size_t mgmt__set_rank_req__get_packed_size
                     (const Mgmt__SetRankReq   *message);
size_t mgmt__set_rank_req__pack
                     (const Mgmt__SetRankReq   *message,
                      uint8_t             *out);
size_t mgmt__set_rank_req__pack_to_buffer
                     (const Mgmt__SetRankReq   *message,
                      ProtobufCBuffer     *buffer);
Mgmt__SetRankReq *
       mgmt__set_rank_req__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   mgmt__set_rank_req__free_unpacked
                     (Mgmt__SetRankReq *message,
                      ProtobufCAllocator *allocator);
/* Mgmt__PoolMonitorReq methods */
void   mgmt__pool_monitor_req__init
                     (Mgmt__PoolMonitorReq         *message);
size_t mgmt__pool_monitor_req__get_packed_size
                     (const Mgmt__PoolMonitorReq   *message);
size_t mgmt__pool_monitor_req__pack
                     (const Mgmt__PoolMonitorReq   *message,
                      uint8_t             *out);
size_t mgmt__pool_monitor_req__pack_to_buffer
                     (const Mgmt__PoolMonitorReq   *message,
                      ProtobufCBuffer     *buffer);
Mgmt__PoolMonitorReq *
       mgmt__pool_monitor_req__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   mgmt__pool_monitor_req__free_unpacked
                     (Mgmt__PoolMonitorReq *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*Mgmt__DaosResp_Closure)
                 (const Mgmt__DaosResp *message,
                  void *closure_data);
typedef void (*Mgmt__GroupUpdateReq__Engine_Closure)
                 (const Mgmt__GroupUpdateReq__Engine *message,
                  void *closure_data);
typedef void (*Mgmt__GroupUpdateReq_Closure)
                 (const Mgmt__GroupUpdateReq *message,
                  void *closure_data);
typedef void (*Mgmt__GroupUpdateResp_Closure)
                 (const Mgmt__GroupUpdateResp *message,
                  void *closure_data);
typedef void (*Mgmt__JoinReq_Closure)
                 (const Mgmt__JoinReq *message,
                  void *closure_data);
typedef void (*Mgmt__JoinResp_Closure)
                 (const Mgmt__JoinResp *message,
                  void *closure_data);
typedef void (*Mgmt__LeaderQueryReq_Closure)
                 (const Mgmt__LeaderQueryReq *message,
                  void *closure_data);
typedef void (*Mgmt__LeaderQueryResp_Closure)
                 (const Mgmt__LeaderQueryResp *message,
                  void *closure_data);
typedef void (*Mgmt__GetAttachInfoReq_Closure)
                 (const Mgmt__GetAttachInfoReq *message,
                  void *closure_data);
typedef void (*Mgmt__ClientNetHint_Closure)
                 (const Mgmt__ClientNetHint *message,
                  void *closure_data);
typedef void (*Mgmt__GetAttachInfoResp__RankUri_Closure)
                 (const Mgmt__GetAttachInfoResp__RankUri *message,
                  void *closure_data);
typedef void (*Mgmt__GetAttachInfoResp_Closure)
                 (const Mgmt__GetAttachInfoResp *message,
                  void *closure_data);
typedef void (*Mgmt__PrepShutdownReq_Closure)
                 (const Mgmt__PrepShutdownReq *message,
                  void *closure_data);
typedef void (*Mgmt__PingRankReq_Closure)
                 (const Mgmt__PingRankReq *message,
                  void *closure_data);
typedef void (*Mgmt__SetRankReq_Closure)
                 (const Mgmt__SetRankReq *message,
                  void *closure_data);
typedef void (*Mgmt__PoolMonitorReq_Closure)
                 (const Mgmt__PoolMonitorReq *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCMessageDescriptor mgmt__daos_resp__descriptor;
extern const ProtobufCMessageDescriptor mgmt__group_update_req__descriptor;
extern const ProtobufCMessageDescriptor mgmt__group_update_req__engine__descriptor;
extern const ProtobufCMessageDescriptor mgmt__group_update_resp__descriptor;
extern const ProtobufCMessageDescriptor mgmt__join_req__descriptor;
extern const ProtobufCMessageDescriptor mgmt__join_resp__descriptor;
extern const ProtobufCEnumDescriptor    mgmt__join_resp__state__descriptor;
extern const ProtobufCMessageDescriptor mgmt__leader_query_req__descriptor;
extern const ProtobufCMessageDescriptor mgmt__leader_query_resp__descriptor;
extern const ProtobufCMessageDescriptor mgmt__get_attach_info_req__descriptor;
extern const ProtobufCMessageDescriptor mgmt__client_net_hint__descriptor;
extern const ProtobufCMessageDescriptor mgmt__get_attach_info_resp__descriptor;
extern const ProtobufCMessageDescriptor mgmt__get_attach_info_resp__rank_uri__descriptor;
extern const ProtobufCMessageDescriptor mgmt__prep_shutdown_req__descriptor;
extern const ProtobufCMessageDescriptor mgmt__ping_rank_req__descriptor;
extern const ProtobufCMessageDescriptor mgmt__set_rank_req__descriptor;
extern const ProtobufCMessageDescriptor mgmt__pool_monitor_req__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_svc_2eproto__INCLUDED */
