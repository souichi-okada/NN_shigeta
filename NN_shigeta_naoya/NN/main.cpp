#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include"Header.h"
#pragma warning(disable: 4996)

//NN(層数、素子数を指定)
//層数2、一層の素子数4
//入力数3



int main(void) {

	//入力次元数，出力次元数，データ数，未学習データ数(テストデータ)
	int in = 3, out = 1, data = 6,ldata = 2;
	//層数、素子数
	int layer = 2, element = 4;		

	int i, j, k, count = 0;		//作業用

	double E;							//二乗誤差

	int learn = 1;						//学習方法(0・・・逐次,1・・・一括)

	printf("NNの層数を指定・・・");
	scanf_s("%d", &layer);
	printf("NNの1層当たりの素子数を指定・・・");
	scanf_s("%d",& element);
	do {
		printf("学習方法の指定(0・・・逐次,　　1・・・一括)");
		scanf_s("%d", &learn);
	} while (learn != 0 && learn != 1) ;

	FILE *fp;

	printf("NN\n");
	//***********各値のための領域確保と初期化***************
	eldata el;
	//教師データ
	el.Tin = (double **)malloc(sizeof(double*)*data);
	el.Tin[0] = (double*)malloc(sizeof(double)*data*in);				//2次元
	for (i = 1; i < data; i++) {
		el.Tin[i] = el.Tin[0] + i*in;
	}
	//教師出力データ
	el.Tout = (double**)malloc(sizeof(double)*data);
	el.Tout[0] = (double*)malloc(sizeof(double) * data * out);				//2次元
	for (i = 1; i < data; i++) {
		el.Tout[i] = el.Tout[0] + i * out;
	}

	if ((fp = fopen("Traning_Data.txt", "r")) == NULL) {
		printf("file open error!!\n");
		exit(EXIT_FAILURE);	/* (3)エラーの場合は通常、異常終了する */
	}
	
	printf("教師データ読み込み\n");
	i = 0;
	/*while ((fscanf_s(fp, "%lf %lf %lf %lf", &el.Tin[i][0], &el.Tin[i][1], &el.Tin[i][2], &el.Tout[i])) != EOF) {
		printf("%f %f %f %f\n", el.Tin[i][0], el.Tin[i][1], el.Tin[i][2], el.Tout[i]);
		i++;
	}*/
	for (i = 0; i < data; i++) {
		for (j = 0; j < in; j++){
			//教師入力データ読み込み
			if (fscanf_s(fp, "%lf", &el.Tin[i][j]) == EOF) {
				printf("EOF!\n");
				fclose(fp);
				break;
			}
		}
		for (k = 0; k < out; k++){
			//教師出力データ読み込み
			if (fscanf_s(fp, "%lf", &el.Tout[i][k]) == EOF) {
				printf("EOF!\n");
				fclose(fp);
				break;
			}
		}

	}

	//}
	fclose(fp);
	
	/*
	int count = 0;
	for (i = 0; i < data; i++) {
		for (j = 0; j < in; j++) {
			el.Tin[i][j] = count;
			printf("%f %f %f %f\n", el.Tin[i][0], el.Tin[i][1], el.Tin[i][2], el.Tout[i]);
			count++;
		}
	}
*/
	//出力層二次元に対応
	el.out = (double**)malloc(sizeof(double) * data);
	el.out[0] = (double*)malloc(sizeof(double) * data * out);				//2次元
	for (i = 1; i < data; i++) {
		el.out[i] = el.out[0] + i * out;
	}
	
	//中間層
	el.mid = (double ***)malloc(sizeof(double**)*data);
	el.mid[0] = (double **)malloc(sizeof(double*)*data*layer);
	el.mid[0][0] = (double *)malloc(sizeof(double)*data*layer*element);	//3次元
	for (i = 0; i < data; i++) {
		el.mid[i] = el.mid[0] + i*layer;
		for (j = 0; j < layer; j++) {
			el.mid[i][j] = el.mid[0][0] + i*layer*element+j*element;
		}
	}
	for (i = 0; i < data; i++) {
		for (j = 0; j < layer; j++) {
			for (k = 0; k < element; k++) {
				el.mid[i][j][k] = 0;
				//printf("%f", el.mid[i][j][k]);
			}
		}
	}
	//printf("NN\n");

	

	errordata error;
	error.in = (double **)malloc(sizeof(double*)*data);
	error.in[0] = (double *)malloc(sizeof(double)*data*in);				//2次元
	for (i = 1; i < data; i++) {
		error.in[i] = error.in[0] + i*in;
	}
	for (i = 0; i < data; i++) {
		for (j = 0; j < in; j++) {
			error.in[i][j] = 0;
			//printf("error.in %f", error.in[i][j]);
		}
	}

	error.mid = (double ***)malloc(sizeof(double**)*data);
	error.mid[0] = (double **)malloc(sizeof(double*)*data*layer);
	error.mid[0][0] = (double *)malloc(sizeof(double)*data*layer*(element+1));	//3次元
	for (i = 0; i < data; i++) {
		error.mid[i] = error.mid[0] + i*layer;
		for (j = 0; j < layer; j++) {
			error.mid[i][j] = error.mid[0][0] + i*layer*element + j*(element+1);
		}
	}
	for (i = 0; i < data; i++) {
		for (j = 0; j < layer; j++) {
			for (k = 0; k < element+1; k++) {
				error.mid[i][j][k] = 0;
				//printf("errormid %f  ", error.mid[i][j][k]);
			}
			//printf("\n");
		}
		//printf("\n");
	}
	//printf("\n");

	//出力層の次元数に対応
	error.out = (double**)malloc(sizeof(double) * data);
	error.out[0] = (double*)malloc(sizeof(double) * data * out);				//2次元
	for (i = 1; i < data; i++) {
		error.out[i] = error.out[0] + i * out;
	}
	

	//重みの領域確保と初期化
	wdata w;
	w.w1 = (double **)malloc(sizeof(double*)*in+1);
	w.w1[0] = (double *)malloc(sizeof(double)*(in+1)*element);						//2次元
	for (i = 1; i < (in + 1); i++) {
		w.w1[i] = w.w1[0] + i*element;
	}

	w.w2 = (double ***)malloc(sizeof(double**)*(layer-1));
	w.w2[0] = (double **)malloc(sizeof(double*)*(layer-1)*(element+1));
	w.w2[0][0] = (double *)malloc(sizeof(double)*(layer-1)*(element+1)*element);	//3次元
	for (i = 0; i < (layer-1); i++) {
		w.w2[i] = w.w2[0] + i*(element+1);
		for (j = 0; j < (element+1); j++) {
			w.w2[i][j] = w.w2[0][0] + i*(element+1)*element + j*element;
		}
	}

	//出力層の次元数の変化に対応
	w.w3 = (double**)malloc(sizeof(double*) * (element+1));
	w.w3[0] = (double*)malloc(sizeof(double) * (element + 1) * out);						//2次元
	for (i = 1; i < (element + 1); i++) {
		w.w3[i] = w.w3[0] + i * out;
	}

	//乱数の初期化
	srand((unsigned int)time(NULL));

	printf("重みの初期値\n");
	printf("w1\n");
	for (i = 0; i < in + 1; i++) {
		for (j = 0; j < element; j++) {
			w.w1[i][j] = rand() / double(RAND_MAX) * 2 - 1;
			//w.w1[i][j] = 0.5;
			printf("%f ", w.w1[i][j]);
		}
		printf("\n");
	}
	//1から2
	printf("w2\n");
	for (i = 0; i < layer - 1; i++) {
		for (j = 0; j < element + 1; j++) {
			for (k = 0; k < element; k++) {
				w.w2[i][j][k] = rand() / double(RAND_MAX) * 2 - 1;
				//w.w2[i][j][k] = 0.5;
				printf("%f ", w.w2[i][j][k]);
			}
			printf("\n");
		}
	}
	//2から出力
	//出力の数の変化に対応
	printf("w3\n");
	for (i = 0; i < element + 1; i++) {
		for (j = 0; j < out; j++) {
			w.w3[i][j] = rand() / double(RAND_MAX) * 2 - 1;
			//w.w3[i] = 0.5;
			printf("%f ", w.w3[i][j]);
		}
		printf("\n");
	}
	printf("\n\n");


	if (learn == 0) {
		printf("逐次学習\n");
	}
	else {
		printf("一括学習\n");
	}


	//********************使うデータの設定が完了*****************************************

	//*******************************一括学習********************************************
	//以下をループし、順方向、逆方向をくりかえして学習
	do {

		//printf("count = %d\n", count);
		printf("count = %d", count);

		//データ数分順方向に伝搬させる
		//printf("colel\n");
		colel(in, layer, element, out, data,&el,&error,&w);

		/*
		for (i = 0; i < data; i++) {
			for (j = 0; j < in; j++) {
				printf("%f  ", el.Tin[i][j]);
			}
			printf("\n");
		}
		printf("\n");
		

		for (i = 0; i < data; i++) {
			for (j = 0; j < layer; j++) {
				for (k = 0; k < element; k++) {
					printf("%f  ", el.mid[i][j][k]);
				}
				printf("\n");
			}
			printf("\n");
		}
		printf("\n");
		*/

		/*for (i = 0; i < data; i++) {
			printf("%f  ", el.out[i]);
		}
		printf("\n");*/
		

		/*
		for (i = 0; i < data; i++) {
			for (j = 0; j < layer; j++) {
				for (k = 0; k < element; k++) {
					printf("%f ", el.mid[i][j][k]);
				}
				printf("\n");
			}
			printf("\n\n");
		}
		*/

		//誤差の計算二乗誤差errorと普通の誤差error.out
		E = colerror(out,&el,data,&error);

		//逆方向
		//printf("spread\n");
		spread(in, layer, element, out, data, &el, &error, &w,learn);
		//printf("end spread\n");
		/*
		//printf("重みの初期値\n");
		printf("w1\n");
		for (i = 0; i < in + 1; i++) {
			for (j = 0; j < 4; j++) {
				//w.w1[i][j] = rand() / double(RAND_MAX) * 2 - 1;
				//w.w1[i][j] = 0.5;
				printf("%f ", w.w1[i][j]);
			}
			printf("\n");
		}
		//1から2
		printf("w2\n");
		for (i = 0; i < layer - 1; i++) {
			for (j = 0; j < element + 1; j++) {
				for (k = 0; k < element; k++) {
					//w.w2[i][j][k] = rand() / double(RAND_MAX) * 2 - 1;
					//w.w2[i][j][k] = 0.5;
					printf("%f ", w.w2[i][j][k]);
				}
				printf("\n");
			}
		}
		//2から出力
		printf("w3\n");
		for (i = 0; i < element + 1; i++) {
			//w.w3[i] = rand() / double(RAND_MAX) * 2 - 1;
			//w.w3[i] = 0.5;
			printf("%f ", w.w3[i]);
			printf("\n");
		}
		printf("\n\n");
		*/


		count++;
	} while (count < N && E > setE);
	//********************************************************************************************************************

	printf("学習の終了\n");
	printf("count = %d  error = %f\n", count, E);
	if (count == 30000) {
		printf("count = %d\n",count);
	}
	else if (E <= setE) {
		printf("学習を終えました\n");
		for (i = 0; i < data; i++) {
			for (j = 0; j < out; j++) {
				printf("Tout = %f\n", el.Tout[i][j]);
				printf("lout = %f\n", el.out[i][j]);
			}
		}
		printf("error = %f\n",E);
	}
	

	//領域の解放
	
	free(error.out);
	free(error.mid[0][0]);
	free(error.mid[0]);
	free(error.mid);
	free(error.in[0]);
	free(error.in);
	//printf("cerror = %f\n", E);
	
	
	
	free(el.Tout[0]);
	free(el.Tout);
	free(el.Tin[0]);
	free(el.Tin);
	//printf("derror = %f\n", E);
	

	//エンター待ち
	do {
		//printf("エンターキーを押して下さい。");
	} while (getchar() != '\n');


	//未学習データを取り込む
	//*********************************************************************************
	el.lin = (double **)malloc(sizeof(double*)*ldata);
	el.lin[0] = (double*)malloc(sizeof(double)*data*in);				//2次元
	for (i = 1; i < ldata; i++) {
		el.lin[i] = el.lin[0] + i*in;
	}
	el.lout = (double**)malloc(sizeof(double)*ldata);
	el.lout[0] = (double*)malloc(sizeof(double) * data * out);				//2次元
	for (i = 1; i < ldata; i++) {
		el.lout[i] = el.lout[0] + i * out;
	}

	if ((fp = fopen("Traning_Data.txt", "r")) == NULL) {
		printf("file open error!!\n");
		exit(EXIT_FAILURE);	/* (3)エラーの場合は通常、異常終了する */
	}

	printf("未学習データ\n");

	if ((fp = fopen("Test_Data.txt", "r")) == NULL) {
		printf("file open error!!\n");
		exit(EXIT_FAILURE);	/* (3)エラーの場合は通常、異常終了する */
	}
	i = 0;
	while ((fscanf_s(fp, "%lf %lf %lf", &el.lin[i][0], &el.lin[i][1], &el.lin[i][2])) != EOF) {
		printf("%f %f %f\n", el.lin[i][0], el.lin[i][1], el.lin[i][2]);
		i++;
	}
	fclose(fp);


	printf("未学習データに対する出力\n");
	lcolel(in, layer, element, out, ldata, &el, &w);

	//printf("aerror = %f\n", E);
	free(w.w3);
	//printf("aerror = %f\n", E);
	free(&w.w2[0][0][0]);
	free(w.w2[0]);
	free(w.w2);
	//printf("aerror = %f\n", E);
	//free(w.w1[0]);
	//free(w.w1);
	//printf("berror = %f\n", E);

	free(el.mid[0][0]);
	free(el.mid[0]);
	free(el.mid);
	free(el.out);

	//エンター待ち
	do {
		printf("終了します。エンターキーを押して下さい。");
	} while (getchar() != '\n');

	return 0;

}

