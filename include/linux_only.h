#pragma once

#ifndef WIN32

inline void get_future_time(timespec& ts, int millisecond) 
{
	clock_gettime(CLOCK_REALTIME, &ts);

	const int million = 1000000;
	const int billion = 1000000000;
	ts.tv_sec += millisecond/1000;
	ts.tv_nsec += (millisecond % 1000) * million; 
	if (ts.tv_nsec >= billion) {
		ts.tv_sec++;
		ts.tv_nsec -= billion;
	}
}

inline bool createProcess(const char* cmd, const char* dir, PROCESS_INFORMATION* pi)
{
	pid_t pid = fork();
	if (pid < 0) {
		return false;
	} else if (pid == 0) {
		chdir(dir);   
		execl("/bin/sh", "sh", "-c", cmd, (char *)0);   
		exit(127);
	} else if (pid > 0) {
		pi->hProcess = pid;
		pi->hThread = NULL;
		pi->dwProcessId = pid;
		pi->dwThreadId = pid;  
	}
	return true;
}

inline BOOL makeSocketNonBlocking(SOCKET s, bool block = false)  
{
/*	#ifdef WIN32
		u_long val = block ? 0 : 1;
		return ioctlsocket(s, FIONBIO, &val) == 0;
	#else
*/		int flags = fcntl(s, F_GETFL, 0);
		flags = block ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK);
		return fcntl(s, F_SETFL, flags) != -1;
//	#endif
}  

#endif
  
