#ifndef	LIBCHEN_TIMESEGMENT_H_
#define LIBCHEN_TIMESEGMENT_H_
#include <ctime>

#ifndef WIN32
	#include "Linux_Common.h"     
#endif

namespace chen{

	class TimeSegment
	{
	public:
		TimeSegment(void);
		~TimeSegment(void);
		TimeSegment(int days,int hours,int minutes,int seconds);
		TimeSegment(const TimeSegment &ts);
		TimeSegment(time_t time);

		TimeSegment &operator=(time_t time);

		bool operator<(const TimeSegment &ts) const;
		bool operator>(const TimeSegment &ts) const;
		bool operator>=(const TimeSegment &ts) const;
		bool operator<=(const TimeSegment &ts) const;
		bool operator!=(const TimeSegment &ts) const;
		bool operator==(const TimeSegment &ts) const;
		TimeSegment operator+(const TimeSegment &ts) const;
		TimeSegment& operator+=(const TimeSegment &ts);
		TimeSegment operator-(const TimeSegment &ts) const;
		TimeSegment& operator-=(const TimeSegment &ts);
		operator time_t() { return m_times; } 


		time_t GetTime_t();
		unsigned int GetTotalDays() const;
		int GetDays() const;
		unsigned int GetTotalHours() const;
		int GetHours() const;
		unsigned int GetTotalMinutes() const;
		int GetMinutes() const;
		unsigned int GetTotalSeconds() const;
		int GetSeconds() const;

	private:
		time_t m_times;
		friend class TimeFace;
	};
}

#endif
