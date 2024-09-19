#include <stdio.h>
#include <stdlib.h>

/* Defines constants representing all possible states in the DFA */
enum Statetype{NORMAL, SLASH, COMMENT, POTENTIAL_END, SINGLE_QUOTE, 
    DOUBLE_QUOTE, CHARLIT_BACKSLASH, STRLIT_BACKSLASH};

/*  Implements the normal state of the DFA. Prints to stdout as 
determined by the DFA. Returns state of either SLASH, SINGLE_QUOTE, 
DOUBLE_QUOTE, or NORMAL depending on value of c, the current character 
from stdin. */
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

/*  Implements the SLASH state, when a slash is encountered. Prints to 
stdout as determined by the DFA. Returns state of either SLASH, 
SINGLE_QUOTE, DOUBLE_QUOTE, or COMMENT depending on value of c, the 
current character from stdin. */
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

/*  Implements the COMMENT state which handles the in-comment part of 
the DFA. Prints to stdout as determined by the DFA. Returns either 
POTENTIAL_END or COMMENT states depending on value of c, the current 
character from stdin. */
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

/*  Implements the POTENTIAL_END state which checks for a potential 
comment termination. Prints to stdout as determined by the DFA. Returns
a POTENTIAL_END, NORMAL, or COMMENT state depending on value of c, the 
current character from stdin.*/
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

/*  Implements the SINGLE_QUOTE state when ' is encountered. Prints to 
stdout as determined by the DFA. Returns CHARLIT_BACKSLASH, NORMAL, or 
SINGLE_QUOTE state depending on value of c, the current character from 
stdin. */
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

/*  Implements the DOUBLE_QUOTE state when a " is encountered. Prints to
stdout as determined by the DFA. Returns a STRLIT_BACKSLASH, NORMAL, 
DOUBLE_QUOTE state depending on value of c, the current character from 
stdin. */
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

/*  Implements the CHARLIT_BACKSLASH state when an escape character 
is encountered inside a character literal. Prints to stdout as 
determined by the DFA. Returns SINGLE_QUOTE state regardless of value c, 
the current character from stdin. */
enum Statetype handleCharLitBackslashState(int c) {
    enum Statetype state;
    state = SINGLE_QUOTE;
    putchar(c);
    return state;
}

/*  Implements the STRLIT_BACKSLASH state when an escape character 
is encountered inside a string literal. Prints to stdout as determined 
by the DFA. Returns DOUBLE_QUOTE state regardless of value c, the 
current character from stdin. */
enum Statetype handleStringLitBackslashState(int c) {
    enum Statetype state;
    state = DOUBLE_QUOTE;
    putchar(c);
    return state;
}

int main(void) {
    /* stores character from stdin*/
    int c;
    /* tracks the line number*/
    int lineNumber = 1;
    /* tracks the line number of most recent comment start*/
    int commentLineNumber = 0;
    /* start state of DFA*/
    enum Statetype state = NORMAL;

    /* reads each char from stdin*/
    while((c = getchar()) != EOF) {   
        /* increments line number*/
        if (c == '\n') {
        lineNumber++;
    }
        /* sets line number of most recent comment to current line 
        number */
        if (state == SLASH && c == '*') {
            commentLineNumber = lineNumber;
        }
        /* all possible states of DFA */
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

    /* corner case if code ends in a slash */
    if (state == SLASH) {
        putchar('/');
    }

    /* prints to stderr an error message and line number of an 
    unterminated comment */
    if (state == COMMENT ||state == POTENTIAL_END) {
        fprintf(stderr, "Error: line %i: unterminated comment\n", 
            commentLineNumber);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}