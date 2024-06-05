#include "SG_Engine/SEFileUtils.h"

NS_SDL_BEGIN

class DictMaker
{
public:
	typedef enum class Result
	{
		None = 0,
		Dict,
		Array,
	} Result;
	typedef enum class State
	{
		None,
		Integer,
		Real,
		String,
		Key,
		Dict,
		Array,
	} State;

public:
	std::stack<State> _stateStack;

	std::stack<ValueMap *> _dictStack;
	std::stack<ValueVector *> _arrayStack;

	Result _resultType;

	ValueMap _rootDict;
	ValueVector _rootArray;

	State _state;

	std::string _curKey;
	std::string _curValue;

	ValueMap *_curDict;
	ValueVector *_curArray;

public:
	DictMaker()
		: _resultType(Result::None), _state(State::None), _curDict(nullptr), _curArray(nullptr)
	{
	}
	~DictMaker() {}
	ValueMap dictionaryWithContentsOfString(char *text)
	{
		_resultType = Result::Dict;

		rapidxml::xml_document<> doc;

		doc.parse<0>(text);

		rapidxml::xml_node<> *root = doc.first_node();

		auto dict_root = root->first_node();

		this->accept(dict_root);

		return _rootDict;
	}

	ValueMap dictionaryWithContentsOfFile(const std::string &fileName)
	{
		_resultType = Result::Dict;

		auto uniquePtr = std::move(FileUtils::getInstance()->getUniqueDataFromFile(fileName));

		if (uniquePtr == nullptr)
			return ValueMap();

		auto text = uniquePtr.get();

		rapidxml::xml_document<> doc;

		doc.parse<0>(text);

		rapidxml::xml_node<> *root = doc.first_node();

		auto dict_root = root->first_node();

		this->accept(dict_root);

		return _rootDict;
	}
	ValueVector arrayWithContentsOfFile(const std::string &fileName)
	{
		_resultType = Result::Array;

		auto uniquePtr = std::move(FileUtils::getInstance()->getUniqueDataFromFile(fileName));

		if (uniquePtr == nullptr)
			return ValueVector();
		auto text = uniquePtr.get();

		rapidxml::xml_document<> doc;

		doc.parse<0>(text);

		rapidxml::xml_node<> *root = doc.first_node();

		auto dict_root = root->first_node();

		this->accept(dict_root);

		return _rootArray;
	}
	bool accept(rapidxml::xml_node<> *root)
	{
		if (root->type() == rapidxml::node_type::node_element)
		{
			this->visitEnter(root);

			for (auto node = root->first_node(); node != nullptr; node = node->next_sibling())
			{
				if (!this->accept(node))
					break;
			}
			return this->visitExit(root);
		}
		else if (root->type() == rapidxml::node_type::node_data)
		{
			this->textHandler(root);
			return true;
		}

		return false;
	}
	bool visitEnter(rapidxml::xml_node<> *node)
	{
		const char *name = node->name();

		std::string sName = std::string(name);

		if (sName == "key")
		{
			_state = State::Key;
		}
		else if (sName == "integer")
		{
			_state = State::Integer;
		}
		else if (sName == "real")
		{
			_state = State::Real;
		}
		else if (sName == "string")
		{
			_state = State::String;
		}
		else if (sName == "dict")
		{
			_state = State::Dict;

			if (_resultType == Result::Dict && _rootDict.empty())
			{
				_curDict = &_rootDict;
			}
			State preState = State::None;
			if (!_stateStack.empty())
			{
				preState = _stateStack.top();
			}
			if (preState == State::Dict)
			{
				ValueMap *preDict = _dictStack.top();
				(*preDict)[_curKey] = Value(ValueMap());

				_curDict = &(*preDict)[_curKey].asValueMap();
			}
			else if (preState == State::Array)
			{
				_curArray->push_back(Value(ValueMap()));
				_curDict = &(_curArray->rbegin())->asValueMap();
			}
			_stateStack.push(_state);
			_dictStack.push(_curDict);
		}
		else if (sName == "array")
		{
			_state = State::Array;

			if (_resultType == Result::Array && _rootArray.empty())
			{
				_curArray = &_rootArray;
			}
			State preState = State::None;

			if (!_stateStack.empty())
			{
				preState = _stateStack.top();
			}

			if (preState == State::Dict)
			{
				(*_curDict)[_curKey] = Value(ValueVector());
				_curArray = &(*_curDict)[_curKey].asValueVector();
			}
			else if (preState == State::Array)
			{
				ValueVector *preArray = _arrayStack.top();
				preArray->push_back(Value(ValueVector()));

				_curArray = &(preArray->rbegin())->asValueVector();
			}
			_stateStack.push(_state);
			_arrayStack.push(_curArray);
		}
		return true;
	}
	bool visitExit(rapidxml::xml_node<> *node)
	{
		const char *name = node->name();
		std::string sName = std::string(name);
		State curState = _stateStack.empty() ? State::Dict : _stateStack.top();

		if (sName == "dict")
		{
			_stateStack.pop();
			_dictStack.pop();

			if (!_dictStack.empty())
				_curDict = _dictStack.top();
		}
		else if (sName == "array")
		{
			_stateStack.pop();
			_arrayStack.pop();

			if (!_arrayStack.empty())
				_curArray = _arrayStack.top();
		}
		else if (sName == "true")
		{
			if (curState == State::Array)
				_curArray->push_back(Value(true));
			else if (curState == State::Dict)
				(*_curDict)[_curKey] = Value(true);
		}
		else if (sName == "false")
		{
			if (curState == State::Array)
				_curArray->push_back(Value(false));
			else if (curState == State::Dict)
				(*_curDict)[_curKey] = Value(false);
		}
		else if (sName == "integer" || sName == "real" || sName == "string")
		{
			if (curState == State::Array)
			{
				if (sName == "string")
					_curArray->push_back(Value(_curValue));
				else if (sName == "integer")
					_curArray->push_back(Value(atoi(_curValue.c_str())));
				else
					_curArray->push_back(Value(atof(_curValue.c_str())));
			}
			else if (curState == State::Dict)
			{
				if (sName == "string")
					(*_curDict)[_curKey] = Value(_curValue);
				else if (sName == "integer")
					(*_curDict)[_curKey] = Value(atoi(_curValue.c_str()));
				else
					(*_curDict)[_curKey] = Value(atof(_curValue.c_str()));
			}
			_curValue.clear();
		}

		_state = State::None;
		return true;
	}
	void textHandler(rapidxml::xml_node<> *node)
	{
		if (_state == State::None || _state == State::Dict || _state == State::Array)
			return;
		const char *value = node->value();
		std::string sValue = std::string(value);

		switch (_state)
		{
		case State::Key:
			_curKey = sValue;
			break;
		case State::Integer:
		case State::Real:
		case State::String:
			_curValue.append(sValue);
			break;
		default:
			break;
		}
	}
};

