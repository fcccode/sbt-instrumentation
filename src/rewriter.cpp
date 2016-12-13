#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include "rewriter.hpp"
#include "json/json.h"

using namespace std;

void Rewriter::parseConfig(ifstream &config_file) {
	Json::Value json_rules;
	Json::Reader reader;
	bool parsingSuccessful = reader.parse(config_file, json_rules);
	if (!parsingSuccessful)	{
		cerr  << "Failed to parse configuration\n"
			  << reader.getFormattedErrorMessages();
		throw runtime_error("Config parsing failure.");
	}

	// load paths to analyses
	for(uint i = 0; i < json_rules["analyses"].size(); ++i){
		this->analysisPaths.push_back(json_rules["analyses"][i].asString());
	}

	RewriterConfig rw_config;

	// load rewrite rules for instructions
	for (uint i = 0; i < json_rules["instructionRules"].size(); ++i) {
		RewriteRule r;

		// TODO make function from this
		// Get findInstructions
		for (uint k = 0; k < json_rules["instructionRules"][i]["findInstructions"].size(); ++k) {
			InstrumentInstruction instr;
			instr.returnValue = json_rules["instructionRules"][i]["findInstructions"][k]["returnValue"].asString();
			instr.instruction = json_rules["instructionRules"][i]["findInstructions"][k]["instruction"].asString();
			for (uint j = 0; j < json_rules["instructionRules"][i]["findInstructions"][k]["operands"].size(); ++j) {
				instr.parameters.push_back(json_rules["instructionRules"][i]["findInstructions"][k]["operands"][j].asString());
			}
			instr.getSize = json_rules["instructionRules"][i]["findInstructions"][k]["getSize"].asString();
			instr.getAllocatedTypeSize = json_rules["instructionRules"][i]["findInstructions"][k]["getAllocatedTypeSize"].asString();
			instr.stripInboundsOffsets = json_rules["instructionRules"][i]["findInstructions"][k]["stripInboundsOffsets"].asString();
			r.foundInstrs.push_back(instr);
		}

		// Get newInstruction
		r.newInstr.returnValue = json_rules["instructionRules"][i]["newInstruction"]["returnValue"].asString();
		r.newInstr.instruction = json_rules["instructionRules"][i]["newInstruction"]["instruction"].asString();
		for (uint j = 0; j < json_rules["instructionRules"][i]["newInstruction"]["operands"].size(); ++j) {
			r.newInstr.parameters.push_back(json_rules["instructionRules"][i]["newInstruction"]["operands"][j].asString());
		}

		if (json_rules["instructionRules"][i]["where"] == "before") {
			r.where = InstrumentPlacement::BEFORE;
		}
		else if (json_rules["instructionRules"][i]["where"] == "after") {
			r.where = InstrumentPlacement::AFTER;
		}
		else if (json_rules["instructionRules"][i]["where"] == "replace") {
			r.where = InstrumentPlacement::REPLACE;
		}

		r.inFunction = json_rules["instructionRules"][i]["in"].asString();

		// Get conditions for instruction
		for(uint j = 0; j < json_rules["instructionRules"][i]["conditions"].size(); ++j){
			list<string> condition;
			for(uint k = 0; k < json_rules["instructionRules"][i]["conditions"][j].size(); k++){
				condition.push_back(json_rules["instructionRules"][i]["conditions"][j][k].asString());
			}
			r.conditions.push_back(condition);
		}

		rw_config.push_back(r);
	}

	this->config = rw_config;

	GlobalVarsRule rw_globals_rule;

	// Get rule for global variables
	rw_globals_rule.globalVar.globalVariable = json_rules["globalVariablesRule"]["findGlobals"]["globalVariable"].asString();
	rw_globals_rule.globalVar.getAllocatedTypeSize = json_rules["globalVariablesRule"]["findGlobals"]["getAllocatedTypeSize"].asString();

	rw_globals_rule.newInstr.returnValue = json_rules["globalVariablesRule"]["newInstruction"]["returnValue"].asString();
	rw_globals_rule.newInstr.instruction = json_rules["globalVariablesRule"]["newInstruction"]["instruction"].asString();

	for (uint j = 0; j < json_rules["globalVariablesRule"]["newInstruction"]["operands"].size(); ++j) {
		rw_globals_rule.newInstr.parameters.push_back(json_rules["globalVariablesRule"]["newInstruction"]["operands"][j].asString());
	}

	// Get conditions for instrumenting global variables
	for(uint j = 0; j < json_rules["globalVariablesRule"]["conditions"].size(); ++j){
		list<string> condition;
		for(uint k = 0; k < json_rules["globalVariablesRule"]["conditions"][j].size(); k++){
			condition.push_back(json_rules["globalVariablesRule"]["conditions"][j][k].asString());
		}
		rw_globals_rule.conditions.push_back(condition);
	}

	rw_globals_rule.inFunction = json_rules["globalVariablesRule"]["in"].asString();

	this->globalVarsRule = rw_globals_rule;
}

RewriterConfig Rewriter::getConfig() {
	return this->config;
}

GlobalVarsRule Rewriter::getGlobalsConfig() {
	return this->globalVarsRule;
}
