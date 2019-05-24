#ifndef	LIBCHEN_TIMEFACE_H_
#define LIBCHEN_TIMEFACE_H_
#include <ctime>
#include <iostream>
#include <string.h>
#include "TimeSegment.h"

#ifndef WIN32
	#include "Linux_Common.h"     
#endif

#ifndef __NOWINDOWS_H_
#ifdef WIN32
#define _WINSOCKAPI_   /* Prevent inclusion of winsock.h in windows.h */
#include <windows.h>
#endif
#endif

namespace chen{

	class TimeFace
	{
	public:
		TimeFace(void);
		~TimeFace(void);

		TimeFace(time_t tt);
		TimeFace(int nYear, int nMonth, int nDay, int nHour, int nMinute, int nSecond);
		TimeFace(struct tm &stm);
		TimeFace(const TimeFace &tf);

		static TimeFace GetCurrent();
		static TimeFace GMTToLocalTTime(int nYear, int nMonth, int nDay, int nHour, int nMinute, int nSecond);

		static long GetTickCount();

		time_t GetTime() const;

		TimeFace& operator=(const TimeFace &tf);
		TimeFace operator+(const TimeSegment &ts) const;
		TimeFace& operator+=(const TimeSegment &ts);
		TimeSegment operator-(const TimeFace &time) const;
		TimeFace operator-(const TimeSegment &ts) const;
		TimeFace& operator-=(const TimeSegment &ts);
		bool operator<(const TimeFace &time) const;
		bool operator>(const TimeFace &time) const;
		bool operator!=(const TimeFace &time) const;
		bool operator==(const TimeFace &time) const;
		bool operator>=(const TimeFace &time) const;
		bool operator<=(const TimeFace &time) const;

		int GetGMTYear() const;
		int GetGMTMonth() const;
		int GetGMTDay() const;
		int GetGMTHour() const;
		int GetGMTMinute() const;
		int GetGMTSecond() const;
		int GetGMTWeekDay() const;
		int GetGMTYearDay() const;
		int GetGMTMonthDay() const;
		std::string ToString(void) const;
		std::string ToHttpGMTString(void) const;
		std::string ToUSAString(void) const;

		// %Y year,%M month,%D day,%H hour,%m minute,%S seconds
		std::string Format(const char *szFormat);

		bool CreateFromHttpGMTString(const char *HttpGMTString);
		int GetYear() const;
		int GetMonth() const;
		int GetDay() const;
		int GetHour() const;
		int GetMinute() const;
		int GetSecond() const;
		int GetDayOfWeek() const;
		int GetDayOfYear() const;
		int GetDayOfMonth() const;


#ifndef __NOWINDOWS_H_
#ifdef WIN32
		bool ToSystemTime(LPSYSTEMTIME pst);
#endif
#endif


	private:
		time_t m_times;


	};
}

#endif
