#include "collection/Tuple.hpp"
#include "debug.hpp"
#include "parser/CSV.hpp"
#include "parser/HTLInterpreter.hpp"
#include "parser/LoginInterpreter.hpp"
#include "parser/Scanner.hpp"
#include "parser/Token.hpp"
#include <iostream>

int main() {


  Travel::CommandList lcl;
  lcl.registerCommand("login", TokenType::LOGIN, ScannerContext::ATOPEN,
                      "login <user> <password>."); // DONE
  lcl.registerCommand("register", TokenType::REGISTER, ScannerContext::ATOPEN,
                      "register <email> <user> <password>."); // DONE

  Travel::CommandList::setCommandList(&lcl);

  Travel::CommandList cl;
  cl.registerCommand("exit", TokenType::EXIT, ScannerContext::ALL,
                     "Exit the program."); // DONE
  cl.registerCommand("help", TokenType::HELP, ScannerContext::ALL,
                     "Print this help message."); // DONE
  cl.registerCommand("friend", TokenType::FRIEND, ScannerContext::GENERAL,
                     "friend <command> <args> => " // DONE
                     "prefix for all friend commands");
  cl.registerCommand("add", TokenType::ADD, ScannerContext::GENERAL,
                     "friend add <name> " // DONE
                     "=> add user <name> to friend's list");
  cl.registerCommand("remove", TokenType::REMOVE, ScannerContext::GENERAL,
                     "friend remove <name> " // DONE
                     "=> remove user <name> from friend's list");
  cl.registerCommand("list", TokenType::LIST, ScannerContext::GENERAL,
                     "friend list " // DONE
                     "=> lists friends.");
  cl.registerCommand("visited", TokenType::VISITED, ScannerContext::GENERAL,
                     "friend visited <destination> " // DONE
                     "=> show all friends who \nhave visited <destination>, "
                     "along with their comments and ratings.\n"
                     "friend visited <destination> <user> " // DONE
                     "=> show if friend <user> \nhas visited <destination>"
                     ", and if they have: Their comment and rating.\n"
                     "destination visited <destination> <user> " // DONE
                     "=> show if <user> \nhas visited <destination>"
                     ", and if they have: their comment and rating");
  cl.registerCommand("visit", TokenType::VISIT, ScannerContext::GENERAL,
                     "visit <destination> " // DONE
                     "=> visit <destination>");

  cl.registerCommand("from", TokenType::FROM, ScannerContext::VISIT,
                     "from <date> " // DONE
                     "=> mark the beginning of your visit as <date>");
  cl.registerCommand("to", TokenType::TO, ScannerContext::VISIT,
                     "to <date> " // DONE
                     "=> mark the end of your visit as <date>");
  cl.registerCommand("rate", TokenType::RATE, ScannerContext::VISIT,
                     "rate <rating : 1 - 5> " // DONE
                     "=> rate your visit with a <rating> between 1 and 5");
  cl.registerCommand("comment", TokenType::COMMENT, ScannerContext::VISIT,
                     "comment <comment> "
                     "=> add comment <comment> to your visit");
  cl.registerCommand("photo", TokenType::PHOTO, ScannerContext::VISIT,
                     "photo " // DONE
                     "=> prefix for photo commands>");
  cl.registerCommand("upload", TokenType::UPLOAD, ScannerContext::VISIT,
                     "photo upload <uri> " // DONE
                     "=> upload photo with path <uri>");
  cl.registerCommand("delete", TokenType::DELETE, ScannerContext::VISIT,
                     "photo delete <uri> " // DONE
                     "=> delete photo with path <uri>");
  cl.registerCommand("show", TokenType::SHOW, ScannerContext::VISIT,
                     "photo show" // DONE
                     "=> show uploaded photos>");
  cl.registerCommand("save", TokenType::SAVE, ScannerContext::VISIT,
                     "save " // DONE
                     "=> save the information for this visit and return to"
                     " the main menu");
  cl.registerCommand("cancel", TokenType::CANCEL, ScannerContext::VISIT,
                     "cancel " // DONE
                     "=> cancel the information for this visit \n"
                     "and return to the main menu");

  cl.registerCommand("destination", TokenType::DESTINATION,
                     ScannerContext::GENERAL,
                     "prefix for all friend commands"); // DONE
  cl.registerCommand("exists", TokenType::EXISTS, ScannerContext::GENERAL,
                     "destination exists <destination>\n" // DONE
                     "=> Check if <destination> exists in the database.");
  cl.registerCommand("rating", TokenType::RATING, ScannerContext::GENERAL,
                     "destination rating <destination>\n" // DONE
                     "=> Show an average of all ratings for <destination>");
  cl.registerCommand("comments", TokenType::COMMENTS, ScannerContext::GENERAL,
                     "destination comments <destination>\n" // DONE
                     "=> Show all comments for <destination>");

  Travel::TravelState state{};
  bool run = true;

  Travel::Scanner lsc(ScannerContext::ATOPEN, &std::cin, &lcl);

  while (run) {
    std::cout << "login$ ";
    auto tokens = lsc.scanNext();
    /*tokens->foreach([](Token const& t){ std::cout<<t<<std::endl;});*/
    Travel::LoginInterpreter interpreter{tokens};
    run = interpreter.parse(state);
  }

  Travel::CommandList::setCommandList(&cl);
  // cl.printCommandsWithDescription();

  Travel::Scanner sc(ScannerContext::ALL, &std::cin, &cl);

  run = true;
  while (run) {
    std::cout << "$ ";
    auto tokens = sc.scanNext();
    /*tokens->foreach([](Token const& t){ std::cout<<t<<std::endl;});*/
    Travel::HTLInterpreter interpreter{tokens};
    run = interpreter.parse(state);
  }
  return 0;
}
