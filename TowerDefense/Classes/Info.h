#pragma once
#include "cocos2d.h"
#include "ReaderJson.h"
using namespace cocos2d;
class Info {
private:
	ValueVector vec_name;
	std::string path_load;
public:
	Info();
	~Info();
	static Info* create(ValueVector vec_name,std::string path);

	ValueVector GetInfoVectorByID(int index);
};
