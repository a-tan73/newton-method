/*
 * ニュートン法を計算するプログラム
 * (2次関数の場合)
 */
#define _MAIN

#include <stdio.h>
#include <math.h>
#include "mat.h" // 行列計算のライブラリ

// 定義
#define REPEAT 1000 // 計算回数
#define PI 3.141592 // パイ

// 関数プロトタイプ宣言
void calc_mat(double *);

int main(void)
{
    // 初期値 x0, y0
    double x[2] = {1.0, 1.0};

    // ニュートン法実行
    for (int i = 0; i < REPEAT; i++)
    {
        calc_mat(x);
    }

    // 結果表示
    printf("x = %lf \ny = %lf\n", x[0], x[1]);

    return 0;
}

// ニュートン法で計算する関数
void calc_mat(double *x)
{
    // 式１：f(x,y) = x^2 + y^2 - 1
    // 式２：f(x,y) = 3x^2 * y - y^3

    MATRIX *Bmat, *Fmat;
    double Bdat[] = {
        2 * x[0],                            // 式１をxで偏微分
        2 * x[1],                            // 式１をyで偏微分
        6 * x[0] * x[1],                     // 式２をxで偏微分
        3 * pow(x[0], 2) - 3 * pow(x[1], 2), // 式２をyで偏微分
    };
    double Fdat[] = {
        -1 * (pow(x[0], 2) + pow(x[1], 2) - 1),
        -1 * (3 * pow(x[0], 2) * x[1] - pow(x[1], 3)),
    };

    // 行列の初期化
    Bmat = MatGenInit(2, 2, Bdat); // 2x2の行列
    Fmat = MatGenInit(1, 2, Fdat); // 1x2の行列
    //MatPrint(Bmat, "%4.4lf ");
    //MatPrint(Fmat, "%4.4lf ");

    // ヤコビアン行列が０なら計算終了
    if (Bmat->dat[0] == 0 && Bmat->dat[1] && Bmat->dat[2] && Bmat->dat[3])
        return;

    // 逆行列の計算
    Bmat = MatGenInvGJ(Bmat);

    // ２つの行列を掛け算
    Bmat = MatGenMul(Bmat, Fmat);

    // 計算されたΔxをｘに加える
    x[0] += Bmat->dat[0];
    x[1] += Bmat->dat[1];
    //printf("%f, %f\n", x[0], x[1]);

    // 開放
    MatFreeN(2, Bmat, Fmat);
}
