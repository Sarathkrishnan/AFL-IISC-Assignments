#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/CallingConv.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/DebugInfo.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/User.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/IR/LegacyPassManager.h"
#include <stdint.h>
#include<stdio.h>
#include <unistd.h>

using namespace llvm;

namespace {
    struct SymbiosisBBPass : public ModulePass {
        static char ID;
        static int bbcount;
        Constant *InstFuncConst;
        Constant* MyPTCreateConst;
	Constant* MyAssertConst;
        const Type* pthreadTy;
	static bool bbAfterAssert;
	static bool flagAssert; 
        
        SymbiosisBBPass() : ModulePass(ID){}
        
	
        virtual bool runOnModule(Module &M)
        {
           LLVMContext &Context = M.getContext();
	IntegerType *Int8Ty  = IntegerType::getInt8Ty(Context);
  	IntegerType *Int16Ty = IntegerType::getInt16Ty(Context);
	IntegerType *Int32Ty = IntegerType::getInt32Ty(Context);
	GlobalVariable* gvar_ptr_abc = new GlobalVariable(M, Int32Ty,false,GlobalValue::CommonLinkage,0,"temp");
	gvar_ptr_abc->setAlignment(4);
	PointerType *p = PointerType::get(M.getTypeByName("struct.c_buffo_data"),0);
	Value *cbd_ptr = M.getGlobalVariable("cbd_ptr",p);
	StructType *s = StructType::get(Int16Ty,Int8Ty,Int8Ty,NULL);
	Value *c_buffo_data = M.getOrInsertGlobal("struct.c_buffo_data",s);

	/*Value *cbd_ptr_32 =*/ M.getOrInsertGlobal("cbd_ptr_u32",PointerType::get(Int32Ty, 0));

		for (Module::iterator F = M.begin(), E = M.end(); F != E; ++F) {
                if (F->isDeclaration()) continue;
                for (Function::iterator BB = F->begin(), E = F->end(); BB != E; ++BB) {
                    BasicBlock& B = *BB;
                    SymbiosisBBPass::runOnBasicBlock(B,gvar_ptr_abc,cbd_ptr,p);
		     if(flagAssert)
			bbAfterAssert = true;
                }
            }
            
            return true;
        }
        
        virtual bool runOnBasicBlock(BasicBlock& BB,GlobalVariable* gvar_ptr_abc, Value* cbd_ptr,PointerType *p) {
            
            LLVMContext &Context = BB.getContext();
            bbcount++;
	    IntegerType *Int8Ty  = IntegerType::getInt8Ty(Context);
  	    IntegerType *Int16Ty = IntegerType::getInt16Ty(Context);
		IntegerType *Int32Ty = IntegerType::getInt32Ty(Context);
            for (BasicBlock::iterator I = BB.begin(), E = BB.end(); I != E; ++I)
            {	if(ICmpInst *II = dyn_cast<ICmpInst>(I)){
					Value *Op0 = II->getOperand(0);
					Value *Op1 = II->getOperand(1);	
		Value *Const_value = llvm::ConstantInt::get(Type::getInt32Ty(Context),128);
		BinaryOperator *v1 = BinaryOperator::Create(Instruction::Mul, Op0,Const_value,"",II);
		BinaryOperator *v2 = BinaryOperator::Create(Instruction::SDiv, v1, Op1,"",II);
		BinaryOperator *v3 = BinaryOperator::Create(Instruction::Sub, Const_value, v2,"",II);	
		LoadInst *ll = new LoadInst(gvar_ptr_abc, "", false,II);
		/*StoreInst* m =*/ new StoreInst(gvar_ptr_abc, v3, II);
		LoadInst *l = new LoadInst(cbd_ptr, "", false,II);
		l->setAlignment(8);
		//llvm::errs() << "CREATING VALUES! " << "\n";
		Value *cval = llvm::ConstantInt::get(Int32Ty,2);
		//llvm::errs() << "CREATING VALUES - 1 ! " << "\n";
		Value *base = llvm::ConstantInt::get(p,APInt(32,0));
             	Value* indexList[2] = {base,cval};
                //llvm::errs() << *indexList[0] << "\n";
                //llvm::errs() << *indexList[1] << "\n";
		//llvm::errs() << "cbd ptr " << cbd_ptr << "\n";
		//llvm::errs() << "CREATING VALUES - 2 ! " << "\n";
		//llvm::errs() << "Pointer operand " << l->getPointerOperand() << "\n";
		llvm::GetElementPtrInst *p = llvm::GetElementPtrInst::CreateInBounds(l,ArrayRef<Value *>(indexList,2),"cval",II);
		//llvm::errs() << "CREATING VALUES - 3 ! " << "\n";
		}
                if (CallInst *CI = dyn_cast<CallInst>(I))
                {
                    Function *FUNC = CI->getCalledFunction();
                   //llvm::errs() << "function call: " << FUNC->getName() << "\n";
                   if(FUNC && FUNC->getName() == "__assert_fail")
                    {
			//llvm::errs() << "assert fail!\n";
			SymbiosisBBPass::runOnPreviousBasicBlock(BB,gvar_ptr_abc);
			flagAssert = true;
		    }	
		    else if(flagAssert && bbAfterAssert)
		    {
			//llvm::errs() << "assert success!\n";
			flagAssert = false;
			bbAfterAssert = false;
		    }
                }
            }
            
            return true;
        }

