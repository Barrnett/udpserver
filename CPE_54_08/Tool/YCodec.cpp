#include <iostream>
#include "YCodec.h"

using namespace std;

#define _IP_MARK "."

YCodec::YCodec()
{
}

YCodec::~YCodec()
{
}


// char to hex code
// error: return -1 
int YCodec::Char2Hex(char ch)
{
	int n = -1;
	switch(ch)
	{
	case '0':	n = 0;	break;
	case '1':	n = 1;	break;
	case '2':	n = 2;	break;
	case '3':	n = 3;	break;
	case '4':	n = 4;	break;
	case '5':	n = 5;	break;
	case '6':	n = 6;	break;
	case '7':	n = 7;	break;
	case '8':	n = 8;	break;
	case '9':	n = 9;	break;
	case 'A':
	case 'a':	n = 10;	break;
	case 'B':
	case 'b':	n = 11;	break;
	case 'C':
	case 'c':	n = 12;	break;
	case 'D':
	case 'd':	n = 13;	break;
	case 'E':
	case 'e':	n = 14;	break;
	case 'F':
	case 'f':	n = 15;	break;
	default:	break;
	}

	return n;
}

// hex to char
// error: return -1 
char YCodec::Hex2Char(unsigned int n)
{
	char ch;
	if(n>=0 && n<=9)	ch = 48 + n;
	else if(n>=10 && n<=15)	ch = 65 - 10 + n;
	else ch = -1;

	return ch;
}

// num^index
long YCodec::MIndex(int num, int index)
{
	long s = 1;
	int i=0;
	while(i<index)
	{
		s *= num;
		i++;
	}

	return s;
}

// string to hex code
// error: return -1 
long YCodec::String2Hex(char* string, int strlen)
{
	long hex=-1;
	int i=0, n=0;
	char *p = string;
	p += strlen - 1;
	if(string == NULL)	return hex;
	if(strlen <= 0 || strlen > 10)	return hex;

	hex = 0;
	do
	{
		n = Char2Hex(*p--);
		hex += n*MIndex(16, i++);
	}while(i<strlen);

	return hex;
}

// hex to char*
// string==NULL,slen = the size of string(slen as output)
// string!=NULL,input the length of string
// error: return -1
int YCodec::Hex2String(long hex, char* string, int* slen)
{
	char tmp[11] = {0};
	if(hex < 0)	return -1;
	if(string == NULL){// count the length it will be used
		sprintf(tmp, "%x", hex);
		*slen = strlen(tmp);
		return 1;
	}
	memset(string, 0, *slen);
	sprintf(string, "%x", hex);

	return 1;
}

// hex to wchar_t code
// eg: input 0x5e74, return 年
// error: return -1
wchar_t YCodec::Hex2Wchart(long hex)
{
	wchar_t wch = -1;
	if(hex <0)	return wch;
	wch = (wchar_t)hex;

	return wch;
}

// hex string to wchar_t*
// UNICODE十六进制字符串转成中英文
// hexstr每eachchar转换为一个wchar_t
// wchs == NULL, wchsLen as output(the size of wchs will be used)
// error: return -1
int YCodec::HexStr2WChars(char *hexstr, int hexstrlen, int eachchar, wchar_t *wchs, int *wchsLen)
{
	if(hexstr == NULL || hexstrlen <= 0 || eachchar <= 0)
		return -1;
	if(wchs == NULL)
	{// count the size wchs it will be used
		*wchsLen = hexstrlen/eachchar + (hexstrlen%eachchar>0 ? 1 : 0);
		return 1;
	}
	memset(wchs, 0, *wchsLen * sizeof(wchar_t));
	char* tmp = new char[eachchar+1];
	char* p = hexstr;
	wchar_t* pwch = wchs;
	for(int i=0; i<hexstrlen; i+=eachchar){
		memset(tmp, 0, eachchar+1);
		// get eachchar char
		for(int j=0; j<eachchar; j++){
			if(i+j > hexstrlen)	break;
			tmp[j] = *p++;
		}
		// char* to hex
		long hex = String2Hex(tmp, strlen(tmp));
		if(hex == -1)	continue;
		// hex to wchar_t
		*pwch++ = Hex2Wchart(hex);
	}

	if(tmp)	delete []tmp;

	return 1;
}

// wchar_t to hex code
long YCodec::Wchart2Hex(wchar_t wch)
{
	return (long)wch;
}

