#include <config.c>

QTRSensors sensor;

const uint8_t sensorCount = 15;
uint16_t sensorValues[sensorCount];
const uint8_t valorBase = 500*(SensorCount - 1);

void setup(){
    sensor.setTypeRC();
    sensor.setSensorPins((const uint8_t[]) {PB12, PB13, PB14, PB15, PA8, PA11, PA6, PA7}, 8);
    sensor.setTimeout (1000);

    delay(500);
    pinMode(PC13, OUTPUT);
    pinMode(ML1, OUTPUT);
    pinMode(ML2, OUTPUT);
    pinMode(MR1, OUTPUT);
    pinMode(MR2, OUTPUT);
    pinMode(MLpot, OUTPUT);
    pinMode(MLpot, OUTPUT);
    digitalWrite(PC13, LOW);

    for (uint16_t i = 0; i < 1500; i++){ sensor.calibrate(); }
    digitalWrite(PC13, HIGH);
}

speedL = 255;
speedR = 255;

/*
Com o objetivo de ficar o mais próximo possível da linha e ainda se mover rápido,
o código verifica onde a linha está, e usa isso para calcular a diferença de onde deveria estar usando o pid,
que é aplicado a velocidade atual, gerando as curvas e direcionando a linha
also a ideia é ter diferentes modificadores no pid para algumas situações, como linha retas, curvas menores, médias e maiores
*/
void  loop(){
    /* utilizado para para armazenar o valor retornado pelo sensor de linha
    posição da linha no sensor, retornada em forma de vetor, readLineBlack() para linha preta
    valor vai de 0 até 5000 */
    unsigned int posit = sensor.readLineWhite(sensorValues);
    int stat = getCurvStat();
    dif = getDif(posit, stat);

    speedL = constrain(speedL - dif, -MAXVelocity, MAXVelocity); //muda a velocidade de acordo com a velocidade anterior e a correção de direção
    speedR = constrain(speedR + dif, -MAXVelocity, MAXVelocity); //a função constrain é para manter o valor de velocidade entre os limites de -255 e 255

    md.dcMotorRun(MLpot, speedL);
    md.dcMotorRun(MLpot, speedR);

    checkFinish();

}