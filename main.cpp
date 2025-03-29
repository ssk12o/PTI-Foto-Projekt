#include <iostream>
#include <vector>
#include "photo.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: program.exe <path to photo>" << endl;
        return 1;
    }

    string path = argv[1];

    Photo image(path);
    image.show("Original image");
    image.showHist();
    image.showHE();
    image.showCLAHE();
    image.showGC();
    image.showBHE();
    
    waitKey(0);
    return 0;
}