//eldata el;
//errordata error;
//wdata w;
//順方向
//引数は入力の数inと値、中間層の層数と素子数と値を保持するためのもの,出力,データ数
void colel(int in, int layer, int element, int out,int data,struct eldata *el,struct errordata *error,struct wdata *w) {
	int i, j, k,l;
	for (i = 0; i < data; i++) {
		//********************入力から中間******************************
		//printf("el %f\n", el->mid[0][0][0]);
		//中間層の素子数分
		//printf("入力から中間\n");
		for (j = 0; j < element; j++) {
			el->mid[i][0][j] = 0;
			//入力数+1(バイアス分)
			for (k = 0; k < in + 1; k++) {
				if (k == in) {
					el->mid[i][0][j] += w->w1[k][j];
				}
				else {
					//printf("elmid %f\n", el->mid[i][0][j]);
					el->mid[i][0][j] += el->Tin[i][k] * w->w1[k][j];
				}
			}
			el->mid[i][0][j] = sigmoid(el->mid[i][0][j]);
		}
		//******************中間層から中間層****************************
		//printf("中間層から中間\n");
		for (j = 0; j < layer - 1; j++) {
			//素子数分
			for (k = 0; k < element; k++) {
				el->mid[i][j + 1][k] = 0;
				//素子数+1(バイアス分
				for (l = 0; l < element + 1; l++) {
					if (l == element) {
						el->mid[i][j + 1][k] += w->w2[j][l][k];
					}
					else {
						el->mid[i][j + 1][k] += el->mid[i][j][l] * w->w2[j][l][k];
					}
				}
				el->mid[i][j+1][k] = sigmoid(el->mid[i][j+1][k]);
			}
			
		}
		//**********************中間層から出力******************************
		for (j = 0; j < out; j++) {
			//素子数+1(バイアス分)
			el->out[i][j] = 0;
			for (k = 0; k < element + 1; k++) {
				if (k == element) {
					//printf("%f  %f  %f\n", el->out[i], el->mid[i][layer - 1][j], w->w3[j]);
					el->out[i][j] += w->w3[k][j];
				}
				else {
					//printf("%f  %f  %f\n", el->out[i], el->mid[i][layer - 1][j], w->w3[j]);
					el->out[i][j] += el->mid[i][layer - 1][k] * w->w3[k][j];
				}
			}
			//printf("el = %f\n", el->out[i]);
			el->out[i][j] = sigmoid(el->out[i][j]);
			//printf("el = %f\n\n", el->out[i]);
		}
	}
}