FileUtils *FileUtils::_pInstance = nullptr;

FileUtils::FileUtils()
{
}

FileUtils::~FileUtils()
{
	_searchPathArray.clear();
	_fullPathCache.clear();
}

void FileUtils::purge()
{
	SDL_SAFE_RELEASE(_pInstance);
}

std::string FileUtils::fullPathForFilename(const std::string &filename)
{

	if (isAbsolutePath(filename))
		return filename;

	auto iter = _fullPathCache.find(filename);
	if (iter != _fullPathCache.end())
		return iter->second;

	std::string fullPath;

	if (isFileExist(filename))
	{
		_fullPathCache.insert(std::make_pair(filename, filename));
		return filename;
	}

	for (auto it = _searchPathArray.begin(); it != _searchPathArray.end(); it++)
	{
		fullPath = this->getFullPath(filename, *it);

		if (fullPath.length() > 0)
		{

			_fullPathCache.insert(std::make_pair(filename, fullPath));
			return fullPath;
		}
	}

	return "";
}

void FileUtils::addSearchPath(const std::string &filename)
{

	auto iter = std::find_if(_searchPathArray.begin(), _searchPathArray.end(), [filename](std::string name)
							 { return filename == name; });
	if (iter == _searchPathArray.end())
		_searchPathArray.push_back(filename);
}

