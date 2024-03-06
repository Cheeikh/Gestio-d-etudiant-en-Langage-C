#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>    // Pour usleep()
#include <sys/types.h> // Pour pid_t
#include <sys/wait.h>  // Pour waitpid()

#define MAX_LONGUEUR_NOM 50
#define MAX_LONGUEUR_MDP 50
#define MAX_LONGUEUR_STATUT 50
#define MAX_UTILISATEURS 100

typedef struct
{
    char nom[MAX_LONGUEUR_NOM];
    char mot_de_passe[MAX_LONGUEUR_MDP];
    char statut[MAX_LONGUEUR_STATUT];
} Utilisateur;

int lire_utilisateurs(Utilisateur utilisateurs[])
{
    FILE *fichier;
    int nombre_utilisateurs = 0;

    fichier = fopen("utilisateurs.txt", "r");
    if (fichier == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier.\n");
        exit(1);
    }

    while (fscanf(fichier, "%s %s %s", utilisateurs[nombre_utilisateurs].nom, utilisateurs[nombre_utilisateurs].mot_de_passe, utilisateurs[nombre_utilisateurs].statut) == 3)
    {
        nombre_utilisateurs++;
    }

    fclose(fichier);
    return nombre_utilisateurs;
}

int verifier_authentification(char nom_utilisateur[], char mot_de_passe[], Utilisateur utilisateurs[], int nombre_utilisateurs)
{
    for (int i = 0; i < nombre_utilisateurs; i++)
    {
        if (strcmp(nom_utilisateur, utilisateurs[i].nom) == 0 && strcmp(mot_de_passe, utilisateurs[i].mot_de_passe) == 0)
        {
            return i; // Renvoie l'indice de l'utilisateur authentifié
        }
    }
    return -1; // Utilisateur non trouvé ou mot de passe incorrect
}

void afficher_menu_admin()
{
    printf("\nMenu Administrateur:\n");
    printf("1. Gestion des Étudiants.\n");
    printf("2. Générer les fichiers.\n");
    printf("3. Marquer les Présences.\n");
    printf("4. Envoyer un message.\n");
    printf("5. Quitter.\n");
}

void afficher_menu_etudiant()
{
    printf("\nMenu Étudiant:\n");
    printf("1. Marquer ma présence\n");
    printf("2. Consulter le nombre de minutes d'absence\n");
    printf("3. Consulter mes messages\n");
    printf("4. Quitter\n");
}

void afficher_menu(Utilisateur utilisateur)
{
    if (strcmp(utilisateur.statut, "admin") == 0)
    {
        afficher_menu_admin();
    }
    else if (strcmp(utilisateur.statut, "etudiant") == 0)
    {
        afficher_menu_etudiant();
    }
    else
    {
        printf("Statut inconnu.\n");
    }
}

int main()
{
    Utilisateur utilisateurs[MAX_UTILISATEURS];
    int nombre_utilisateurs;

    nombre_utilisateurs = lire_utilisateurs(utilisateurs);

    char nom_utilisateur[MAX_LONGUEUR_NOM];
    char mot_de_passe[MAX_LONGUEUR_MDP];
    int indice_utilisateur;

    do
    {
        printf("Nom d'utilisateur : ");
        scanf("%s", nom_utilisateur);
        printf("Mot de passe : ");
        scanf("%s", mot_de_passe);

        indice_utilisateur = verifier_authentification(nom_utilisateur, mot_de_passe, utilisateurs, nombre_utilisateurs);

        if (indice_utilisateur == -1)
        {
            printf("Nom d'utilisateur ou mot de passe incorrect. Veuillez réessayer.\n");
        }
    } while (indice_utilisateur == -1);

    printf("Connexion réussie.\n");

    // Affichage d'un temps de chargement
    printf("Chargement");
    fflush(stdout);
    for (int i = 0; i < 3; i++)
    {
        printf(".");
        fflush(stdout);
        usleep(500000); // Pause de 500 millisecondes
    }
    printf("\n");

    // Effacer l'écran
    pid_t pid;
    pid = fork();
    if (pid < 0)
    {
        fprintf(stderr, "Erreur lors de la création du processus fils\n");
        exit(1);
    }
    else if (pid == 0)
    { // Processus fils
        execlp("clear", "clear", NULL);
    }
    else
    { // Processus parent
        waitpid(pid, NULL, 0);
        // Afficher le menu après l'effacement de l'écran
        afficher_menu(utilisateurs[indice_utilisateur]);
    }

    return 0;
}
