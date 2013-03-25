/*
 * Copyright (c) 2001, 2002, 2009
 *	The President and Fellows of Harvard College.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE UNIVERSITY AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE UNIVERSITY OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * Driver code for bathroom problem
 */
#include <types.h>
#include <lib.h>
#include <thread.h>
#include <test.h>
#include <synch.h>
#include <clock.h>

#define NPEOPLE 20
#define MAX_NUMBER 3

// synchprobs variables
static struct semaphore *sem_done;
static struct semaphore *sem_max;
static struct lock *lk;
static struct cv *cv;

// count variables
volatile int boy_showering,girl_showering;

static
void
init_bathroom(void){
    // init all the synchprobs variables
    sem_done = sem_create("sem_done",0);
    if(sem_done == NULL)
        panic("Bathroom: sem_create failed");
    sem_max = sem_create("sem_max",MAX_NUMBER);
    if(sem_max == NULL)
        panic("Bathroom: sem_create failed");
    lk = lock_create("lk");
    if(lk == NULL)
        panic("Whale mating: lock_create failed");
    cv  = cv_create("cv");
    if(cv == NULL)
        panic("Whale mating: cv_create failed");
    // set all the count variables to 0
    boy_showering = 0;
    girl_showering = 0;
}

static
void
clean_bathroom(void){
    // destroy all the synchprobs variables
    sem_destroy(sem_done);
    sem_destroy(sem_max);
    lock_destroy(lk);
    cv_destroy(cv);
}

static
    void
shower()
{
    // The thread enjoys a refreshing shower!
    clocksleep(1);
}

static
    void
boy(void *p, unsigned long which)
{
    (void)p;
    // boy start
    kprintf("boy #%ld starting\n", which);
    // get a lock for atomic action
    lock_acquire(lk);
    while(true){
        // if nobody in bathrooom
        if((boy_showering == 0) &&(girl_showering == 0)){
            P(sem_max);
            boy_showering++;
            break;
        }
        else{
            // if boy are showering and the bathroom are not full
            if((boy_showering > 0)&&(boy_showering < 3)){
                P(sem_max);
                boy_showering++;
                break;
            }
            else
                // bothroom are full or girls are using, should waiting
                cv_wait(cv,lk);
        }
    }
    // release the lock
    lock_release(lk);

    // enter room
    kprintf("+++++++boy #%ld ENTER bathroom\n", which);
    // enjoy the shower
    shower();
    // leave room
    kprintf("<<<<<<<boy #%ld LEAVE bathroom\n", which);

    // get a lock for atomic action
    lock_acquire(lk);
    // change the count and put the sem back when thread leave
    boy_showering--;
    V(sem_max);
    // last one to leave, weak all the people waiting outside
    if(boy_showering == 0)
        cv_broadcast(cv,lk);
    // release the lock
    lock_release(lk);
    // put the sem back
    V(sem_done);
}

static
    void
girl(void *p, unsigned long which)
{
    (void)p;
    // girl start
    kprintf("girl #%ld starting\n", which);
    // get a lock for atomic action
    lock_acquire(lk);
    while(true){
        // if nobody in bathrooom
        if((boy_showering == 0) &&(girl_showering == 0)){
            P(sem_max);
            girl_showering++;
            break;
        }
        else{
            // if girl are showering and the bathroom are not full
            if((girl_showering > 0)&&(girl_showering < 3)){
                P(sem_max);
                girl_showering++;
                break;
            }
            else
                // bothroom are full or girls are using, should waiting
                cv_wait(cv,lk);
        }
    }
    // release the lock
    lock_release(lk);

    // enter room
    kprintf("+++++++girl #%ld ENTER bathroom\n", which);
    // enjoy the shower
    shower();
    // leave room
    kprintf("<<<<<<<girl #%ld LEAVE bathroom\n", which);

    // get a lock for atomic action
    lock_acquire(lk);
    // change the count and put the sem back when thread leave
    girl_showering--;
    V(sem_max);
    // last one to leave, weak all the people waiting outside
    if(girl_showering == 0)
        cv_broadcast(cv,lk);
    // release the lock
    lock_release(lk);
    // put the sem back
    V(sem_done);
}

// Change this function as necessary
    int
bathroom(int nargs, char **args)
{

    int i, err=0;

    (void)nargs;
    (void)args;
    // start init
    init_bathroom();

    for (i = 0; i < NPEOPLE; i++) {
        switch(i % 2) {
            case 0:
                err = thread_fork("Boy Thread",
                        boy, NULL, i, NULL);
                break;
            case 1:
                err = thread_fork("Girl Thread",
                        girl, NULL, i, NULL);
                break;
        }
        if (err) {
            panic("bathroom: thread_fork failed: %s)\n",
                    strerror(err));
        }
    }
    for (i = 0; i< NPEOPLE;i++){
        // give every thread a sem when they created
        P(sem_done);
    }
    //destroy all
    clean_bathroom();

    return 0;
}