bool FileUtils::isAbsolutePath(const std::string &filename) const
{
	return false;
}

bool FileUtils::isFileExist(const std::string &filename) const
{
	// SDL_RWops *file = SDL_RWFromFile(filename.c_str(), "r");

	FILE *rw = fopen(filename.c_str(), "rb");

	if (rw == NULL)
	{
		return false;
	}

	SDL_RWops *file = SDL_RWFromFP(rw, SDL_TRUE);

	if (file)
	{
		SDL_RWclose(file);
		return true;
	}

	return false;
}
std::string FileUtils::getDataFromFile(const std::string &filename)
{

	auto path = this->fullPathForFilename(filename);

	if (path.empty())
		return std::string();

	// SDL_RWops *file = SDL_RWFromFile(path.c_str(), "r");

	FILE *rw = fopen(path.c_str(), "rb");

	SDL_RWops *file = SDL_RWFromFP(rw, SDL_TRUE);

	unsigned int size = (unsigned int)SDL_RWsize(file);

	char *text = new char[size + 1];

	memset(text, '\0', size + 1);

	SDL_RWread(file, text, 1, size);

	SDL_RWclose(file);

	std::string str(text);

	delete[] text;
	return str;
}

ValueMap FileUtils::getValueMapFromFile(const std::string &plist)
{
	DictMaker dictMaker;
	dictMaker.dictionaryWithContentsOfFile(plist);

	return dictMaker._rootDict;
}

ValueVector FileUtils::getValueVectorFromFile(const std::string &plist)
{
	DictMaker dictMaker;
	dictMaker.arrayWithContentsOfFile(plist);

	return dictMaker._rootArray;
}

ValueMap FileUtils::stringToValueMap(char *text)
{
	DictMaker dictMaker;
	dictMaker.dictionaryWithContentsOfString(text);

	return dictMaker._rootDict;
}

std::string FileUtils::valueMapToString(ValueMap &dict)
{
	rapidxml::xml_document<> doc;

	rapidxml::xml_node<> *head = doc.allocate_node(rapidxml::node_type::node_pi, doc.allocate_string("xml version='1.0' encoding='UTF-8'"));

	if (head == nullptr)
	{
		return "";
	}
	doc.append_node(head);

	rapidxml::xml_node<> *root = doc.allocate_node(rapidxml::node_type::node_element, "plist");
	if (root == nullptr)
		return "";

	root->append_attribute(doc.allocate_attribute("version", "1.0"));
	rapidxml::xml_node<> *innerDict = this->generateElementForDict(dict, doc);

	if (innerDict == nullptr)
		return "";

	root->append_node(innerDict);
	doc.append_node(root);

	std::string text;
	rapidxml::print(std::back_inserter(text), doc, 0);

	return text;
}

bool FileUtils::writeValueMapToFile(ValueMap &dict, const std::string &fullpath)
{
	std::string text = this->valueMapToString(dict);

	SDL_RWops *fout = SDL_RWFromFile(fullpath.c_str(), "w");
	if (fout == nullptr)
	{
		LOG("error:FileUtils::writeValueMapToFile:%s\n", SDL_GetError());
		return false;
	}
	else
	{
		SDL_RWwrite(fout, text.c_str(), text.size(), 1);
		SDL_RWclose(fout);
	}

	return true;
}

bool FileUtils::writeValueVectorToFile(ValueVector &array, const std::string &fullpath)
{
	rapidxml::xml_document<> doc;

	rapidxml::xml_node<> *head = doc.allocate_node(rapidxml::node_type::node_pi, doc.allocate_string("xml version='1.0' encoding='UTF-8'"));

	if (head == nullptr)
	{
		return false;
	}
	doc.append_node(head);

	rapidxml::xml_node<> *root = doc.allocate_node(rapidxml::node_type::node_element, "plist");
	if (root == nullptr)
		return false;

	root->append_attribute(doc.allocate_attribute("version", "1.0"));
	rapidxml::xml_node<> *innerArray = this->generateElementForArray(array, doc);

	if (innerArray == nullptr)
		return false;

	root->append_node(innerArray);
	doc.append_node(root);

	std::string text;
	rapidxml::print(std::back_inserter(text), doc, 0);

	SDL_RWops *fout = SDL_RWFromFile(fullpath.c_str(), "w");
	if (fout == nullptr)
	{
		LOG("error:FileUtilr::writeValueMapToFile:%s\n", SDL_GetError());
		return false;
	}
	else
	{
		SDL_RWwrite(fout, text.c_str(), text.size(), 1);
		SDL_RWclose(fout);
	}
	return true;
}

