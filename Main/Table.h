#ifndef TABLE_H
#define TABLE_H

// inline float params[101][3] = {
//     {  6,    26,   0},
//     {  6,    26,  15},
//     {  6,    26,  18},
//     {  6,    26,  21},
//     {  6,    26,  25},
//     {  6,    26,  28},
//     {  6,    26,  31},
//     {  6,    26,  31},
//     {  6,    26,  31},
//     {  6,    26,  31},
//     {  6,    26,  40},
//     {  6,    26,  40},
//     {  6, 16000,  27},
//     {  6, 16000,  28},
//     {  6, 16000,  29},
//     {  6, 16000,  30},
//     {  6, 16000,  31},
//     {  6, 16000,  32},
//     {  6, 16000,  33},
//     {  6, 16000,  34},
//     {  6, 16000,  35},
//     {  6, 16000,  36},
//     {  6, 16000,  37},
//     {  6, 16000,  39},
//     {  6, 16000,  40},
//     {  6, 16000,  41},
//     {  6, 16000,  42},
//     {  6, 16000,  43},
//     {  6, 16000,  44},
//     {  6, 16000,  45},
//     {  6, 16000,  46},
//     {  6, 16000,  47},
//     {  6, 16000,  48},
//     {  6, 16000,  49},
//     {  6, 16000,  50},
//     {  6, 16000,  51},
//     {  6, 16000,  52},
//     {  6, 16000,  53},
//     {  6, 16000,  54},
//     {  6, 16000,  55},
//     {  6, 16000,  56},
//     {  6, 16000,  57},
//     {  6, 16000,  59},
//     {  6, 16000,  60},
//     {  6, 16000,  61},
//     {  6, 16000,  62},
//     {  6, 16000,  63},
//     {  6, 16000,  64},
//     {  6, 16000,  65},
//     {  6, 16000,  66},
//     {  6, 16000,  67},
//     {  6, 16000,  68},
//     {  6, 16000,  69},
//     {  6, 16000,  70},
//     {  6, 16000,  71},
//     {  6, 16000,  72},
//     {  6, 16000,  73},
//     {  6, 16000,  74},
//     {  6, 16000,  75},
//     {  6, 16000,  76},
//     {  6, 16000,  77},
//     {  6, 16000,  78},
//     {  6, 16000,  79},
//     {  6, 16000,  80},
//     {  6, 16000,  81},
//     {  6, 16000,  82},
//     {  6, 16000,  83},
//     {  6, 16000,  84},
//     {  6, 16000,  85},
//     {  6, 16000,  86},
//     {  6, 16000,  87},
//     {  6, 16000,  88},
//     {  6, 16000,  89},
//     {  6, 16000,  90},
//     {  6, 16000,  91},
//     {  6, 16000,  92},
//     {  6, 16000,  93},
//     {  6, 16000,  94},
//     {  6, 16000,  95},
//     {  6, 16000,  96},
//     {  6, 16000,  97},
//     {  6, 16000,  98},
//     {  6, 16000,  99},
//     {  6, 16000, 100},
//     {  6, 16000, 100},
//     {  6, 16000, 100},
//     {  6, 16000, 100},
//     {  6, 16000, 100},
//     {  6, 16000, 100},
//     {  6, 16000, 100},
//     {  6, 16000, 100},
//     {  6, 16000, 100},
//     {  6, 16000, 100},
//     {  6, 16000, 100},
//     {  6, 16000, 100},
//     {  6, 16000, 100},
//     {  6, 16000, 100},
//     {  6, 16000, 100},
//     {  6, 16000, 100},
//     {  6, 16000, 100},
//     {  6, 16000, 100}
// };

