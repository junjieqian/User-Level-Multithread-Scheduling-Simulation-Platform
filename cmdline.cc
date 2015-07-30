/* cmdline.cc
 *
 */

#include <iostream>
#include "cmdline.h"

int main(int argc, char ** argv) {
    // start with helper info
    helper();
    settings set;
    if (getarg(argc, argv, &set) < 1) exit(0);
}

void helper() {
    std::cout << "user level multithreaded scheduling simulation platform\n";
    std::cout << "usage: ./scheduler <algorithm> <runfile> <mode>\n";
    std::cout << "\t <algorithm>, d: default OS; power: power efficient; performance: best performance, perf on the cache miss and memory balance; (more to add later)\n";
    std::cout << "\t <runfile>, the file contains the application(s)\n";
    std::cout << "\t <mode>, all the applications in the runfile in parallel or serial\n";
    return;
}

int getarg(int argc, char ** argv, settings * set) {

}