std::unique_ptr<char> FileUtils::getUniqueDataFromFile(const std::string &filename, size_t *sz)
{
	std::unique_ptr<char> points;

	auto path = this->fullPathForFilename(filename);

	if (path.empty())
		return points;

	SDL_RWops *file = SDL_RWFromFile(path.c_str(), "r");

	unsigned int size = (unsigned int)SDL_RWsize(file);

	char *text = new char[size + 1];

	memset(text, '\0', size + 1);

	SDL_RWread(file, text, size, 1);

	if (sz != nullptr)
		*sz = size;

	points = std::unique_ptr<char>(text);

	SDL_RWclose(file);
	return points;
}

std::string FileUtils::getWritablePath() const
{

	char *prefPath = SDL_GetPrefPath(_org.c_str(), _app.c_str());
	std::string path(prefPath);

	SDL_free(prefPath);

	return path;
}

std::string FileUtils::getFullPath(const std::string &filename, const std::string &path) const
{
	std::string fullPath = path + "/" + filename;

	if (isFileExist(fullPath))
		return fullPath;
	return "";
}

rapidxml::xml_node<> *FileUtils::generateElementForArray(ValueVector &array, rapidxml::xml_document<> &doc)
{
	rapidxml::xml_node<> *rootNode = doc.allocate_node(rapidxml::node_type::node_element, "array");

	for (const auto &value : array)
	{
		rapidxml::xml_node<> *node = this->generateElementForObject(value, doc);

		if (node)
			rootNode->append_node(node);
	}
	return rootNode;
}

rapidxml::xml_node<> *FileUtils::generateElementForDict(ValueMap &dict, rapidxml::xml_document<> &doc)
{
	rapidxml::xml_node<> *rootNode = doc.allocate_node(rapidxml::node_type::node_element, "dict");

	for (const auto &iter : dict)
	{

		rapidxml::xml_node<> *keyNode = doc.allocate_node(rapidxml::node_type::node_element, "key", iter.first.c_str());
		rootNode->append_node(keyNode);

		rapidxml::xml_node<> *element = this->generateElementForObject(iter.second, doc);

		if (element)
			rootNode->append_node(element);
	}
	return rootNode;
}

rapidxml::xml_node<> *FileUtils::generateElementForObject(const Value &value, rapidxml::xml_document<> &doc)
{
	rapidxml::xml_node<> *node = nullptr;
	auto type = value.getType();

	switch (type)
	{
	case SDL::Value::Type::INTEGER:
		node = doc.allocate_node(rapidxml::node_type::node_element, "integer");
		node->value(doc.allocate_string(value.asString().c_str()));
		break;
	case SDL::Value::Type::FLOAT:
	case SDL::Value::Type::DOUBLE:
		node = doc.allocate_node(rapidxml::node_type::node_element, "real");
		node->value(doc.allocate_string(value.asString().c_str()));
		break;
	case SDL::Value::Type::BOOLEAN:
		node = doc.allocate_node(rapidxml::node_type::node_element, doc.allocate_string(value.asString().c_str()));
		break;
	case SDL::Value::Type::STRING:
		node = doc.allocate_node(rapidxml::node_type::node_element, "string");
		node->value(doc.allocate_string(value.asString().c_str()));
		break;
	case SDL::Value::Type::VECTOR:
		node = this->generateElementForArray(value.asValueVector(), doc);
		break;
	case SDL::Value::Type::MAP:
		node = this->generateElementForDict(value.asValueMap(), doc);
		break;
	default:
		break;
	}
	return node;
}
NS_SDL_END
