#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int gerar_codigo(int secret, int timestep)
{
    long long mix = (long long)timestep * 1103515245 + secret;
    int code = mix % 1000000;

    if (code < 0)
        code *= -1;

    return code;
}

int buscar_secret(const char *usuario)
{
    FILE *file = fopen("/etc/security/2fa_secrets.conf", "r");
    if (!file)
        return -1;

    char linha[100];
    char user[50];
    int secret;

    while (fgets(linha, sizeof(linha), file))
    {
        if (sscanf(linha, "%[^:]:%d", user, &secret) == 2)
        {
            if (strcmp(user, usuario) == 0)
            {
                fclose(file);
                return secret;
            }
        }
    }

    fclose(file);
    return -1;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Uso: ./2fa <usuario>\n");
        return 1;
    }

    int secret = buscar_secret(argv[1]);

    if (secret == -1)
    {
        printf("Usuario nao encontrado\n");
        return 1;
    }

    time_t now = time(NULL);
    int timestep = now / 60;

    int codigo = gerar_codigo(secret, timestep);

    printf("Codigo para %s: %06d\n", argv[1], codigo);

    return 0;
}