	virtual bool runOnPreviousBasicBlock(BasicBlock& BB,GlobalVariable* gvar_ptr_abc){
		BasicBlock* B = BB.getSinglePredecessor();
		LLVMContext &Context = B->getContext();
		//Value *Op0;
		//Value *Op1;
		TerminatorInst *T = B->getTerminator();
		//unsigned int i=0;
		for (BasicBlock::iterator I = B->begin(), E = B->end(); I != E; ++I)
            	{
			 if (BranchInst *BI = dyn_cast<BranchInst>(T)) {				
				if (BI->isUnconditional()) {
					return false;
				}
			 	else{
					//llvm::errs() << "Trying!\n";
					BasicBlock* Dest = cast<BasicBlock>(BI->getSuccessor(0));
					/*BasicBlock* Assert_Block =*/ cast<BasicBlock>(BI->getSuccessor(1));
					//BranchInst* Old_Inst = BI;
					SymbiosisBBPass::runOnOldblock(*Dest,gvar_ptr_abc);
					BasicBlock* New_Block_1 = BasicBlock::Create(Context,"",B->getParent(),0);
					BI->setSuccessor(1,New_Block_1);
					/*ReturnInst *return_b =*/ReturnInst::Create(Context, New_Block_1);
					SymbiosisBBPass::runOnAssertblock(*New_Block_1,gvar_ptr_abc);
					break; 
					
				} 
			}
		}		
	 return true;
     	}
		virtual bool runOnOldblock(BasicBlock& BB,GlobalVariable* gvar_ptr_abc){
				LLVMContext &Context = BB.getContext();
				//IntegerType *Int32Ty = IntegerType::getInt32Ty(Context);
				//IntegerType *Int8Ty  = IntegerType::getInt8Ty(Context);
				Value *size = llvm::ConstantInt::get(Type::getInt32Ty(Context),31);
		        	TerminatorInst *T = BB.getTerminator();
				 
				Value *Cond = new ICmpInst(T,ICmpInst::ICMP_ULT,gvar_ptr_abc,size, "");
				BasicBlock* New_Block_2 = BasicBlock::Create(Context,"",BB.getParent(),0);
				BasicBlock* New_Block_3 = BasicBlock::Create(Context,"",BB.getParent(),0);
						
				BranchInst *BI = llvm::BranchInst::Create(New_Block_2,New_Block_3,Cond);
				BI->insertBefore(T);
				/*BranchInst *BI_2 =*/ llvm::BranchInst::Create(New_Block_3,New_Block_2);
				/*ReturnInst *return_b =*/ReturnInst::Create(Context, New_Block_3);
					
			
		return true;
		}
		
		virtual bool runOnAssertblock(BasicBlock& BB,GlobalVariable* gvar_ptr_abc){
				LLVMContext &Context = BB.getContext();
				TerminatorInst *T = BB.getTerminator();
				IntegerType *Int32Ty = IntegerType::getInt32Ty(Context);
				AllocaInst* __temp = new AllocaInst(Int32Ty,"cval",T); 
				Value *Const_value = llvm::ConstantInt::get(Type::getInt32Ty(Context),0);	
				/*StoreInst* m =*/ new StoreInst(__temp, Const_value, T);
				
		return true;
		}			

    };
}

char SymbiosisBBPass::ID = 0;
int SymbiosisBBPass::bbcount = 0;
bool SymbiosisBBPass::flagAssert = false;
bool SymbiosisBBPass::bbAfterAssert = false;
static void registerSymbiosisPass(const PassManagerBuilder &,
                            legacy::PassManagerBase &PM) {

  PM.add(new SymbiosisBBPass());

}

static RegisterStandardPasses RegisterSymbiosisPass(
    PassManagerBuilder::EP_OptimizerLast, registerSymbiosisPass);

static RegisterStandardPasses RegisterSymbiosisPass0(
    PassManagerBuilder::EP_EnabledOnOptLevel0, registerSymbiosisPass);
