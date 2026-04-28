/* This file is derived from source code for the Nachos
   instructional operating system.  The Nachos copyright notice
   is reproduced in full below. */

/* Copyright (c) 1992-1996 The Regents of the University of California.
   All rights reserved.

   Permission to use, copy, modify, and distribute this software
   and its documentation for any purpose, without fee, and
   without written agreement is hereby granted, provided that the
   above copyright notice and the following two paragraphs appear
   in all copies of this software.

   IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO
   ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR
   CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OF THIS SOFTWARE
   AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF CALIFORNIA
   HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

   THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
   PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS ON AN "AS IS"
   BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION TO
   PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR
   MODIFICATIONS.
   */

#include "threads/synch.h"
#include <stdio.h>
#include <string.h>
#include <threads/thread.h> 
#include "threads/interrupt.h"
#include "threads/thread.h"

/* One semaphore in a list. */
struct semaphore_elem {
	struct list_elem elem;              /* List element. */
	struct semaphore semaphore;         /* This semaphore. */
};


/* 헬퍼 함수 */
bool get_max_priority (const struct list_elem *a,
			  const struct list_elem *b,
			  void *aux UNUSED) {
	const struct thread *ta = list_entry (a, struct thread, elem);
	const struct thread *tb = list_entry (b, struct thread, elem);
	return ta->priority < tb->priority; 
}

static struct thread * 
pop_highest_priority_waiter (struct semaphore *sema) {
	ASSERT(sema != NULL); 
	ASSERT(!list_empty (&sema->waiters)); 

	// merge sort 정렬 방식이고 시간 복잡도는 O(n log n)이다 
    // list_sort (&sema->waiters, compare_priority, NULL);

	struct list_elem *max_elem; 

	// 전체 정렬보다 가장 큰거를 선택하는게 O(n) 측면에서 이득이기 때문에 list_max 사용
	max_elem = list_max(&sema->waiters, get_max_priority, NULL); 

	// 앞뒤 포인터만 바꾸는 O(1) 함수 
	list_remove(max_elem);

	return list_entry(max_elem, struct thread, elem); 
}

static int
get_cond_waiter_priority(struct semaphore_elem *se)
{
    struct list_elem *e;
    struct thread *t;

    ASSERT(se != NULL);
    ASSERT(!list_empty(&se->semaphore.waiters));

    e = list_max(&se->semaphore.waiters, get_max_priority, NULL);
    t = list_entry(e, struct thread, elem);

    return t->priority;
}

static struct list_elem *
find_highest_priority_cond_waiter(struct condition *cond)
{
    struct list_elem *curr;
    struct list_elem *max_elem;

    ASSERT(cond != NULL);
    ASSERT(!list_empty(&cond->waiters));

    max_elem = list_begin(&cond->waiters);

    for (curr = list_next(max_elem);
         curr != list_end(&cond->waiters);
         curr = list_next(curr)) {

		// 지금 보고 있는 waiter
        struct semaphore_elem *cur_se =
            list_entry(curr, struct semaphore_elem, elem);

		// 지금까지 찾은 최고 priority waiter
        struct semaphore_elem *max_se =
            list_entry(max_elem, struct semaphore_elem, elem);

        if (get_cond_waiter_priority(cur_se) >
            get_cond_waiter_priority(max_se)) {
            max_elem = curr;
        }
    }

    return max_elem;
}

static void 
update_lock_holder_priority(struct lock *lock, struct thread *cur) {
    ASSERT(lock != NULL);
    ASSERT(cur != NULL);

	struct list_elem *e;
    int max_priority;

	// 현재 lock holder의 priority를 기준값으로 잡습니다
    max_priority = lock->holder->priority;

	// 이미 이 lock을 기다리고 있는 스레드들을 순회하면서
    // 가장 높은 priority를 찾습니다
    for (e = list_begin(&lock->semaphore.waiters);
         e != list_end(&lock->semaphore.waiters);
         e = list_next(e)) {
        struct thread *t = list_entry(e, struct thread, elem);

        if (t->priority > max_priority) {
            max_priority = t->priority;
        }
    }

	// 현재 lock_acquire()를 호출한 cur는 아직 semaphore.waiters에
    // 들어가기 전일 수 있으므로 따로 비교합니다
    if (cur->priority > max_priority) {
        max_priority = cur->priority;
    }


	// lock holder가 donation 받은 priority 중 최댓값으로 실행되도록 갱신합니다
    lock->holder->priority = max_priority;
}

