#include "CommandList.hpp"
#include "../debug.hpp"

Travel::CommandList* Travel::CommandList::globalCommandList = nullptr;

void Travel::CommandList::copy(CommandList const &other) {
    LOG(INFO, "Copying CommandList instance.");
    nameToToken = other.nameToToken;
    commandMap = other.commandMap;
    nameToDescr = other.nameToDescr;
}

Travel::CommandList::CommandList() {
    LOG(INFO, "Create CommandList.");
}

Travel::CommandList::CommandList(CommandList const &other){
    if (&other != this) {
        copy(other);
    }
}

Travel::CommandList &Travel::CommandList::operator=(Travel::CommandList const &other){
    if (&other != this) {
        copy(other);
    }
    return *this;
}

void Travel::CommandList::registerCommand(SimpleString const& name, TokenType tokenType,
                                         ScannerContext ctx,
                                         const char* descr) {
    LOG(INFO, "Registering command: " << name);
    nameToDescr.put(name, descr);
    commandMap.put(tokenType, ctx);
    nameToToken.put(name, tokenType);
    LOG(INFO, nameToDescr.length() << " commands registered.");
}

void Travel::CommandList::printCommandsWithDescription(){
    std::cout<<"Simple Command-line interface for Travel Management"<<std::endl;
    auto keys = nameToDescr.keys();
    auto values = nameToDescr.values();
    // using Pair = Tuple<SimpleString, const char*>;
    // auto keysAndValues = keys->zip(*values)
    //     ->sort([](Tuple<SimpleString, const char*> const& t1,
    //               Tuple<SimpleString, const char*> const& t2){
    //                return t1.head() > t2.head;
    //            });
    for(int i = 0; i < keys->length(); i++){
        std::cout<<"["<<keys->get(i)<<"] "
                 <<values->get(i)<<std::endl;
    }
}

bool Travel::CommandList::isCommand(TokenType tokenType){
    return commandMap.contains(tokenType);
}

bool Travel::CommandList::isCommand(SimpleString const& name){
    return nameToToken.contains(name);
}

unique_ptr<Nullable<ScannerContext>> Travel::CommandList::contextFor
(TokenType tokenType){
    LOG(VERBOSE, "Get context for token type");
    return commandMap.get(tokenType);
}

unique_ptr<Nullable<TokenType>> Travel::CommandList::tokenFor
(SimpleString const& name) {
    LOG(VERBOSE, "Get token type for command.");
    return nameToToken.get(name);
}
