/*
 * timer.h
 *
 * Created: 29/05/2012 09:50:47 p.m.
 *  Author: Luis Pichio
 */ 


#ifndef SIMPLE_TIMER_H_
#define SIMPLE_TIMER_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "esp_timer.h"

#define SIMPLE_TIMER_MS						simple_timer_get_ms()
#define SIMPLE_TIMER_SEC					simple_timer_get_seconds()

struct s_simple_timer{
	uint64_t start;
	uint32_t delay;
	bool enabled;
};

typedef struct s_simple_timer t_simple_timer;

extern uint64_t simple_timer_get_ms(void);
extern uint64_t simple_timer_get_seconds(void);
extern void simple_timer_start(t_simple_timer *timer, uint32_t delay);
extern void simple_timer_reset(t_simple_timer *timer);
extern void simple_timer_enable(t_simple_timer *timer);
extern void simple_timer_disable(t_simple_timer *timer);
extern bool simple_timer_timeout(t_simple_timer *timer);
extern bool simple_timer_timeout_and_reset(t_simple_timer *timer);
extern bool simple_timer_timeout_and_disable(t_simple_timer *timer);
extern uint64_t simple_timer_get(t_simple_timer *timer);
extern uint64_t simple_timer_get_and_reset(t_simple_timer *timer);

#endif /* SIMPLE_TIMER_H_ */
