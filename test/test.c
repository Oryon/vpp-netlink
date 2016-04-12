#include <librtnl/netns.h>

#include <vnet/plugin/plugin.h>

u32 handles[10];

static void
test_notify(void *obj, netns_type_t type, u32 flags, uword opaque) {
  u32 index = (u32) opaque;
  const char *action = (flags & NETNS_F_ADD)?"add":(flags & NETNS_F_DEL)?"del":"mod";

  switch (type) {
    case NETNS_TYPE_ADDR:
      clib_warning("%d: addr %s %U", index, action, format_ns_addr, (ns_addr_t *)obj);
      break;
    case NETNS_TYPE_ROUTE:
      clib_warning("%d: route %s %U", index, action, format_ns_route, (ns_route_t *)obj);
      break;
    case NETNS_TYPE_LINK:
      clib_warning("%d:link %s %U", index, action, format_ns_link, (ns_link_t *)obj);
      break;
    case NETNS_TYPE_NEIGH:
      clib_warning("%d: neigh %s %U", index, action, format_ns_neigh, (ns_neigh_t *)obj);
      break;
  }
}

static clib_error_t *
test_enable_command_fn (vlib_main_t * vm,
                        unformat_input_t * input,
                        vlib_cli_command_t * cmd)
{
  char *nsname = 0;
  u32 index;
  if (!unformat(input, "%s", &nsname)) {
    return clib_error_return(0, "unknown input `%U'",
                             format_unformat_error, input);
  }
  if (!unformat(input, "%d", &index)) {
    return clib_error_return(0, "unknown input `%U'",
                             format_unformat_error, input);
  }

  if (!strcmp(nsname, "default"))
    nsname[0] = 0;

  netns_sub_t sub;
  sub.notify = test_notify;
  sub.opaque = index;
  handles[index] = netns_open(nsname, &sub);
  if (handles[index] == ~0) {
    return clib_error_create("Could not open netns with name %s", nsname);
  }
  return 0;
}

static clib_error_t *
test_disable_command_fn (vlib_main_t * vm,
                         unformat_input_t * input,
                         vlib_cli_command_t * cmd)
{
  u32 index;
  if (!unformat(input, "%d", &index)) {
    return clib_error_return(0, "unknown input `%U'",
                             format_unformat_error, input);
  }

  netns_close(handles[index]);
  return 0;
}

VLIB_CLI_COMMAND (rtnl_enable_command, static) = {
    .path = "test netns enable",
    .short_help = "test netns enable [<ns-name>|default] <index>",
    .function = test_enable_command_fn,
};

VLIB_CLI_COMMAND (rtnl_disable_command, static) = {
    .path = "test netns disable",
    .short_help = "test rtnl disable <index>",
    .function = test_disable_command_fn,
};

clib_error_t *
vlib_plugin_register (vlib_main_t * vm, vnet_plugin_handoff_t * h,
                      int from_early_init)
{
  clib_warning("Loaded module");
  return 0;
}

