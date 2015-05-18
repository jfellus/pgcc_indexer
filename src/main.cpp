/*
 * main.cpp
 *
 *  Created on: 18 mai 2015
 *      Author: jfellus
 */

#include "utils/utils.h"
#include "index.h"

void USAGE() {
	DBG("usage : pgcc_indexer <file1.cpp> <file2.h> <file3.h> ... <out_index_directory>");
	exit(-1);
}


int main(int argc, char **argv) {
	if(argc<=2) USAGE();
	std::string out = argv[1];

	mkdir((const std::string&)argv[argc-1]);
	for(int i=1; i<argc-1; i++) index_file(argv[i], argv[argc-1]);

	return 0;
}
