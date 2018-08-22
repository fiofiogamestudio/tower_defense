#include "Info.h"

Info::Info()
{
}

Info::~Info()
{
}

Info * Info::create(ValueVector vec_name,std::string path)
{
	Info* info = new Info();
	info->vec_name = vec_name;
	info->path_load = path;
	return info;
}

ValueVector Info::GetIntInfoVectorByID(int index)
{
	std::string key = vec_name.at(index).asString();
	return ReaderJson::LoadJsonIntFromFile(path_load, key);
}

ValueVector Info::GetStringInfoVectorByID(int index)
{
	std::string key = vec_name.at(index).asString();
	return ReaderJson::LoadJsonStringFromFile(path_load, key);
}
