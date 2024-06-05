#include <string>
#include <iostream>
#include <cctype>
#include <cstring>
#include "fd.h"
#include "scanner.h"

using namespace std;
#define _CRT_SECURE_NO_WARNINGS

// Helper function to skip comments
void SCANNER::skip_comments() {
    char ch;
    bool flagForComment = false;
    while (1) {
        ch = Fd->readChar();
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

    for (int i = 0; i < keywords; ++i) {
        if (strcmp(str, keyword[i]) == 0) {
            TOKEN* newToken = new TOKEN();
            newToken->type = key_type[i];
            size_t length = strlen(str) + 1; // Include space for null terminator
            newToken->str_ptr = new char[length];
            if (strcpy_s(newToken->str_ptr, length, str) != 0) {
                cerr << "Error copying string." << endl;
                delete newToken;
                return nullptr;
            }
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

// Helper function to get an operator token
TOKEN* SCANNER::get_op(const char* op) {
    for (int i = 0; i < operators; ++i) {
        if (strcmp(op, operator_list[i]) == 0) {
            TOKEN* newToken = new TOKEN();
            newToken->type = operator_type[i];
            size_t length = strlen(op) + 1; // Include space for null terminator
            newToken->str_ptr = new char[length];
            if (strcpy_s(newToken->str_ptr, length, op) != 0) {
                cerr << "Error copying string." << endl;
                delete newToken;
                return nullptr;
            }
            return newToken;
        }
    }
    return nullptr;
}

// Helper function to get an integer token
TOKEN* SCANNER::get_int(const char* first_char) {
    TOKEN* newToken = new TOKEN();
    newToken->type = lx_integer;
    cout << (newToken->value = atoi(first_char)) <<endl;//convert string to int

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

SCANNER::SCANNER() {
    Fd = nullptr;
}

SCANNER::SCANNER(FileDescriptor* fd) {
    Fd = fd;
    keywords = 24;
    operators = 22;
}

// Main scanning function
TOKEN* SCANNER::Scan(FileDescriptor* fd) {
    Fd = fd;
    if (Fd == nullptr) {
        printf("Error: File descriptor is null.\n");
        return nullptr;
    }
    char c = Fd->readChar();
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

        while (isdigit(c = Fd->readChar())) {
            num_string += c;
        }

        if (c == EOF || isspace(c) || strchr("():=+-*/!<>.;[],{}", c) != NULL) {
            Fd->ungett();
            cout << "The token is oparation: " << num_string << endl;
            return get_int(num_string.c_str());
        }
        else {
            Fd->ReportError((char*)"(Unexpected character in number)");
            return nullptr;
        }
    }
    else if (c == '\"') {
        string str = "";

        while ((c = Fd->readChar()) != '\"') {
            if (c == EOF || c == '\n') {
                Fd->ReportError((char*)"Unexpected EOF or newline in string");
                return nullptr;
            }
            str += c;
        }
        cout << "The token is string: " << str << endl;
        return get_string(str.c_str());
    }
    else if (isalpha(c) || c == '_') {
        string word = "";
        word += c;

        while (isalpha(c = Fd->readChar()) || isdigit(c) || c == '_') {
            word += c;
        }

        if (c == EOF || isspace(c) || strchr("():=+-*/=!<>.;[],{}", c) != NULL) {
            Fd->ungett();
            TOKEN* keyword_token = check_keyword(word.c_str());
            if (keyword_token != nullptr) {
                cout << "The token is keyword: " << word << endl;
                return keyword_token;
            }
            else {
                cout << "The token is identifier: " << word << endl;
                return get_id(word.c_str());
            }
        }
        else {
            Fd->ReportError((char*)"Unexpected character in identifier/keyword");
            return nullptr;
        }
    }
    else if (strchr("():=+-*/=!<>.;[],{}", c) != NULL) {
        string op = "";
        op += c;
        if ((c_eof = Fd->peek()) != EOF && strchr("=:<>", c) != NULL) {
            op += c_eof;
            Fd->readChar(); // Consume the second character of the operator
        }
        cout << "The token is operation: " << op << endl;
        return get_op(op.c_str());
    }
    else if (isspace(c)) {
        while ((c = Fd->readChar()) != EOF && isspace(c)) {
            // Just consume the whitespace
        }
        Fd->ungett();
        return Scan(Fd);
    }
    else if (c == '\0') {
        return nullptr;
    }
    else {
        Fd->ReportError((char*)"Unexpected character ---");
        return nullptr;
    }
}



// Main function to test the scanner
int main() {
    FileDescriptor fd((char*)"test_input.txt");
    cout << "The tokens are: " << endl;
    SCANNER scanner(&fd);
    TOKEN* token;
    while ((token = scanner.Scan(&fd))->type != lx_eof) {
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
    return 0;
}
