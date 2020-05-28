#ifndef TRAVELLERAPP_COMMANDLIST_HPP
#define TRAVELLERAPP_COMMANDLIST_HPP

#include "ScannerContext.hpp"
#include "../collection/HashMap.hpp"
#include "Token.hpp"
#include "../SimpleString.hpp"

namespace Travel {
    /**
     * This class represents a list of unique, reserved keywords for the scanner.
     * It stores their text representation,
     * their corresponding {@link TokenType} and {@link ScannerContext},
     * as well as a description for the <code> help </code> command
     */
    class CommandList {
    private:
        HashMap<TokenType, ScannerContext> commandMap;
        HashMap<SimpleString, TokenType> nameToToken;
        HashMap<SimpleString, SimpleString> nameToDescr;

        /**
         * A convenience value so CommandList doesn't need
         * to be passed around as much.
         */
        static CommandList& globalCommandList;

        /**
         * Convenience method for copying another CommandList to this one.
         */
        void copy(CommandList const &other) ;

    public:

        /**
         * Returns the global command list.
         * @return the global command list
         */
        static CommandList& getCommandList() {
            return globalCommandList;
        }

        /**
         * Sets the global command list.
         * @param cl the new command list
         */
        static void setCommandList(CommandList& cl){
            globalCommandList = cl;
        }



        CommandList() ;

        CommandList(CommandList const &other) ;

        CommandList &operator=(CommandList const &other) ;

        /**
         * Registers the command with the given information.
         * A second call to registerCommand with different parameters
         * but the same command name will overwrite the previous one.
         */
        void registerCommand(SimpleString const& name, TokenType tokenType,
                             ScannerContext ctx,
                             SimpleString const& descr) ;

        /**
         * Prints the registered commands along with their description.
         * No promises are made in regards to the order they are printed in.
         * Meant to be used as a  documentation for the registered commands.
         */
        void printCommandsWithDescription();

        /**
         * Returns true if there is a registered command with the provided
         * {@link TokenType}
         * @param tokenType the TokenType to check for
         * @return is there a registered command with that TokenType
         */
        bool isCommand(TokenType tokenType);

        /**
         * Returns true if there is a registered command with the provided name
         * @param name the name to check for
         * @return is there a registered command with that name
         */
        bool isCommand(SimpleString const& name) ;

        /**
         * Returns a pointer to a {@link NotNull} of the {@link ScannerContext}
         * associated with the given {@link TokenType} if such exists
         * and {@link Null} otherwise.
         * @param tokenType the tokenType to find the context for
         * @returns the context
         */
        std::unique_ptr<Nullable<ScannerContext>> contextFor(TokenType tokenType) ;

        /**
         * Returns a pointer to a {@link NotNull} of the {@link TokenType}
         * associated with the given name if such exists
         * and {@link Null} otherwise.
         * @param name the name to find the token type for
         * @returns the token type
         */
        std::unique_ptr<Nullable<TokenType>> tokenFor(SimpleString const& name) ;
    };
}

#endif //TRAVELLERAPP_COMMANDLIST_HPP
