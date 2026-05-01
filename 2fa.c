#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Função para gerar código baseado no tempo (1 minuto por código)
int gerar_codigo_tempo(int secret, int offset)
{
    time_t now = time(NULL);

    // Divide o tempo em blocos de 60 segundos (1 minuto)
    int timestep = (now / 60) + offset;

    // Geração do código (6 dígitos)
    int code = (timestep ^ secret) % 1000000;

    // Garantir que seja positivo
    if (code < 0)
        code *= -1;

    return code;
}

int main()
{
    int secret = 123456; // depois vamos substituir por segredo por usuário
    int input;

    // Gera código atual e anterior (tolerância de tempo)
    int codigo_atual = gerar_codigo_tempo(secret, 0);
    int codigo_anterior = gerar_codigo_tempo(secret, -1);

    // Apenas para teste (remover depois no PAM)
    printf("Codigo atual (debug): %06d\n", codigo_atual);

    printf("Digite o codigo: ");
    scanf("%d", &input);

    // Validação
    if (input == codigo_atual || input == codigo_anterior)
    {
        printf("Acesso liberado!\n");
    }
    else
    {
        printf("Codigo incorreto!\n");
    }

    return 0;
}