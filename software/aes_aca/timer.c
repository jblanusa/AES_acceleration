/*
 * timer.c
 *
 *  Created on: Mar 24, 2012
 *      Author: xavier.jimenez@epfl.ch
 */
#include <altera_avalon_timer_regs.h>
#include <system.h>

// user timer
#define TIMER_USER_BASE TIMER_0_BASE
#define TIMER_USER_FREQ TIMER_0_FREQ
// global system timer
#define TIMER_GLOBAL_BASE TIMER_1_BASE
#define TIMER_GLOBAL_FREQ TIMER_1_FREQ

// internal functions
void __timer_reset(int base) {
	IOWR_ALTERA_AVALON_TIMER_CONTROL(base,
			ALTERA_AVALON_TIMER_CONTROL_STOP_MSK);
	IOWR_ALTERA_AVALON_TIMER_STATUS(base, 0);
}
int __timer_elapsed(int base, int freq) {
	// read the period
	unsigned int period = IORD_ALTERA_AVALON_TIMER_PERIODH(base)<<16;
	period |= IORD_ALTERA_AVALON_TIMER_PERIODL(base);
	// take a snapshot
	IOWR_ALTERA_AVALON_TIMER_SNAPL(base, 0);
	// read it
	unsigned int snap = IORD_ALTERA_AVALON_TIMER_SNAPH(base)<<16;
	snap |= IORD_ALTERA_AVALON_TIMER_SNAPL(base);
	return (period-snap)/(freq/1000);
}
void __timer_start(int base, unsigned int period) {
	__timer_reset(base);
	IOWR_ALTERA_AVALON_TIMER_PERIODL(base, period & 0xFFFF);
	IOWR_ALTERA_AVALON_TIMER_PERIODH(base, period >> 16);
	IOWR_ALTERA_AVALON_TIMER_CONTROL(base,
			ALTERA_AVALON_TIMER_CONTROL_START_MSK);
}

/**
 * Start the global timer (max period).
 */
void timer_start_global() {
	__timer_start(TIMER_GLOBAL_BASE, 0xFFFFFFFF);
}

/**
 * Returns the time elapsed since the global timer was initialized in ms.
 */
int timer_elapsed_global() {
	return __timer_elapsed(TIMER_GLOBAL_BASE, TIMER_GLOBAL_FREQ);
}

/**
 * stops the timer and reset TO bit
 */
void timer_reset() {
	__timer_reset(TIMER_USER_BASE);
}

/**
 * initializes the user timer to run for "ms" ms.
 */
void timer_start(int ms) {
	unsigned int period = (TIMER_USER_FREQ / 1000) * ms - 1;
	__timer_start(TIMER_USER_BASE, period);
}

/**
 * Reads timeout bit.
 * True if the timer reached 0.
 */
int timer_read_to() {
	return IORD_ALTERA_AVALON_TIMER_STATUS(TIMER_USER_BASE) & ALTERA_AVALON_TIMER_STATUS_TO_MSK;
}

/**
 * Wait for "ms" ms.
 */
void timer_wait(int ms) {
	timer_start(ms);
	while(timer_read_to()==0);
}

/**
 * Wait for the timer to time-out.
 */
void timer_wait_for_to() {
	while(timer_read_to()==0);
}

/**
 * Reads the elapsed time in ms since the timer was started.
 */
int timer_elapsed() {
	return __timer_elapsed(TIMER_USER_BASE, TIMER_USER_BASE);
}

