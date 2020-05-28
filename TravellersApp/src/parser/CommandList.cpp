#include "CommandList.hpp"
#include "../debug.hpp"

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
                                         SimpleString const& descr) {
    LOG(INFO, "Registering command: " << name);
    commandMap.put(tokenType, ctx);
    nameToToken.put(name, tokenType);
    nameToDescr.put(name, descr);
}

void Travel::CommandList::printCommandsWithDescription(){
    std::cout<<"Simple Command-line interface for Travel Management"<<std::endl;
    std::unique_ptr<ArrayList<SimpleString>> keys = nameToDescr.keys();
    std::unique_ptr<ArrayList<SimpleString>> values = nameToDescr.values();
    for(int i = 0; i < keys->length(); i++){
        std::cout<<keys->get(i)<<" "<<values->get(i)<<std::endl;
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
