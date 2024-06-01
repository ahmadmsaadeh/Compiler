#include <string>
#include <iostream>
#include <cctype>
#include <cstring>
#include "fd.h"
#include "scanner.h"

using namespace std;

// Helper function to skip comments
void SCANNER::skip_comments() {
    char ch;
    bool flagForComment = false;
    while (1) {
        ch = Fd->GetChar();
        if (ch == '#') {
            flagForComment = true;
        }
        else if (ch == '\n' || ch == EOF) {
            break;
        }
        else {
            flagForComment = false;
        }
    }
}

// Helper function to check if a string is a keyword
TOKEN* SCANNER::check_keyword(const char* str) {
    if (str == nullptr) {
        return nullptr;
    }

    for (int i = 0; i < keys; ++i) {
        if (strcmp(str, keyword[i]) == 0) {
            TOKEN* newToken = new TOKEN();
            newToken->type = key_type[i];
            return newToken;
        }
    }
    return nullptr;
}


// Helper function to get an identifier token
TOKEN* SCANNER::get_id(const char* first_char) {
    TOKEN* newToken = new TOKEN();
    newToken->type = lx_identifier;
    size_t length = strlen(first_char) + 1; // Include space for null terminator
    newToken->str_ptr = new char[length];
    if (strcpy_s(newToken->str_ptr, length, first_char) != 0) {
        cerr << "Error copying string." << endl;
        delete newToken;
        return nullptr;
    }
    return newToken;
}


TOKEN* SCANNER::get_op(const char* op)
{
    for (int i = 0; i < operators; ++i) {
        if (strcmp(op, operator_list[i]) == 0) {
            TOKEN* newToken = new TOKEN();
            newToken->type = operator_type[i];
            return newToken;
        }
    }
    return nullptr;
}

// Helper function to get an integer token
TOKEN* SCANNER::get_int(const char* first_char) {
    TOKEN* newToken = new TOKEN();
    newToken->type = lx_integer;
    newToken->value = atoi(first_char);
    return newToken;
}

// Helper function to get a string token
TOKEN* SCANNER::get_string(const char* str) {
    TOKEN* newToken = new TOKEN();
    newToken->type = lx_string;
    size_t length = strlen(str) + 1; // Include space for null terminator
    newToken->str_ptr = new char[length];
    if (strcpy_s(newToken->str_ptr, length, str) != 0) {
        cerr << "Error copying string." << endl;
        delete newToken;
        return nullptr;
    }
    return newToken;
}


SCANNER::SCANNER()
{
    Fd = nullptr;
}

SCANNER::SCANNER(FileDescriptor* fd)
{
    Fd = fd;
    keys = 24;
    operators = 22;
}

// Main scanning function
TOKEN* SCANNER::Scan(FileDescriptor* fd) {
    Fd = fd;
    if (Fd == nullptr) {
        printf("Error: File descriptor is null.\n");
        return nullptr;
    }
    char c = Fd->GetChar();
    if (c_eof == EOF)
        return nullptr;
    if (c == EOF)
        return nullptr;
    if (c == '#') {
        skip_comments();
        return Scan(Fd);  // Call Scan again after skipping the comment
    }

    if (isdigit(c) || (c == '-' && isdigit(Fd->peek()))) {
        string num_string = "";
        num_string += c;

        while (isdigit(c = Fd->GetChar())) {
            num_string += c;
        }

        if (c == EOF || isspace(c) || strchr("():=+-*/=!<>.;[],{}", c) != NULL) {
            Fd->unget();
            return get_int(num_string.c_str());
        }
        else {
            cerr << ("Unexpected character in number")<<endl;
            return nullptr;
        }
    }
    else if (c == '\"') {
        string str = "";

        while ((c = Fd->GetChar()) != '\"') {
            if (c == EOF || c == '\n') {
                cerr<<("Unexpected EOF or newline in string")<<endl;
                return nullptr;
            }
            str += c;
        }

        return get_string(str.c_str());
    }
    else if (isalpha(c) || c == '_') {
        string word = "";
        word += c;

        while (isalpha(c = Fd->GetChar()) || isdigit(c) || c == '_') {
            word += c;
        }

        if (c == EOF || isspace(c) || strchr("():=+-*/=!<>.;[],{}", c) != NULL) {
            Fd->unget();
            TOKEN* keyword_token = check_keyword(word.c_str());
            if (keyword_token != nullptr) {
                return keyword_token;
            }
            else {
                return get_id(word.c_str());
            }
        }
        else {
            cerr<<("Unexpected character in identifier/keyword")<<endl;
            return nullptr;
        }
    }
    else if (strchr("():=+-*/=!<>.;[],{}", c) != NULL) {
        string op = "";
        op += c;
        if ((c_eof = Fd->peek()) != EOF && strchr("=:<>", c) != NULL) {
            op += c_eof;
            Fd->GetChar(); // Consume the second character of the operator
        }
        return get_op(op.c_str());
    }
    else if (isspace(c)) {
        while ((c = Fd->GetChar()) != EOF && isspace(c)) {
            // Just consume the whitespace
        }
        Fd->unget();
        return Scan(Fd);
    }
    else if (c == '\0') {
        return nullptr;
    }
    else {
        cerr<<("Unexpected character ---")<<endl;
        return nullptr;
    }
}

// Main function to test the scanner
int main() {
    FileDescriptor fd((char*)"test_input.txt");
    SCANNER scan(&fd);
/*
    TOKEN* token;
    while ((token = scanner.Scan())->type != lx_eof) {
        cout << "Token Type: " << token->type << endl;
        if (token->str_ptr) {
            cout << "Token Value: " << token->str_ptr << endl;
            delete[] token->str_ptr; // Free allocated memory
        }
        else {
            cout << "Token Value: " << token->value << endl;
        }
        delete token; // Free token memory
    }
    delete token; // delete the last EOF token
*/
    return 0;
}
