#ifndef POLINOM_H
#define POLINOM_H
#include "boolean.h"

#define IdxMax 1000
#define IdxMin 0
#define Undef 0

typedef int ElType;
typedef struct {
    ElType Koef[IdxMax];
    int Neff;
} Polinom;

/* KONSTRUKTOR */
void MakeEmpty (Polinom *P);
void InputKeyboardPolinom (Polinom *P);
void InputRandomKoefPolinom (Polinom *P);

/* FUNGSI LAIN */
void TulisPolinom (Polinom P);
void PerkalianPolinomBF (Polinom P1, Polinom P2, Polinom *POut, int *tambah, int *kali);
Polinom PerkalianPolinomDC (Polinom P1, Polinom P2, int numOfKoef, int *tambah, int *kali);

#endif