// wchar_t* to char*(hex string)
// 中英文转成UNICODE十六进制字符串
// eachchar用于控制每个wchar_t转换成多少个char字符
// hexstr == NULL,hexstrlen as output(the size of hexstr will be used)
// error: return -1
int YCodec::Wchart2HexStr(wchar_t *wchs, int wchslen, int eachchar, char* hexstr, int* hexstrlen)
{
	if(wchs == NULL || wchslen <= 0 || eachchar <= 0)	return -1;
	if(hexstr == NULL){// count the size of hexstr will be used
		*hexstrlen = wchslen*eachchar;
		return 1;
	}
	memset(hexstr, 0, *hexstrlen);
	char* p = hexstr;
	wchar_t* pwch = wchs;
	char* tmp = new char[eachchar+1];
	for(int i=0; i<wchslen; i++){
		// wchar_t to hex
		long hex = Wchart2Hex(*pwch++);
		// hex to char*
		int charlen = 0;
		if(Hex2String(hex, NULL, &charlen) != -1){
			char* str = new char[charlen+1];
			memset(str, 0, charlen+1);
			int n = Hex2String(hex, str, &charlen);
			if(n != -1){
				int k=0;
				memset(tmp, 0, eachchar+1);
				for(k=0; k<eachchar-charlen; k++)	tmp[k] = '0';
				tmp = strcat(tmp, str);
				p = strcat(p, tmp);
			}
			if(str)	delete []str;
		}
		if(i > *hexstrlen)	break;
	}
	if(tmp)	delete []tmp;

	return 1;
}

//提取前4个字节，从左到右为高位到地位顺序。转换成int
int YCodec::BytesToInt(char* bytes)
{
	int a = bytes[3] & 0xFF;
	a |= ((bytes[2] << 8) & 0xFF00);
	a |= ((bytes[1] << 16) & 0xFF0000);
	a |= ((bytes[0] << 24) & 0xFF000000);

	return a;
}

//int转换成一个字节byte
void YCodec::charIntToByte(int i, char bytes)
{
	memset(&bytes, 0, sizeof(char));
	bytes = (char) (0xff & i);
}


//int转换成二个字节byte
void YCodec::shortIntToBytes(int i, char *bytes)
{
	//byte[] bytes = new byte[2];
	memset(bytes, 0, sizeof(char)*2);
	bytes[1] = (char) (0xff & i);
	bytes[0] = (char) ((0xff00 & i) >> 8);
}

//int转四个字节byte
void YCodec::intToBytes(int i, char *bytes)
{
	//byte[] bytes = new byte[4];
	memset(bytes,0,sizeof(char)*4);
	bytes[3] = (char) (0xff & i);
	bytes[2] = (char) ((0xff00 & i) >> 8);
	bytes[1] = (char) ((0xff0000 & i) >> 16);
	bytes[0] = (char) ((0xff000000 & i) >> 24);
}

//int转四个、二个、一个字节bytes
bool YCodec::IntToBytes(int i, char *bytes, int size)
{
	bool temp = false;
	memset(bytes, 0, sizeof(bytes));

	switch (size)
	{
	case 4:
		{
			temp = true;
			bytes[0] = (char) ((0xff000000 & i) >> 24);
			bytes[1] = (char) ((0xff0000 & i) >> 16);
			bytes[2] = (char) ((0xff00 & i) >> 8);
			bytes[3] = (char) (0xff & i);
		}
		break;
	case 2:
		{
			temp = true;
			bytes[0] = (char) ((0xff00 & i) >> 8);
			bytes[1] = (char) (0xff & i);
		}
		break;
	case 1:
		{
			temp = true;
			bytes[0] = (char) (0xff & i);
		}
		break;
	}

	return temp;
}

//int转四个、二个、一个字节bytes(网络序)
bool YCodec::IntToBytes_Network(int i, char *bytes, int size)
{
	bool temp = false;
	memset(bytes, 0, sizeof(bytes));

	switch (size)
	{
	case 4:
		{
			temp = true;
			bytes[0] = (char) (0xff & i);
			bytes[1] = (char) ((0xff00 & i) >> 8);
			bytes[2] = (char) ((0xff0000 & i) >> 16);
			bytes[3] = (char) ((0xff000000 & i) >> 24);
		}
		break;
	case 2:
		{
			temp = true;
			bytes[0] = (char) (0xff & i);
			bytes[1] = (char) ((0xff00 & i) >> 8);
		}
		break;
	case 1:
		{
			temp = true;
			bytes[0] = (char) (0xff & i);
		}
		break;
	}

	return temp;
}

//四个、二个、一个字节bytes转int
bool YCodec::bytesToInt(char* bytes, int &a, int size)
{
	bool temp = false;

	if (size == 4)
	{
		temp = true;
		a |= ((bytes[0] << 24) & 0xFF000000);
		a |= ((bytes[1] << 16) & 0xFF0000);
		a |= ((bytes[2] << 8) & 0xFF00);
		a |= bytes[3] & 0xFF;
	}
	else if (size == 2)
	{
		temp = true;
		a |= ((bytes[0] << 8) & 0xFF00);
		a |= bytes[1] & 0xFF;
	}
	else if (size == 1)
	{
		temp = true;
		a = bytes[1] & 0xFF;
	}
	return temp;
}

