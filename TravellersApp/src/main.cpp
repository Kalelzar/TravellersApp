#include <iostream>
#include "debug.hpp"
#include "parser/Scanner.hpp"
#include "parser/Token.hpp"
#include "parser/HTLInterpreter.hpp"
#include "parser/LoginInterpreter.hpp"
#include "collection/Tuple.hpp"
#include <csignal>
#include "parser/CSV.hpp"

void signalHandler( int signum );

int main(){

    std::signal(SIGINT, signalHandler);

    typename forType<const char*>::repeat<4>::apply<CSV>
        csv("username", "password", "email", "stuff");

    csv.load("test.csv");
    csv.load("test2.csv");
    csv.save("test.csv");

    // const int ind = csv.header().indexOf("key");
    // auto i = csv.getRow(0).template get<ind>();
    // std::cout<<i<<std::endl;

    // Travel::CommandList lcl;
    // lcl.registerCommand("login",    TokenType::LOGIN,    ScannerContext::ATOPEN,
    //                     "login <user> <password>.");
    // lcl.registerCommand("register", TokenType::REGISTER, ScannerContext::ATOPEN,
    //                     "register <email> <user> <password>.");

    // Travel::CommandList::setCommandList(&lcl);

    // Travel::CommandList cl;
    // cl.registerCommand("exit",     TokenType::EXIT,     ScannerContext::ALL,
    //                    "Exit the program.");
    // cl.registerCommand("help",     TokenType::HELP,     ScannerContext::ALL,
    //                    "Print this help message.");
    // cl.registerCommand("friend",   TokenType::FRIEND,   ScannerContext::GENERAL,
    //                    "friend <command> <args> => "
    //                    "prefix for all friend commands");
    // cl.registerCommand("add",      TokenType::ADD,      ScannerContext::GENERAL,
    //                    "friend add <name> "
    //                    "=> add user <name> to friend's list");
    // cl.registerCommand("remove",   TokenType::REMOVE,   ScannerContext::GENERAL,
    //                    "friend remove <name> "
    //                    "=> remove user <name> from friend's list");
    // cl.registerCommand("list",     TokenType::LIST,     ScannerContext::GENERAL,
    //                    "friend list "
    //                    "=> lists friends.");
    // cl.registerCommand("visited",  TokenType::VISITED,  ScannerContext::GENERAL,
    //                    "friend visited <destination> "
    //                    "=> show all friends who \nhave visited <destination>, "
    //                    "along with their comments and ratings.\n"
    //                    "friend visited <destination> <user> "
    //                    "=> show if friend <user> \nhas visited <destination>"
    //                    ", and if they have: Their comment and rating.\n"
    //                    "destination visited <destination> <user> "
    //                    "=> show if <user> \nhas visited <destination>"
    //                    ", and if they have: their comment and destination");
    // cl.registerCommand("visit",    TokenType::VISIT,    ScannerContext::GENERAL,
    //                    "visit <destination> "
    //                    "=> visit <destination>");

    // cl.registerCommand("from",     TokenType::FROM,     ScannerContext::VISIT,
    //                    "from <date> "
    //                    "=> mark the beginning of your visit as <date>");
    // cl.registerCommand("to",       TokenType::TO,       ScannerContext::VISIT,
    //                    "to <date> "
    //                    "=> mark the end of your visit as <date>");
    // cl.registerCommand("rate",     TokenType::RATE,     ScannerContext::VISIT,
    //                    "rate <rating : 1 - 5> "
    //                    "=> rate your visit with a <rating> between 1 and 5");
    // cl.registerCommand("comment",  TokenType::COMMENT,  ScannerContext::VISIT,
    //                    "comment <comment> "
    //                    "=> add comment <comment> to your visit");
    // cl.registerCommand("photo",    TokenType::PHOTO,    ScannerContext::VISIT,
    //                    "photo "
    //                    "=> prefix for photo commands>");
    // cl.registerCommand("upload",   TokenType::UPLOAD,   ScannerContext::VISIT,
    //                    "photo upload <uri> "
    //                    "=> upload photo with path <uri>");
    // cl.registerCommand("delete",   TokenType::DELETE,   ScannerContext::VISIT,
    //                    "photo delete <uri> "
    //                    "=> delete photo with path <uri>");
    // cl.registerCommand("show",     TokenType::SHOW,     ScannerContext::VISIT,
    //                    "photo show"
    //                    "=> show uploaded photos>");
    // cl.registerCommand("save",     TokenType::SAVE,     ScannerContext::VISIT,
    //                    "save "
    //                    "=> save the information for this visit and return to"
    //                    " the main menu");
    // cl.registerCommand("cancel",   TokenType::CANCEL,   ScannerContext::VISIT,
    //                    "cancel "
    //                    "=> cancel the information for this visit \n"
    //                    "and return to the main menu");

    // cl.registerCommand("destination", TokenType::DESTINATION,
    //                    ScannerContext::GENERAL,
    //                    "prefix for all friend commands");
    // cl.registerCommand("exists",      TokenType::EXISTS,
    //                    ScannerContext::GENERAL,
    //                    "destination exists <destination>\n"
    //                    "=> Check if <destination> exists in the database.");
    // cl.registerCommand("rating",      TokenType::RATING,
    //                    ScannerContext::GENERAL,
    //                    "destination rating <destination>\n"
    //                    "=> Show an average of all ratings for <destination>");
    // cl.registerCommand("comments",      TokenType::COMMENTS,
    //                    ScannerContext::GENERAL,
    //                    "destination comments <destination>\n"
    //                    "=> Show all comments for <destination>");

    // Travel::TravelState state{};
    // bool run = true;

    // Travel::Scanner lsc(ScannerContext::ATOPEN, &std::cin, &lcl);

    // while (run){
    //     std::cout<<"login$ ";
    //     auto tokens = lsc.scanNext();
    //     /*tokens->foreach([](Token const& t){ std::cout<<t<<std::endl;});*/
    //     Travel::LoginInterpreter interpreter{tokens};
    //     run = interpreter.parse(state);
    // }

    // Travel::CommandList::setCommandList(&cl);
    // cl.printCommandsWithDescription();

    // Travel::Scanner sc(ScannerContext::ALL, &std::cin, &cl);

    // run = true;
    // while (run){
    //     std::cout<<"$ ";
    //     auto tokens = sc.scanNext();
    //     /*tokens->foreach([](Token const& t){ std::cout<<t<<std::endl;});*/
    //     Travel::HTLInterpreter interpreter{tokens};
    //     run = interpreter.parse(state);
    // }
    return 0;
}



void signalHandler(int signum){
    LOG(INFO, "Recieved interupt signal: "<<signum );
    exit(signum);
}
