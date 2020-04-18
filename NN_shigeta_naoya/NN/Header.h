#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>



int N = 50000;
double setE = 0.001;
//各素子の値を持つ構造体
struct eldata {

	//教師データ
	double **Tin;
	double **Tout;

	double ***mid;
	double **out;
	
	//未学習データ
	double **lin;
	double **lout;
};

//誤差伝搬時に各素子に入力される値
struct errordata {
	double **in;
	double ***mid;
	double **out;
};

//各素子間の重み
struct wdata {
	double **w1;
	double ***w2;
	double **w3;
};
eldata el;
errordata error;
wdata w;


//順方向
//引数は入力の数in、中間層の層数と素子数,出力数,データ数
void colel(int in, int layer, int element, int out, int data, struct eldata *el, struct errordata *error, struct wdata *w);

//シグモイド関数
double sigmoid(double s);

//誤差を求める
double colerror(int out,struct eldata *el, int data, struct errordata *error);

//誤差の伝搬と重みの更新
void spread(int in, int layer, int element, int out, int data, struct eldata *el, struct errordata *error, struct wdata *w,int learn);

void lcolel(int in, int layer, int element, int out, int ldata, struct eldata *el, struct wdata *w);