//int转换成IP
//int转换成IP
string YCodec::INTtoIP(int num)
{  

	string strRet = "";  
	for (int i=0;i<4;i++)  
	{  
		int tmp=(num>>((3-i)*8))&0xFF;  

		char chBuf[8] = "";
		sprintf(chBuf,"%d",tmp);
		strRet += chBuf;

		if (i < 3)
		{
			strRet += _IP_MARK;
		}
	}  

	return strRet;  
}


//将16进制字符串转化为byte数组
/*
hexstring: 输入的16进制字符串
output：输出的字符类型
length：输出字符长度
*/
bool YCodec::HexString2Bytes(const string &hexstring, char *output, unsigned int length)
{
	unsigned long tmp = 0;
	const char *instr = hexstring.c_str();
	char tmpchr[4+1];
	memset(tmpchr, 0, sizeof(tmpchr));
	memcpy(tmpchr, instr, 2);
	
	if (strcmp(tmpchr, "0x"))
	{
		return false;
	}

	int size = hexstring.size();
	
	for(unsigned int i = 2, c = 0; i < size, c < length; i += 2, c++)
	{
		memcpy(tmpchr+2, instr + i, 2);
		tmp = strtoul(tmpchr, NULL, 16);
		output[c] = (char)tmp;
	}

	return true;
}

//计算CCITT 标准的CRC-16检验码
static unsigned short ccitt_table[256]=
{
	0x0000,0x1021,0x2042,0x3063,0x4084,0x50A5,0x60C6,0x70E7,
	0x8108,0x9129,0xA14A,0xB16B,0xC18C,0xD1AD,0xE1CE,0xF1EF,
	0x1231,0x0210,0x3273,0x2252,0x52B5,0x4294,0x72F7,0x62D6,
	0x9339,0x8318,0xB37B,0xA35A,0xD3BD,0xC39C,0xF3FF,0xE3DE,
	0x2462,0x3443,0x0420,0x1401,0x64E6,0x74C7,0x44A4,0x5485,
	0xA56A,0xB54B,0x8528,0x9509,0xE5EE,0xF5CF,0xC5AC,0xD58D,
	0x3653,0x2672,0x1611,0x0630,0x76D7,0x66F6,0x5695,0x46B4,
	0xB75B,0xA77A,0x9719,0x8738,0xF7DF,0xE7FE,0xD79D,0xC7BC,
	0x48C4,0x58E5,0x6886,0x78A7,0x0840,0x1861,0x2802,0x3823,
	0xC9CC,0xD9ED,0xE98E,0xF9AF,0x8948,0x9969,0xA90A,0xB92B,
	0x5AF5,0x4AD4,0x7AB7,0x6A96,0x1A71,0x0A50,0x3A33,0x2A12,
	0xDBFD,0xCBDC,0xFBBF,0xEB9E,0x9B79,0x8B58,0xBB3B,0xAB1A,
	0x6CA6,0x7C87,0x4CE4,0x5CC5,0x2C22,0x3C03,0x0C60,0x1C41,
	0xEDAE,0xFD8F,0xCDEC,0xDDCD,0xAD2A,0xBD0B,0x8D68,0x9D49,
	0x7E97,0x6EB6,0x5ED5,0x4EF4,0x3E13,0x2E32,0x1E51,0x0E70,
	0xFF9F,0xEFBE,0xDFDD,0xCFFC,0xBF1B,0xAF3A,0x9F59,0x8F78,
	0x9188,0x81A9,0xB1CA,0xA1EB,0xD10C,0xC12D,0xF14E,0xE16F,
	0x1080,0x00A1,0x30C2,0x20E3,0x5004,0x4025,0x7046,0x6067,
	0x83B9,0x9398,0xA3FB,0xB3DA,0xC33D,0xD31C,0xE37F,0xF35E,
	0x02B1,0x1290,0x22F3,0x32D2,0x4235,0x5214,0x6277,0x7256,
	0xB5EA,0xA5CB,0x95A8,0x8589,0xF56E,0xE54F,0xD52C,0xC50D,
	0x34E2,0x24C3,0x14A0,0x0481,0x7466,0x6447,0x5424,0x4405,
	0xA7DB,0xB7FA,0x8799,0x97B8,0xE75F,0xF77E,0xC71D,0xD73C,
	0x26D3,0x36F2,0x0691,0x16B0,0x6657,0x7676,0x4615,0x5634,
	0xD94C,0xC96D,0xF90E,0xE92F,0x99C8,0x89E9,0xB98A,0xA9AB,
	0x5844,0x4865,0x7806,0x6827,0x18C0,0x08E1,0x3882,0x28A3,  
	0xCB7D,0xDB5C,0xEB3F,0xFB1E,0x8BF9,0x9BD8,0xABBB,0xBB9A,
	0x4A75,0x5A54,0x6A37,0x7A16,0x0AF1,0x1AD0,0x2AB3,0x3A92,
	0xFD2E,0xED0F,0xDD6C,0xCD4D,0xBDAA,0xAD8B,0x9DE8,0x8DC9,
	0x7C26,0x6C07,0x5C64,0x4C45,0x3CA2,0x2C83,0x1CE0,0x0CC1,
	0xEF1F,0xFF3E,0xCF5D,0xDF7C,0xAF9B,0xBFBA,0x8FD9,0x9FF8,
	0x6E17,0x7E36,0x4E55,0x5E74,0x2E93,0x3EB2,0x0ED1,0x1EF0
};
unsigned short YCodec::crc_ccitt(char *q, int len)
{
	unsigned short crc = 0;

	while(len-- > 0)
		crc = ccitt_table[(crc>> 8^*q++)&0xff]^(crc<<8);
	return crc;
}

