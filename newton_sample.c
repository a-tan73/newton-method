/*
 * ニュートン法を計算するサンプルプログラム
 * （１次元の場合）
 */

#include <stdio.h>
#include <math.h>

// 定義
#define REPEAT 1000 // 計算回数
#define PI 3.141592 // パイ

int main(void)
{
    // 計算したい関数
    // f(x) = ax^3 + bx^2 + cx + d
    double a = 1.0;
    double b = -2.0;
    double c = 3.0;
    double d = -4.0;

    // 初期値ｘ
    double x = 5.0;

    // ニュートン法実行
    for (int i = 0; i < REPEAT; i++)
    {
        x = x - (a * pow(x, 3) + b * pow(x, 2) + c * x * d) / (3 * a * pow(x, 2) + 2 * b * x + c);
    }

    // 結果表示
    printf("x = %lf\n", x);
    printf("%lf\n", a * pow(x, 3) + b * pow(x, 2) + c * x * d); // ゼロにならなければ発散している

    return 0;
}
