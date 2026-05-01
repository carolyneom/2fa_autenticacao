#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Agora recebe o tempo como parâmetro
int gerar_codigo_tempo(int secret, int timestep)
{
    int code = (timestep ^ secret) % 1000000;

    if (code < 0)
        code *= -1;

    return code;
}

int main()
{
    int secret = 123456;
    int input;

    // Captura o tempo UMA VEZ só
    time_t now = time(NULL);
    int timestep_atual = now / 60;

    int codigo_atual = gerar_codigo_tempo(secret, timestep_atual);
    int codigo_anterior = gerar_codigo_tempo(secret, timestep_atual - 1);

    printf("Codigo atual (debug): %06d\n", codigo_atual);

    printf("Digite o codigo: ");
    scanf("%d", &input);

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
