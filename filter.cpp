#include "filter.hpp"


Mat getBilateralFilter(Photo image){
    int d = 5;;
    double sigma = 0.0;
    Mat value = image.get_value();
    Mat FValue;
    
    cout << "Input filter size (int x)\n";
    cin >> d;
    cout << "Filter size = " << d  << "\n";
    cout << "Input sigma value (double  x.y)\n";
    cin >> sigma;
    cout << "sigma = " << sigma  << "\n";

    bilateralFilter(value,FValue,d,sigma,sigma);

    return FValue;
}


Mat getGaussianFilter(Photo image){
    int d = 5;;
    double sigma = 0.0;
    Mat value = image.get_value();
    Mat FValue;
    
    cout << "Input filter size (int x)\n";
    cin >> d;
    if(d%2 == 0)
        d++;
    cout << "Filter size = " << d  << "\n";
    cout << "Input sigma value (double  x.y)\n";
    cin >> sigma;
    cout << "sigma = " << sigma  << "\n";

    GaussianBlur(value,FValue,Size(d,d),sigma,sigma);

    return FValue;
}

Mat deNoise(Photo image){
    float h = 5;
    Mat value = image.get_value();
    Mat FValue;
    
    cout << "Input filter strength (float x)\n";
    cin >> h;
    cout << "Filter strength = " << h  << "\n";

    fastNlMeansDenoising(value,FValue,h);

    return FValue;
}

Mat getUnsharpMask(Photo image){

    Mat value = image.get_value();
    Mat FValue;
    int d = 3;
    float sigma = 1.0f;

    cout << "Input filter size (int x)\n";
    cin >> d;
    if(d%2 == 0)
        d++;
    cout << "Filter size = " << d  << "\n";
    cout << "Input sigma value (double  x.y)\n";
    cin >> sigma;
    cout << "sigma = " << sigma  << "\n";

    GaussianBlur(value,FValue,Size(d,d),sigma,sigma);
    addWeighted(value, 1.0 +sigma, FValue, -sigma,0.0,FValue);

    return FValue;
}

Mat getAdaptive(Photo image){
    Mat value = image.get_value();
    Mat Fvalue;
    float con = getContrastMetric(value);
    int d;
    float noise;
    float sharp;
    float sigma;

    if(con <0.2){
        value = getHnorm(value);
    }
    noise = getNoiseMetric(value);

    d = (int)noise;
    d=d*9;
    if(d%2 == 0)
        d++;
    sigma = noise*3;
    bilateralFilter(value,Fvalue,d,sigma,sigma);
    value = Fvalue;

    sharp = getSharpnessMetric(value);
    
    if(sharp <5.0){
        sigma = 5.0 - sharp;
        d = (int)(sigma*2);
        if(d%2 == 0)
            d++;
        GaussianBlur(value,Fvalue,Size(d,d),sigma,sigma);
        addWeighted(value, 1.0 +sigma, Fvalue, -sigma,0.0,Fvalue);
    }
    
    
    return Fvalue;
}