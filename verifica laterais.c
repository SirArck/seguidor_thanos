#include <config.c>

/*
essa função fica rodando e contando quantas vezes o robô passou por cada curva e se já saiu de uma,
unico para cada pista e [[[TEM QUE CONFIGURAR QUANDO CHEGAR NA COMPETIÇÃO]]] chegar na competição,
o count é usado para saber onde o robô esta na pista, logo que tipo de comportamento o robô tem que ter

o retorno padrão é de retas, então os cases só precisam ser declarado para mudanças em curvas,
also a função also conta quando sai de uma corva, passando automaticamente para o modo de retas.

also² não sei como isso funcionaria, mas tenho a ideia de resetar o valor de erro integral a cada mudança de estado,
pq acho que dependendo doq estiver acontecendo na pista, o acumulo pode fazer ter um comportamente estranho,
fora que poderia ter um modificador maior e ter mais estabilidade :thinking:

ideia futura é fazer o valor de erro decrementar com o tempo quando o robô acaba de sair de uma curva, então 
sempre que ele sair de curvas, o valor atual do erro seria jogado para outra variável que iria perder o valor com o tempo
e ser utilizado 
*/
int getCurvStat(){
    if(digitalRead(sensorCurv) == 1){ 
        countCurv =+ 1; 
        switch (countCruv){
            case 2 || 4: //sempre que o robô sair de uma curva...
                sumErrorControl = abs(sumError);
                sumErrorMod = 1;
                sumError = 0;
                break;
            
            default:
                break;
        }
    }
    switch (countCurv){
    case 1: return 0; //apenas exemplo, mas aqui seria para retornar de acordo com qual parte ele ta, curva, reta etc
    default: return 3;
    }
}

/*
verifica se o robô finalizou a pista, usando o sensor de chega,
uma coisa que acontece é ter cruzamentos, então o count garante que chegou de fato no final,
unico para cada pista e [[[TEM QUE CONFIGURAR QUANDO CHEGAR NA COMPETIÇÃO]]] chegar na competição
*/
void checkFinish(){
    if(digitalRead(sensorFinish) == 1) { countFinish =+ 1; }

    if(countFinish == 2) {
        delay(100);
        motorL.breake();
        motorR.breake();
        delay(1000*20); //depois que finalizar a pista, o robô deve ficar parado por 10 segundos, o tempo extra é por questão de segurança e pra tirar ele da pista

    }
}