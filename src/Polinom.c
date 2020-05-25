#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>   
#include <sys/resource.h> 
#include "Polinom.h"
#include "boolean.h"

void MakeEmpty (Polinom *P)
{
    for (int i=IdxMin; i<=IdxMax; i++) {
        (*P).Koef[i] = Undef;
    }
}

void InputKeyboardPolinom (Polinom *P)
{
    /* KAMUS */
    int n;
    // input n
    printf("Input n: "); scanf("%d",&n);
    MakeEmpty(P);
    (*P).Neff = n;
    for (int i=IdxMin; i<=(*P).Neff; i++){
        printf("Input koefisien ke-%d: ",i);
        scanf("%d", &(*P).Koef[i]);
    }
}

void InputRandomKoefPolinom (Polinom *P)
{
    /* KAMUS */
    int n;
    srand(time(NULL));
    // input n
    printf("Input n: "); scanf("%d",&n);
    MakeEmpty(P);
    (*P).Neff = n;
    for (int i=IdxMin; i<=(*P).Neff; i++){
        (*P).Koef[i] = rand() % 100;;
    }
}

void TulisPolinom (Polinom P)
{
    for (int i=IdxMin; i<=P.Neff; i++) {
        if (P.Koef[i] == 0) {

        } else {
            if (i==P.Neff) {
                printf("%dX^%d\n",P.Koef[i],i);
            } else {
                if (i==0) {
                    printf("%d + ", P.Koef[i]);
                } else {
                    printf("%dX^%d + ", P.Koef[i], i);
                }
            }
        }
    }
}

void PerkalianPolinomBF (Polinom P1, Polinom P2, Polinom *POut, int *tambah, int *kali)
{
    int idx, koef, countTambah=0, countKali=0;
    MakeEmpty(POut);
    (*POut).Neff = P1.Neff+P2.Neff;
    for (int i=IdxMin; i<=P1.Neff; i++) {
        for (int j=IdxMin; j<=P2.Neff; j++) {
            koef = P1.Koef[i]*P2.Koef[j]; countKali++;
            idx = i+j;
            (*POut).Koef[idx] += koef; countTambah++;
        }
    }
    *tambah = countTambah; *kali = countKali;
}

Polinom PerkalianPolinomDC (Polinom P1, Polinom P2, int numOfKoef, int *tambah, int *kali) {
    int countKali=0, countTambah=0;
    Polinom res;
    MakeEmpty(&res);
    res.Neff = 2*numOfKoef-1;
    if (numOfKoef == 1) {
        res.Koef[0] = P1.Koef[0] * P2.Koef[0];
        return res;
    } 
    // divide
    int halfSize = numOfKoef/2;
    // container
    Polinom P1Low, P1High, P2Low, P2High, P1LowHigh, P2LowHigh;
    MakeEmpty(&P1Low);MakeEmpty(&P1High);MakeEmpty(&P2Low);MakeEmpty(&P2High);
    MakeEmpty(&P1LowHigh); MakeEmpty(&P2LowHigh);
    P1Low.Neff = halfSize-numOfKoef%2; P1High.Neff = halfSize;
    P2Low.Neff = halfSize-numOfKoef%2; P2High.Neff = halfSize;
    P1LowHigh.Neff = halfSize; P2LowHigh.Neff = halfSize;
    // isi
    for (int i=0; i<halfSize; i++) {
        P1Low.Koef[i] = P1.Koef[i];
        P1High.Koef[i] = P1.Koef[i+halfSize];
        P1LowHigh.Koef[i] = P1Low.Koef[i] + P1High.Koef[i];
        P2Low.Koef[i] = P2.Koef[i];
        P2High.Koef[i] = P2.Koef[i+halfSize];
        P2LowHigh.Koef[i] = P2Low.Koef[i] + P2High.Koef[i];
    }
    countTambah = 2; *tambah+= countTambah;
    countKali = 3; *kali+=countKali;
    // divide
    Polinom kiri,kanan,tengah;
    MakeEmpty(&kiri); MakeEmpty(&kanan); MakeEmpty(&tengah);
    kiri = PerkalianPolinomDC(P1Low,P2Low,halfSize,tambah,kali);
    kanan = PerkalianPolinomDC(P1High,P2High,halfSize,tambah,kali);
    tengah = PerkalianPolinomDC(P1LowHigh,P2LowHigh,halfSize,tambah,kali);
    // masuk
    for (int i=0; i<numOfKoef-1; i++) {
        res.Koef[i] += kiri.Koef[i];
        res.Koef[i+halfSize] += tengah.Koef[i] - kiri.Koef[i] - kanan.Koef[i];
        res.Koef[i+2*halfSize] += kanan.Koef[i];
    }
    return res;
}

int main() {
    int countTambah, countKali, tmbh, kl;
    struct timeval start, end, s, e;
    srand(time(NULL));
    Polinom P1,P2,POut,POutt;
    InputRandomKoefPolinom(&P1);
    TulisPolinom(P1);
    InputRandomKoefPolinom(&P2);
    TulisPolinom(P2);
    gettimeofday(&start,NULL);
    PerkalianPolinomBF(P1,P2,&POut,&countTambah, &countKali);
    gettimeofday(&end, NULL);
    long seconds = (end.tv_sec-start.tv_sec);
    long micros = ((seconds*1000000) + end.tv_usec) - (start.tv_usec);
    printf("\nTime execution fungsi PerkalianPolinomBF(P1,P2,*POut): %ld micros\n", micros);
    printf("Operasi perkalian dan penjumlah dengan Brute Force programg ini %d dan %d\n", countTambah, countKali);
    printf("Hasil perkalian dengan brute force\n");
    TulisPolinom(POut);
    MakeEmpty(&POutt);
    gettimeofday(&s,NULL);
    POutt = PerkalianPolinomDC(P1,P2,P1.Neff+1,&tmbh,&kl);
    gettimeofday(&e, NULL);
    long sec = (e.tv_sec-s.tv_sec);
    long mic = ((sec*1000000) + e.tv_usec) - (s.tv_usec);
    printf("\nTime execution fungsi PerkalianPolinomDC(P1,P2,numOfKoef): %ld micros\n", mic);
    printf("Operasi perkalian dan penjumlah dengan Divide & Conquer program ini %d dan %d\n", tmbh,kl);
    printf("Hasil perkalian dengan divide & conquer\n");
    TulisPolinom(POut);
}
