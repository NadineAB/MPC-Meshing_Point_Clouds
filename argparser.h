#ifndef __ARG_PARSER_H__
#define __ARG_PARSER_H__

#include "string.h"
#include "stdio.h"
#include "assert.h"
#include "stdlib.h"

class ArgParser {

public:

  ArgParser() { DefaultValues(); }

  ArgParser(int argc, char *argv[]) {
    DefaultValues();

    for (int i = 1; i < argc; i++) {
      if (!strcmp(argv[i],"-input")) {
	i++; assert (i < argc); 
	input_file = argv[i];
      } else if (!strcmp(argv[i],"-size")) {
	i++; assert (i < argc); 
	width = height = atoi(argv[i]);
      } else if (!strcmp(argv[i],"-wireframe")) {
        wireframe = true;
      } else if (!strcmp(argv[i],"-gouraud")) {
        gouraud = true;
      } else {
	printf ("whoops error with command line argument %d: '%s'\n",i,argv[i]);
	assert(0);
      }
    }
  }

  void DefaultValues() {
    input_file = NULL;
    width = 1024;
    height = 640;
    wireframe = 0;
  }

  // ==============
  // REPRESENTATION
  // all public! (no accessors)

  char *input_file;
  int width;
  int height;
  bool wireframe;
  bool gouraud;

};

#endif
