#include <config.c>

/*
controle de velocidade dos motores
motor = 0 para o motor esquerdo
motor = 1 para o motor direito

por algumm motivo que eu não sei ainda o controle de motor quando ta indo para frente é feito
com o MAXVelocity (255) - a velocidade calculada
se for MAXVelocity o robô para, se for 0 ele anda pra frente em velocidade MAXVelocityima, eu acho

com isso a ideia é que os valores sejam tratados na função, mas ainda não sei como exatamente
arrumar esses valores, tentar melhorar depois quando souber pq eles se comportam dessa forma

also, os valores de velocidade, no código antigo, quando enviados para a função, no caso do 
motor direito a velocidade é a subtração de linear e diferencial, já para o esquerdo é uma 
adição :thinking:
*/
/*
void setMotorSpeed (int motor, int speed){
    if (motor == 0){ //motor esquerdo
        digitalWrite(ML1, HIGH);
        digitalWrite(ML2, LOW);
        if(speed > 0){
            if (speed > MAXVelocity) speed = MAXVelocity;
            analogWrite(MLpot, MAXVelocity - speed); //não sei pq é feito assim, mas é oq eu falei antes
        }else{
            if (speed < -MAXVelocity) speed = -MAXVelocity;
            analogWrite(MLpot, -speed);
        } 
        
    }
    else{ //motor direito
        digitalWrite(MR1, HIGH);
        digitalWrite(MR2, LOW);
        if(speed > 0){
            if (speed > MAXVelocity) speed = MAXVelocity;
            analogWrite(MRpot, MAXVelocity - speed); //não sei pq é feito assim, mas é oq eu falei antes
        }else{
            if (speed < -MAXVelocity) speed = -MAXVelocity;
            analogWrite(MRpot, -speed);
        } 
    }
}
*/

/*
Calculo do PID 
meio complicado de explicar, mas a ideia é calcular o quão desalinhado o robô está da linha
quanto mais desalinhado, maior será o valor de erro, resultando em uma curva maior em direção a linha, fazendo o robô voltar para a rota

os valores de K são multiplicadores para os parametrôs de erro, cada vetor de K tem multiplicadores diferentes pra cada situação
do robô, essa parte muda conforme a pista e o estado é determinado pelo contador de curvas
*/
float getDif(unsigned int posit, int stat){
    error = LineCenter - posit;
    sumError += error;
    switch (stat){
        case 0: dif = (kSC[0] * error) + (kSC[1] * (error - lastError) + (kSC[2] * sumError)); break; //Curva lenta

        case 1: dif = (kMC[0] * error) + (kMC[1] * (error - lastError) + (kMC[2] * sumError)); break; //Curva média

        case 2: dif = (kFC[0] * error) + (kFC[1] * (error - lastError) + (kFC[2] * sumError)); break; //Curva rápida

        case 3: dif = (kSt[0] * error) + (kSt[1] * (error - lastError) + (kSt[2] * sumError)); break; //Reta

        case 4: dif = (kSp[0] * error) + (kSp[1] * (error - lastError) + (kSp[2] * sumError)); break; //Parar
    }

    lastError = error;

    return dif;
}
