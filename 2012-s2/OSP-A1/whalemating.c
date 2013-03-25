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
 * Driver code for whale mating problem
 */
#include <types.h>
#include <lib.h>
#include <thread.h>
#include <test.h>
#include <synch.h>

#define NMATING 10
#define QUEUE 1000

// synchprobs variables
static struct semaphore *sem_done;
static struct lock *lk;
static struct cv *cv;

// count variables
volatile unsigned long male_list[QUEUE];
volatile unsigned long female_list[QUEUE];
volatile int male_start ,male_end;
volatile int female_start ,female_end;

static 
void
init_whalemating(void){
    // init all the synchprobs variables
    sem_done = sem_create("sem_done",0);
    if(sem_done == NULL)
        panic("Whale mating: sem_create failed");
    lk = lock_create("lk");
    if(lk == NULL)
        panic("Whale mating: lock_create failed");
    cv  = cv_create("cv");
    if(cv == NULL)
        panic("Whale mating: cv_create failed");
    // set all the count variables to 0
    male_start = 0;
    male_end = 0;
    female_start = 0;
    female_end = 0;
}

static
    void
male(void *p, unsigned long which)
{
    (void)p;
    // male start
    kprintf(">>> male whale #%ld starting\n", which);
    // get the lock 
    lock_acquire(lk);
    // put the male in the male list
    male_list[male_end++] = which;
    if(female_start != female_end)
        // weak up when female show up
        cv_signal(cv,lk);
    // unlock 
    lock_release(lk);
    //put the sem back
    V(sem_done);
}

static
    void
female(void *p, unsigned long which)
{
    (void)p;
    // female start
    kprintf(">>> female whale #%ld starting\n", which);
    // get the lock
    lock_acquire(lk);
    // put the female in the female list
    female_list[female_end++] = which;
    if(male_start != male_end)
        // weak up when male show up
        cv_signal(cv,lk);
    // unlock
    lock_release(lk);
    // put the sem back
    V(sem_done);
}

static
    void
matchmaker(void *p, unsigned long which)
{
    unsigned long male_num;
    unsigned long female_num;
    (void)p;
    // matchmaker start
    kprintf(">>> matchmaker #%ld starting\n", which);
    // get the lock
    lock_acquire(lk);
    // wait the thread when missing male or female
    while((male_start == male_end) ||(female_start == female_end))
        cv_wait(cv,lk);
    // get the current male and female
    male_num = male_list[male_start++];
    female_num = female_list[female_start++];
    // get maker and male whale and female whale together
    kprintf("Maker #%ld are helping Male whale #%ld and Female whale #%ld \n",which,male_num,female_num);
    kprintf("!!! Matting... done\n");
    kprintf("<<< male whale #%ld exit\n", male_num);
    kprintf("<<< female whale #%ld exit\n", female_num);
    kprintf("<<< matchmaker #%ld exit\n", which);
    // unlock
    lock_release(lk);
    // put the sem back
    V(sem_done);
}

static
void
clean_whalemating(void){
    // destroy all the synchprobs variables
    sem_destroy(sem_done);
    lock_destroy(lk);
    cv_destroy(cv);
}


// Change this function as necessary
    int
whalemating(int nargs, char **args)
{

    int i, j, err=0;

    (void)nargs;
    (void)args;
    // start init
    init_whalemating();

    for (i = 0; i < 3; i++) {
        for (j = 0; j < NMATING; j++) {
            switch(i) {
                case 0:
                    err = thread_fork("Male Whale Thread",
                            male, NULL, j, NULL);
                    break;
                case 1:
                    err = thread_fork("Female Whale Thread",
                            female, NULL, j, NULL);
                    break;
                case 2:
                    err = thread_fork("Matchmaker Whale Thread",
                            matchmaker, NULL, j, NULL);
                    break;
            }
            if (err) {
                panic("whalemating: thread_fork failed: %s)\n",
                        strerror(err));
            }
        }
    }
    for (i = 0; i < 3; i++) {
        for (j = 0; j < NMATING; j++) {
            // give every thread a sem when they created
            P(sem_done);
        }
    }
    //destroy all
    clean_whalemating();

    return 0;
}
