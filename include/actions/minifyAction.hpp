#pragma once
#include <clang/Frontend/FrontendAction.h>
#include <clang/Rewrite/Core/Rewriter.h>
#include <clang/Tooling/Tooling.h>
#include <clang/Tooling/Core/Replacement.h>
#include <memory>
#include <set>
#include <string>
using namespace llvm;
class MinifierAction : public clang::ASTFrontendAction
{
private:
    clang::tooling::Replacements *replacements;
    std::set<std::string> *definitions;

public:
    MinifierAction(clang::tooling::Replacements *replacements, std::set<std::string> *definitions);
    virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &compiler,
                                                                  llvm::StringRef inFile) override;
    /**
     * @brief
     *
     * @param replacements out
     * @return std::unique_ptr<clang::tooling::FrontendActionFactory>
     */
    static std::unique_ptr<clang::tooling::FrontendActionFactory> newMinifierAction(clang::tooling::Replacements *replacements, std::set<std::string> *definitions);
};
