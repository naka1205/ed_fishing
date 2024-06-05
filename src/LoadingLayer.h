#ifndef __LoadingLayer_H__
#define __LoadingLayer_H__
#include<vector>
#include<map>
#include<string>
#include<stack>
#include "SG_Engine.h"
using namespace std;
using namespace SDL;

class LoadingLayer:public Layer
{
private:
	ProgressTimer*m_pProgress1;
	ProgressTimer*m_pProgress2;
	int m_nCurIndex;
	int m_nSize;
	deque<string> m_images;
	deque<string> m_musics;
	deque<string> m_chunks;
public:
	LoadingLayer();
	~LoadingLayer();
	CREATE_FUNC(LoadingLayer);
	static Scene* createScene();
	bool init();
private:
	void loadImageCallback(Texture*texture);
	void loadMusicCallback(Object*sound,const string&type);
	void loadChunkCallback(Object*sound,const string&type);

	void loadResAsyncCallback();
};
#endif