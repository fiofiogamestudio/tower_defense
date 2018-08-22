#include "ReaderJson.h"

ValueVector ReaderJson::LoadJsonFromFile(std::string path,std::string key)
{
	ValueVector vec_info;

	rapidjson::Document doc;
	std::string data = FileUtils::getInstance()->getStringFromFile(path);
	doc.Parse<0>(data.c_str());
	const rapidjson::Value& arr = doc[key.c_str()];
	if (arr.IsArray()) {
		for (unsigned int i = 0; i < arr.Size(); ++i) {
			const rapidjson::Value &val = arr[i];
			log("%d", val.GetInt());
			vec_info.push_back(Value(val.GetInt()));
		}

	}
	return vec_info;
}
