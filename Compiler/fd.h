#include <cstdio>

#ifndef FD_H
#define FD_H

#define SET 1 
#define UNSET 0
#define BUFFER_SIZE 256
class FileDescriptor
{
	/* You must fill in these fields */
private:
	FILE* fp;
	char* file; /* file name, allocate memory for this */
	char ungetBuffer;
	int line_number; /* line number in the file */
	int char_number; /* character number in the line */
	int buf_size; /* stores the buffer size */
	char* buffer; /* buffer to store a line */
	int flag2;
	char last_char;
	int flag; /* to prevents two ungets in a row */

	// add other fields or functions if you want
public:
	/* Externally-visible functions: */
	FileDescriptor(char* FileName); // opens the stdin FileName is Null
	~FileDescriptor();
	char readChar(); // Gets the current character in the file
	void ungett();
	char peek();
	int getLineNumber();
	void ReportError(char* msg); // reports the error specifying the current line and 
};

#endif FD_H
