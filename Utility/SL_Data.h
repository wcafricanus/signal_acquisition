/*
 * SL_Data.h
 *
 *  Created on: 2018/1/3
 *      Author: AndyLai
 */
#include <time.h>

#ifndef SL_DATA_H_
#define SL_DATA_H_

void SaveRawdata(double * I ,double * Q, time_t * T, int * M, int size ,const char* DirName ,int Is_title);

#endif /* SL_DATA_H_ */
