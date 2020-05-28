#include "CommandList.hpp"

Travel::CommandList Travel::CommandList::globalCommandList{};

void Travel::CommandList::copy(CommandList const &other) {
    nameToToken = other.nameToToken;
    commandMap = other.commandMap;
    nameToDescr = other.nameToDescr;
}

Travel::CommandList::CommandList() {
    nameToToken = {};
    commandMap = {};
    nameToDescr = {};
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

void Travel::CommandList::registerCommand(SimpleString name, TokenType tokenType,
                                         ScannerContext ctx,
                                         SimpleString descr) {
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

bool Travel::CommandList::isCommand(SimpleString name){
    return nameToToken.contains(name);
}

std::unique_ptr<Nullable<ScannerContext>> Travel::CommandList::contextFor(TokenType tokenType){
    return commandMap.get(tokenType);
}

std::unique_ptr<Nullable<TokenType>> Travel::CommandList::tokenFor(SimpleString name) {
    return nameToToken.get(name);
}