double sigmoid(double s) {
	return 1 / (1 + exp(-s));
}

double colerror(int out,struct eldata *el,int data,struct errordata *error) {
	double E = 0;
	int i,j;
	for (i = 0; i < data; i++) {
		for (j = 0; j < out; j++) {
			error->out[i][j] = -(el->Tout[i][j] - el->out[i][j]);
			E += (error->out[i][j]) * (error->out[i][j]) / out;
			//printf("error  %f\n", error->out[i]);
			//printf("%f \n",el->out[i]);
		}
	}
	printf("E %f\n", E);

	return E;
}

void spread(int in, int layer, int element, int out, int data, struct eldata *el, struct errordata *error, struct wdata *w,int learn) {
	int i, j, k, l;
	double total = 0;

	double ep = 0.5;		//学習率


	if (learn == 1) {
		//*****************一括学習**********************************
		//*****************出力層から中間層**************************
		//printf("出力層から中間\n");
		//重みをかけて左の層へ誤差を伝搬
		for (i = 0; i < data; i++) {
			for (j = 0; j < element + 1; j++) {
				for (k = 0; k < out; k++) {
					error->mid[i][layer - 1][j] += (1 - el->out[i][k]) * el->out[i][k] * error->out[i][k] * w->w3[j][k];
					//printf("%f\n", error->mid[i][layer - 1][j]);
				}
			}
			//printf("\n");
		}
		//重み更新のためのtotal(by*誤差の入力*(1-y)*yの和)
		for (j = 0; j < element + 1; j++) {
			if (j == element) {
				//バイアスの更新
				total = 0;
				for (i = 0; i < data; i++) {
					for (k = 0; k < out; k++) {
						total += error->out[i][k] * (1 - el->out[i][k]) * el->out[i][k];
					}
				}
				//ここで更新
				//printf("total = %f\n", total);
				//printf("w3 = %f  ", w->w3[j]);
				for (k = 0; k < out; k++) {
					w->w3[j][k] = w->w3[j][k] - ep / data * total;
					//printf("w3 = %f\n ", w->w3[j]);
				}
			}
			else {
				//バイアス以外の更新
				total = 0;
				for (i = 0; i < data; i++) {
					for (k = 0; k < out; k++) {
						total += el->mid[i][layer - 1][j] * error->out[i][k]* (1 - el->out[i][k]) * el->out[i][k];
					}
				}
				//ここで更新
				//printf("total = %f\n", total);
				//printf("w3 = %f  ", w->w3[j]);
				for (k = 0; k < out; k++) {
					w->w3[j][k] = w->w3[j][k] - ep / data * total;
					//printf("w3 = %f\n", w->w3[j]);
				}
			}
		}
		//*********************中間層から中間層**************************
		//printf("中間層から中間\n");
		//層数-1
		for (l = layer - 1; l > 0; l--) {
			//printf("%d\n", l);
			//重みをかけて左の層へ誤差を伝搬
			for (i = 0; i < data; i++) {
				//printf("%d\n", i);
				for (j = 0; j < element + 1; j++) {
					//printf("%d\n", j);
					error->mid[i][l - 1][j] = 0;
					for (k = 0; k < element; k++) {
						//printf("%d\n", k);
						error->mid[i][l - 1][j] += (1 - el->mid[i][l][k])*el->mid[i][l][k] * error->mid[i][l][k] * w->w2[l - 1][j][k];
						//	printf("aaa\n");
					}
					//printf("%f\n", error->mid[i][l-1][j]);
				}
				//printf("\n");
			}
			//printf("重みを更新\n");
			//重み更新のためのtotal(by*誤差の入力*(1-y)*yの和)
			for (j = 0; j < element + 1; j++) {
				if (j == element) {
					//バイアスの更新
					total = 0;
					for (k = 0; k < element; k++) {
						for (i = 0; i < data; i++) {
							total += error->mid[i][l][k] * (1 - el->mid[i][l][k])*el->mid[i][l][k];
						}
						//ここで更新
						//printf("total = %f\n", total);
						//printf("w2 = %f  ", w->w2[l - 1][j][k]);
						w->w2[l - 1][j][k] = w->w2[l - 1][j][k] - ep / data*total;
						//printf("w2 = %f\n", w->w2[l - 1][j][k]);
					}

				}
				else {
					//バイアス以外の更新
					total = 0;
					for (k = 0; k < element; k++) {
						for (i = 0; i < data; i++) {
							total += el->mid[i][l - 1][j] * error->mid[i][l][k] * (1 - el->mid[i][l][k])*el->mid[i][l][k];
						}
						//ここで更新
						//printf("total = %f\n", total);
						//printf("w2 = %f  ", w->w2[l - 1][j][k]);
						w->w2[l - 1][j][k] = w->w2[l - 1][j][k] - ep / data*total;
						//printf("w2 = %f\n", w->w2[l - 1][j][k]);
					}

				}
			}

		}

		//**********************中間層から入力***************************
		//printf("中間層から入力\n");
		//重み更新のためのtotal(by*誤差の入力*(1-y)*yの和)
		for (j = 0; j < in + 1; j++) {
			if (j == in) {
				//バイアスの更新
				total = 0;
				for (k = 0; k < element; k++) {
					for (i = 0; i < data; i++) {
						total += error->mid[i][0][k] * (1 - el->mid[i][0][k])*el->mid[i][0][k];
					}
					//ここで更新
					//printf("total = %f\n", total);
					//printf("w = %f  ", w->w1[j][k]);
					w->w1[j][k] = w->w1[j][k] - ep / data*total;
					//printf("w = %f\n", w->w1[j][k]);
				}

			}
			else {
				//バイアス以外の更新
				total = 0;
				for (k = 0; k < element; k++) {
					for (i = 0; i < data; i++) {
						total += el->Tin[i][j] * error->mid[i][0][k] * (1 - el->mid[i][0][k])*el->mid[i][0][k];
					}
					//ここで更新
					//printf("total = %f\n", total);
					//printf("w = %f  ", w->w1[j][k]);
					w->w1[j][k] = w->w1[j][k] - ep / data*total;
					//printf("w = %f\n", w->w1[j][k]);
				}

			}
		}
	}
	else {
		//*****************逐次学習**********************************
		//*****************出力層から中間層**************************
		//printf("出力層から中間\n");
		//重みをかけて左の層へ誤差を伝搬
		for (i = 0; i < data; i++) {
			for (j = 0; j < element + 1; j++) {
				for (k = 0; k < out; k++) {
					error->mid[i][layer - 1][j] += (1 - el->out[i][k]) * el->out[i][k] * error->out[i][k] * w->w3[j][k];
					//printf("%f\n", error->mid[i][layer - 1][j]);
				}
			}
			//printf("\n");

		//重み更新のため (by*誤差の入力*(1-y)*yの和)
			for (j = 0; j < element + 1; j++) {

				if (j == element) {
					//バイアスの更新
					//ここで更新
					for (k = 0; k < out; k++) {
						///printf("w3 = %f  ", w->w3[j]);
						w->w3[j][k] = w->w3[j][k] - ep / data * error->out[i][k] * (1 - el->out[i][k]) * el->out[i][k];
						//printf("w3 = %f\n ", w->w3[j]);
					}
				}
				else {
					//バイアス以外の更新
					//ここで更新
					for (k = 0; k < out; k++) {
						//printf("w3 = %f  ", w->w3[j]);
						w->w3[j][k] = w->w3[j][k] - ep / data * el->mid[i][layer - 1][j] * error->out[i][k] * (1 - el->out[i][k]) * el->out[i][k];
						//printf("w3 = %f\n", w->w3[j]);
					}
				}
			}
			//*********************中間層から中間層**************************
			//printf("中間層から中間\n");
			//層数-1
			for (l = layer - 1; l > 0; l--) {
				//printf("l%d\n", l);
				//重みをかけて左の層へ誤差を伝搬
				for (j = 0; j < element + 1; j++) {
					//printf("j%d\n", j);
					error->mid[i][l - 1][j] = 0;
					for (k = 0; k < element; k++) {
						//printf("k%d\n", k);
						//printf("i:%d j:%d k:%d l:%d\n", i, j, k, l);
						error->mid[i][l - 1][j] += (1 - el->mid[i][l][k])*el->mid[i][l][k] * error->mid[i][l][k] * w->w2[l - 1][j][k];
					}
				}

				//printf("重みを更新\n");
				//printf("xxx\n");
				//重み更新のためのtotal(by*誤差の入力*(1-y)*yの和)
				for (j = 0; j < element + 1; j++) {
					//printf("j%d\n", j);
					if (j == element) {
						//バイアスの更新
						for (k = 0; k < element; k++) {
							//printf("bk%d\n", k);
							//printf("aaai:%d j:%d k:%d l:%d\n", i, j, k, l);
							//ここで更新
							//printf("w2 = %f  ", w->w2[l - 1][j][k]);
							w->w2[l - 1][j][k] = w->w2[l - 1][j][k] - ep / data* error->mid[i][l][k] * (1 - el->mid[i][l][k])*el->mid[i][l][k];;
							//printf("w2 = %f\n", w->w2[l - 1][j][k]);
						}
					}
					else {
						//バイアス以外の更新
						for (k = 0; k < element; k++) {
							//printf("ck%d\n", k);
							//ここで更新
							//printf("lll%d\n", l);
							//printf("w2 = %f  ", w->w2[l - 1][j][k]);
							w->w2[l - 1][j][k] = w->w2[l - 1][j][k] - ep / data*el->mid[i][l - 1][j] * error->mid[i][l][k] * (1 - el->mid[i][l][k])*el->mid[i][l][k];
							//printf("w2 = %f\n", w->w2[l - 1][j][k]);
						}

					}
				}
			}

			//**********************中間層から入力***************************
			//printf("中間層から入力\n");
			//重み更新のためのtotal(by*誤差の入力*(1-y)*yの和)
			for (j = 0; j < in + 1; j++) {
				if (j == in) {
					//バイアスの更新
					total = 0;
					for (k = 0; k < element; k++) {
						//ここで更新
						//printf("w = %f  ", w->w1[j][k]);
						w->w1[j][k] = w->w1[j][k] - ep / data*error->mid[i][0][k] * (1 - el->mid[i][0][k])*el->mid[i][0][k];
						//printf("w = %f\n", w->w1[j][k]);
					}

				}
				else {
					//バイアス以外の更新
					total = 0;
					for (k = 0; k < element; k++) {
						//ここで更新
						//printf("w = %f  ", w->w1[j][k]);
						w->w1[j][k] = w->w1[j][k] - ep / data*el->Tin[i][j] * error->mid[i][0][k] * (1 - el->mid[i][0][k])*el->mid[i][0][k];
						//printf("w = %f\n", w->w1[j][k]);
					}

				}
			}

		}
	}
}

