#include <iostream>
#include "debug.hpp"
#include "parser/Scanner.hpp"
#include "parser/Token.hpp"
#include "parser/HTLInterpreter.hpp"
#include "collection/Tuple.hpp"
#include <csignal>

void signalHandler( int signum );


int main(){

    std::signal(SIGINT, signalHandler);

    Travel::CommandList cl;

    cl.registerCommand("exit",     TokenType::EXIT,     ScannerContext::ALL,
                       "Exit the program.");
    cl.registerCommand("help",     TokenType::HELP,     ScannerContext::ALL,
                       "Print this help message.");
    cl.registerCommand("friend",   TokenType::FRIEND,   ScannerContext::GENERAL,
                       "friend <command> <args> => "
                       "prefix for all friend commands");
    cl.registerCommand("add",      TokenType::ADD,      ScannerContext::GENERAL,
                       "friend add <name>"
                       "=> add user <name> to friend's list");
    cl.registerCommand("remove",   TokenType::REMOVE,   ScannerContext::GENERAL,
                       "friend remove <name>"
                       "=> remove user <name> from friend's list");
    cl.registerCommand("list",     TokenType::LIST,     ScannerContext::GENERAL,
                       "friend list"
                       "=> lists friends.");
    cl.registerCommand("visited",  TokenType::VISITED,  ScannerContext::GENERAL,
                       "friend visited <destination>"
                       "=> show all friends who have visited <destination>"
                       ", along with their comments and ratings.\n"
                       "friend visited <destination> <user>"
                       "=> show if friend <user> has visited <destination>"
                       ", and if they have: Their comment and rating.\n"
                       "destination visited <destination> <user>"
                       "=> show if <user> has visited <destination>"
                       ", and if they have: their comment and destination");
    cl.registerCommand("visit",    TokenType::VISIT,    ScannerContext::GENERAL,
                       "visit <destination>"
                       "=> visit <destination>");

    cl.registerCommand("from",     TokenType::FROM,     ScannerContext::VISIT,
                       "from <date>"
                       "=> mark the beginning of your visit as <date>");
    cl.registerCommand("to",       TokenType::TO,       ScannerContext::VISIT,
                       "to <date>"
                       "=> mark the end of your visit as <date>");
    cl.registerCommand("rate",     TokenType::RATE,     ScannerContext::VISIT,
                       "rate <rating : 1 - 5>"
                       "=> rate your visit with a <rating> between 1 and 5");
    cl.registerCommand("comment",  TokenType::COMMENT,  ScannerContext::VISIT,
                       "comment <comment>"
                       "=> add comment <comment> to your visit");
    cl.registerCommand("photo",    TokenType::PHOTO,    ScannerContext::VISIT,
                       "photo"
                       "=> prefix for photo commands>");
    cl.registerCommand("upload",   TokenType::UPLOAD,   ScannerContext::VISIT,
                       "photo upload <uri>"
                       "=> upload photo with path <uri>");
    cl.registerCommand("delete",   TokenType::DELETE,   ScannerContext::VISIT,
                       "photo delete <uri>"
                       "=> delete photo with path <uri>");
    cl.registerCommand("show",     TokenType::SHOW,     ScannerContext::VISIT,
                       "photo show"
                       "=> show uploaded photos>");
    cl.registerCommand("save",     TokenType::SAVE,     ScannerContext::VISIT,
                       "save"
                       "=> save the information for this visit and return to"
                       " the main menu");
    cl.registerCommand("cancel",   TokenType::CANCEL,   ScannerContext::VISIT,
                       "cancel"
                       "=> cancel the information for this visit and return to"
                       " the main menu");

    cl.registerCommand("destination", TokenType::DESTINATION,
                       ScannerContext::GENERAL,
                       "prefix for all friend commands");
    cl.registerCommand("exists",      TokenType::EXISTS,
                       ScannerContext::GENERAL,
                       "destination exists <destination> "
                       "=> Check if <destination> exists in the database.");
    cl.registerCommand("rating",      TokenType::RATING,
                       ScannerContext::GENERAL,
                       "destination rating <destination>"
                       " => Show an average of all ratings for <destination>");
    cl.registerCommand("comments",      TokenType::COMMENTS,
                       ScannerContext::GENERAL,
                       "destination comments <destination>"
                       " => Show all comments for <destination>");


    Travel::CommandList::setCommandList(&cl);
    //cl.printCommandsWithDescription();

    Travel::Scanner sc(ScannerContext::ALL, &std::cin, &cl);

    Travel::TravelState state{};
    bool run = true;
    while (run){
        auto tokens = sc.scanNext();
        //tokens->foreach([](Token const& t){ std::cout<<t<<std::endl;});
        Travel::HTLInterpreter interpreter{tokens};
        run = interpreter.parse(state);
    }
    return 0;
}



void signalHandler(int signum){
    LOG(INFO, "Recieved interupt signal: "<<signum );
    exit(signum);
}
