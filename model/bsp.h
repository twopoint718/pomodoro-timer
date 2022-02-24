/*.$file${.::bsp.h} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
/*
* Model: pomodoro-timer.qm
* File:  ${.::bsp.h}
*
* This code has been generated by QM 5.1.1 <www.state-machine.com/qm/>.
* DO NOT EDIT THIS FILE MANUALLY. All your changes will be lost.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
* or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
* for more details.
*/
/*.$endhead${.::bsp.h} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
#ifndef BSP_H
#define BSP_H

/* a very simple Board Support Package (BSP) -------------------------------*/
enum { BSP_TICKS_PER_SEC = 100 }; /* number of clock ticks in a second */
void BSP_init(void);
void BSP_ledOff(void);
void BSP_ledOn(void);

/* define the event signals used in the application ------------------------*/
enum BlinkySignals {
    TIMEOUT_SIG = Q_USER_SIG, /* offset the first signal by Q_USER_SIG */
    MAX_SIG /* keep last (the number of signals) */
};

/* active object(s) used in this application -------------------------------*/
extern QActive * const AO_Blinky; /* opaque pointer to the Blinky AO */
/*.$declare${AOs::Pomodoro_ctor} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
/*.${AOs::Pomodoro_ctor} ...................................................*/
void Pomodoro_ctor(void);
/*.$enddecl${AOs::Pomodoro_ctor} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

#endif /* BSP_H */