/* Initializes semaphore SEMA to VALUE.  A semaphore is a
   nonnegative integer along with two atomic operators for
   manipulating it:

   - down or "P": wait for the value to become positive, then
   decrement it.

   - up or "V": increment the value (and wake up one waiting
   thread, if any). */
void
sema_init (struct semaphore *sema, unsigned value) {
	ASSERT (sema != NULL);

	sema->value = value;
	list_init (&sema->waiters);
}

/* sema_up 함수의 역할은 다음과 같습니다 
   1. 락을 획득하려고 시도하는 함수입니다 */
void
sema_down (struct semaphore *sema) {
	enum intr_level old_level;

	ASSERT (sema != NULL);
	ASSERT (!intr_context ());

	old_level = intr_disable ();

	// 현재 스레드가 락을 얻으려고 할때 sema->value == 0이여서 락 획득이 불가능하면 
	// thread_block()을 이용해서 BLOCKED상태로 바꾸고 컨텍스트 스위칭으로 다른 스레드로 전환한다 
	while (sema->value == 0) {
		list_push_back (&sema->waiters, &thread_current ()->elem);
		thread_block ();
	}
	sema->value--;
	intr_set_level (old_level);
}

/* Down or "P" operation on a semaphore, but only if the
   semaphore is not already 0.  Returns true if the semaphore is
   decremented, false otherwise.

   This function may be called from an interrupt handler. */
bool
sema_try_down (struct semaphore *sema) {
	enum intr_level old_level;
	bool success;

	ASSERT (sema != NULL);

	old_level = intr_disable ();
	if (sema->value > 0)
	{
		sema->value--;
		success = true;
	}
	else
		success = false;
	intr_set_level (old_level);

	return success;
}

/* sema_up 함수는 2가지 역할을 합니다 
   1. 락을 획득할 수 있는 기회를 하나 늘립니다 
   2. 획득할 수 있는 락이 하나 늘었음으로 락을 기다리고 있던 스레드를 하나 깨웁니다 */
void
sema_up (struct semaphore *sema) {
	// 만약에 sema_up 함수로 NULL을 보낸다면 
	// sema->value++ 같은 곳에서 존재하지 않는 접근이기 때문에 오류가 생깁니다 
	ASSERT (sema != NULL);

    enum intr_level old_level;
    struct thread *sema_waiter_thread = NULL;

    old_level = intr_disable ();

    if (!list_empty (&sema->waiters)) {	
		// 세마포어를 기다리는 스레드들 중 가장 높은 priority를 가진 스레드를 깨웁니다 
        sema_waiter_thread = pop_highest_priority_waiter(sema); 
        list_remove(sema_waiter_thread);
		// 선택한 스레드를 깨워 READY 상태로 전환합니다 
		// 그리고 ready_list에 넣어서 스케줄링 후보로 만듭니다 
        thread_unblock (sema_waiter_thread);
    }

	// lock release로 인해 락을 획득할 수 있는 기회가 하나 생겼으므로 value 증가
    sema->value++;

    intr_set_level (old_level);

	// 만약 깨어난 스레드의 priority가 현재 실행중인 스레드보다 priority가 높으면 컨텍스트 스위칭 
    if (sema_waiter_thread != NULL && thread_get_priority () < sema_waiter_thread->priority) {
        thread_yield ();
    }
}

static void sema_test_helper (void *sema_);

