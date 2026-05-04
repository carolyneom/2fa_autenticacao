#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int gerar_codigo(int secret, int timestep)
{
    long long mix = (long long)timestep * 1103515245 + secret;
    int code = mix % 1000000;

    if (code < 0)
        code *= -1;

    return code;
}

int main()
{
    int secret = 123456;
    int input;

    time_t now = time(NULL);
    int timestep = now / 60;

    int codigo_atual = gerar_codigo(secret, timestep);
    int codigo_anterior = gerar_codigo(secret, timestep - 1);

    printf("Codigo atual: %06d\n", codigo_atual);

    printf("Digite o codigo: ");
    scanf("%d", &input);

    if (input == codigo_atual || input == codigo_anterior)
        printf("Acesso liberado!\n");
    else
        printf("Codigo incorreto!\n");

    return 0;
}
