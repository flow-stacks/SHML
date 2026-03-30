
#ifndef SHML_H
#define SHML_H

#ifdef __cplusplus
extern "C" {
#endif

// maximum size of the stack
#define TABLE_MAX 1024

typedef struct {
	char* key;
	char* form;
} Tag;

// very important to use before ending the program
// it clens the stack to avoid memory leaks
void Cleanup();

// use thisto create a tag like
// DefineTag("echon", "echo -n %s")
void DefTag(char* tag, char* fmt);

// returns the number of where the tag is inside Stack
// return -1 if it fails
int GetTagIndex(char* tag);

// wraps the text in the coresponding tag
// WrapTag( "\'hello world!\'", "echon" ) -> "echo -n \'hello world!\'"
char* WrapTag(char* text, char* tag);

// runs the standart markup language for SHML
// returns 1 if an error happened
// if else returns 0 
int SHML(char* sml);

#ifdef __cplusplus
}
#endif

#endif
