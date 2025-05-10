#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <filesystem>
#include "photo.hpp"
#include "hnorm.hpp"


using namespace std::filesystem;


int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Usage: program.exe <path to folder with images> <path to destination folder>" << endl;
        return 1;
    }

    path inputPath = argv[1];
    path outputPath = argv[2];
    string pti = "/PTI_";

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