//未学習データに対するし出力を求める
void lcolel(int in, int layer, int element, int out, int ldata, struct eldata *el, struct wdata *w) {
	int i, j, k, l;
	for (i = 0; i < ldata; i++) {
		//********************入力から中間******************************
		//printf("el %f\n", el->mid[0][0][0]);
		//中間層の素子数分
		//printf("入力から中間\n");
		for (j = 0; j < element; j++) {
			el->mid[i][0][j] = 0;
			//入力数+1(バイアス分)
			for (k = 0; k < in + 1; k++) {
				if (k == in) {
					el->mid[i][0][j] += w->w1[k][j];
				}
				else {
					//printf("elmid %f\n", el->mid[i][0][j]);
					el->mid[i][0][j] += el->lin[i][k] * w->w1[k][j];
				}
			}
			el->mid[i][0][j] = sigmoid(el->mid[i][0][j]);
		}
		//******************中間層から中間層****************************
		//printf("中間層から中間\n");
		for (j = 0; j < layer - 1; j++) {
			//素子数分
			for (k = 0; k < element; k++) {
				el->mid[i][j + 1][k] = 0;
				//素子数+1(バイアス分
				for (l = 0; l < element + 1; l++) {
					if (l == element) {
						el->mid[i][j + 1][k] += w->w2[j][l][k];
					}
					else {
						el->mid[i][j + 1][k] += el->mid[i][j][l] * w->w2[j][l][k];
					}
				}
				el->mid[i][j + 1][k] = sigmoid(el->mid[i][j + 1][k]);
			}

		}
		//**********************中間層から出力******************************
		//素子数+1(バイアス分)
		for (j = 0; j < out; j++) {
			el->lout[i][j] = 0;
			for (k = 0; k< element + 1; k++) {
				if (k == element) {
					//printf("%f  %f  %f\n", el->out[i], el->mid[i][layer - 1][j], w->w3[j]);
					el->lout[i][j] += w->w3[k][j];
				}
				else {
					//printf("%f  %f  %f\n", el->out[i], el->mid[i][layer - 1][j], w->w3[j]);
					el->lout[i][j] += el->mid[i][layer - 1][k] * w->w3[k][j];
				}
			}
			//printf("el = %f\n", el->out[i]);
			el->lout[i][j] = sigmoid(el->lout[i][j]);
			printf("out = %f\n\n", el->lout[i][j]);
		}

	}
}