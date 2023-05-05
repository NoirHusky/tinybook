#include <getopt.h>
#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

bool file_exists(const char *filename) {
    bool res = false;
    FILE *fp = fopen(filename, "r");

    if (fp) {
        res = true;
        fclose(fp);
    }
    return res;
}

void usage() {

    exit(1);
}
pair<int, int> parse_pages(const char *arg) { return {1, 0}; }

int main(int argc, char **argv) {

    int n;
    // char *filename = nullptr;
    int startPage, endPage;

    while (n != -1) {
        switch (n) {
        case 'p': {
            auto [startPage, endPage] = parse_pages(optarg);
            break;
        }
        // case 'f':
        //     filename = strdup(optarg);
        //     break;
        // };
        n = getopt(argc, argv, "p:f:");
    }

    // if ( filename == nullptr ) {
    //     usage();
    // }
    // if (file_exists(filename) == false) {
    //     cout << "ERROR: the " << filename << " doesn't exist!\n";
    //     exit(1);
    // }
    cout << startPage << " ::: " << endPage << endl;

    return 0;
}
