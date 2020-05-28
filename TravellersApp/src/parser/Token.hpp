#ifndef TRAVELLERAPP_TOKEN_HPP
#define TRAVELLERAPP_TOKEN_HPP

#include<iostream>

/**
 * Describes the different tokens that {@link Travel::Scanner Scanner}
 * recognises.
 * Some may need to be provided via {@link Travel::CommandList CommandList}
 */
enum class TokenType {

    // HTL tokens - File only
    ADD, /**< Matches the <code>add</code> command in .htl files. File Only */
    REMOVE, /**< Matches the <code>remove</code> command in .htl files. File Only */

    // All projects - Console only
    OPEN, /**< Matches the <code>open</code> command. Console Only */
    CLOSE, /**< Matches the <code>close</code> command. Console Only */
    SAVE, /**< Matches the <code>save</code> command. Console Only */
    SAVE_AS, /**< Matches the <code>save_as</code> command. Console Only */
    HELP, /**< Matches the <code>help</code> command. Console Only */
    EXIT, /**< Matches the <code>exit</code> command. Console Only */

    // Extensions - Console only
    LIST, /**< Matches the <code>list</code> command. Console Only */

    // HTL tokens - All contexts
    CHECKIN, /**< Matches the <code>checkin</code> command in .htl files. All contexts */
    CHECKOUT,/**< Matches the <code>checkout</code> command in .htl files. All contexts */
    UNAVAILABLE, /**< Matches the <code>unavailable</code> command in .htl files. All contexts */

    // HTL tokens - Console only
    REPORT, /**< Matches the <code>report</code> command in .htl files. Console only */
    FIND, /**< Matches the <code>find</code> command in .htl files. Console only */
    FIND_F, /**< Matches the <code>find!</code> command in .htl files. Console only */
    AVAILABILITY, /**< Matches the <code>availablility</code> command in .htl files. Console only */

    // Types? I guess
    STRING, /**< Any single non-reserved word and everything between " " is
             considered a string.*/
    NUMBER, /**< A signed integer. */
    DATE, /**< Date in the ISO 8601 format (year-month-day). */
    NUMERIC_RANGE, /**< An inclusive range made up of two integers (start-end).*/

    // Parser/Scanner info
    ERROR, /**< Stores information for errors encountered by the
            {@link Travel::Scanner Scanner}.*/
    EOF_T /**< The end of the input stream */
};

/**
 * Generic token for use in scanners/parsers.
 * It stores the type of the token, it's original text representation
 * and the line it was found on.
 */
class Token {
private:
    /**
     * Convenience method for initializing a token
     */
    void create(TokenType _t, const char *_lexeme, int _line);

    /**
     * Convenience method for copying another token.
     * It simply calls {@link create} with the other token's fields
     */
    void copy(Token const &other) ;

public:
    /** The token type of this token */
    TokenType t;
    /** The original text representation of this token */
    char *lexeme;
    /** The line it was found on */
    int line;

    Token(TokenType _t, const char *_lexeme, int _line);

    /**
     * Frees the dynamic memory associated with the {@link lexeme}
     */
    ~Token();

    /**
     * Creates a default token.
     * Equivalent to <code> Token(TokenType::EOF_T, "", 0) </code>
     */
    Token();

    /**
     * Copy constructor.
     */
    Token(const Token &other);

    Token &operator=(Token const &other);

    /**
     * Tests for the equality of two tokens.
     * Two tokens are considered equal
     * when they are of the same type
     * have the same lexeme
     * and are found on the same line
     */
    bool operator==(Token const &other) const;
};

#endif //TRAVELLERAPP_TOKEN_HPP
