
#ifdef __cplusplus
extern "C" {
#endif

// SHML needs popen and pclose for executing commands 
// but these functions are called differently on windows
#ifdef _WIN32
	#define OPEN_PIPE _popen
	#define CLOSE_PIPE _pclose
	#define ENV_SHELL "COMPSEC"
	#define DEFAULT_SHELL "cmd.exe"
#else
	#define OPEN_PIPE popen
	#define CLOSE_PIPE pclose
	#define ENV_SHELL "SHELL"
	#define DEFAULT_SHELL "sh"
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "shml.h"

// defining where the tags will be saved
Tag Stack[TABLE_MAX];
int StackSize = 0;

// defining how to use the stack
void DefTag(char* tag, char* fmt) {
	if (StackSize >= TABLE_MAX) return;
	
	Stack[StackSize].key = strdup(tag);
	Stack[StackSize].form = strdup(fmt);
	StackSize++;
}

int GetTagIndex(char* tag) {
	for ( int i = 0; i < TABLE_MAX; i++ ) {
		if (Stack[i].key != NULL && strcmp(Stack[i].key, tag) == 0)
			return i;
	}

	return -1;
}

char* WrapTag(char* text, char* tag) {
	int id = GetTagIndex(tag);
	if (id == -1) return NULL;

	char* format = Stack[id].form;

	int needed = snprintf(NULL, 0, format, text) + 1;
	char* result = malloc(needed);

	if (result) {
        snprintf(result, needed, format, text);
    }
    
    return result;	
	
}

//garbage collector
void Cleanup() {
    for (int i = 0; i < StackSize; i++) {
        if (Stack[i].key) {
            free(Stack[i].key);
            Stack[i].key = NULL;
        }
        if (Stack[i].form) {
            free(Stack[i].form);
            Stack[i].form = NULL;
        }
    }
    StackSize = 0;
}

//read the shml
int SHML(char* sml) {

	char* ShellName = getenv(ENV_SHELL);
	if (!ShellName) {
		ShellName = DEFAULT_SHELL;
	}
	FILE* Pipe = OPEN_PIPE(ShellName, "w");
	
    char* TagName = malloc(4);
    char* InTag = malloc(8);
    int TagCap = 4, InCap = 8;
    
    for (int i = 0; sml[i] != '\0'; i++) {
        switch (sml[i]) {
        	case '?': {
        		int j = 0;
        		i++;
        		while (sml[i] != ':' && sml[i] != '\0') {
        			if (j >= TagCap - 1) {
        				TagCap *= 2;
        				TagName = realloc(TagName, TagCap);
        			}
        			TagName[j++] = sml[i++];
        		}
        		TagName[j] = '\0';

				i++; // move past '='
				int kay = 0;

				while (sml[i] != '?' && sml[i] != '\0') {
					if (kay >= InCap - 1) {
				    	InCap *= 2;
				        InTag = realloc(InTag, InCap);
				    }
				    InTag[kay++] = sml[i++];
				}
				InTag[kay] = '\0';

				if (InTag[0] == '*') {
					int id = GetTagIndex(&InTag[1]);
					if (id != -1) {
						DefTag(TagName, strdup(Stack[id].form));
					} 
				} else DefTag(TagName, InTag);
				
        		break;
        	}
            case '<': {
                int j = 0;
                i++; // move past '<'
                while (sml[i] != '>' && sml[i] != '\0') {
                    if (j >= TagCap - 1) {
                        TagCap *= 2;
                        TagName = realloc(TagName, TagCap);
                    }
                    TagName[j++] = sml[i++];
                }
                TagName[j] = '\0';
                
                int id = GetTagIndex(TagName);
                if (id != -1) {
                
                    i++; // move past '>'
                    int k = 0;
                    
                    while (sml[i] != '<' && sml[i] != '\0') {
                        if (k >= InCap - 1) {
                            InCap *= 2;
                            InTag = realloc(InTag, InCap);
                        }
                        InTag[k++] = sml[i++];
                    }
                    InTag[k] = '\0';

                    char* result = WrapTag(InTag, TagName);
                    if (result) {
                    	// run the command
                        fprintf(Pipe, "%s\n", result);
						fflush(Pipe);
                        free(result);
                    }
                    
                    // skip the closing tag </name>
                    while (sml[i] != '>' && sml[i] != '\0') i++;
                }
                break;
            }
            case '#': {
            	i++;
            	while (sml[i] != '\n' || sml[i] != '\0') 
            		i++;
            	break;
            }

            default: break;
        }
    }
    
    free(TagName);
    free(InTag);
    return 0;
}

#ifdef __cplusplus
}
#endif
