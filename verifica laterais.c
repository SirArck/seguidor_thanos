#include <config.h>

/*
essa função fica rodando e contando quantas vezes o robô passou por cada curva e se já saiu de uma,
unico para cada pista e [[[TEM QUE CONFIGURAR QUANDO CHEGAR NA COMPETIÇÃO]]] chegar na competição,
o count é usado para saber onde o robô esta na pista, logo que tipo de comportamento o robô tem que ter

o retorno padrão é de retas, então os cases só precisam ser declarado para mudanças em curvas,
also a função also conta quando sai de uma corva, passando automaticamente para o modo de retas.

also² não sei como isso funcionaria, mas tenho a ideia de resetar o valor de erro integral a cada mudança de estado,
pq acho que dependendo doq estiver acontecendo na pista, o acumulo pode fazer ter um comportamente estranho,
fora que poderia ter um modificador maior e ter mais estabilidade :thinking:
*/
int getCurvStat(){
    if(digitalRead(sensorCurv) == 1){ 
        countCurv =+ 1; 
        sumError = 0;
        }

    switch (countCurv)
    {
    case 1:
        return 0; //apenas exemplo
    break;
    default: return 3; break;
    }
}

/*
verifica se o robô finalizou a pista, usando o sensor de chega,
uma coisa que acontece é ter cruzamentos, então o count garante que chegou de fato no final,
unico para cada pista e [[[TEM QUE CONFIGURAR QUANDO CHEGAR NA COMPETIÇÃO]]] chegar na competição
*/
void checkFinish(){
    if(digitalRead(sensorFinish) == 1) { countFinish =+ 1; }

    if(countFinish == 1) {
        delay(100);
        md.dcMotorRun(MLpot, 0);
        md.dcMotorRun(MRpot, 0);
    }
}