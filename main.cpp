#include <iostream>
#include <vector>
#include <string>
#include "photo.hpp"


int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Usage: program.exe <path to photo> <path to destination/name>" << endl;
        return 1;
    }

    string input = argv[1];
    string output = argv[2];

    Photo image(input);    
    
    imwrite(output, image.getNR());

    waitKey(0);
    return 0;
}