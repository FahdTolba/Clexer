# Clexer
This is a very a simple C lexical analyzer, it's based on a deterministic finite state machine.
The list of recognized tokens can be found in token.txt.
This was supposed to be a lexerless parser in the beginning but then i went back to conventional compiler phase 1,hence the S_error() function may give error messages even if the tokens are correct but the statement is syntactically wrong ,also the compound arithmetic operators like ++ += and the like may result in an error.I may complete the C token list in the future, or be my guest to do so.
The scanner_end() function outputs the tokens in an html file to be displayed in a "friendly" way which i could care less about, but anyway if you have made it this far it should be a piece of cake for you to modify it however you like or any other part of the code.
