#include <QTRSensors.h>
#include <Wire.h>
#include "Grove_Motor_Driver_TB6612FNG.h"

#define ML1 PA1
#define ML2 PA0
#define MLpot PB1
#define MR1 PA3
#define MR2 PA2
#define MRpot PB0
#define MAXVelocity 255

MotorDriver md;
int LineCenter = 3500; //referente ao valor esperado de retorno do sensor de linha, representa um robô totalmente centralizado

int countCurv = 0;
int countFinish = 0;
float speedL = 0;
float speedR = 0;
int sum = 0;
int sumError = 0;//integral dos valores de erros obtidos durante o processo como um todo, utilizado no calculo do pid
int error;//diferença entre o valor medido pelo sensor de linha e o valor esperado
int lastError = 0;
int dif;//resultado do pid, ultilizado como valor para correção da posição do robô

const float kSC[3] = {0.30, 0.00020, 0.0000035};// 0 - kp: erro | 1 - kd: variação do erro | 2 - ki: somatório do erro | Curva lenta
const float kMC[3] = {0.40, 0.00020, 0.0000035};// 0 - kp: erro | 1 - kd: variação do erro | 2 - ki: somatório do erro | Curva média
const float kFC[3] = {0.70, 0.00022, 0.0000075};// 0 - kp: erro | 1 - kd: variação do erro | 2 - ki: somatório do erro | Curva rápida
const float kSt[3] = {1.00, 0.00018, 0.0000075};// 0 - kp: erro | 1 - kd: variação do erro | 2 - ki: somatório do erro | Reta
const float kSp[3] = {0.10, 0.00010, 0.0000010};// 0 - kp: erro | 1 - kd: variação do erro | 2 - ki: somatório do erro | Parar