#ifndef REWRITER_H
#define REWRITER_H

#include <list>
#include <string>
#include <iostream>
#include <fstream>


// Configuration
enum class InstrumentPlacement {
	BEFORE,
	AFTER,
	REPLACE
};

class InstrumentInstruction {
 public:
	std::string returnValue;
	std::string instruction;
	std::list<std::string> parameters;
	std::string getSize;
	std::string getAllocatedTypeSize;
	std::string stripInboundsOffsets;
};

class InstrumentGlobalVar {
 public:
	std::string globalVariable;
	std::string getAllocatedTypeSize;
};

class GlobalVarsRule {
 public:
	InstrumentGlobalVar globalVar;
	InstrumentInstruction newInstr;
	std::string inFunction;
	std::list<std::list<std::string>> conditions;
};

typedef std::list<InstrumentInstruction> InstrumentSequence;

class RewriteRule {
 public:
	InstrumentSequence foundInstrs;
	InstrumentInstruction newInstr;
	InstrumentPlacement where;
	std::string inFunction;
	std::list<std::list<std::string>> conditions;
};

typedef std::list<RewriteRule> RewriterConfig;

// Rewriter
class Rewriter {
	RewriterConfig config;
	GlobalVarsRule globalVarsRule;
	public:
		RewriterConfig getConfig();
		GlobalVarsRule getGlobalsConfig();
		void parseConfig(std::ifstream &config_file);
		std::list<std::string> analysisPaths;
};

#endif
