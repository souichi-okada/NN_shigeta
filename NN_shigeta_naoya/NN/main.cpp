#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include"Header.h"
#pragma warning(disable: 4996)

//NN(�w���A�f�q�����w��)
//�w��2�A��w�̑f�q��4
//���͐�3



int main(void) {

	//���͎������C�o�͎������C�f�[�^���C���w�K�f�[�^��(�e�X�g�f�[�^)
	int in = 3, out = 1, data = 6,ldata = 2;
	//�w���A�f�q��
	int layer = 2, element = 4;		

	int i, j, k, count = 0;		//��Ɨp

	double E;							//���덷

	int learn = 1;						//�w�K���@(0�E�E�E����,1�E�E�E�ꊇ)

	printf("NN�̑w�����w��E�E�E");
	scanf_s("%d", &layer);
	printf("NN��1�w������̑f�q�����w��E�E�E");
	scanf_s("%d",& element);
	do {
		printf("�w�K���@�̎w��(0�E�E�E����,�@�@1�E�E�E�ꊇ)");
		scanf_s("%d", &learn);
	} while (learn != 0 && learn != 1) ;

	FILE *fp;

	printf("NN\n");
	//***********�e�l�̂��߂̗̈�m�ۂƏ�����***************
	eldata el;
	//���t�f�[�^
	el.Tin = (double **)malloc(sizeof(double*)*data);
	el.Tin[0] = (double*)malloc(sizeof(double)*data*in);				//2����
	for (i = 1; i < data; i++) {
		el.Tin[i] = el.Tin[0] + i*in;
	}
	//���t�o�̓f�[�^
	el.Tout = (double**)malloc(sizeof(double)*data);
	el.Tout[0] = (double*)malloc(sizeof(double) * data * out);				//2����
	for (i = 1; i < data; i++) {
		el.Tout[i] = el.Tout[0] + i * out;
	}

	if ((fp = fopen("Traning_Data.txt", "r")) == NULL) {
		printf("file open error!!\n");
		exit(EXIT_FAILURE);	/* (3)�G���[�̏ꍇ�͒ʏ�A�ُ�I������ */
	}
	
	printf("���t�f�[�^�ǂݍ���\n");
	i = 0;
	/*while ((fscanf_s(fp, "%lf %lf %lf %lf", &el.Tin[i][0], &el.Tin[i][1], &el.Tin[i][2], &el.Tout[i])) != EOF) {
		printf("%f %f %f %f\n", el.Tin[i][0], el.Tin[i][1], el.Tin[i][2], el.Tout[i]);
		i++;
	}*/
	for (i = 0; i < data; i++) {
		for (j = 0; j < in; j++){
			//���t���̓f�[�^�ǂݍ���
			if (fscanf_s(fp, "%lf", &el.Tin[i][j]) == EOF) {
				printf("EOF!\n");
				fclose(fp);
				break;
			}
		}
		for (k = 0; k < out; k++){
			//���t�o�̓f�[�^�ǂݍ���
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
	//�o�͑w�񎟌��ɑΉ�
	el.out = (double**)malloc(sizeof(double) * data);
	el.out[0] = (double*)malloc(sizeof(double) * data * out);				//2����
	for (i = 1; i < data; i++) {
		el.out[i] = el.out[0] + i * out;
	}
	
	//���ԑw
	el.mid = (double ***)malloc(sizeof(double**)*data);
	el.mid[0] = (double **)malloc(sizeof(double*)*data*layer);
	el.mid[0][0] = (double *)malloc(sizeof(double)*data*layer*element);	//3����
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
	error.in[0] = (double *)malloc(sizeof(double)*data*in);				//2����
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
	error.mid[0][0] = (double *)malloc(sizeof(double)*data*layer*(element+1));	//3����
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

	//�o�͑w�̎������ɑΉ�
	error.out = (double**)malloc(sizeof(double) * data);
	error.out[0] = (double*)malloc(sizeof(double) * data * out);				//2����
	for (i = 1; i < data; i++) {
		error.out[i] = error.out[0] + i * out;
	}
	

	//�d�݂̗̈�m�ۂƏ�����
	wdata w;
	w.w1 = (double **)malloc(sizeof(double*)*in+1);
	w.w1[0] = (double *)malloc(sizeof(double)*(in+1)*element);						//2����
	for (i = 1; i < (in + 1); i++) {
		w.w1[i] = w.w1[0] + i*element;
	}

	w.w2 = (double ***)malloc(sizeof(double**)*(layer-1));
	w.w2[0] = (double **)malloc(sizeof(double*)*(layer-1)*(element+1));
	w.w2[0][0] = (double *)malloc(sizeof(double)*(layer-1)*(element+1)*element);	//3����
	for (i = 0; i < (layer-1); i++) {
		w.w2[i] = w.w2[0] + i*(element+1);
		for (j = 0; j < (element+1); j++) {
			w.w2[i][j] = w.w2[0][0] + i*(element+1)*element + j*element;
		}
	}

	//�o�͑w�̎������̕ω��ɑΉ�
	w.w3 = (double**)malloc(sizeof(double*) * (element+1));
	w.w3[0] = (double*)malloc(sizeof(double) * (element + 1) * out);						//2����
	for (i = 1; i < (element + 1); i++) {
		w.w3[i] = w.w3[0] + i * out;
	}

	//�����̏�����
	srand((unsigned int)time(NULL));

	printf("�d�݂̏����l\n");
	printf("w1\n");
	for (i = 0; i < in + 1; i++) {
		for (j = 0; j < element; j++) {
			w.w1[i][j] = rand() / double(RAND_MAX) * 2 - 1;
			//w.w1[i][j] = 0.5;
			printf("%f ", w.w1[i][j]);
		}
		printf("\n");
	}
	//1����2
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
	//2����o��
	//�o�͂̐��̕ω��ɑΉ�
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
		printf("�����w�K\n");
	}
	else {
		printf("�ꊇ�w�K\n");
	}


	//********************�g���f�[�^�̐ݒ肪����*****************************************

	//*******************************�ꊇ�w�K********************************************
	//�ȉ������[�v���A�������A�t���������肩�����Ċw�K
	do {

		//printf("count = %d\n", count);
		printf("count = %d", count);

		//�f�[�^�����������ɓ`��������
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

		//�덷�̌v�Z���덷error�ƕ��ʂ̌덷error.out
		E = colerror(out,&el,data,&error);

		//�t����
		//printf("spread\n");
		spread(in, layer, element, out, data, &el, &error, &w,learn);
		//printf("end spread\n");
		/*
		//printf("�d�݂̏����l\n");
		printf("w1\n");
		for (i = 0; i < in + 1; i++) {
			for (j = 0; j < 4; j++) {
				//w.w1[i][j] = rand() / double(RAND_MAX) * 2 - 1;
				//w.w1[i][j] = 0.5;
				printf("%f ", w.w1[i][j]);
			}
			printf("\n");
		}
		//1����2
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
		//2����o��
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

	printf("�w�K�̏I��\n");
	printf("count = %d  error = %f\n", count, E);
	if (count == 30000) {
		printf("count = %d\n",count);
	}
	else if (E <= setE) {
		printf("�w�K���I���܂���\n");
		for (i = 0; i < data; i++) {
			for (j = 0; j < out; j++) {
				printf("Tout = %f\n", el.Tout[i][j]);
				printf("lout = %f\n", el.out[i][j]);
			}
		}
		printf("error = %f\n",E);
	}
	

	//�̈�̉��
	
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
	

	//�G���^�[�҂�
	do {
		//printf("�G���^�[�L�[�������ĉ������B");
	} while (getchar() != '\n');


	//���w�K�f�[�^����荞��
	//*********************************************************************************
	el.lin = (double **)malloc(sizeof(double*)*ldata);
	el.lin[0] = (double*)malloc(sizeof(double)*data*in);				//2����
	for (i = 1; i < ldata; i++) {
		el.lin[i] = el.lin[0] + i*in;
	}
	el.lout = (double**)malloc(sizeof(double)*ldata);
	el.lout[0] = (double*)malloc(sizeof(double) * data * out);				//2����
	for (i = 1; i < ldata; i++) {
		el.lout[i] = el.lout[0] + i * out;
	}

	if ((fp = fopen("Traning_Data.txt", "r")) == NULL) {
		printf("file open error!!\n");
		exit(EXIT_FAILURE);	/* (3)�G���[�̏ꍇ�͒ʏ�A�ُ�I������ */
	}

	printf("���w�K�f�[�^\n");

	if ((fp = fopen("Test_Data.txt", "r")) == NULL) {
		printf("file open error!!\n");
		exit(EXIT_FAILURE);	/* (3)�G���[�̏ꍇ�͒ʏ�A�ُ�I������ */
	}
	i = 0;
	while ((fscanf_s(fp, "%lf %lf %lf", &el.lin[i][0], &el.lin[i][1], &el.lin[i][2])) != EOF) {
		printf("%f %f %f\n", el.lin[i][0], el.lin[i][1], el.lin[i][2]);
		i++;
	}
	fclose(fp);


	printf("���w�K�f�[�^�ɑ΂���o��\n");
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

	//�G���^�[�҂�
	do {
		printf("�I�����܂��B�G���^�[�L�[�������ĉ������B");
	} while (getchar() != '\n');

	return 0;

}

//eldata el;
//errordata error;
//wdata w;
//������
//�����͓��͂̐�in�ƒl�A���ԑw�̑w���Ƒf�q���ƒl��ێ����邽�߂̂���,�o��,�f�[�^��
void colel(int in, int layer, int element, int out,int data,struct eldata *el,struct errordata *error,struct wdata *w) {
	int i, j, k,l;
	for (i = 0; i < data; i++) {
		//********************���͂��璆��******************************
		//printf("el %f\n", el->mid[0][0][0]);
		//���ԑw�̑f�q����
		//printf("���͂��璆��\n");
		for (j = 0; j < element; j++) {
			el->mid[i][0][j] = 0;
			//���͐�+1(�o�C�A�X��)
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
		//******************���ԑw���璆�ԑw****************************
		//printf("���ԑw���璆��\n");
		for (j = 0; j < layer - 1; j++) {
			//�f�q����
			for (k = 0; k < element; k++) {
				el->mid[i][j + 1][k] = 0;
				//�f�q��+1(�o�C�A�X��
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
		//**********************���ԑw����o��******************************
		for (j = 0; j < out; j++) {
			//�f�q��+1(�o�C�A�X��)
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

	double ep = 0.5;		//�w�K��


	if (learn == 1) {
		//*****************�ꊇ�w�K**********************************
		//*****************�o�͑w���璆�ԑw**************************
		//printf("�o�͑w���璆��\n");
		//�d�݂������č��̑w�֌덷��`��
		for (i = 0; i < data; i++) {
			for (j = 0; j < element + 1; j++) {
				for (k = 0; k < out; k++) {
					error->mid[i][layer - 1][j] += (1 - el->out[i][k]) * el->out[i][k] * error->out[i][k] * w->w3[j][k];
					//printf("%f\n", error->mid[i][layer - 1][j]);
				}
			}
			//printf("\n");
		}
		//�d�ݍX�V�̂��߂�total(by*�덷�̓���*(1-y)*y�̘a)
		for (j = 0; j < element + 1; j++) {
			if (j == element) {
				//�o�C�A�X�̍X�V
				total = 0;
				for (i = 0; i < data; i++) {
					for (k = 0; k < out; k++) {
						total += error->out[i][k] * (1 - el->out[i][k]) * el->out[i][k];
					}
				}
				//�����ōX�V
				//printf("total = %f\n", total);
				//printf("w3 = %f  ", w->w3[j]);
				for (k = 0; k < out; k++) {
					w->w3[j][k] = w->w3[j][k] - ep / data * total;
					//printf("w3 = %f\n ", w->w3[j]);
				}
			}
			else {
				//�o�C�A�X�ȊO�̍X�V
				total = 0;
				for (i = 0; i < data; i++) {
					for (k = 0; k < out; k++) {
						total += el->mid[i][layer - 1][j] * error->out[i][k]* (1 - el->out[i][k]) * el->out[i][k];
					}
				}
				//�����ōX�V
				//printf("total = %f\n", total);
				//printf("w3 = %f  ", w->w3[j]);
				for (k = 0; k < out; k++) {
					w->w3[j][k] = w->w3[j][k] - ep / data * total;
					//printf("w3 = %f\n", w->w3[j]);
				}
			}
		}
		//*********************���ԑw���璆�ԑw**************************
		//printf("���ԑw���璆��\n");
		//�w��-1
		for (l = layer - 1; l > 0; l--) {
			//printf("%d\n", l);
			//�d�݂������č��̑w�֌덷��`��
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
			//printf("�d�݂��X�V\n");
			//�d�ݍX�V�̂��߂�total(by*�덷�̓���*(1-y)*y�̘a)
			for (j = 0; j < element + 1; j++) {
				if (j == element) {
					//�o�C�A�X�̍X�V
					total = 0;
					for (k = 0; k < element; k++) {
						for (i = 0; i < data; i++) {
							total += error->mid[i][l][k] * (1 - el->mid[i][l][k])*el->mid[i][l][k];
						}
						//�����ōX�V
						//printf("total = %f\n", total);
						//printf("w2 = %f  ", w->w2[l - 1][j][k]);
						w->w2[l - 1][j][k] = w->w2[l - 1][j][k] - ep / data*total;
						//printf("w2 = %f\n", w->w2[l - 1][j][k]);
					}

				}
				else {
					//�o�C�A�X�ȊO�̍X�V
					total = 0;
					for (k = 0; k < element; k++) {
						for (i = 0; i < data; i++) {
							total += el->mid[i][l - 1][j] * error->mid[i][l][k] * (1 - el->mid[i][l][k])*el->mid[i][l][k];
						}
						//�����ōX�V
						//printf("total = %f\n", total);
						//printf("w2 = %f  ", w->w2[l - 1][j][k]);
						w->w2[l - 1][j][k] = w->w2[l - 1][j][k] - ep / data*total;
						//printf("w2 = %f\n", w->w2[l - 1][j][k]);
					}

				}
			}

		}

		//**********************���ԑw�������***************************
		//printf("���ԑw�������\n");
		//�d�ݍX�V�̂��߂�total(by*�덷�̓���*(1-y)*y�̘a)
		for (j = 0; j < in + 1; j++) {
			if (j == in) {
				//�o�C�A�X�̍X�V
				total = 0;
				for (k = 0; k < element; k++) {
					for (i = 0; i < data; i++) {
						total += error->mid[i][0][k] * (1 - el->mid[i][0][k])*el->mid[i][0][k];
					}
					//�����ōX�V
					//printf("total = %f\n", total);
					//printf("w = %f  ", w->w1[j][k]);
					w->w1[j][k] = w->w1[j][k] - ep / data*total;
					//printf("w = %f\n", w->w1[j][k]);
				}

			}
			else {
				//�o�C�A�X�ȊO�̍X�V
				total = 0;
				for (k = 0; k < element; k++) {
					for (i = 0; i < data; i++) {
						total += el->Tin[i][j] * error->mid[i][0][k] * (1 - el->mid[i][0][k])*el->mid[i][0][k];
					}
					//�����ōX�V
					//printf("total = %f\n", total);
					//printf("w = %f  ", w->w1[j][k]);
					w->w1[j][k] = w->w1[j][k] - ep / data*total;
					//printf("w = %f\n", w->w1[j][k]);
				}

			}
		}
	}
	else {
		//*****************�����w�K**********************************
		//*****************�o�͑w���璆�ԑw**************************
		//printf("�o�͑w���璆��\n");
		//�d�݂������č��̑w�֌덷��`��
		for (i = 0; i < data; i++) {
			for (j = 0; j < element + 1; j++) {
				for (k = 0; k < out; k++) {
					error->mid[i][layer - 1][j] += (1 - el->out[i][k]) * el->out[i][k] * error->out[i][k] * w->w3[j][k];
					//printf("%f\n", error->mid[i][layer - 1][j]);
				}
			}
			//printf("\n");

		//�d�ݍX�V�̂��� (by*�덷�̓���*(1-y)*y�̘a)
			for (j = 0; j < element + 1; j++) {

				if (j == element) {
					//�o�C�A�X�̍X�V
					//�����ōX�V
					for (k = 0; k < out; k++) {
						///printf("w3 = %f  ", w->w3[j]);
						w->w3[j][k] = w->w3[j][k] - ep / data * error->out[i][k] * (1 - el->out[i][k]) * el->out[i][k];
						//printf("w3 = %f\n ", w->w3[j]);
					}
				}
				else {
					//�o�C�A�X�ȊO�̍X�V
					//�����ōX�V
					for (k = 0; k < out; k++) {
						//printf("w3 = %f  ", w->w3[j]);
						w->w3[j][k] = w->w3[j][k] - ep / data * el->mid[i][layer - 1][j] * error->out[i][k] * (1 - el->out[i][k]) * el->out[i][k];
						//printf("w3 = %f\n", w->w3[j]);
					}
				}
			}
			//*********************���ԑw���璆�ԑw**************************
			//printf("���ԑw���璆��\n");
			//�w��-1
			for (l = layer - 1; l > 0; l--) {
				//printf("l%d\n", l);
				//�d�݂������č��̑w�֌덷��`��
				for (j = 0; j < element + 1; j++) {
					//printf("j%d\n", j);
					error->mid[i][l - 1][j] = 0;
					for (k = 0; k < element; k++) {
						//printf("k%d\n", k);
						//printf("i:%d j:%d k:%d l:%d\n", i, j, k, l);
						error->mid[i][l - 1][j] += (1 - el->mid[i][l][k])*el->mid[i][l][k] * error->mid[i][l][k] * w->w2[l - 1][j][k];
					}
				}

				//printf("�d�݂��X�V\n");
				//printf("xxx\n");
				//�d�ݍX�V�̂��߂�total(by*�덷�̓���*(1-y)*y�̘a)
				for (j = 0; j < element + 1; j++) {
					//printf("j%d\n", j);
					if (j == element) {
						//�o�C�A�X�̍X�V
						for (k = 0; k < element; k++) {
							//printf("bk%d\n", k);
							//printf("aaai:%d j:%d k:%d l:%d\n", i, j, k, l);
							//�����ōX�V
							//printf("w2 = %f  ", w->w2[l - 1][j][k]);
							w->w2[l - 1][j][k] = w->w2[l - 1][j][k] - ep / data* error->mid[i][l][k] * (1 - el->mid[i][l][k])*el->mid[i][l][k];;
							//printf("w2 = %f\n", w->w2[l - 1][j][k]);
						}
					}
					else {
						//�o�C�A�X�ȊO�̍X�V
						for (k = 0; k < element; k++) {
							//printf("ck%d\n", k);
							//�����ōX�V
							//printf("lll%d\n", l);
							//printf("w2 = %f  ", w->w2[l - 1][j][k]);
							w->w2[l - 1][j][k] = w->w2[l - 1][j][k] - ep / data*el->mid[i][l - 1][j] * error->mid[i][l][k] * (1 - el->mid[i][l][k])*el->mid[i][l][k];
							//printf("w2 = %f\n", w->w2[l - 1][j][k]);
						}

					}
				}
			}

			//**********************���ԑw�������***************************
			//printf("���ԑw�������\n");
			//�d�ݍX�V�̂��߂�total(by*�덷�̓���*(1-y)*y�̘a)
			for (j = 0; j < in + 1; j++) {
				if (j == in) {
					//�o�C�A�X�̍X�V
					total = 0;
					for (k = 0; k < element; k++) {
						//�����ōX�V
						//printf("w = %f  ", w->w1[j][k]);
						w->w1[j][k] = w->w1[j][k] - ep / data*error->mid[i][0][k] * (1 - el->mid[i][0][k])*el->mid[i][0][k];
						//printf("w = %f\n", w->w1[j][k]);
					}

				}
				else {
					//�o�C�A�X�ȊO�̍X�V
					total = 0;
					for (k = 0; k < element; k++) {
						//�����ōX�V
						//printf("w = %f  ", w->w1[j][k]);
						w->w1[j][k] = w->w1[j][k] - ep / data*el->Tin[i][j] * error->mid[i][0][k] * (1 - el->mid[i][0][k])*el->mid[i][0][k];
						//printf("w = %f\n", w->w1[j][k]);
					}

				}
			}

		}
	}
}

//���w�K�f�[�^�ɑ΂��邵�o�͂����߂�
void lcolel(int in, int layer, int element, int out, int ldata, struct eldata *el, struct wdata *w) {
	int i, j, k, l;
	for (i = 0; i < ldata; i++) {
		//********************���͂��璆��******************************
		//printf("el %f\n", el->mid[0][0][0]);
		//���ԑw�̑f�q����
		//printf("���͂��璆��\n");
		for (j = 0; j < element; j++) {
			el->mid[i][0][j] = 0;
			//���͐�+1(�o�C�A�X��)
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
		//******************���ԑw���璆�ԑw****************************
		//printf("���ԑw���璆��\n");
		for (j = 0; j < layer - 1; j++) {
			//�f�q����
			for (k = 0; k < element; k++) {
				el->mid[i][j + 1][k] = 0;
				//�f�q��+1(�o�C�A�X��
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
		//**********************���ԑw����o��******************************
		//�f�q��+1(�o�C�A�X��)
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