inline float params[101][3] = {
    {  6,    10,   0},
    {  6,    10,   5},
    {  6,    10,   5},//2
    {  6,    10,   5},
    {  6,    10,   7},
    {  6,    10,   9},
    {  6,    10,  10},//6
    {  6,    10,  11},
    {  6,    10,  12},
    {  6,    10,  13},
    {  6,    10,  15},//10
    {  6,    10,  17},
    {  6,    10,  19},
    {  6,    10,  20},//13
    {  6,    10,  20},
    {  6,    10,  21},
    {  6,    10,  22},
    {  6,    10,  23},
    {  6,  8000,  23},
    {  6,  8000,  24},
    {  6,  8000,  25},//20
    {  6,  8000,  26},
    {  6,  8000,  27},
    {  6,  8000,  28},
    {  6,  8000,  30},//24
    {  6,  8000,  31},
    {  6,  8000,  32},
    {  6,  8000,  33},
    {  6,  8000,  34},
    {  6,  8000,  35},
    {  6,  8000,  36},
    {  6,  8000,  37},
    {  6,  8000,  38},
    {  6,  8000,  40},//33
    {  6,  8000,  41},
    {  6,  8000,  42},
    {  6,  8000,  43},
    {  6,  8000,  44},
    {  6,  8000,  45},
    {  6,  8000,  46},
    {  6,  8000,  47},
    {  6,  8000,  48},
    {  6,  8000,  50},//42
    {  6,  8000,  50},
    {  6,  8000,  51},
    {  6,  8000,  52},
    {  6,  8000,  53},
    {  6,  8000,  54},
    {  6,  8000,  55},
    {  6,  8000,  56},
    {  6,  8000,  57},
    {  6,  8000,  58},
    {  6,  8000,  60},//52
    {  6,  8000,  61},
    {  6,  8000,  62},
    {  6,  8000,  63},
    {  6,  8000,  64},
    {  6,  8000,  65},
    {  6,  8000,  66},
    {  6,  8000,  67},
    {  6,  8000,  68},
    {  6,  8000,  70},//61
    {  6,  8000,  71},
    {  6,  8000,  72},
    {  6,  8000,  73},
    {  6,  8000,  74},
    {  6,  8000,  75},
    {  6,  8000,  76},
    {  6,  8000,  77},
    {  6,  8000,  78},
    {  6,  8000,  80},//70
    {  6,  8000,  81},
    {  6,  8000,  82},
    {  6,  8000,  83},
    {  6,  8000,  83},
    {  6,  8000,  84},
    {  6,  8000,  85},
    {  6,  8000,  86},
    {  6,  8000,  87},
    {  6,  8000,  88},
    {  6,  8000,  90},//80
    {  6,  8000,  91},
    {  6,  8000,  92},
    {  6,  8000,  93},
    {  6,  8000,  94},
    {  6,  8000,  95},
    {  6,  8000,  96},
    {  6,  8000,  97},
    {  6,  8000,  98},
    {  6,  8000,  99},
    {  6,  8000, 100},
    {  6,  8000, 100},//91
    {  6,  8000, 100},
    {  6,  8000, 100},
    {  6,  8000, 100},
    {  6,  8000, 100},
    {  6,  8000, 100},
    {  6,  8000, 100},
    {  6,  8000, 100},
    {  6,  8000, 100},
    {  6,  8000, 100}
};

//inline float params[101][3] = {
//    {2.5,     7,   0},
//    {2.5,     7,   8},
//    {2.5,     7,  10},
//    {2.5,     7,  11},
//    {2.5,    10,  16},
//    {2.5,    10,  23},
//    {2.5,    15,  29},
//    {2.5,    15,  34},
//    {2.5,    15,  36},
//    {2.5,    15,  40},
//    {2.5,    15,  41},
//    {2.5,    15,  42},
//    {  6, 16000,  27},
//    {  6, 16000,  28},
//    {  6, 16000,  29},
//    {  6, 16000,  30},
//    {  6, 16000,  31},
//    {  6, 16000,  32},
//    {  6, 16000,  33},
//    {  6, 16000,  34},
//    {  6, 16000,  35},
//    {  6, 16000,  36},
//    {  6, 16000,  37},
//    {  6, 16000,  39},
//    {  6, 16000,  40},
//    {  6, 16000,  41},
//    {  6, 16000,  42},
//    {  6, 16000,  43},
//    {  6, 16000,  44},
//    {  6, 16000,  45},
//    {  6, 16000,  46},
//    {  6, 16000,  47},
//    {  6, 16000,  48},
//    {  6, 16000,  49},
//    {  6, 16000,  50},
//    {  6, 16000,  51},
//    {  6, 16000,  52},
//    {  6, 16000,  53},
//    {  6, 16000,  54},
//    {  6, 16000,  55},
//    {  6, 16000,  56},
//    {  6, 16000,  57},
//    {  6, 16000,  59},
//    {  6, 16000,  60},
//    {  6, 16000,  61},
//    {  6, 16000,  62},
//    {  6, 16000,  63},
//    {  6, 16000,  64},
//    {  6, 16000,  65},
//    {  6, 16000,  66},
//    {  6, 16000,  67},
//    {  6, 16000,  68},
//    {  6, 16000,  69},
//    {  6, 16000,  70},
//    {  6, 16000,  71},
//    {  6, 16000,  72},
//    {  6, 16000,  73},
//    {  6, 16000,  74},
//    {  6, 16000,  75},
//    {  6, 16000,  76},
//    {  6, 16000,  77},
//    {  6, 16000,  78},
//    {  6, 16000,  79},
//    {  6, 16000,  80},
//    {  6, 16000,  81},
//    {  6, 16000,  82},
//    {  6, 16000,  83},
//    {  6, 16000,  84},
//    {  6, 16000,  85},
//    {  6, 16000,  86},
//    {  6, 16000,  87},
//    {  6, 16000,  88},
//    {  6, 16000,  89},
//    {  6, 16000,  90},
//    {  6, 16000,  91},
//    {  6, 16000,  92},
//    {  6, 16000,  93},
//    {  6, 16000,  94},
//    {  6, 16000,  95},
//    {  6, 16000,  96},
//    {  6, 16000,  97},
//    {  6, 16000,  98},
//    {  6, 16000,  99},
//    {  6, 16000, 100},
//    {  6, 16000, 100},
//    {  6, 16000, 100},
//    {  6, 16000, 100},
//    {  6, 16000, 100},
//    {  6, 16000, 100},
//    {  6, 16000, 100},
//    {  6, 16000, 100},
//    {  6, 16000, 100},
//    {  6, 16000, 100},
//    {  6, 16000, 100},
//    {  6, 16000, 100},
//    {  6, 16000, 100},
//    {  6, 16000, 100},
//    {  6, 16000, 100},
//    {  6, 16000, 100},
//    {  6, 16000, 100},
//    {  6, 16000, 100}
//};

#endif