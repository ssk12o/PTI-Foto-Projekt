#include "hnorm.hpp"

using namespace std;

vector<pair<unsigned char, int>> createValueList(vector<vector<unsigned char>> Ivals) {

    unordered_map<unsigned, int> map;
    vector<pair<unsigned char, int>> List;
    unsigned char u;

    for (int i = 0;i < Ivals.size();i++) {
        for (int j = 0;j < Ivals[0].size();j++) {
            map[Ivals[i][j]]++;
        }
    }

    for (int i = 0;i <= 255;i++) {
        u = (unsigned char)i;
        if (map.count(u)) {
            List.push_back(make_pair(u, map[u]));
        }
    }

    return List;
}

vector<pair<unsigned char, int>> createDist(vector<pair<unsigned char, int>> List) {
    int sum = 0;
    vector<pair<unsigned char, int>> Dist;

    for (int i = 0;i < List.size();i++) {
        sum += List[i].second;
        Dist.push_back(make_pair(List[i].first, sum));
    }

    return Dist;
}

unordered_map<unsigned char, int> createDistMap(vector<pair<unsigned char, int>> Dist) {

    unordered_map<unsigned char, int> DistMap;

    for (int i = 0;i < Dist.size();i++) {
        DistMap[Dist[i].first] = Dist[i].second;
    }

    return DistMap;
}

unsigned char hFun(int amount, int size, int min) {
    double val;
    double l = amount - min;
    double m = size - min;
    val = l / m;
    val *= 255;
    return (unsigned char)round(val);
}

vector<vector<unsigned char>> createFixed(vector<vector<unsigned char>> Ivals, unordered_map<unsigned char, int> Dist, int min) {
    vector<vector<unsigned char>> FIvals = vector<vector<unsigned char>>(Ivals.size(), vector<unsigned char>(Ivals[0].size(), 0));
    int TotalSize = Ivals.size() * Ivals[0].size();

    for (int i = 0;i < Ivals.size();i++) {
        for (int j = 0;j < Ivals[0].size();j++) {
            FIvals[i][j] = hFun(Dist[Ivals[i][j]], TotalSize, min);
        }
    }

    return FIvals;
}


Mat getHnorm(Photo image) {
    uchar u;
    Mat value = image.get_value();
    Mat FValue = Mat(value.rows, value.cols, CV_8UC1);
    vector<vector<unsigned char>> Ivals = vector<vector<unsigned char>>(value.rows, vector<unsigned char>(value.cols, 0));
    vector<vector<unsigned char>> FIvals;
    vector<pair<unsigned char, int>> List;
    vector<pair<unsigned char, int>> Dist;
    unordered_map<unsigned char, int> DistMap;

    for (int i = 0; i < value.rows;i++) {
        for (int j = 0;j < value.cols;j++) {
            Ivals[i][j] = value.at<uchar>(i, j);
        }
    }
    List = createValueList(Ivals);
    Dist = createDist(List);
    DistMap = createDistMap(Dist);
    FIvals = createFixed(Ivals, DistMap, Dist[0].second);

    for (int i = 0; i < value.rows;i++) {
        for (int j = 0;j < value.cols;j++) {
            FValue.at<uchar>(i, j) = FIvals[i][j];
        }
    }

    return FValue;
}