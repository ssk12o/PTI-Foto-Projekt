#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

class Photo
{
    private:
        Mat image;
        Mat hsv;
        Mat value;
        string path;

        void calcHsv();
        void calcValue();
        void calc();
    public:
        Photo(Mat);
        Photo(string);

        Mat get_image();
        void set_image(Mat);
        Mat get_hsv();
        Mat get_value();
        void set_path(string);
        
        void show(string);
        void changeValue(Mat*, Mat);
        Mat apply_new_value(Mat);
        int mediumValue();
        void histSplit(Mat*, Mat*);
        Mat histMerge(Mat, Mat);
        void valueSplit(Mat*, Mat*);
        Mat valueMerge(Mat, Mat);
        Mat getMask();
        int countNoise();        

        void showHist(string title = "Histogram");
        void showHE(string title = "HE image");
        void showCLAHE(string title = "CLAHE image");
        void showBHE(string title = "BHE image");
        void showGC(float gamma = 0.8, string title = "GC image");
        void showNR(int count = 5, string title = "NR image");
        void showMBOBHE(string title = "MBOBHE image");
        void showMSRCR(string title = "MSRCR image");
        
        Mat getHist();
        Mat getHE();
        Mat getCLAHE();
        Mat getBHE();
        Mat getGC(float gamma = 0.8);
        Mat getNR(int count = 5);
        Mat getMBOBHE();
        Mat getMSRCR();
};