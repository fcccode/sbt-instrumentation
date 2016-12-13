#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <llvm/Support/raw_os_ostream.h>
#include "instr_plugin.hpp"
#include <llvm/IR/Instructions.h>

class GlobalsAnalysisPlugin : public InstrPlugin{
	private:
	 llvm::Module* M;
	public:
	 GlobalsAnalysisPlugin(llvm::Module* module) {
		M = module;
		// llvm::errs() << "Running analysis on global variables...\n";
	 }

	 bool isGlobal(llvm::Value* variable){
		llvm::Module::global_iterator GI = M->global_begin(), GE = M->global_end();
		for ( ; GI != GE; ++GI) {
		  llvm::GlobalVariable *GV = llvm::dyn_cast<llvm::GlobalVariable>(GI);
		  if (!GV) continue;
		  if(GV == variable->stripPointerCasts()->stripInBoundsOffsets() || GV == variable){
		    return true;
		  }
		}
	   
		 //if(!(llvm::dyn_cast<llvm::GlobalVariable>(variable)))
			  //return false;

		 return false;
	 }

	 bool isUsed(llvm::Value *variable){
		 // Check if variable is global variable
		 if(!(llvm::dyn_cast<llvm::GlobalVariable>(variable))){
			 return false;
		 }

		 // variable is not used anywhere
		 if(variable->getNumUses() == 0){
			 return false;
		 }
		 else{
			 for (auto I = variable->use_begin(), E = variable->use_end(); I != E; ++I) {
				#if (LLVM_VERSION_MINOR < 5)
					const llvm::Value *use = *I;
				#else
					const llvm::Value *use = I->getUser();
				#endif
				const llvm::StoreInst *SI = llvm::dyn_cast<llvm::StoreInst>(use);
				if (SI && SI->getValueOperand()->stripPointerCasts() == variable) {
					return true;
				}
				const llvm::LoadInst *LI = llvm::dyn_cast<llvm::LoadInst>(use);
				if(LI && LI->getOperand(0)->stripPointerCasts() == variable){
				  return true;
				}
			}
		 }

		 return false;
	 }
};

extern "C" InstrPlugin* create_object(llvm::Module* module){
    return new GlobalsAnalysisPlugin(module);
}
