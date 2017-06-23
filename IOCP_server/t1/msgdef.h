#ifndef __MSG_DEF_H
#define __MSG_DEF_H

class MsgBase
{
public:
	virtual void onMsg();
	int _type;
};

class MsgA : public MsgBase
{
public:
	MsgA() : _type(100) {}
	void onMsg()
	{
		switch (typeSub)
		{
		default:
			break;
		}
		
	}
	void updateLevel(int level)
	{
		
	}
	void fun2(int data);


};
MsgBase *msg = ()

#endif