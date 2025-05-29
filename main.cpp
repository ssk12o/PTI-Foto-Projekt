#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <filesystem>
#include <map>
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

    map<string, double> params {
        {"N", 1},
        {"STEP", 2},
        {"MAX_DIFF", 0.65},
        {"MAX_N", 69},
        {"NR_ITERATIONS", 5}
    };

    int param_opt;
    cout << "Wybierz opcję:\n 1. Automatyczny dobór parametrów\n 2. Ręczne ustawienie parametrów\n";
    cin >> param_opt;

    if(param_opt == 2){
        bool break_while = false;
        while(!break_while){
            int opt;
            cout<< "Parameters to change - 5:\n"
                << "\t1 - Początkowa szerokość filtru gaussowskiego = 1 piks. (musi być nieparzysta)\n"
                << "\t2 - Pojedyncza zmiana szerokości filtru gaussowskiego = 2 piks. (musi być parzysta)\n"
                << "\t3 - Graniczna szerokość filtru gaussowskiego = 69 piks.\n"
                << "\t4 - Maksymalna dopuszczalna zmiana zdjęcia = 0.65 %\n"
                << "\t5 - Ilość iteracji filtru usuwającego farfocle = 5 razy\n"
                << "Pomoc - 6\n"
                << "Gotowe - 7\n";
            cin >> opt;
            double opVal;
            switch (opt)
            {
            case 1:
                cout << "Początkowa szerokość filtru gaussowskiego = ";
                cin >> opVal;
                if(static_cast<int>(opVal) % 2 == 0){
                    cout << "\n Parametr nie został zmieniony. Wartość tego parametru powinna być nieparzysta.\n";
                    break;
                }
                params["N"] = opVal;
                cout << "\nParameter został pomyślnie zmieniony.\n";
                break;
            case 2:
                cout << "Pojedyncza zmiana szerokości filtru gaussowskiego = ";
                cin >> opVal;
                if(static_cast<int>(opVal) % 2 == 1){
                    cout << "\nParametr nie został zmieniony. Wartość tego parametru powinna być parzysta.\n";
                    break;
                }
                params["STEP"] = opVal;
                cout << "\nParameter został pomyślnie zmieniony.\n";
                break;
            case 3:
                cout << "Graniczna szerokość filtru gaussowskiego = ";
                cin >> params["MAX_DIFF"];
                cout << "\nParameter został pomyślnie zmieniony.\n";
                break;
            case 4:
                cout << "Maksymalna dopuszczalna zmiana zdjęcia = ";
                cin >> opVal;
                if(opVal > 5){
                    cout << "\n Parametr nie został zmieniony. Wartość tego parametru nie może przekraczać 5%.\n";
                    break;
                }
                params["MAX_N"] = opVal;
                cout << "\nParameter został pomyślnie zmieniony.\n";
                break;
            case 5:
                cout << "Ilość iteracji filtru usuwającego farfocle = ";
                cin >> params["NR_ITERATIONS"];
                cout << "\nParameter został pomyślnie zmieniony.\n";
                break;
            case 6:
                cout << "Nie chce mie się tego pisać...\n";
                break;
            case 7:
                break_while = true;
                break;
            default:
                break;
            }
        }
    }

    
    
    int i = 1;
    cout << "============================================================================\n" << "START\n";
    for (const auto& entry : directory_iterator(inputPath)){
        if(is_regular_file(entry.status())){

            Photo image(entry.path().string());    

            double imgsize = (image.get_value().cols*image.get_value().rows);
            int n = params["N"];
            double pix_diff = 0;
            
            cout << "image " << i << " - " << entry.path().filename().string() << ": ";

            while(100*pix_diff/imgsize < params["MAX_DIFF"] && n <= params["MAX_N"])
            {
                cout << "|";
                n += params["STEP"];
                Photo nr(image.getNR(params["NR_ITERATIONS"], n));
                pix_diff = nr.pix_difference(image.get_value());
            }

            int save_n = n;

            while(save_n <= params["MAX_N"]){
                cout << '-';
                this_thread::sleep_for(chrono::milliseconds(10));
                save_n += params["STEP"];
            }
    
            imwrite(outputPath.string() + pti + entry.path().filename().string(), image.getNR(params["NR_ITERATIONS"], n));

            cout << "\t\t" << n << '/' << params["MAX_N"]+1 << '\n';
            i++;
        }
    }
    cout << "All images were processed." << '\n';
    cout << "============================================================================\n";

    return 0;
}