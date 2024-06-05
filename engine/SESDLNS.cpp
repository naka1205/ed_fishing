#include "SG_Engine/SESDLNS.h"
NS_SDL_BEGIN
typedef std::vector<std::string> strArray;
static void split(const std::string &src, const std::string &token, strArray &vect)
{
	size_t nend = 0;
	size_t nbegin = 0;
	size_t tokenSize = token.size();

	while (nend != std::string::npos)
	{
		nend = src.find(token, nbegin);
		if (nend == std::string::npos)
			vect.push_back(src.substr(nbegin, src.length() - nbegin));
		else
			vect.push_back(src.substr(nbegin, nend - nbegin));
		nbegin = nend + tokenSize;
	}
}
static bool splitWithForm(const std::string &content, strArray &strs, const std::string &splitStr = ",")
{
	bool bRet = false;

	do
	{
		SDL_BREAK_IF(content.empty());
		auto nPosLeft = content.find("{");
		auto nPosRight = content.find("}");

		SDL_BREAK_IF(nPosLeft == std::string::npos || nPosRight == std::string::npos);

		SDL_BREAK_IF(nPosRight < nPosLeft);

		const std::string pointStr = content.substr(nPosLeft + 1, nPosRight - nPosLeft - 1);

		SDL_BREAK_IF(pointStr.size() == 0);

		auto nPos1 = pointStr.find("{");
		auto nPos2 = pointStr.find("}");

		SDL_BREAK_IF(nPos1 != std::string::npos || nPos2 != std::string::npos);

		split(pointStr, splitStr, strs);

		if (strs.size() != 2 || strs[0].length() == 0 || strs[1].length() == 0)
		{
			strs.clear();
			break;
		}
		bRet = true;
	} while (0);

	return bRet;
}

Point PointFromString(const std::string &str, char token)
{
	Point ret;
	do
	{
		std::string sToken;
		strArray strs;
		sToken.push_back(token);

		SDL_BREAK_IF(!splitWithForm(str, strs, sToken));

		float x = (float)atof(strs.at(0).c_str());
		float y = (float)atof(strs.at(1).c_str());

		ret.set(x, y);
	} while (0);
	return ret;
}
Size SizeFromString(const std::string &str, char token)
{
	Size ret;
	do
	{
		std::string sToken;
		strArray strs;

		sToken.push_back(token);

		SDL_BREAK_IF(!splitWithForm(str, strs, sToken));

		float w = (float)atof(strs.at(0).c_str());
		float h = (float)atof(strs.at(1).c_str());

		ret.setSize(w, h);
	} while (0);
	return ret;
}
Rect RectFromString(const std::string &str, char token)
{
	Rect result;
	std::string sToken;

	sToken.push_back(token);
	do
	{
		SDL_BREAK_IF(str.empty());
		std::string content = str;

		auto nPosLeft = content.find('{');
		auto nPosRight = content.find('}');
		for (int i = 1; i < 3; i++)
		{
			if (nPosRight == std::string::npos)
				break;
			nPosRight = content.find('}', nPosRight + 1);
		}
		SDL_BREAK_IF(nPosLeft == std::string::npos || nPosRight == std::string::npos);

		content = content.substr(nPosLeft + 1, nPosRight - nPosLeft - 1);

		auto nPointEnd = content.find('}');
		SDL_BREAK_IF(nPointEnd == std::string::npos);

		nPointEnd = content.find(token, nPointEnd);
		SDL_BREAK_IF(nPointEnd == std::string::npos);

		std::string pointStr = content.substr(0, nPointEnd);
		std::string sizeStr = content.substr(nPointEnd + 1, content.length() - nPointEnd);

		strArray pointInfo;
		SDL_BREAK_IF(!splitWithForm(pointStr, pointInfo, sToken));
		strArray sizeInfo;
		SDL_BREAK_IF(!splitWithForm(sizeStr, sizeInfo, sToken));

		float x = (float)atof(pointInfo.at(0).c_str());
		float y = (float)atof(pointInfo.at(1).c_str());
		float w = (float)atof(sizeInfo.at(0).c_str());
		float h = (float)atof(sizeInfo.at(1).c_str());

		result = Rect(x, y, w, h);
	} while (0);

	return result;
}
NS_SDL_END
