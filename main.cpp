#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <filesystem>
#include "photo.hpp"
#include "hnorm.hpp"
#include "filter.hpp"

using namespace std;
using namespace std::filesystem;

void other(path input,path output,char read){

    Photo image(input.string());    
    Mat fixed;
    bool skip = false;
    
    switch (read) {
        case '2':
            fixed = getHnorm(image);
            break;
        case '3':
            fixed = getBilateralFilter(image);
            break;
        case '4':
            fixed = getGaussianFilter(image);
            break;
        case '5':
            fixed = deNoise(image);
            break;
        case '6':
            fixed = getUnsharpMask(image);
            break;
        default:
            skip = true;
            break;
    }
    if(!skip)
        imwrite(output.string(), fixed);    
    

    waitKey(0);
    return;

}



int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Usage: program.exe <path to folder with images> <path to destination folder>" << endl;
        return 1;
    }

    path inputPath = argv[1];
    path outputPath = argv[2];
    string pti = "/PTI_";
    char read;


    cout << "Wybierz operacje:\n"
    "1 - usuwanie farfocli\n"
    "2 - normalizacja histogramu jasnosci\n"
    "3 - filtracja filtrem bilateralnym\n"
    "4 - filtracja filtrem gaussowskim\n"
    "5 - usuwanie szumu usrednianiem pikseli\n"
    "6 - wyostrzanie maska wyostrzajaca\n";
    cin >> read;

    if(read != '1'){
        other(inputPath,outputPath, read);
        return 0;
    }

    if (!exists(inputPath)) {
        cerr << "Error: no such file or directory.\n";
        return 1;
    }

    if (!exists(outputPath) || !is_directory(outputPath)) {
        create_directories(outputPath);
    }

    if (!is_directory(inputPath)) {
        imwrite((outputPath / inputPath.filename()).string(), imread(inputPath.string()));
        inputPath = outputPath;
    }
    
    int i = 1;
    cout << "============================================================================\n" << "START\n";
    for (const auto& entry : directory_iterator(inputPath)){
        if(is_regular_file(entry.status())){

            Photo image(entry.path().string());    

            double imgsize = (image.get_value().cols*image.get_value().rows);
            int n = 1;
            double pix_diff = 0;
            
            cout << "image " << i << " - " << entry.path().filename().string() << ": ";

            while(100*pix_diff/imgsize < 0.65 && n <= 70)
            {
                cout << "|";
                n += 2;
                Photo nr(image.getNR(5, n));
                pix_diff = nr.pix_difference(image.get_value());
            }

            int save_n = n;

            while(save_n <= 70){
                cout << '-';
                this_thread::sleep_for(chrono::milliseconds(10));
                save_n += 2;
            }
    
            imwrite(outputPath.string() + pti + entry.path().filename().string(), image.getNR(5, n));

            cout << "\t\t" << n << '/' << "71" << '\n';
            i++;
        }
    }
    cout << "All images were processed." << '\n';
    cout << "============================================================================\n";

    return 0;
}