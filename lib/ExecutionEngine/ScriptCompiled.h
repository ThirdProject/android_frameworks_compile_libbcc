/*
 * Copyright 2010, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef BCC_SCRIPTCOMPILED_H
#define BCC_SCRIPTCOMPILED_H

#include "Compiler.h"

#include <bcc/bcc.h>

#include <list>
#include <map>
#include <string>
#include <utility>
#include <vector>

namespace llvm {
  class Module;
}

namespace bcc {
  class Script;

  class ScriptCompiled {
    friend class Compiler;
    friend class CodeEmitter;

  private:
    typedef std::list<std::pair<std::string, std::string> > PragmaList;
    typedef std::list<void*> ExportVarList;
    typedef std::list<void*> ExportFuncList;
    typedef std::map<std::string, FuncInfo *> FuncInfoMap;
    typedef std::list<uint32_t> ObjectSlotList;

  private:
    Script *mpOwner;

    Compiler mCompiler;

    ExportVarList mExportVars;
    ExportFuncList mExportFuncs;
    PragmaList mPragmas;
    ObjectSlotList mObjectSlots;

    FuncInfoMap mEmittedFunctions;

    char *mContext; // Context of BCC script (code and data)

  public:
    ScriptCompiled(Script *owner)
      : mpOwner(owner), mCompiler(this), mContext(NULL) {
    }

    ~ScriptCompiled();

    void setCachePath(char const *cachePath) {
      mCompiler.setCachePath(cachePath);
      return;
    }

    llvm::Module *parseBitcodeFile(llvm::MemoryBuffer *MEM) {
      return mCompiler.parseBitcodeFile(MEM);
    }

    int readModule(llvm::Module *module) {
      return mCompiler.readModule(module);
    }

    int linkModule(llvm::Module *module) {
      return mCompiler.linkModule(module);
    }

    int compile() {
      return mCompiler.compile();
    }

    char const *getCompilerErrorMessage() {
      return mCompiler.getErrorMessage();
    }

    void *lookup(const char *name);


    size_t getExportVarCount() const {
      return mExportVars.size();
    }

    size_t getExportFuncCount() const {
      return mExportFuncs.size();
    }

    size_t getPragmaCount() const {
      return mPragmas.size();
    }

    size_t getFuncCount() const {
      return mEmittedFunctions.size();
    }

    size_t getObjectSlotCount() const {
      return mObjectSlots.size();
    }

    void getExportVarList(size_t varListSize, void **varList);

    void getExportFuncList(size_t funcListSize, void **funcList);

    void getPragmaList(size_t pragmaListSize,
                       char const **keyList,
                       char const **valueList);

    void getFuncInfoList(size_t funcInfoListSize,
                         FuncInfo *funcInfoList);

    void getObjectSlotList(size_t objectSlotListSize,
                           uint32_t *objectSlotList);

    char *getContext() {
      return mContext;
    }
#if USE_MCJIT
    const char *getELF() const {
      return &*mCompiler.getELF().begin();
    }

    size_t getELFSize() const {
      return mCompiler.getELF().size();
    }
#endif

    void registerSymbolCallback(BCCSymbolLookupFn pFn, void *pContext) {
      mCompiler.registerSymbolCallback(pFn, pContext);
    }
  };

} // namespace bcc

#endif // BCC_SCRIPTCOMPILED_H
