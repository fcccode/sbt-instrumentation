#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <llvm/Support/raw_os_ostream.h>
#include "instr_plugin.hpp"

class GlobalsAnalysisPlugin : public InstrPlugin{
	public:
	 GlobalsAnalysisPlugin(llvm::Module* module) {
		 llvm::errs() << "Running analysis on global variables...\n";
	 }
};

extern "C" InstrPlugin* create_object(llvm::Module* module){
    return new GlobalsAnalysisPlugin(module);
}
