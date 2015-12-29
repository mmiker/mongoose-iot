#ifndef CLUBBY_PROTO_H
#define CLUBBY_PROTO_H

#include "mongoose.h"
#include "common/ubjserializer.h"

#ifndef DISABLE_C_CLUBBY

typedef struct ub_ctx clubby_ctx_t;

enum clubby_event_type {
  CLUBBY_NET_CONNECT /* net_connect in `clubby_event` struct */,
  CLUBBY_CONNECT /* no params */,
  CLUBBY_FRAME /* frame */,
  CLUBBY_DISCONNECT /* no params */,
  CLUBBY_REQUEST /* request */,
  CLUBBY_RESPONSE /* response */
};

struct clubby_event {
  enum clubby_event_type ev;
  union {
    struct {
      int success;
    } net_connect;
    struct {
      struct mg_str data;
    } frame;
    struct {
      struct json_token *resp_body;
      /* TODO(alashkin): change id type to int64_t */
      int32_t id;
      int status;
      struct json_token *status_msg;
      struct json_token *resp;
    } response;
    struct {
      struct json_token *req;
    } request;
  };
};

typedef void (*clubby_callback)(struct clubby_event *evt);

void clubby_proto_send_cmd(struct ub_ctx *ctx, const char *dst, ub_val_t cmds);
void clubby_proto_send_resp(const char *dst, int64_t id, int status,
                            const char *status_msg);

void clubby_proto_init(clubby_callback cb);
int clubby_proto_connect(struct mg_mgr *mgr);
void clubby_proto_disconnect();

/* Utility */
int64_t clubby_proto_get_new_id();

#endif /* DISABLE_C_CLUBBY */

#endif /* CLUBBY_PROTO_H */