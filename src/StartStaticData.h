#ifndef __StartStaticData_H__
#define __StartStaticData_H__
#include<string>
#include "SG_Engine.h"
using namespace std;
using namespace SDL;

typedef struct FishingGround
{
	string nameImg;
	string thumbnailImg;
	string ratioImg;
	string desc;
	string levelName;
}FishingGround;
class StartStaticData:public Object
{
private:
	static StartStaticData*m_pInstance;
	StartStaticData();
	~StartStaticData();
public:
	static StartStaticData*getInstance();
	void purge();


};
#endif