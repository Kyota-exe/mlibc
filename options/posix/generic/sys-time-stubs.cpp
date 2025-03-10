
#include <errno.h>
#include <sys/time.h>
#include <time.h>

#include <bits/ensure.h>
#include <mlibc/debug.hpp>
#include <mlibc/posix-sysdeps.hpp>

int gettimeofday(struct timeval *__restrict result, void *__restrict unused) {
	(void)unused; // Linux just ignores gettimeofday().

	if(result) {
		long nanos;
		if(int e = mlibc::sys_clock_get(CLOCK_REALTIME, &result->tv_sec, &nanos); e) {
			errno = e;
			return -1;
		}
		result->tv_usec = nanos / 1000;
	}
	return 0;
}

void timeradd(const struct timeval *a, const struct timeval *b, struct timeval *res) {
	res->tv_sec = a->tv_sec + b->tv_sec;
	res->tv_usec = a->tv_usec + b->tv_usec;
	while(res->tv_usec > 999999) {
		res->tv_usec -= 1000000;
		res->tv_sec += 1;
	}
}

void timersub(const struct timeval *a, const struct timeval *b, struct timeval *res) {
	res->tv_sec = a->tv_sec - b->tv_sec;
	res->tv_usec = a->tv_usec - b->tv_usec;
	while(res->tv_usec < 0) {
		res->tv_usec += 1000000;
		res->tv_sec -= 1;
	}
}

void timerclear(struct timeval *tvp) {
	tvp->tv_sec = 0;
	tvp->tv_usec = 0;
}

int timerisset(struct timeval *tvp) {
	if(tvp->tv_sec != 0 || tvp->tv_usec != 0) {
		return 1;
	}
	return 0;
}

int getitimer(int which, struct itimerval *curr_value) {
	if(!mlibc::sys_getitimer) {
		MLIBC_MISSING_SYSDEP();
		errno = ENOSYS;
		return -1;
	}
	if(int e = mlibc::sys_getitimer(which, curr_value); e) {
		errno = e;
		return -1;
	}
	return 0;
}

int setitimer(int which, const struct itimerval *new_value, struct itimerval *old_value) {
	if(!mlibc::sys_setitimer) {
		MLIBC_MISSING_SYSDEP();
		errno = ENOSYS;
		return -1;
	}
	if(int e = mlibc::sys_setitimer(which, new_value, old_value); e) {
		errno = e;
		return -1;
	}
	return 0;
}
