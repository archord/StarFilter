/* 
 * File:   StarFile.cpp
 * Author: xy
 * 
 * Created on December 4, 2014, 9:15 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "StarFile.h"

StarFile::StarFile() {
    this->fileName = NULL;
    this->starList = NULL;
    this->starNum = 0;
}

StarFile::StarFile(char* fileName) {
    this->fileName = fileName;
    this->starList = NULL;
    this->starNum = 0;
}

StarFile::~StarFile() {

    if (NULL != content) {
        free(content);
    }

    if (NULL != starList) {

        for (int j = 0; j < starNum; j++) {
            free(starList[j].line);
        }
        free(starList);
    }

}

void StarFile::readFile() {
    readFile(fileName);
}

int StarFile::readFile(char *fileName) {

    FILE * fp;
    size_t result;

    fp = fopen(fileName, "rb");
    if (fp == NULL) {
        fputs("File error", stderr);
        exit(1);
    }

    fseek(fp, 0, SEEK_END);
    fileSize = ftell(fp);
    rewind(fp);
    content = (char*) malloc(sizeof (char)*fileSize);
    if (content == NULL) {
        fputs("Memory error", stderr);
        exit(2);
    }

    result = fread(content, 1, fileSize, fp);
    //    printf("%s", content);
    if (result != fileSize) {
        fputs("Reading error", stderr);
        exit(3);
    }
    fclose(fp);

    return fileSize;
}

int StarFile::countLine() {

    fileLine = 0;
    for (int j = 0; j < fileSize; j++) {
        if (content[j] == '\n') {//windows是\r\n，Unix是\n，Mac是\r，这里不考虑mac
            fileLine++;
        }
    }
    return fileLine;
}

void StarFile::parseStar() {

    countLine();

    float x, y;
    starNum = 0;
    starList = (CMStar *) malloc(fileLine * sizeof (CMStar));

    char line[MaxStringLength];
    for (int j = 0; j < fileSize; j++) {
        int lineEnd = j;
        while (content[lineEnd++] != '\n');

        memcpy(line, content + j, lineEnd - j);
        line[lineEnd - j] = '\0';
        j = lineEnd - 1; //因为for循环中的j++

        if (2 == sscanf(line, "%f%f", &x, &y)) {
            starList[starNum].id = starNum;
            starList[starNum].pixx = x;
            starList[starNum].pixy = y;
            starList[starNum].status = 0;
            starList[starNum].next = NULL;
            starList[starNum].line = (char*) malloc(strlen(line) + 1);
            strcpy(starList[starNum].line, line);
            starNum++;
        } else {
            printf("line illegal: %s\n", line);
        }
    }

#ifdef PRINT_CM_DETAIL
    printf("%d %s lines, read %d stars\n", fileLine, fileName, starNum);
#endif
}

void StarFile::writeStar(char * outFile) {

    FILE * fp = fopen(outFile, "w");
    if (fp == NULL) {
        fputs("File open error", stderr);
    }
    int tnum = 0;
    for (int i = 0; i < starNum; i++) {
        if (starList[i].status == 0) {
            fprintf(fp, "%s", starList[i].line);
            tnum++;
        }
    }
    printf("total %d stars, filter %d stars, left %d stars\n", starNum, starNum - tnum, tnum);
    fclose(fp);
}