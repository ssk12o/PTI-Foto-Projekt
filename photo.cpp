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
    if(this->image.channels() == 3){
        cvtColor(this->image, this->hsv, COLOR_BGR2HSV);
    }
}
void Photo::calcValue(){
    if(this->image.channels() == 1){ this->value = this->image.clone(); return; }
    Mat valueImg(this->hsv.rows, this->hsv.cols, CV_8UC1);
    for (int y = 0; y < this->hsv.rows; y++) {
        for (int x = 0; x < this->hsv.cols; x++) {
            valueImg.at<uchar>(y, x) = this->hsv.at<Vec3b>(y, x)[2];
        }
    }
    this->value = valueImg.clone();
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
    if(this->image.channels() == 1) { return value;}
    Mat hsv_new = this->hsv.clone();
    changeValue(&hsv_new, value);
    Mat image_new;
    cvtColor(hsv_new, image_new, COLOR_HSV2BGR);
    return image_new;
}
int Photo::mediumValue(){
    int sum = 0;
    for(int y = 0; y < image.rows; y++){
        for(int x = 0; x < image.cols; x++){
            sum += static_cast<int>(this->value.at<uchar>(y, x));
        }
    }
    return sum/(image.rows*image.cols);
}
void Photo::histSplit(Mat* hist1, Mat* hist2){
    Mat hist = this->getHist().clone();
    int size = hist.rows;
    *hist1 = Mat::zeros(size, 1, CV_32F);
    *hist2 = Mat::zeros(size, 1, CV_32F);
    int threshold = this->mediumValue();
    
    for(int i = 0; i < size; i++) {
        if (i < threshold) {
            (*hist1).at<float>(i) = hist.at<float>(i);
        } else {
            (*hist2).at<float>(i) = hist.at<float>(i);
        }
    }
}
Mat Photo::histMerge(Mat hist1, Mat hist2){
    Mat hist(hist1.rows, 1, CV_32F);
    for(int i = 0; i < hist1.rows; i++) {
        hist.at<float>(i) = hist1.at<float>(i) + hist2.at<float>(i); 
    }
    return hist;
}
void Photo::valueSplit(Mat* value1, Mat* value2){
    *value1 = Mat::zeros(value.rows, value.cols, CV_8UC1);
    *value2 = Mat::zeros(value.rows, value.cols, CV_8UC1);

    int threshold = this->mediumValue();

    for (int y = 0; y < this->image.rows; y++) {
        for (int x = 0; x < this->image.cols; x++) {
            int intensity = static_cast<int>(this->value.at<uchar>(y, x));
            if(intensity < threshold){
                (*value1).at<uchar>(y, x) = static_cast<uchar>(intensity);
            } else {
                (*value2).at<uchar>(y, x) = static_cast<uchar>(intensity);
            }
        }
    }
}
Mat Photo::valueMerge(Mat value1, Mat value2){
    Mat value(value1.rows, value1.cols, CV_8UC1);
    for(int y = 0; y < this->image.rows; y++) {
        for(int x = 0; x < this->image.cols; x++) {
            int sum = static_cast<int>(value1.at<uchar>(y, x)) + static_cast<int>(value2.at<uchar>(y, x));
            value.at<uchar>(y, x) = static_cast<uchar>(sum);
        }
    }
    return value;
}
int Photo::countNoise(){
    Mat blurred;
    medianBlur(this->value, blurred, 3);

    Mat diff;
    absdiff(this->value, blurred, diff);

    Mat mask;
    threshold(diff, mask, 40, 255, THRESH_BINARY);

    int sum = 0;

    for(int y = 0; y < this->value.rows; y++){
        for(int x = 0; x < this->value.cols; x++){
            if(static_cast<int>(mask.at<uchar>(y, x)) == 255){
                sum++;
            }
        }
    }

    return sum;
}
Mat Photo::getNR(int count){
    if(count == 0){return this->value;}

    Mat blurred;
    medianBlur(this->value, blurred, 3);

    Mat diff;
    absdiff(this->value, blurred, diff);

    Mat mask;
    threshold(diff, mask, 40, 255, THRESH_BINARY);



    Mat valueNR = this->value.clone();
    int k = 15;
    for(int y = 0; y < this->value.rows; y++){
        for(int x = 0; x < this->value.cols; x++){
            valueNR.at<uchar>(y, x) = this->value.at<uchar>(y, x);
            if(static_cast<int>(mask.at<uchar>(y, x)) == 255){
                int sum = 0;
                int n = 0;
                for(int i = -k; i <= k; i++){
                    for(int j = -k; j <= k; j++){
                        if(y + j >= valueNR.rows || y + j < 0 || x + i >= valueNR.cols || x + i < 0) { continue; }
                        if(static_cast<int>(mask.at<uchar>(y + j, x + i)) == 255) { continue; }
                        sum += static_cast<int>(valueNR.at<uchar>(y + j, x + i));
                        n++;
                    }
                }
                if (n == 0) { valueNR.at<uchar>(y, x) = static_cast<uchar>(128); continue; }
                int av = sum / n;
                valueNR.at<uchar>(y, x) = static_cast<uchar>(av);
            }
        }
    }

    Photo cleaned(valueNR);

    count--;
    return cleaned.getNR(count);
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
void Photo::showBHE(){
    namedWindow("BHE image", WINDOW_NORMAL);
    resizeWindow("BHE image", 600, 450);
    imshow("BHE image", this->getBHE());
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
void Photo::showNR(int count){
    namedWindow("NR image", WINDOW_NORMAL);
    resizeWindow("NR image", 600, 450);
    imshow("NR image", this->getNR(count));
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
Mat Photo::getBHE(){
    Mat value1, value2;
    this->valueSplit(&value1, &value2);

    Photo img1(value1);
    Photo img2(value2);
    Mat clahe1 = img1.getCLAHE();
    Mat clahe2 = img2.getCLAHE();

    return this->valueMerge(clahe1, clahe2);
}
Mat Photo::getGC(float gamma){
    Mat value_GC = this->value.clone();
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







