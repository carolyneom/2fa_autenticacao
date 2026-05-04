#include <security/pam_modules.h>
#include <security/pam_ext.h>
#include <security/pam_appl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Gera código baseado no tempo
int gerar_codigo(int secret, int timestep)
{
    long long mix = (long long)timestep * 1103515245 + secret;
    int code = mix % 1000000;

    if (code < 0)
        code *= -1;

    return code;
}

// Busca secret do usuário
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

// Função para pedir input corretamente via PAM
int pedir_codigo(pam_handle_t *pamh, const char *msg, char *resp, int tam)
{
    struct pam_conv *conv;
    struct pam_message msgp;
    const struct pam_message *msgpp;
    struct pam_response *response = NULL;

    if (pam_get_item(pamh, PAM_CONV, (const void **)&conv) != PAM_SUCCESS)
        return PAM_AUTH_ERR;

    msgp.msg_style = PAM_PROMPT_ECHO_ON;
    msgp.msg = msg;
    msgpp = &msgp;

    if (conv->conv(1, &msgpp, &response, conv->appdata_ptr) != PAM_SUCCESS)
        return PAM_AUTH_ERR;

    if (response == NULL || response->resp == NULL)
        return PAM_AUTH_ERR;

    strncpy(resp, response->resp, tam - 1);
    resp[tam - 1] = '\0';

    free(response->resp);
    free(response);

    return PAM_SUCCESS;
}

// Função principal do PAM
PAM_EXTERN int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
    const char *usuario;

    if (pam_get_user(pamh, &usuario, NULL) != PAM_SUCCESS)
        return PAM_AUTH_ERR;

    int secret = buscar_secret(usuario);

    if (secret == -1)
        return PAM_AUTH_ERR;

    time_t now = time(NULL);
    int timestep = now / 60;

    int codigo_atual = gerar_codigo(secret, timestep);
    int codigo_anterior = gerar_codigo(secret, timestep - 1);

    char input[20];

    if (pedir_codigo(pamh, "Codigo 2FA: ", input, sizeof(input)) != PAM_SUCCESS)
        return PAM_AUTH_ERR;

    int user_code = atoi(input);

    if (user_code == codigo_atual || user_code == codigo_anterior)
        return PAM_SUCCESS;

    return PAM_AUTH_ERR;
}

// Necessário para PAM
PAM_EXTERN int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
    return PAM_SUCCESS;
}
