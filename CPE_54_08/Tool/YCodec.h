#ifndef __YCODEC_H__
#define __YCODEC_H__

#include <iostream>
#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <string.h>
using namespace std;

class YCodec
{
public:
	YCodec();
	~YCodec();
	// char to hex code
	static int Char2Hex(char ch);
	// hex to char
	static char Hex2Char(unsigned int n);
	// num^index
	static long MIndex(int num, int index);
	// string to hex code
	static long String2Hex(char* string, int strlen);
	// hex to char*
	static int Hex2String(long hex, char* string, int* slen);
	// hex to wchar_t code
	static wchar_t Hex2Wchart(long hex);
	// UNICODE十六进制字符串转成中英文
	// hex string to wchar_t*
	static int HexStr2WChars(char* hexstr, int hexstrlen, int eachchar, wchar_t* wchs, int* wchslen);
	// wchar_t to hex code
	static long Wchart2Hex(wchar_t wch);
	// 中英文转成UNICODE十六进制字符串
	// wchar_t* to char*(hex string)
	static int Wchart2HexStr(wchar_t* wchs, int wchslen, int eachchar, char* hexstr, int* hexstrlen);

	//新的
	//提取前4个字节，从左到右为高位到地位顺序。转换成int
	static int BytesToInt(char* bytes);
	//int转换成一个字节byte
	static void charIntToByte(int i, char bytes);
	//int转换成二个字节byte
	static void shortIntToBytes(int i, char *bytes);
	//int转四个字节byte
	static void intToBytes(int i, char *bytes);
	//int转四个字节bytes
	static bool IntToBytes(int i, char *bytes,  int size = 4);
	//int转四个、二个、一个字节bytes(网络序)
	static bool IntToBytes_Network(int i, char *bytes, int size);
	//四个、二个、一个字节bytes转int
	static bool bytesToInt(char* bytes, int &a, int size = 4);
	//int转换成IP
	static string INTtoIP(int num);
	//将16进制字符串转化为byte数组
	static bool HexString2Bytes(const string &hexstring, char *output, unsigned int length);
	//计算CCITT 标准的CRC-16检验码
	static unsigned short crc_ccitt(char *q, int len);
	//从字符串里面提取数字
	static list<int> GetStringByNumArr(string str);
	//浮点数转换成字符串
	static void num2char(char *str, double number, int frontBit, int afterBit);
	static long StrToIP(char *szIpAddr);
	static void Double2str(double value_input,char* new_buffer);

};
#endif
