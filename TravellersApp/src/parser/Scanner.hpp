//
// Created by Kalelzar on 25/03/2020.
//

#ifndef TRAVELLERAPP_SCANNER_HPP
#define TRAVELLERAPP_SCANNER_HPP

#include<iostream>
#include<cstring>
#include<cctype>
#include "../collection/ArrayList.hpp"
#include "ScannerContext.hpp"
#include "CommandList.hpp"
#include "Token.hpp"

namespace Travel {

    /**
     * A general-purpose scanner that can be adopted to scan most forms of
     * structured text files.
     *
     * It can scan text from any input stream implemented through
     * <code>std::istream</code> regardless if it is a file, standard input
     * or, potentially, a remote source.

     * This class provides methods for scanning the entire stream at once
     * as well as scanning the stream line-by-line for cases such as accepting
     * from the user rather than a static source (file, remote resource, etc).
     *
     * The scanner is not reusable. Rather they are meant to read a single input
     * stream once and consequently be disposed of.
     *
     * You can provide a list of reserved keywords with {@link Travel::CommandList CommandList}.
     * Some keywords may only be provided in a certain context (File, Command-line or all)
     * as such the scanner needs to know it's own context which is provided at instantiation
     * through {@link ScannerContext}
     *
     */
    class Scanner {
    private:
        ScannerContext sc;
        CommandList* cl;
        std::istream *input;
        int line = 1;

        /**
         * Tokenizes the provided line
         *
         * @param line the line to tokenize
         * @return a shared pointer to a list containing the resulting tokens
         */
        std::shared_ptr<ArrayList<Token>> scanLine(const char *line) ;

    public:

        /**
         * Set to true only if the scanner encounters an error during scanning
         * i.e a token of type {@link TokenType::ERROR ERROR} is generated.
         * This is done so more than one error can be detected in a single scan.
         */
        bool error = false;

        /**
         * Constructs a scanner for the provided input stream using the
         * specified context and command list.
         *
         * @param sc the context of the scanner
         * @param input the input stream to scan
         * @param cl the list of reserved keywords
         */
        Scanner(ScannerContext sc, std::istream *input, CommandList* cl);

        /**
         * Scans the entire input stream.
         *
         * This is done by reading from the stream line-by-line
         * until the end of the stream is reached.
         * Due to technical limitations the lines are assumed to be of
         * 512 characters or less.
         *
         * The last token of the returned list is always of type {@link TokenType::EOF_T EOF}
         *
         * @return a shared pointer to a list containing the resulting tokens
         */
        std::shared_ptr<ArrayList<Token>> scan();

        /**
         * Scans the next line in the input stream.
         *
         * Due to technical limitations the lines are assumed to be of
         * 512 characters or less.
         *
         * The last token of the returned list is always of type {@link TokenType::EOF_T EOF}
         *
         * @return a shared pointer to a list containing the resulting tokens
         */
        std::shared_ptr<ArrayList<Token>> scanNext() ;
    };

}


#endif //TRAVELLERAPP_SCANNER_HPP
