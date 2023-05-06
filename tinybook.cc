#include <cstring>
#include <getopt.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <podofo/podofo.h>

using namespace std;
using namespace PoDoFo;

bool file_exists(const char *filename) {
    bool res = false;
    FILE *fp = fopen(filename, "r");

    if (fp) {
        res = true;
        fclose(fp);
    }
    return res;
}

void usage() { exit(1); }
std::vector<std::string> split(const std::string &str, char delim) {
    std::vector<std::string> strings;
    size_t start;
    size_t end = 0;
    while ((start = str.find_first_not_of(delim, end)) != std::string::npos) {
        end = str.find(delim, start);
        strings.push_back(str.substr(start, end - start));
    }
    return strings;
}



int main(int argc, char **argv) {

    int n;
    // char *filename = nullptr;
    int startPage, endPage;

    while (n != -1) {
        switch (n) {
            case 'p': {
                auto str = split(string(optarg), '-');
                startPage = stoi(str[0]);
                endPage = stoi(str[1]);
                break;
            }
            // case 'f':
            //     filename = strdup(optarg);
            //     break;
        };
        n = getopt(argc, argv, "p:f:");
    }

    // if ( filename == nullptr ) {
    //     usage();
    // }
    // if (file_exists(filename) == false) {
    //     cout << "ERROR: the " << filename << " doesn't exist!\n";
    //     exit(1);
    // }

    PdfMemDocument input;
    input.Load("test.pdf");
    for ( int i = 0; i < input.GetPageCount(); i++ ) {
        PdfPage* pPage = input.GetPage(i);
    }
    input.Write("output.pdf");


    int pages = endPage - startPage;
    return 0;
}
