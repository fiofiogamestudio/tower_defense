#include "ReaderJson.h"

Vector<std::string> ReaderJson::LoadJsonFromFile(std::string path,std::string key)
{
	std::string path = path;
	rapidjson::Document doc;
	const rapidjson::Value& arr = doc[key];
	if (arr.IsArray()) {
		for (unsigned int i = 0; i < arr.Size(); i++) {
			const rapidjson::Value& value = arr[i];
			log("%d", value.GetInt());
		}
	}
}
