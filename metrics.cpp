#include "metrics.hpp"

using namespace std;

double getNoiseMetric(Mat value){
    Mat filtered;
    Mat mask  = Mat(3,3, CV_16S);
    double sigma;
    int W = value.cols;
    int H = value.rows;
    unsigned int sum = 0;

    
    vector<vector<short>> vMask = {{1,-2,1},{-2,4,-2},{1,-2,1}};
    for(int i = 0;i<3;i++){
        for(int j=0;j<3;j++){
            mask.at<short>(i, j) = vMask[i][j];
        }
    }

    
    filter2D(value,filtered, CV_8UC1, mask);
    sigma = 6.0 * (W-2) * (H-2);

    for(int i = 0;i<H;i++){
        for(int j = 0;j<W;j++){
            sum  += filtered.at<uchar>(i,j);
            //cout << sum << "\n";
        }
    }
    sigma = sum/sigma;
    sigma = sigma * sqrt(3.14159/2);


    
    return sigma;
}

double getContrastMetric(Mat value){
    double RMS = 0.0;
    double AVG = 0.0;
    double count = value.cols * value.rows;
    double temp;

    for(int i = 0;i<value.rows;i++){
        for(int j = 0;j<value.cols;j++){
            AVG += value.at<uchar>(i,j)/255.0;
        }
    }
    
    AVG/=count;

    for(int i = 0;i<value.rows;i++){
        for(int j = 0;j<value.cols;j++){
            temp = value.at<uchar>(i,j)/255.0 - AVG;
            RMS += temp*temp;
        }
    }
    RMS/=count;

    RMS = sqrt(RMS);

    return RMS;
    
}

double getSharpnessMetric(Mat value){
    Mat filtered;
    Mat mask  = Mat(3,3, CV_16S);
    double AVG = 0.0;
    double count = value.cols * value.rows;

    
    vector<vector<short>> vMask = {{0,1,0},{1,-4,1},{0,1,0}};
    for(int i = 0;i<3;i++){
        for(int j=0;j<3;j++){
            mask.at<short>(i, j) = vMask[i][j];
        }
    }

    filter2D(value,filtered, CV_8UC1, mask);

    for(int i = 0;i<value.rows;i++){
        for(int j = 0;j<value.cols;j++){
            AVG += filtered.at<uchar>(i,j);
        }
    }
    AVG/=count;

    return AVG;
}