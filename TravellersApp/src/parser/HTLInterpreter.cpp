//
// Created by Kalelzar on 25/03/2020.
//

#include "HTLInterpreter.hpp"

namespace Travel {


    bool HTLInterpreter::general(TravelState& state){
        switch(next().t){
        case TokenType::HELP:
            CommandList::getCommandList()->
                printCommandsWithDescription ();
            break;
        case TokenType::EXIT:
            return false;
            break;
        case TokenType::STRING:
            errorflag = true;
            std::cerr<<"(Line "<<prev().line
                     <<") "<<"Unexpected String -> "
                     << prev().lexeme <<std::endl;
            break;
        case TokenType::NUMBER:
            errorflag = true;
            std::cerr<<"(Line "<<prev().line
                     <<") "<<"Unexpected Number -> "
                     << prev().lexeme <<std::endl;
            break;
        case TokenType::DATE:
            errorflag = true;
            std::cerr<<"(Line "<<prev().line
                     <<") "<<"Unexpected Date -> "
                     << prev().lexeme <<std::endl;
            break;
        case TokenType::ERROR:
            errorflag = true;
            std::cerr<<"(Line "<<prev().line
                     <<") "<<"Parsing error: "
                     << prev().lexeme << std::endl;
            break;
        case TokenType::EOF_T:
            return true;
            break;
        }

        return true;
    }

    void HTLInterpreter::friendAdd(TravelState& state){
        auto name = consume(TokenType::STRING, "Expected user name.");
        std::cout<<"Adding "<<name.lexeme<<" to friend's list."<<std::endl;
        //state.friendAdd(name);
    }

    void HTLInterpreter::friendRemove(TravelState& state){
        auto name = consume(TokenType::STRING, "Expected user name.");
        std::cout<<"Removing "<<name.lexeme<<" from friend's list."<<std::endl;
        //state.friendRemove(name);
    }

    void HTLInterpreter::friendList(TravelState& state){
        std::cout<<"Listing friends."<<std::endl;
        //state.friendList();
    }

    void HTLInterpreter::friendVisited(TravelState& state){
        auto destination = consume(TokenType::STRING,
                                   "Expected destination name.");
        if(matches(TokenType::STRING)){
            auto user = next();
            std::cout<<"Checking if "<<user.lexeme << " has visited "
                     <<destination.lexeme<<std::endl;
            //state.friendVisited(destination, user);
        }else{
            std::cout<<"Showing all friends who have visited "
                     <<destination.lexeme<<std::endl;
        }
    }

    void HTLInterpreter::destinationExists(TravelState& state) {
        auto destination = consume(TokenType::STRING,
                                   "Expected destination name.");
        std::cout<<"Checking if destination "
                 << destination.lexeme
                 <<" exists."<<std::endl;
        //state.destinationExists(destination)
    }

    void HTLInterpreter::destinationRating(TravelState& state) {
        auto destination = consume(TokenType::STRING,
                                   "Expected destination name.");
        std::cout<<"Showing average rating for destination "
                 << destination.lexeme
                 <<"."<<std::endl;
        //state.destinationRating(destination)
    }



    void HTLInterpreter::destinationComments(TravelState& state) {
        auto destination = consume(TokenType::STRING,
                                   "Expected destination name.");
        std::cout<<"Showing comments for destination "
                 << destination.lexeme
                 <<"."<<std::endl;
        //state.destinationComments(destination)
    }

    void HTLInterpreter::destinationVisited(TravelState& state) {
        auto destination = consume(TokenType::STRING,
                                   "Expected destination name.");
        auto user = consume(TokenType::STRING, "Expected user name.");

        std::cout<<"Checking if "<<user.lexeme << " has visited "
                 <<destination.lexeme<<std::endl;
        //state.destinationVisited(destination, user);
    }

