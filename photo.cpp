#include "photo.hpp"

Photo::Photo(Mat image){
    this->image = image;
    calc();
}
Photo::Photo(string path){
    this->image = imread(path);
    calc();
}

void Photo::calc(){
    calcHsv();
    calcValue();
}
void Photo::calcHsv(){
    cvtColor(this->image, this->hsv, COLOR_BGR2HSV);
}
void Photo::calcValue(){
    Mat valueImg(this->hsv.rows, this->hsv.cols, CV_8UC1);
    for (int y = 0; y < this->hsv.rows; y++) {
        for (int x = 0; x < this->hsv.cols; x++) {
            valueImg.at<uchar>(y, x) = this->hsv.at<Vec3b>(y, x)[2];
        }
    }
    this->value = valueImg;
}

Mat Photo::get_image(){
    return this->image;
}
void Photo::set_image(Mat image){
    this->image = image;
}
Mat Photo::get_hsv(){
    return this->hsv;
}
Mat Photo::get_value(){
    return this->value;
}
void Photo::set_path(string path){
    this->path = path;
}

void Photo::show(string name){
    namedWindow(name, WINDOW_NORMAL);
    resizeWindow(name, 600, 450);
    imshow(name, this->image);
}
void Photo::changeValue(Mat *hsv, Mat value){
    for (int y = 0; y < this->image.rows; y++) {
        for (int x = 0; x < this->image.cols; x++) {
            (*hsv).at<Vec3b>(y, x)[2] = value.at<uchar>(y, x);
        }
    }
}
Mat Photo::apply_new_value(Mat value){
    Mat hsv_new = this->hsv;
    changeValue(&hsv_new, value);
    Mat image_new;
    cvtColor(hsv_new, image_new, COLOR_HSV2BGR);
    return image_new;
}

void Photo::showHist(){
    Mat hist = this->getHist();
    int histSize = 256;
    int hist_w = 512; 
    int hist_h = 400; 
    int bin_w = cvRound((double) hist_w / histSize);

    Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(255, 255, 255)); 

    normalize(hist, hist, 0, histImage.rows, NORM_MINMAX);

    for (int i = 0; i < histSize; i++) 
    {
        line(histImage,
            Point(bin_w * i, hist_h),
            Point(bin_w * i, hist_h - cvRound(hist.at<float>(i))),
            Scalar(255, 0, 0), 
            1, 8, 0);
    }

    imshow("Histogram", histImage);

}
void Photo::showHE(){
    namedWindow("HE image", WINDOW_NORMAL);
    resizeWindow("HE image", 600, 450);
    imshow("HE image", this->getHE());
}
void Photo::showCLAHE(){
    namedWindow("CLAHE image", WINDOW_NORMAL);
    resizeWindow("CLAHE image", 600, 450);
    imshow("CLAHE image", this->getCLAHE());
}
void Photo::showGC(float gamma){
    namedWindow("GC image", WINDOW_NORMAL);
    resizeWindow("GC image", 600, 450);
    imshow("GC image", this->getGC(gamma));
}
void Photo::showMBOBHE(){

}
void Photo::showMSRCR(){

}

Mat Photo::getHist(){
    int histSize = 256; 
    float range[] = { 0, 256 }; 
    const float* histRange = { range };

    Mat hist;

    calcHist(&(this->value), 1, 0, Mat(), hist, 1, &histSize, &histRange);
    return hist;
}
Mat Photo::getHE(){
    Mat value_HE;
    equalizeHist(this->value, value_HE);
    
    return this->apply_new_value(value_HE);
}
Mat Photo::getCLAHE(){
    Ptr<CLAHE> clahe = createCLAHE();

    clahe->setClipLimit(2.0);
    clahe->setTilesGridSize(Size(8,8));

    Mat value_CLAHE;
    clahe->apply(this->value, value_CLAHE);

    return this->apply_new_value(value_CLAHE);
}
Mat Photo::getGC(float gamma){
    Mat value_GC = this->value;
    for (int y = 0; y < this->image.rows; y++){
        for (int x = 0; x < this->image.cols; x++){
            float i1 = static_cast<int>(this->value.at<uchar>(y, x));
            float i2 = (255*pow(i1/255, gamma));
            value_GC.at<uchar>(y, x) = static_cast<uchar>(i2);
        }
    }

    return this->apply_new_value(value_GC);
}
// Mat Photo::getMBOBHE(){}
// Mat Photo::getMSRCR(){}







