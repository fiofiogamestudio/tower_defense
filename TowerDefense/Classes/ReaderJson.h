#pragma once
#include "cocos2d.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/filereadstream.h"
using namespace cocos2d;
class ReaderJson {
public:
	static ValueVector LoadJsonIntFromFile(std::string path,std::string key);
	static ValueVector LoadJsonStringFromFile(std::string path, std::string key);
};