/* Self-test for semaphores that makes control "ping-pong"
   between a pair of threads.  Insert calls to printf() to see
   what's going on. */
void
sema_self_test (void) {
	struct semaphore sema[2];
	int i;

	printf ("Testing semaphores...");
	sema_init (&sema[0], 0);
	sema_init (&sema[1], 0);
	thread_create ("sema-test", PRI_DEFAULT, sema_test_helper, &sema);
	for (i = 0; i < 10; i++)
	{
		sema_up (&sema[0]);
		sema_down (&sema[1]);
	}
	printf ("done.\n");
}

/* Thread function used by sema_self_test(). */
static void
sema_test_helper (void *sema_) {
	struct semaphore *sema = sema_;
	int i;

	for (i = 0; i < 10; i++)
	{
		sema_down (&sema[0]);
		sema_up (&sema[1]);
	}
}

/* Initializes LOCK.  A lock can be held by at most a single
   thread at any given time.  Our locks are not "recursive", that
   is, it is an error for the thread currently holding a lock to
   try to acquire that lock.

   A lock is a specialization of a semaphore with an initial
   value of 1.  The difference between a lock and such a
   semaphore is twofold.  First, a semaphore can have a value
   greater than 1, but a lock can only be owned by a single
   thread at a time.  Second, a semaphore does not have an owner,
   meaning that one thread can "down" the semaphore and then
   another one "up" it, but with a lock the same thread must both
   acquire and release it.  When these restrictions prove
   onerous, it's a good sign that a semaphore should be used,
   instead of a lock. */
void
lock_init (struct lock *lock) {
	ASSERT (lock != NULL);

	lock->holder = NULL;
	sema_init (&lock->semaphore, 1);
}

/* lock_acquire 함수의 역할: 
   1. 현재 스레드가 lock을 얻으려고 시도하는 함수 
   2. 이미 다른 스레드가 들고 있으면 기다리게 하는 함수 
   3. donations에 priority를 추가하는 함수 */
void
lock_acquire (struct lock *lock) {

    ASSERT (lock != NULL);
    ASSERT (!intr_context ());
    ASSERT (!lock_held_by_current_thread (lock));

	struct thread *cur = thread_current ();

	if (lock->holder != NULL) {
		update_lock_holder_priority(lock, cur);
	}

    list_push_back(&lock->holder->donations, &cur->donation_elem); 
    sema_down (&lock->semaphore);
    lock->holder = cur;
}

/* Tries to acquires LOCK and returns true if successful or false
   on failure.  The lock must not already be held by the current
   thread.

   This function will not sleep, so it may be called within an
   interrupt handler. */
bool
lock_try_acquire (struct lock *lock) {
	bool success;

	ASSERT (lock != NULL);
	ASSERT (!lock_held_by_current_thread (lock));

	success = sema_try_down (&lock->semaphore);
	if (success)
		lock->holder = thread_current ();
	return success;
}

/* lock_release 함수가 하는 역할: 
   1. 현재 스레드가 들고 있던 lock의 소유권을 내려놓는다 
   2. 그 lock을 기다리던 다른 스레드가 있으면 깨워준다 */
void
lock_release (struct lock *lock) {
	ASSERT (lock != NULL);
	ASSERT (lock_held_by_current_thread (lock));
    
	struct thread *t = NULL; 

    if (!list_empty(&lock->semaphore.waiters)) {
        struct list_elem *max = list_max(&lock->semaphore.waiters, compare_priority, NULL); 
        t = list_entry(max, struct thread, elem);
        list_remove(&t->donation_elem); 
    }

    // lock을 놓으면서, donation을 받아서 올라갔을 수 있는 priority를
	// 스레드가 처음 가지고 있던 original_priority로 되돌립니다
	lock->holder->priority = lock->holder->original_priority;    

    if (!list_empty(&lock->holder->donations)) {
        struct list_elem *d_max = list_max(&t->donations, compare_priority, NULL); 
        struct thread *td = list_entry(d_max, struct thread, donation_elem); 
    
        lock->holder->priority = td->original_priority; 
    }
	// 이 lock은 더 이상 현재 스레드가 소유하지 않으므로 holder을 비웁니다 
	lock->holder = NULL;
    
	// lock 내부 semaphore를 up 해서,
    // 이 lock을 기다리던 스레드 하나를 READY 상태로 전환할 기회를 줍니다.
	sema_up (&lock->semaphore);
}

