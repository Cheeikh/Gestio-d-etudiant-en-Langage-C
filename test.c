#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#define MAX_PASSWORD_LENGTH 25

// Cette fonction vérifie les informations de connexion
// Elle retourne 1 si la connexion est valide et 0 sinon
int checkConnection(const char *login, const char *password) {
    // Implémentation factice pour les besoins de démonstration
    // Vous devez remplacer ceci par votre propre mécanisme d'authentification
    if (strcmp(login, "utilisateur") == 0 && strcmp(password, "motdepasse") == 0) {
        return 1;
    } else {
        return 0;
    }
}

int connecter() {
    char login[15];
    char pwd[MAX_PASSWORD_LENGTH];

    char reponse = 'n';
    int result;
    int isvalid;
    do {
        do {
            isvalid = 0;

            printf("Veuillez saisir votre login : ");
            getchar(); // pour consommer le caractère '\n' dans le buffer d'entrée
            fgets(login, sizeof(login), stdin);

            if (login[0] == '\n' || login[0] == '\0') {
                puts("Login invalide");
                isvalid = 1;
            }
        } while (isvalid == 1);

        saisieMotDePasse(pwd);

        login[strcspn(login, "\n")] = '\0'; // Supprimer le caractère de nouvelle ligne
        pwd[strcspn(pwd, "\n")] = '\0';     // Supprimer le caractère de nouvelle ligne

        result = checkConnection(login, pwd);

        if (result == 0) {
            puts("Login ou mot de passe invalide");
            printf("Voulez-vous réessayer de vous connecter ? (o/n) : ");
            scanf(" %c", &reponse); // L'espace avant %c permet de consommer les espaces, les retours chariot, etc.
        } else {
            break;
        }
    } while (reponse != 'n');

    return result;
}

void disableEcho() {
    struct termios oldt, newt;

    // Sauvegarder les paramètres du terminal
    tcgetattr(STDIN_FILENO, &oldt);

    // Copier les paramètres
    newt = oldt;

    // Désactiver l'écho dans les caractères saisis
    newt.c_lflag &= ~(ECHO | ICANON);

    // Appliquer les nouveaux paramètres
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

void enableEcho() {
    struct termios oldt;

    // Restaurer les paramètres du terminal
    tcgetattr(STDIN_FILENO, &oldt);

    // Rétablir l'écho
    oldt.c_lflag |= (ECHO | ICANON);

    // Appliquer les paramètres restaurés
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

void saisieMotDePasse(char *password) {
    printf("Entrez votre mot de passe : ");
    fflush(stdout); // Assurer l'affichage du message avant la saisie

    disableEcho();

    int i = 0;
    char ch;

    while ((ch = getchar()) != '\n' && ch != '\r' && i < MAX_PASSWORD_LENGTH - 1) {
        if (ch == 127 || ch == 8) { // Gérer la touche "Retour arrière"
            if (i > 0) {
                printf("\b \b");
                i--;
            }
        } else {
            password[i++] = ch;
            putchar('*');
        }
    }

    password[i] = '\0';
    enableEcho();
    printf("\n");
}

int main() {
    // Utilisation de la fonction connecter()
    int connectionStatus = connecter();
    if (connectionStatus == 1) {
        printf("Vous êtes connecté.\n");
    } else {
        printf("Échec de la connexion.\n");
    }

    return 0;
}
