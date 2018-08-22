#include "ReaderJson.h"

ValueVector ReaderJson::LoadJsonIntFromFile(std::string path,std::string key)
{
	ValueVector vec_info;

	rapidjson::Document doc;
	std::string data = FileUtils::getInstance()->getStringFromFile(path);
	doc.Parse<0>(data.c_str());
	const rapidjson::Value& arr = doc[key.c_str()];
	if (arr.IsArray()) {
		for (unsigned int i = 0; i < arr.Size(); ++i) {
			const rapidjson::Value &val = arr[i];
			vec_info.push_back(Value(val.GetInt()));
		}

	}
	return vec_info;
}

ValueVector ReaderJson::LoadJsonStringFromFile(std::string path, std::string key)
{
	ValueVector vec_info;

	rapidjson::Document doc;
	std::string data = FileUtils::getInstance()->getStringFromFile(path);
	doc.Parse<0>(data.c_str());
	const rapidjson::Value& arr = doc[key.c_str()];
	if (arr.IsArray()) {
		for (unsigned int i = 0; i < arr.Size(); ++i) {
			const rapidjson::Value &val = arr[i];
			std::string s = std::string(val.GetString());
			vec_info.push_back(Value(s));
		}

	}
	return vec_info;
}