//从字符串里面提取数字
static char numarr[] = {
	'0',
	'1',
	'2',
	'3',
	'4',
	'5',
	'6',
	'7',
	'8',
	'9',
};
list<int> YCodec::GetStringByNumArr(string str) 
{
	list<int> numlist;
	int startIndex = 0;

	while (startIndex != -1)
	{
		vector<char> tempnum;
		startIndex = -1;
		for (size_t i = 0; i < str.length(); i++)
		{
			for (size_t j = 0; j < (sizeof(numarr) / sizeof(numarr[0]) - 1); j++)
			{
				if (str[i] == numarr[j])
				{
					startIndex = i;
					break;
				}
			}

			if (startIndex != -1)
			{
				tempnum.push_back(str[startIndex]);
				int tempindex = 0;
				//向下查找数据
				char tempchar = str[startIndex + (tempindex += 1)];
				//表示为数字
				while (int(tempchar - 48) >= 0 && int(tempchar - 48) <= 9)
				{
					tempnum.push_back(tempchar);
					tempchar = str[startIndex + (tempindex += 1)];
				}
				//删除查询到的数据
				str.erase(startIndex, tempindex);
				break;
			}
		}

		if (!tempnum.empty()) {
			//cout << "tempnum : " << string(tempnum.begin(), tempnum.end()) << endl;
			numlist.push_back(atoi(string(tempnum.begin(), tempnum.end()).c_str()));
		}
	}
	return numlist;
}

static char table[]={'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

void YCodec::num2char(char *str, double number, int frontBit, int afterBit)
{
	int i;
	int temp = number/1;
	double t2 = 0.0;
	for (i = 1; i<=frontBit; i++)
	{
		if (temp==0)
			str[frontBit-i] = table[0];
		else
			str[frontBit-i] = table[temp%10];
		temp = temp/10;
	}
	*(str+frontBit) = '.';
	temp = 0;
	t2 = number;
	for(i=1; i<=afterBit; i++)
	{
		temp = t2*10;
		str[frontBit+i] = table[temp%10];
		t2 = t2*10;
	}
	*(str+frontBit+afterBit+1) = '\0';
}


long YCodec::StrToIP(char *szIpAddr)
{
	if (strlen(szIpAddr)>15)
	{
		return 0;
	}
	char buf[32];
	strncpy(buf,szIpAddr,32);
	char* p=&buf[0];
	char* pNext=p;
	int i=0;
	long dwAddr=0;
	while (p)
	{
		pNext=strchr(p,'.');
		if (pNext)
		{
			*pNext=0;
			pNext++;
		}
		dwAddr|=(atol(p)&0xff)<<((3-i)*8);
		i++;
		p=pNext;
	}
	if (i<4)
	{
		return 0;
	}
	return dwAddr;
}

/*Double型转为string型*/
void YCodec::Double2str(double value_input,char* new_buffer)
{
    int i = 0;
    char buffer[30];
    bool flag;

    memset(buffer,0,sizeof(buffer));
    sprintf(buffer, "%.10f\n", value_input);

    for(i=0;i<10;i++)
    {
        if(('0' != *(buffer + i))&&('.' != *(buffer + i)))
        {
             flag = true;
             break;
        }
    }
/*
    if(*(buffer + i+3) >= '5')
    {
        *(buffer + i + 2) += 1;
    }
*/
//误码率为0时
    if(flag == false)
    {
       new_buffer[0] = '0';
       new_buffer[1] = 'x';
       new_buffer[2] = '0';
       new_buffer[3] = '0';
       new_buffer[4] = '0';
       new_buffer[5] = '0';

     }
     else
     {
        sprintf(&buffer[i+3], "%d\n", i-1);

        new_buffer[0] = '0';
        new_buffer[1] = 'x';
        strncpy(new_buffer+2,buffer+i,4);
     }
     new_buffer[6] = '\0';
}
