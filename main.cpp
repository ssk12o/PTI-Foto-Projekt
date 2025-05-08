#include <iostream>
#include <string>
#include "photo.hpp"
#include "hnorm.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Usage: program.exe <path to photo> <path to destination/name>" << endl;
        return 1;
    }

    string input = argv[1];
    string output = argv[2];

    Photo image(input);    
    Mat fixed;
    char read;
    
    cout << "Wybierz operacje:\n1 - usuwanie farfocli\n2 - normalizacja histogramu jasnosci\n";
    cin >> read;
    switch (read) {

        case '1':
            fixed = image.getNR();
            break;
        case '2':
            fixed = getHnorm(image);
            break;
        default:
            break;
    }

    
    imwrite(output, fixed);

    waitKey(0);
    return 0;
}