#include <config.c>

QTRSensors sensor;

const uint8_t sensorCount = 15;
uint16_t sensorValues[sensorCount];
const uint8_t valorBase = 500*(SensorCount - 1);

void setup(){
    sensor.setTypeRC();
    sensor.setSensorPins((const uint8_t[]) {PB12, PB13, PB14, PB15, PA8, PA11, PA6, PA7}, 8);
    sensor.setTimeout (1000);

    Motor motorL = Motor(ML1, ML2, MLpot, offsetL, Stby);
    Motor motorR = Motor(MR1, MR2, MRpot, offsetR, Stby);

    delay(500);

    pinMode(SensorCurv, INPUT);
    pinMode(SensorFinish, INPUT);

    //pinMode(ML1, OUTPUT);
    //pinMode(ML2, OUTPUT);
    //pinMode(MR1, OUTPUT);
    //pinMode(MR2, OUTPUT);
    //pinMode(MLpot, OUTPUT);
    //pinMode(MLpot, OUTPUT);

    pinMode(PC13, OUTPUT);
    digitalWrite(PC13, LOW);

    for (uint16_t i = 0; i < 1500; i++){ sensor.calibrate(); }
    digitalWrite(PC13, HIGH);
}

speedL = MAXSpeed;
speedR = MAXSpeed;
delay(100);

/*
Com o objetivo de ficar o mais próximo possível da linha e ainda se mover rápido,
o código verifica onde a linha está, e usa isso para calcular a diferença de onde deveria estar usando o pid,
que é aplicado a velocidade atual, gerando as curvas e direcionando a linha
also a ideia é ter diferentes modificadores no pid para algumas situações, como linha retas, curvas menores, médias e maiores
*/
void  loop(){
    int stat = getCurvStat(); //obtem qual parte da pista o robô esta
    
    /* utilizado verificar o valor retornado pelo sensor de linha e enviar para o pid
    posição da linha no sensor, retornada em forma de vetor, readLineBlack() para linha preta
    valor vai de 0 até 5000 */
    dif = getDif(sensor.readLineWhite(sensorValues), stat); 

    if (dif == 0) { //caso que acontece normalmente em linhas retas, also se espera que só seja em linhas retas
        speedL = constrain(speedL + SpeedUP, 0, MAXSpeed + MAXSpeedBonus); //como o dif é zero, não há correção para ser feita, então o robô simplesmente acelera um pouco
        speedR = constrain(speedR + SpeedUP, 0, MAXSpeed + MAXSpeedBonus); //o bonus é para garantir que possa ir um pouco além doq é esperado de velocidade max
    }else{ //caso geral e provavelmente mais comum
        speedL = constrain(speedL - dif, -MAXSpeed, MAXSpeed); //muda a velocidade de acordo com a velocidade anterior e a correção de direção
        speedR = constrain(speedR + dif, -MAXSpeed, MAXSpeed); //a função constrain é para manter o valor de velocidade entre os limites de velocidade max e min(max negativa)
    }

    motorL.drive(speedL); //atualiza a velocidade do robô
    motorR.drive(speedR);

    checkFinish(); //verifica se o robô já finalizou a pista
}