    void HTLInterpreter::cfriend(TravelState& state){
        if(isAtEnd()){
            throw ReachedEndOfStreamException();
        }

        switch(next().t){
        case TokenType::ADD:
            friendAdd(state);
            break;
        case TokenType::REMOVE:
            friendRemove(state);
            break;
        case TokenType::LIST:
            friendList(state);
            break;
        case TokenType::VISITED:
            friendVisited(state);
            break;
        case TokenType::EOF_T:
            std::cout<<"Friend is a command prefix. It doesn't do anything by "
                "itself."<<std::endl;
            break;
        case TokenType::NUMBER:
            errorflag = true;
            std::cerr<<"(Line "<<prev().line
                     <<") "<<"Unexpected Number -> "
                     << prev().lexeme <<std::endl;
            break;
        case TokenType::DATE:
            errorflag = true;
            std::cerr<<"(Line "<<prev().line
                     <<") "<<"Unexpected Date -> "
                     << prev().lexeme <<std::endl;
            break;
        case TokenType::ERROR:
            errorflag = true;
            std::cerr<<"(Line "<<prev().line
                     <<") "<<"Parsing error: "
                     << prev().lexeme << std::endl;
            break;
        default:
            errorflag = true;
            std::cerr<<"(Line "<<prev().line
                     <<") "<<"Unexpected String -> "
                     << prev().lexeme <<std::endl;
            break;
        }
    }

    void HTLInterpreter::destination (TravelState &state){
        if(isAtEnd()) throw ReachedEndOfStreamException();

        switch(next().t){
        case TokenType::EXISTS:
            destinationExists(state);
            break;
        case TokenType::RATING:
            destinationRating(state);
            break;
        case TokenType::COMMENTS:
            destinationComments(state);
            break;
        case TokenType::VISITED:
            destinationVisited(state);
            break;
        case TokenType::EOF_T:
            std::cout<<"Destination is a command prefix. It doesn't do anything"
                " by itself."<<std::endl;
            break;
        case TokenType::NUMBER:
            errorflag = true;
            std::cerr<<"(Line "<<prev().line
                     <<") "<<"Unexpected Number -> "
                     << prev().lexeme <<std::endl;
            break;
        case TokenType::DATE:
            errorflag = true;
            std::cerr<<"(Line "<<prev().line
                     <<") "<<"Unexpected Date -> "
                     << prev().lexeme <<std::endl;
            break;
        case TokenType::ERROR:
            errorflag = true;
            std::cerr<<"(Line "<<prev().line
                     <<") "<<"Parsing error: "
                     << prev().lexeme << std::endl;
            break;
        default:
            errorflag = true;
            std::cerr<<"(Line "<<prev().line
                     <<") "<<"Unexpected String -> "
                     << prev().lexeme <<std::endl;
            break;
        }
    }

    bool HTLInterpreter::parse(TravelState& state) {
        errorflag = false;
        while(!isAtEnd()){
            try{
                if(matches(TokenType::FRIEND)){
                    LOG(INFO, "Friend prefix");
                    next();
                    cfriend(state);
                }else if(matches(TokenType::DESTINATION)){
                    LOG(INFO, "Destination prefix");
                    next();
                    destination(state);
                }else{
                    LOG(INFO, "Generic commandline");
                    if(!general(state)) return false;
                }
            }catch(NoValueException& e){
                errorflag = true;
                std::cerr<<e.what()<<std::endl;
            }catch(InvalidArgumentException& e){
                errorflag = true;
                std::cerr<<e.what()<<std::endl;
            }catch(ReachedEndOfStreamException& e){
                errorflag = true;
                std::cerr<<e.what()<<std::endl;
                return true;
            }catch(WrongTokenTypeException& e){
                errorflag = true;
                std::cerr<<e.what()<<std::endl;
                while(peek().t == TokenType::STRING ||
                      peek().t == TokenType::NUMBER ||
                      peek().t == TokenType::DATE ||
                      peek().t == TokenType::ERROR){
                    Token n = next();
                    if(n.t == TokenType::ERROR){
                        std::cerr<<"Error: "<<n.lexeme<<std::endl;
                    }
                }
            }
        }
        return true;
    }

}
