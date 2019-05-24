#ifndef BS_BSMQDEFINE_H_
#define BS_BSMQDEFINE_H_


#define BSMQ_OF_OPENEXIST		0x00000001
#define BSMQ_OF_CREATENEW		0x00000002
#define BSMQ_OF_OPENMULTI		0x00000004


#define BSMQ_OT_COMMONMQ		0x01
#define BSMQ_OT_PRIORITYMQ		0x02
#define BSMQ_OT_TEMPMQ			0x04
#define BSMQ_OT_ENCRYPT			0x08
#define BSMQ_OT_INDEXRECORD		0x10


#define BS_MQ_MAXNAME		100
#define BS_MQ_MAXPWD		20

//表示最多支持多少个优先级
#define BS_MQ_MAXPRIORITY	9
//普通优先级
#define BS_MQ_COMMONPRIORITY  (BS_MQ_MAXPRIORITY/2+1)

#define BS_MQ_MAXRECORDID	30

///////////////////////////////////////////////////////////////




#endif