#include <cstring>
#include <getopt.h>
#include <iomanip>
#include <iostream>
#include <podofo/doc/PdfPainter.h>
#include <podofo/doc/PdfStreamedDocument.h>
#include <podofo/podofo.h>
#include <vector>

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

std::vector<PdfRect> get_crop_boxes(const char *pszInput) {
    std::vector<PdfRect> rects;

    std::string sLine;
    PdfRect curRect;
    int x, y, w, h;
    if (sscanf(pszInput, "%i %i %i %i\n", &x, &y, &w, &h) != 4) {
        printf("Failed to read bounding box's four numbers from '%s'\n",
               sLine.c_str() + 15);
        exit(1);
    }
    curRect = PdfRect(static_cast<double>(x), static_cast<double>(y),
                      static_cast<double>(w - x), static_cast<double>(h - y));
    rects.push_back(curRect);

    return rects;
}

int main() {
    PdfError::EnableDebug(false);
    PdfMemDocument output;
    PdfMemDocument input;

    try {
        input.Load("test.pdf");
    } catch (const PdfError &e) {
        e.PrintErrorMsg();
        return e.GetError();
    }
    int inputSize = input.GetPageCount();
    cout << "Your pdf has: " << inputSize << endl;

    double a4_w = 595;
    double a4_h = 842;
    double a4_w4 = a4_w / 2;
    double a4_h4 = a4_h / 2;

    double p_w = input.GetPage(0)->GetPageSize().GetWidth();
    double p_h = input.GetPage(0)->GetPageSize().GetHeight();

    double sc_x = (p_w / a4_w4);
    double sc_y = (p_h / a4_h4);

    double rem = sc_x - 1;
    if (sc_x > 1) {
        sc_x = 1 - rem;
    } else if (sc_x < 1) {
        sc_x = 1 + rem;
    }

    rem = sc_y - 1;
    if (sc_y > 1) {
        sc_y = 1 - rem;
    } else if (sc_y < 1) {
        sc_y = 1 + rem;
    }

    for (int i = 0; i < inputSize; i += 4) {
        PdfPage *page =
            output.CreatePage(PdfPage::CreateStandardPageSize(ePdfPageSize_A4));
        if (!page) {
            // PODOFO_RAISE_ERROR(PoDoFo::ePdfError_InvalidHandle);
        }
        PdfPainter painter;
        painter.SetPage(page);

        for (int b = 0; b < inputSize - i && b < 4; b++) {
            PdfXObject page(input, i + b, &output, NULL, true);
            switch (b) {
            case 0:
                painter.DrawXObject(0, a4_h4, &page, sc_x, sc_y);
                break;
            case 1:
                painter.DrawXObject(a4_w4, a4_h4, &page, sc_x, sc_y);
                break;
            case 2:
                painter.DrawXObject(0, 0, &page, sc_x, sc_y);
                break;
            case 3:
                painter.DrawXObject(a4_w4, 0, &page, sc_x, sc_y);
            }
        }
        painter.FinishPage();
    }

    output.Write("output.pdf");

    return 0;
}

int main_t(int argc, char **argv) {

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
    try {
        PdfMemDocument input;
        input.Load("test.pdf");
        PdfPainter painter;
        PdfFont *pfont = input.CreateFont("Arial");
        if (!pfont) {
            PODOFO_RAISE_ERROR(ePdfError_InvalidHandle);
        }

        PdfStreamedDocument output("output.pdf");

        for (int i = 0; i < input.GetPageCount(); i++) {
            PdfPage *iPage = input.GetPage(i);
            PdfPage *oPage = output.CreatePage(
                PdfPage::CreateStandardPageSize(ePdfPageSize_A4));
            if (!oPage) {
                PODOFO_RAISE_ERROR(ePdfError_InvalidHandle);
            }
            // PdfRect crop = {0, 0, 414, 641};
            // PdfVariant var;
            // crop.ToVariant(var);
            // pPage->GetObject()->GetDictionary().AddKey(
            // PdfName("MediaBox"), var );

            painter.SetPage(oPage);
            painter.SetFont(pfont);
            painter.DrawText(20, 20, "hello world");
            painter.FinishPage();
        }
        // input.Write("output.pdf");
        output.Close();
    } catch (const PdfError &e) {
        fprintf(stderr,
                "Error: An error %i ocurred during croppping pages in the pdf "
                "file.\n",
                e.GetError());
        e.PrintErrorMsg();
        return e.GetError();
    }

    int pages = endPage - startPage;
    return 0;
}
