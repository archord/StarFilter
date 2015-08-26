
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "StarFile.h"

/**
 * 对一个星表文件中的星（坏像素）进行过滤，在X或Y方向上一个像素范围类，星的数量大于指定数，
 * 则认为这些星是坏像素导致的。过滤这些星，将剩余的输出。
 * @param argc
 * @param argv
 * @return 
 */
int main(int argc, char **argv) {

    if (argc != 5) {
        printf("Usage: StarFilter inputFileName outFileName filterNum errorBox\n");
        return 0;
    }

    char *inputName = argv[1];
    char *outName = argv[2];
    int filterNum = atoi(argv[3]);
    float errorBox = atof(argv[4]); //atof(argv[3])
    float searchBox = errorBox / 2;

    StarFile *sf = new StarFile();
    sf->fileName = inputName;
    sf->readFile();
    sf->parseStar();

    float maxx = 0;
    float maxy = 0;
    float minx = 3072;
    float miny = 3072;
    float binXNum = 0; //星表视场的宽度
    float binYNum = 0; //星表视场的高度

    CMStar *tStar = sf->starList;
    for (int i = 0; i < sf->starNum; i++) {
        if (tStar[i].pixx > maxx) {
            maxx = tStar[i].pixx;
        }
        if (tStar[i].pixx < minx) {
            minx = tStar[i].pixx;
        }
        if (tStar[i].pixy > maxy) {
            maxy = tStar[i].pixy;
        }
        if (tStar[i].pixy < miny) {
            miny = tStar[i].pixy;
        }
    }

    binXNum = ceil((maxx - minx) / searchBox);
    binYNum = ceil((maxy - miny) / searchBox);

    int *binX = (int*) malloc(binXNum * sizeof (int));
    int *binY = (int*) malloc(binYNum * sizeof (int));

    for (int i = 0; i < binXNum; i++) {
        binX[i] = 0;
    }
    for (int i = 0; i < binYNum; i++) {
        binY[i] = 0;
    }

    tStar = sf->starList;
    for (int i = 0; i < sf->starNum; i++) {
        int tx = (tStar[i].pixx - minx) / searchBox;
        int ty = (tStar[i].pixy - miny) / searchBox;
        binX[tx]++;
        binY[ty]++;
    }

    for (int i = 0; i < binXNum; i++) {
        if (binX[i] > filterNum) {
            float filter1 = minx + i*searchBox;
            float filter2 = filter1 + searchBox;
            int k = 0;
            float ttotal = 0;
            for (int j = 0; j < sf->starNum; j++) {
                if (tStar[j].pixx >= filter1 && tStar[j].pixx < filter2) {
                    tStar[j].status = 1;
                    ttotal += tStar[j].pixx;
                    k++;
                }
            }
            float tavg = ttotal / k;
            filter1 = tavg - searchBox;
            filter2 = tavg + searchBox;
            for (int j = 0; j < sf->starNum; j++) {
                if (tStar[j].pixx >= filter1 && tStar[j].pixx < filter2) {
                    tStar[j].status = 1;
                    k++;
                }
            }
            //            printf("%d %d\n", binX[i], k);
        }
    }

    for (int i = 0; i < binYNum; i++) {
        if (binY[i] > filterNum) {
            float filter1 = miny + i*searchBox;
            float filter2 = filter1 + searchBox;
            int k = 0;
            float ttotal = 0;
            for (int j = 0; j < sf->starNum; j++) {
                if (tStar[j].pixy >= filter1 && tStar[j].pixy < filter2) {
                    tStar[j].status = 1;
                    ttotal += tStar[j].pixy;
                    k++;
                }
            }
            float tavg = ttotal / k;
            //            printf("%d %d\n", binY[i], k);
            filter1 = tavg - searchBox;
            filter2 = tavg + searchBox;
            for (int j = 0; j < sf->starNum; j++) {
                if (tStar[j].pixy >= filter1 && tStar[j].pixy < filter2) {
                    tStar[j].status = 1;
                    k++;
                }
            }
            //            printf("%d %d\n", binY[i], k);
        }
    }

    sf->writeStar(outName);
    
    free(binX);
    free(binY);

    return 0;
}
