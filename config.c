#include <QTRSensors.h>
#include <Wire.h>
#include <SparkFun_TB6612.h>

//input e output
#define ML1 1 //definir as saidas dos motores depois de acordo com oq ficar no projeto
#define ML2 2
#define MLpot 3
#define MR1 4
#define MR2 5
#define MRpot 6
#define Stby 7 //utilizado para desligar os motores completamente
#define SensorCurv 8 //sensor esquerdo que verifica começo e final de curvas
#define SensorFinish 9 //sensor direito que verifica o final da pista

//controle de velocidade e sensor de linha
#define MAXSpeed 100 //o valor maximo real é 255, mas isso pode causar danos aos motores e o robô pode simplesmente não ter tempo de realizar nenhuma leitura
#define SpeedUP 0.5 //valor utilizado como base para aumentar da velocidade do robô em linhas retas
#define MAXSpeedBonus 10 //bonus para velocidade maxima do robô, utilizado em linhas retas, não tem uma forma de determinar o valor, mas tentar manter algo que não 
//  faça o robô ficar insano e não conseguir detectar nada

//valores do sensor de linha
#define LineCenter 3500 //referente ao valor retornado do sensor de linha quando o robô esta totalmente centralizado, valor idealizado e fora da realidade

//controle de erro
#define MAXMODSumError 1.5 //utilizado como limitador do multiplicador do integral de erros
#define SumErrorDec 0.95 //reduz o valor do integral do erro da ultima etapa para chegar o mais perto de zero e ter uma influência menor nas retas conforme os ciclos
#define ExpectedError 200 /* o sensor sempre envia um valor diferente do real de posição da linha, 
    mesmo centralizada há uma incerteza, esse valor evita que o robô fique flicando mesmo totalmente centralizado */

const int offsetL = 1; //parte da biblioteca, pelo oq eu entendi é utilizado para determinar o lado para onde o robô esta indo quando não tem a opção de
const int offsetR = 1; //mudar fisicamente as saidas

//variáveis globais de controle
//posição atual do robô na pista
int countCurv = 0; //conta por quantas curvas o robô já passou, conta as linhas vistas na esquerda e com isso determina a mudança de etapa
int countFinish = 0; //verifica se o robô já chegou no final da pista, necessário para evitar erros com encruzilhadas

//registro de velocidade dos motores
float speedL = 0; //declaração das velocidades dos motores dos robôs
float speedR = 0;

//modificadores de erro do pid
int sumError = 0; //integral dos valores de erros obtidos durante o processo na etapa atual, utilizado no calculo do pid e resetado quando o robô entra em uma nova etapa
int sumErrorControl = 0; //recebe o valor do integral de erro anterior e decrementa ele com o tempo, utilizado no calculo do multiplicador de erro total após curvas
float sumErrorMod = 1; //utilizado como uma forma de alterar do K para erro total em retas, tentativa de fazer o robô estabilizar de maneira rápida
int error = 0; //diferença entre o valor medido pelo sensor de linha e o valor esperado
int lastError = 0; //quarta o valor de erro obtido no ultimo ciclo do robô
float dif = 0; //resultado do pid, ultilizado como valor para correção da posição do robô

/*
os valores ainda não foram testados e são só algumas bases que podem ser utilizadas posteriormente para algo mais preciso

essa ideia foi inspirada pela implementação do sloth, mas acho que da pra aprofundar ela e fazer algo mais diferente e próprio,
por enquanto isso é questão de falta de tempo
*/
const float kSC[3] = {0.30, 0.00020, 0.0000035};// 0 - kp: erro | 1 - kd: variação do erro | 2 - ki: somatório do erro | Curva lenta
const float kMC[3] = {0.40, 0.00020, 0.0000035};// 0 - kp: erro | 1 - kd: variação do erro | 2 - ki: somatório do erro | Curva média
const float kFC[3] = {0.70, 0.00022, 0.0000075};// 0 - kp: erro | 1 - kd: variação do erro | 2 - ki: somatório do erro | Curva rápida
const float kSt[3] = {1.00, 0.00018, 0.0000075};// 0 - kp: erro | 1 - kd: variação do erro | 2 - ki: somatório do erro | Reta
const float kSp[3] = {0.10, 0.00010, 0.0000010};// 0 - kp: erro | 1 - kd: variação do erro | 2 - ki: somatório do erro | Parar