#ifndef MAPPER_H_
#define MAPPER_H_

#include <librtnl/netns.h>

/*
 * Automatically map linux network routes to VPP.
 * Each namespace is associated with an individual fib.
 *
 * One linux interface can only be mapped to a single VPP
 * interface, but one VPP interface can be mapped to
 * multiple linux interfaces.
 * A mapped VPP interface must not have any configured fib.
 */

int mapper_add_ns(char *nsname, u32 v4fib_index, u32 v6fib_index, u32 *nsindex);
int mapper_del_ns(u32 nsindex);
int mapper_add_del(u32 nsindex, int linux_ifindex, u32 sw_if_index, int del);

#endif /* MAPPER_H_ */
