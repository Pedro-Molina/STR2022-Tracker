/*
 * timer.c
 *
 * Created: 11/06/2020 02:54 a.m.
 *  Author: Luis Pichio | luispichio@gmail.com | luispichio@sistemasindustriales.com
 */ 

#include "simple_timer.h"

//-----------------------------------------------------------------------------
uint64_t simple_timer_get_ms(void){
	return esp_timer_get_time() / 1000;
}
//-----------------------------------------------------------------------------
uint64_t simple_timer_get_seconds(void){
	return esp_timer_get_time() / 1000000;
}
//-----------------------------------------------------------------------------
void simple_timer_reset(t_simple_timer *timer){
	timer->start = simple_timer_get_ms();
}
//-----------------------------------------------------------------------------
void simple_timer_enable(t_simple_timer *timer){
	timer->enabled = true;
}
//-----------------------------------------------------------------------------
void simple_timer_disable(t_simple_timer *timer){
	timer->enabled = false;
}
//-----------------------------------------------------------------------------
void simple_timer_start(t_simple_timer *timer, uint32_t delay){
	timer->delay = delay;
	simple_timer_enable(timer);
	simple_timer_reset(timer);
}
//-----------------------------------------------------------------------------
bool simple_timer_timeout(t_simple_timer *timer){
	return timer->enabled && ((simple_timer_get_ms() - timer->start) > timer->delay);
}
//-----------------------------------------------------------------------------
bool simple_timer_timeout_and_reset(t_simple_timer *timer){
	if (simple_timer_timeout(timer)){
		simple_timer_reset(timer);
		return true;
	} else
		return false;
}
//-----------------------------------------------------------------------------
bool simple_timer_timeout_and_disable(t_simple_timer *timer){
	if (simple_timer_timeout(timer)){
		simple_timer_disable(timer);
		return true;
	} else
		return false;
}
//-----------------------------------------------------------------------------
uint64_t simple_timer_get(t_simple_timer *timer){
	return simple_timer_get_ms() - timer->start;
}
//-----------------------------------------------------------------------------
uint64_t simple_timer_get_and_reset(t_simple_timer *timer){
	unsigned long result = simple_timer_get_ms() - timer->start;
	simple_timer_reset(timer);
	return result;
}
//-----------------------------------------------------------------------------
