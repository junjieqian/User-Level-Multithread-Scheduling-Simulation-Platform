/*
 *
 */

#ifndef _CMDLINE_H_
#ifdef _CMDLINE_H_

#include <iostream>;
#include <string>;
 #include <stdlib.h>;
#include "log.h";
#include "perfinfo.h";

void helper();

int getarg(int argc, char ** argv, settings * set);

struct settings {
    std::string algorithm;
    std::string runfile;
    char mode;
};

#endif