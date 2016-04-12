
#ifndef RTNL_H_
#define RTNL_H_

#include <linux/netlink.h>
#include <vppinfra/clib.h>

typedef enum {
  RTNL_ERR_UNKNOWN,
} rtnl_error_t;

#define RTNL_NETNS_NAMELEN 31

/*
 * RTNL stream implements an RTNL overlay
 * for receiving continuous updates for a given namespace.
 * When the stream is initially opened, dump requests are sent
 * in order to retrieve the original state.
 * handle_error is called any time synchronization cannot be
 * achieved. When called, state is reset to its original state and
 * new dump requests are sent.
 */

typedef struct rtnl_stream_s {
  char name[RTNL_NETNS_NAMELEN + 1];
  void (*recv_message)(struct nlmsghdr *hdr, uword opaque);
  void (*error)(rtnl_error_t err, uword opaque);
  uword opaque;
} rtnl_stream_t;

u32 rtnl_stream_open(rtnl_stream_t *template);
void rtnl_stream_close(u32 handle);

#endif
