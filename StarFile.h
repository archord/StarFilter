/* 
 * File:   StarFile.h
 * Author: xy
 *
 * Created on December 4, 2014, 9:15 AM
 */

#ifndef STARFILE_H
#define	STARFILE_H

#include <string.h>
#include "cmhead.h"

class StarFile {
public:
  char * fileName;
  CMStar *starList;
  long starNum;
  long fileLine;
  long fileSize;
  char *content;

public:
  StarFile();
  StarFile(char * fileName);
  virtual ~StarFile();

  void readFile();
  int readFile(char *fileName);
  int countLine();
  void parseStar();
  void writeStar(char * outFile);

private:
};

#endif	/* STARFILE_H */

