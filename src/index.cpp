/*
 * index.cpp
 *
 *  Created on: 18 mai 2015
 *      Author: jfellus
 */


#include "index.h"
#include <fstream>
#include <string.h>
#include <sstream>
#include "utils/utils.h"

void skip_line(std::ifstream& f) {
	while(f.get()!='\n' && f.good());
}

void skip_multiline_comment(std::ifstream& f) {
	char last = 0;
	for(char c = f.get(); f.good() && !(last=='*' && c=='/'); c = f.get()) last = c;
}

void read_kw(std::ifstream& f, const char* kw, char c, int& nb) {
	if(nb==(int)strlen(kw)) nb = 0;
	if(c==kw[nb] || (kw[nb]==' ' && isspace(c))) nb++;
	else nb = 0;
}

std::string read_identifier(std::ifstream& f, char& c) {
	std::ostringstream oss;
	while(f.good() && (isspace(c))) c = f.get();
	while(f.good() && (isalnum(c) || c=='_')) { oss << c; c = f.get();}
	return oss.str();
}

std::string read_type(std::ifstream& f, char& c) {
	std::ostringstream oss;
	for(; f.good() && (isspace(c)); c = f.get()) {}
	for(; f.good() && (isalnum(c) || c=='_' || c=='<' || c=='>' || c==':' || c=='*' || c=='&'); c = f.get()) oss << c;
	return oss.str();
}


std::string read_until(std::ifstream& f, char to) {
	std::ostringstream oss;
	char c;
	for(c = f.get(); f.good() && c!=to; c = f.get()) oss << c;
	return oss.str();
}

std::string read_between(std::ifstream& f, char from, char to) {
	std::ostringstream oss;
	char c;
	for(c = f.get(); f.good() && c!=from; c = f.get()) {}
	for(c = f.get(); f.good() && c!=to; c = f.get()) oss << c;
	return oss.str();
}





FILE* outf = 0;
std::string outdir = ".";


void add_class(const std::string& cls) {
	//DBG("CLASS : " << cls);
	std::string outfile = TOSTRING(outdir << "/" << cls << ".ind");
	outf = fopen(outfile.c_str(), "w");
	if(!outf) DBG("Couldn't create index file : " << outfile);
}


void add_field(const std::string& cls, const std::string& type, const std::string& id) {
	//DBG("  - " << type << " " << id);
	if(!outf) DBG("No defined class for field " << type << " " << id);
	fprintf(outf, "f %s %s\n", type.c_str(), id.c_str());
}

void add_process_method(const std::string& cls, const std::string& params) {
	//DBG("  - process(((" << params << ")))");
	if(!outf) DBG("No defined class for process(" << params << ")");
	fprintf(outf, "p %s\n", params.c_str());
}











void index_file(const char* file, const char* _outdir) {
	outdir = _outdir;
	std::ifstream f(file);
	char last = 0;
	bool is_string = false;
	int nested_level = 0;
	int nClass=0;
	std::string curClass = "";
	std::string curType = "";
	std::string curId = "";
	for(char c = f.get();f.good();c = f.get()) {

		// Process special entities
		if(last=='/' && c=='*') { skip_multiline_comment(f); last = '\n';}
		else if(last=='/' && c=='/') { skip_line(f); last = '\n'; }
		else if(last=='\n' && c=='#') { skip_line(f); last = '\n'; }
		else if(c=='"') is_string = !is_string;
		else if(c=='{') { nested_level++; curType=""; }
		else if(c=='}') { nested_level--; if(nested_level==0) curClass=""; curType=""; curId="";}
		else if(isspace(c)) continue;
		else {

			// Find class
			read_kw(f, "class", c, nClass);
			if(nClass==strlen("class")) {c = f.get(); curClass = read_identifier(f, c); add_class(curClass);}

			// Find members & process method
			if(nested_level==1 && !curClass.empty()) {
				if(curType.empty()) {
					curType = read_type(f, c); curId="";
					if(c=='(') { read_until(f, ')'); curType = ""; }
					else if(str_ends_with(curType, ":")) curType = "";
				}
				if(!curType.empty() && curId.empty()) {
					curId = read_identifier(f, c);
				}
				if(c==';' && !curType.empty() && !curId.empty()) { add_field(curClass, curType, curId); curType = ""; curId = ""; }
				else if(curType=="void" && curId=="process" && c=='(') {
					std::string params = read_until(f, ')');
					add_process_method(curClass, params);
				}
			}
			last = c;
		}
	}
	f.close();
}


