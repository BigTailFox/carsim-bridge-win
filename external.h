// Prototypes for functions implemented in external.c
#include <windows.h>

#include "lcm.h"
#include "lcm_coretypes.h"

#include "carsim_Control.h"
#include "carsim_State.h"
#include "carsim_RoadQuery.h"
#include "carsim_RoadContact.h"

#define LCM_URL "udpm://239.255.76.67:7667?ttl=1"
#define CHANNEL_CONTROL "CARSIM_CONTROL"
#define CHANNEL_CONTACT "CARSIM_CONTACT"
#define CAHNNEL_STATE "CARSIM_STATE"
#define CHANNEL_QUERY "CARSIM_QUERY"

void external_calc(vs_real t, vs_ext_loc where);
void external_echo(vs_ext_loc where);
vs_bool external_scan(char *, char *);
void external_setdef(void);

void roadcontact_handler(const lcm_recv_buf_t *rbuf, const char *channel,
                         const carsim_RoadContact *msg, void *user);
void control_handler(const lcm_recv_buf_t *rbuf, const char *channel,
                     const carsim_Control *msg, void *user);

DWORD WINAPI sub_loop(LPVOID para);
DWORD WINAPI pub_roadquery_loop(LPVOID para);
DWORD WINAPI pub_state_loop(LPVOID para);