/* Returns true if the current thread holds LOCK, false
   otherwise.  (Note that testing whether some other thread holds
   a lock would be racy.) */
bool
lock_held_by_current_thread (const struct lock *lock) {
	ASSERT (lock != NULL);

	return lock->holder == thread_current ();
} 

/* Initializes condition variable COND.  A condition variable
   allows one piece of code to signal a condition and cooperating
   code to receive the signal and act upon it. */
void
cond_init (struct condition *cond) {
	ASSERT (cond != NULL);

	list_init (&cond->waiters);
}

/* Atomically releases LOCK and waits for COND to be signaled by
   some other piece of code.  After COND is signaled, LOCK is
   reacquired before returning.  LOCK must be held before calling
   this function.

   The monitor implemented by this function is "Mesa" style, not
   "Hoare" style, that is, sending and receiving a signal are not
   an atomic operation.  Thus, typically the caller must recheck
   the condition after the wait completes and, if necessary, wait
   again.

   A given condition variable is associated with only a single
   lock, but one lock may be associated with any number of
   condition variables.  That is, there is a one-to-many mapping
   from locks to condition variables.

   This function may sleep, so it must not be called within an
   interrupt handler.  This function may be called with
   interrupts disabled, but interrupts will be turned back on if
   we need to sleep. */
void
cond_wait (struct condition *cond, struct lock *lock) {
	struct semaphore_elem waiter;

	ASSERT (cond != NULL);
	ASSERT (lock != NULL);
	ASSERT (!intr_context ());
	ASSERT (lock_held_by_current_thread (lock));

	sema_init (&waiter.semaphore, 0);

	list_push_back (&cond->waiters, &waiter.elem);
	lock_release (lock);
	sema_down (&waiter.semaphore);
	lock_acquire (lock);
}

/* If any threads are waiting on COND (protected by LOCK), then
   this function signals one of them to wake up from its wait.
   LOCK must be held before calling this function.

   An interrupt handler cannot acquire a lock, so it does not
   make sense to try to signal a condition variable within an
   interrupt handler. */
void
cond_signal (struct condition *cond, struct lock *lock UNUSED) {
	ASSERT (cond != NULL);
	ASSERT (lock != NULL);
	ASSERT (!intr_context ());
	ASSERT (lock_held_by_current_thread (lock));
		
	if (!list_empty(&cond->waiters)) {
		// condition variable을 기다리는 waiter들 중
    	// 가장 높은 priority를 가진 스레드가 들어 있는 semaphore_elem을 찾습니다.
		struct list_elem *max_elem = find_highest_priority_cond_waiter(cond);
		struct semaphore_elem *se;

		// 선택된 waiter를 cond->waiters 리스트에서  제거합니다.
		list_remove(max_elem);
		se = list_entry(max_elem, struct semaphore_elem, elem);

		// 선택된 waiter의 semaphore를 up 해서,
    	// 그 semaphore에서 잠들어 있던 스레드를 깨웁니다.
		sema_up(&se->semaphore);
	}

}

/* Wakes up all threads, if any, waiting on COND (protected by
   LOCK).  LOCK must be held before calling this function.

   An interrupt handler cannot acquire a lock, so it does not
   make sense to try to signal a condition variable within an
   interrupt handler. */
void
cond_broadcast (struct condition *cond, struct lock *lock) {
	ASSERT (cond != NULL);
	ASSERT (lock != NULL);

	while (!list_empty (&cond->waiters))
		cond_signal (cond, lock);
}
