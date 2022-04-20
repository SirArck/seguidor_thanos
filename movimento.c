#include <config.c>

/*
controle de velocidade dos motores
motor = 0 para o motor esquerdo
motor = 1 para o motor direito

por algumm motivo que eu não sei ainda o controle de motor quando ta indo para frente é feito
com o MAXSpeed (255) - a velocidade calculada
se for MAXSpeed o robô para, se for 0 ele anda pra frente em velocidade MAXSpeedima, eu acho

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
            if (speed > MAXSpeed) speed = MAXSpeed;
            analogWrite(MLpot, MAXSpeed - speed); //não sei pq é feito assim, mas é oq eu falei antes
        }else{
            if (speed < -MAXSpeed) speed = -MAXSpeed;
            analogWrite(MLpot, -speed);
        } 
        
    }
    else{ //motor direito
        digitalWrite(MR1, HIGH);
        digitalWrite(MR2, LOW);
        if(speed > 0){
            if (speed > MAXSpeed) speed = MAXSpeed;
            analogWrite(MRpot, MAXSpeed - speed); //não sei pq é feito assim, mas é oq eu falei antes
        }else{
            if (speed < -MAXSpeed) speed = -MAXSpeed;
            analogWrite(MRpot, -speed);
        } 
    }
}
*/

/*
Calculo do PID 
meio complicado de explicar, mas a ideia é calcular o quão desalinhado o robô está da linha
quanto mais desalinhado, maior será o valor de erro, resultando em uma curva maior em direção a linha, fazendo o robô voltar para a rota

os valores de K são multiplicadores para os parametrôs de erro, cada vetor de K tem diferentes pra cada situação do robô, 
com o tempo o plano é ter um range maior de vetores K, especificando mais os casos e melhorando o desempenho
oq pode ser feito é usar alguma forma de juntar os dados obitido de erros e tratar eles para aperfeiçoar os valores dos vetores K
conversar com o professor lusca sobre alguma coisa pra processar esses dados

a variavel stat é retornada pelo detector de curvas, que registra a parte da pista atual e o getDif() usa isso para escolher qual vetor K utilizar

ExpectedError existe para evitar que o robô fique flicando na pista,
no geral o sensor não retorna o valor real, mesmo que a linha esteja no meio, o valor retornado será de como se ela tivesse um poucco deslocada,
o erro esperado elimita casos como esse pelo menos para quando a linha estiver centralizada, isso acaba sendo mais útil para situações de retas,
sem uma verificação do tipo, apesar de quase insiginificante, o robô teria tempo calculando ajuste minimos que nem mesmo poderiam ter impacto,
funcionando mais como algo para aumentar o tempo de processamento.

a operação que determina o dif trata o valor de erro obitido aplicando multiplicadores com o objetivo de determinar qual velocidade necessária retornar a linha como
ponto central

no caso da reta, a operação do calculo do sumerrormod é um multiplicador que diminui quanto mais tempo o robô esta andando em uma parte de linha reta,
a ideia por trás disso é tentar forçar um dif maior e fazer com que o robô não se perca da linha quando sair de uma curva, o valor vai diminuindo e chega próximo de 1

caso o erro do robô esteja dentro do limite, a velocidade dele aumenta com o intuito de ir mais rápido em retas, já que vai estar alinhado, sempre vai ter de velocidade
*/
float getDif(unsigned int posit, int stat){
    error = LineCenter - posit; //determina o quão fora do esperado o robô esta
    if (abs(error) > ExpectedError){ //verifica se o erro é maior que o esperado
        sumError += error; //integral dos erros registrados anteriormente
        switch (stat){
            case 0: dif = (kSC[0] * error) + (kSC[1] * (error - lastError) + (kSC[2] * sumError)); break; //Curva lenta
            case 1: dif = (kMC[0] * error) + (kMC[1] * (error - lastError) + (kMC[2] * sumError)); break; //Curva média
            case 2: dif = (kFC[0] * error) + (kFC[1] * (error - lastError) + (kFC[2] * sumError)); break; //Curva rápida
            case 3: dif = (kSp[0] * error) + (kSp[1] * (error - lastError) + (kSp[2] * sumError)); break; //Parar
            case 4: //Reta
                sumErrorMod = constrain(abs(sumError / (sumErrorControl - abs(sumError))), 1, MAXMODSumError);
                dif = (kSt[0] * error) + (kSt[1] * (error - lastError) + (kSt[2] * sumError * sumErrorMod)); 
                sumErrorControl = sumErrorControl * SumErrorDec;
            break;
        }
        lastError = error;
        return dif;
    } else {
        return 0;
    }
}
multiplicador = constrain(abs(sumerroatual / (sumerroanterior - abs(sumerroatual))), 0, maxmultiplicador);
multiplicadorspeed = constrain(multiplicadorspeed - decrementospeed, 1, 1.5)