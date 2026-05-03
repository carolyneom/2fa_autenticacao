#include <security/pam_modules.h>
#include <security/pam_ext.h>
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
        sscanf(linha, "%[^:]:%d", user, &secret);

        if (strcmp(user, usuario) == 0)
        {
            fclose(file);
            return secret;
        }
    }

    fclose(file);
    return -1;
}

PAM_EXTERN int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
    const char *usuario;
    pam_get_user(pamh, &usuario, NULL);

    int secret = buscar_secret(usuario);

    if (secret == -1)
        return PAM_AUTH_ERR;

    time_t now = time(NULL);
    int timestep = now / 60;

    int codigo = gerar_codigo(secret, timestep);

    const char *input;
pam_get_authtok(pamh, PAM_AUTHTOK, &input, "Codigo 2FA: ");

if (input == NULL)
    return PAM_AUTH_ERR;

int user_code = atoi(input);

    if (user_code == codigo)
        return PAM_SUCCESS;

    return PAM_AUTH_ERR;
}

PAM_EXTERN int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
    return PAM_SUCCESS;
}
