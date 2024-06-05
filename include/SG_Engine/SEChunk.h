#ifndef __SDL_Chunk_H__
#define __SDL_Chunk_H__
#include <string>
#include "SDL_mixer.h"
#include "SEObject.h"
NS_SDL_BEGIN
class Chunk : public Object
{
private:
	Mix_Chunk *_chunk;

public:
	const static std::string TYPE;

public:
	Chunk();
	~Chunk();
	static Chunk *create(const std::string &file);
	bool init(const std::string &file);

	int playChannel(int channel, int loops);

	int volumeChunk(int vloume);

public:
	static void pause(int channel);

	static void resume(int channel);

	static int haltChannel(int channel);

	static int playing(int channel);

	static int volume(int channel, int volume);
};
NS_SDL_END
#endif
