#ifndef _BLUESKY_BLUESKYINT_
#define _BLUESKY_BLUESKYINT_
#ifndef WIN32
#include "Linux_Common.h"  
#endif
typedef		unsigned int		ui32;
typedef		int					i32;
typedef		unsigned short		ui16;
typedef		short				i16;
typedef		unsigned char		ui8;
typedef		char				i8;
typedef		float				bs_float;
typedef		double				bs_double;
typedef		__int64				i64;
typedef		unsigned __int64	ui64;

typedef		i8				bs_bool;

#define		bs_true			1
#define		bs_false		0

///////////////////////////////////////////////////////////
#define BS_MAXUI64		0xFFFFFFFFFFFFFFFE


///////////////////////////////////////////////////////////

#define F_SET(x,f) (x) |= (f)
#define F_CLR(x,f) (x) &= ~(f)
#define F_ISSET(x,f) ((x) & (f))






#endif
