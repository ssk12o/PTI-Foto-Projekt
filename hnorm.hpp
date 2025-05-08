#pragma once
#include "photo.hpp"
#include <vector>
#include <unordered_map>

vector<pair<unsigned char, int>> createValueList(vector<vector<unsigned char>> Ivals);

vector<pair<unsigned char, int>> createDist(vector<pair<unsigned char, int>> List);

unordered_map<unsigned char, int> createDistMap(vector<pair<unsigned char, int>> Dist);

unsigned char hFun(int amount, int size, int min);

vector<vector<unsigned char>> createFixed(vector<vector<unsigned char>> Ivals, unordered_map<unsigned char, int> Dist, int min);

Mat getHnorm(Photo image);



