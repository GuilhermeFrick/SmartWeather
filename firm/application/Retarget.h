/**
 * @file Retarget.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-07-29
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef RETARGET_H_
#define RETARGET_H_

struct __FILE
{
  int dummyVar; //Just for the sake of redefining __FILE, we won't we using it anyways ;)
};

int fputc(int c, FILE * stream);

#endif /* RETARGET_H_*/
