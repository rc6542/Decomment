#include <stdio.h>
#include <stdlib.h>

/* Defines constants representing all possible states in the DFA */
enum Statetype{NORMAL, SLASH, COMMENT, POTENTIAL_END, SINGLE_QUOTE, 
    DOUBLE_QUOTE, CHARLIT_BACKSLASH, STRLIT_BACKSLASH};

/*  Implements the normal state of the DFA. c is the current DFA character.
    Checks the current character and put a char except /, where it can be a potential comment.  */
enum Statetype handleNormalState(int c) {
    enum Statetype state;
    if (c == '/') {
        state = SLASH;
    }
    
    else if (c == '\'') {
        state = SINGLE_QUOTE;
        putchar(c);
    }

    else if (c == '\"') {
        state = DOUBLE_QUOTE;
        putchar(c);
    }

    else {
        state = NORMAL;
        putchar(c);
    }

    return state;
}

/*  Implements the slash state of the DFA. */
enum Statetype handleSlashState(int c) {
    enum Statetype state;
    
    if (c == '/') {
        state = SLASH;
        putchar(c);
    }

    else if (c == '\'') {
        state = SINGLE_QUOTE;
        putchar('/');
        putchar(c);
    }

    else if (c == '"') {
        state = DOUBLE_QUOTE;
        putchar('/');
        putchar(c);
    }

    else if (c == '*') {
        state = COMMENT;
        putchar(' ');
    }

    else {
        state = NORMAL;
        putchar('/');
        putchar(c); 
    }

    return state;
}

/* */
enum Statetype handleCommentState(int c) {
    enum Statetype state;
    if (c == '*') {
        state = POTENTIAL_END;
    }

    else {
        state = COMMENT;
        if (c == '\n') {
            putchar(c);
        }
    }

    return state;
}

/* */
enum Statetype handlePotentialEndState(int c) {
    enum Statetype state;
    if (c == '*') {
        state = POTENTIAL_END;
    }

    else if (c == '/') {
        state = NORMAL;
    }

    else {
        state = COMMENT;
        if (c == '\n') {
            putchar(c);
        }
    }

    return state;
}

/* */
enum Statetype handleSingleQuoteState(int c) {
    enum Statetype state;
    if (c == '\\') { 
        state = CHARLIT_BACKSLASH;
        putchar(c);
    }

    else if (c == '\'') {
        state = NORMAL;
        putchar(c);
    } 

    else {
        state = SINGLE_QUOTE;
        putchar(c);
    }

    return state;
}

/* */
enum Statetype handleDoubleQuoteState(int c) {
    enum Statetype state;
    if (c == '\\') { 
        state = STRLIT_BACKSLASH;
        putchar(c);
    }

    else if (c == '"') {
        state = NORMAL;
        putchar(c);
    } 

    else {
        state = DOUBLE_QUOTE;
        putchar(c);
    }

    return state;
}

/* */
enum Statetype handleCharLitBackslashState(int c) {
    enum Statetype state;
    state = SINGLE_QUOTE;
    putchar(c);
    return state;
}

/* */
enum Statetype handleStringLitBackslashState(int c) {
    enum Statetype state;
    state = DOUBLE_QUOTE;
    putchar(c);
    return state;
}

/* */
int main(void) {
    int c;
    int lineNumber;
    int commentLineNumber;
    enum Statetype state = NORMAL;

    while((c = getchar()) != EOF) {
        if (c == '\n') {
            lineNumber++;
        }

        if (state = SLASH && c == '*') {
            commentLineNumber = lineNumber;
        }

        switch(state) {
            case NORMAL:
                state = handleNormalState(c);
                break;
            case SLASH:
                state = handleSlashState(c);
                break;
            case COMMENT:
                state = handleCommentState(c);
                break;
            case POTENTIAL_END:
                state = handlePotentialEndState(c);
                break;
            case SINGLE_QUOTE:
                state = handleSingleQuoteState(c);
                break;
            case DOUBLE_QUOTE:
                state = handleDoubleQuoteState(c);
                break;
            case CHARLIT_BACKSLASH:
                state = handleCharLitBackslashState(c);
                break;
            case STRLIT_BACKSLASH:
                state = handleStringLitBackslashState(c);
                break;
        }
    }

    if (state == SLASH) {
        putchar('/');
    }

    if (state == COMMENT ||state == POTENTIAL_END) {
        fprintf(stderr, "Error: line %i: unterminated comment\n", lineNumber);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}