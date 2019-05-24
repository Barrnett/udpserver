#ifndef MANAGE_NETWORK_H
#define MANAGE_NETWORK_H


class CManageNetwork
{
public:
	CManageNetwork();
	~CManageNetwork();
	bool Clear();
protected:
private:

public:
	void charIntToByte(int i, char *bytes);
};

#endif //MANAGE_NETWORK_H