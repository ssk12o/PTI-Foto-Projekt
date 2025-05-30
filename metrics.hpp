#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
#include <cmath>
#include "photo.hpp"
double getNoiseMetric(Mat value);

double getContrastMetric(Mat value);

double getSharpnessMetric(Mat value);