/** 
 * Course: 2020FA VE482
 * Author: Zhimin Sun
 */
#include "sign.h"

const static char sign_code[] = {
    /* ' = */20,
    /* " = */21,
    /*   = */22,
    /* | = */23,
    /* < = */24,
    /* > = */25,
    /*\n = */26,
};

void
sign_encode(char* signptr)
{
    if (*signptr == '\'') {
        *signptr = sign_code[0];
    } else if (*signptr == '\"') {
        *signptr = sign_code[1];
    } else if (*signptr == ' ') {
        *signptr = sign_code[2];
    } else if (*signptr == '|') {
        *signptr = sign_code[3];
    } else if (*signptr == '<') {
        *signptr = sign_code[4];
    } else if (*signptr == '>') {
        *signptr = sign_code[5];
    } else if (*signptr == '\n') {
        *signptr = sign_code[6];
    }
}

void
sign_decode(char* signptr)
{
    if (*signptr == sign_code[0]) {
        *signptr = '\'';
    } else if (*signptr == sign_code[1]) {
        *signptr = '\"';
    } else if (*signptr == sign_code[2]) {
        *signptr = ' ';
    } else if (*signptr == sign_code[3]) {
        *signptr = '|';
    } else if (*signptr == sign_code[4]) {
        *signptr = '<';
    } else if (*signptr == sign_code[5]) {
        *signptr = '>';
    } else if (*signptr == sign_code[6]) {
        *signptr = '\n';
    }
}
