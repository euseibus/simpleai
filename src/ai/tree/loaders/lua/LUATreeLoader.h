#pragma once

#include <common/Compiler.h>
#include <tree/loaders/ITreeLoader.h>
#include <string>
#include <map>
#include <vector>

struct LUA;

namespace ai {

class LUATreeLoader: public ai::ITreeLoader {
private:
	LUA *_lua;
public:
	LUATreeLoader(const IAIFactory& aiFactory);
	bool init(const std::string& luaString);
	virtual ~LUATreeLoader();
};

}
