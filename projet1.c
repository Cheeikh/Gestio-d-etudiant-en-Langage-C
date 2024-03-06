#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>    // Pour usleep()
#include <sys/types.h> // Pour pid_t
#include <sys/wait.h>  // Pour waitpid()
#include <termios.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>

#define MAX_LONGUEUR_NOM 50
#define MAX_LONGUEUR_MDP 50
#define MAX_LONGUEUR_STATUT 50
#define MAX_UTILISATEURS 100
#define MAX_CLASSES 50
#define MAX_ETUDIANTS_PAR_CLASSE 50
#define MAX_PRESENCES 1000
#define MAX_LONGUEUR_NOM_UTILISATEUR 50

// Fonction pour masquer le mot de passe lors de la saisie
// Fonction pour masquer le mot de passe lors de la saisie
void saisir_mot_de_passe(char *mot_de_passe)
{
    struct termios oldt, newt;
    int i = 0;
    char ch;

    // Récupérer les paramètres actuels du terminal
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    // Désactiver l'écho
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // Modifier le mode de l'entrée standard pour désactiver le buffering
    setbuf(stdin, NULL);

    // Lire le mot de passe caractère par caractère
    while ((ch = getchar()) != '\n' && i < MAX_LONGUEUR_MDP)
    {
        mot_de_passe[i++] = ch;
        printf("*");
    }

    // Ajouter le caractère de fin de chaîne
    mot_de_passe[i] = '\0';

    // Réinitialiser les paramètres du terminal
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    // Réactiver le buffering de l'entrée standard
    setbuf(stdin, NULL);
}

typedef struct
{
    char nom[MAX_LONGUEUR_NOM];
    char mot_de_passe[MAX_LONGUEUR_MDP];
    char statut[MAX_LONGUEUR_STATUT];
} Utilisateur;

typedef struct
{
    char nom_classe[MAX_LONGUEUR_NOM];
} Classe;

typedef struct
{
    int id;
    char prenom[MAX_LONGUEUR_NOM];
    char nom[MAX_LONGUEUR_NOM];
    bool est_present;
} Etudiant;

typedef struct
{
    int id_etudiant;
    char prenom[MAX_LONGUEUR_NOM];
    char nom[MAX_LONGUEUR_NOM];
    time_t date;
} Presence;

typedef struct
{
    char nom_utilisateur[MAX_LONGUEUR_NOM_UTILISATEUR];
    int id_etudiant;
} LienUtilisateurEtudiant;

int lire_liens_utilisateurs_etudiants(LienUtilisateurEtudiant liens_utilisateurs_etudiants[])
{
    FILE *fichier;
    int nombre_liens = 0;

    fichier = fopen("etudiants.txt", "r");
    if (fichier == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier des étudiants.\n");
        exit(1);
    }

    while (fscanf(fichier, "%s %d", liens_utilisateurs_etudiants[nombre_liens].nom_utilisateur, &liens_utilisateurs_etudiants[nombre_liens].id_etudiant) == 2)
    {
        nombre_liens++;
    }

    fclose(fichier);
    return nombre_liens;
}

int trouver_id_etudiant(char nom_utilisateur[], LienUtilisateurEtudiant liens_utilisateurs_etudiants[], int nombre_liens)
{
    for (int i = 0; i < nombre_liens; i++)
    {
        if (strcmp(nom_utilisateur, liens_utilisateurs_etudiants[i].nom_utilisateur) == 0)
        {
            return liens_utilisateurs_etudiants[i].id_etudiant;
        }
    }
    return -1; // Si le nom d'utilisateur n'est pas trouvé
}


void marquer_presence(int id_etudiant)
{
    // Vérifier si l'étudiant est déjà marqué présent
    FILE *fichier = fopen("presences.txt", "r");
    if (fichier == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier des présences.\n");
        return;
    }

    int etudiant_present = 0;
    int etudiant_id;
    time_t presence_date;
    while (fscanf(fichier, "%d %ld", &etudiant_id, &presence_date) == 2)
    {
        if (etudiant_id == id_etudiant)
        {
            etudiant_present = 1;
            break;
        }
    }
    fclose(fichier);

    if (etudiant_present)
    {
        printf("Déjà marqué présent.\n");
        return;
    }

    // Ajouter la présence uniquement si l'étudiant n'est pas déjà marqué présent
    fichier = fopen("presences.txt", "a");
    if (fichier == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier des présences.\n");
        return;
    }

    time_t now;
    time(&now);
    fprintf(fichier, "%d %ld\n", id_etudiant, now);
    fclose(fichier);

    printf("Présence marquée pour l'étudiant ID %d\n", id_etudiant);
}



void marquer_presence_etudiant(char *nom_utilisateur)
{
    FILE *fichier_etudiants = fopen("etudiants.txt", "r");
    if (fichier_etudiants == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier des étudiants.\n");
        return;
    }

    int id_etudiant;
    char nom_utilisateur_fichier[MAX_LONGUEUR_NOM];
    bool trouve = false;
    while (fscanf(fichier_etudiants, "%s %d", nom_utilisateur_fichier, &id_etudiant) == 2)
    {
        if (strcmp(nom_utilisateur_fichier, nom_utilisateur) == 0)
        {
            trouve = true;
            break;
        }
    }
    fclose(fichier_etudiants);

    if (!trouve)
    {
        printf("Aucun étudiant associé à cet utilisateur.\n");
        return;
    }

    // Marquer la présence
    marquer_presence(id_etudiant);
}

int lire_classes(Classe classes[])
{
    FILE *fichier;
    int nombre_classes = 0;

    fichier = fopen("classes.txt", "r");
    if (fichier == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier des classes.\n");
        exit(1);
    }

    while (fscanf(fichier, "%s", classes[nombre_classes].nom_classe) == 1)
    {
        nombre_classes++;
    }

    fclose(fichier);
    return nombre_classes;
}

int lire_etudiants_par_classe(char nom_classe[], Etudiant etudiants[])
{
    char nom_fichier[100];
    sprintf(nom_fichier, "%s.txt", nom_classe); // Nom du fichier basé sur le nom de la classe

    FILE *fichier;
    int nombre_etudiants = 0;

    fichier = fopen(nom_fichier, "r");
    if (fichier == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier des étudiants de la classe %s.\n", nom_classe);
        exit(1);
    }

    while (fscanf(fichier, "%d %s %s", &etudiants[nombre_etudiants].id, etudiants[nombre_etudiants].prenom, etudiants[nombre_etudiants].nom) == 3)
    {
        etudiants[nombre_etudiants].est_present = false; // Initialiser à non présent
        nombre_etudiants++;
    }

    fclose(fichier);
    return nombre_etudiants;
}

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

int lire_presences(Presence presences[])
{
    FILE *fichier;
    int nombre_presences = 0;

    fichier = fopen("presences.txt", "r");
    if (fichier == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier des présences.\n");
        exit(1);
    }

    while (fscanf(fichier, "%d %s %s %ld", &presences[nombre_presences].id_etudiant, presences[nombre_presences].prenom, presences[nombre_presences].nom, &presences[nombre_presences].date) == 4)
    {
        nombre_presences++;
    }

    fclose(fichier);
    return nombre_presences;
}

void marquer_presence_admin()
{
    printf("Veuillez choisir une classe :\n");
    // Lire les classes disponibles
    Classe classes[MAX_CLASSES];
    int nombre_classes = lire_classes(classes);
    for (int i = 0; i < nombre_classes; i++)
    {
        printf("%d. %s\n", i + 1, classes[i].nom_classe);
    }

    int choix_classe;
    do
    {
        printf("Votre choix : ");
        scanf("%d", &choix_classe);
    } while (choix_classe < 1 || choix_classe > nombre_classes);

    char nom_classe[MAX_LONGUEUR_NOM];
    strcpy(nom_classe, classes[choix_classe - 1].nom_classe);

    printf("Classe choisie : %s\n", nom_classe);

    // Lire les étudiants de la classe choisie
    Etudiant etudiants[MAX_ETUDIANTS_PAR_CLASSE];
    int nombre_etudiants = lire_etudiants_par_classe(nom_classe, etudiants);
    printf("Liste des étudiants de la classe %s :\n", nom_classe);
    printf("ID  Prénom Nom\n");
    for (int i = 0; i < nombre_etudiants; i++)
    {
        printf("%d   %s %s\n", etudiants[i].id, etudiants[i].prenom, etudiants[i].nom);
    }

    // Marquer la présence
    int id_etudiant;
    do
    {
        printf("Veuillez entrer l'ID de l'étudiant pour marquer sa présence (0 pour quitter) : ");
        scanf("%d", &id_etudiant);

        if (id_etudiant == 0)
            return; // Quitter la fonction si l'administrateur choisit de quitter

        bool id_valide = false;
        for (int i = 0; i < nombre_etudiants; i++)
        {
            if (etudiants[i].id == id_etudiant)
            {
                marquer_presence(etudiants[i].id);
                id_valide = true;
                printf("Présence marquée pour l'étudiant ID %d (%s %s)\n", etudiants[i].id, etudiants[i].prenom, etudiants[i].nom);
                break;
            }
        }
        if (!id_valide)
            printf("ID étudiant invalide. Veuillez réessayer.\n");
    } while (1);
}

int comparer_dates(const void *a, const void *b)
{
    Presence *presence_a = (Presence *)a;
    Presence *presence_b = (Presence *)b;

    // Comparaison des dates
    if (presence_a->date < presence_b->date)
        return -1;
    else if (presence_a->date > presence_b->date)
        return 1;
    else
        return 0;
}

int lire_presences_date(Presence presences[], time_t date)
{
    FILE *fichier;
    int nombre_presences = 0;

    fichier = fopen("presences.txt", "r");
    if (fichier == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier des présences.\n");
        exit(1);
    }

    struct tm tm_date;
    localtime_r(&date, &tm_date); // Obtenir la structure de temps pour la date spécifiée

    while (fscanf(fichier, "%d %s %s %ld", &presences[nombre_presences].id_etudiant, presences[nombre_presences].prenom, presences[nombre_presences].nom, &presences[nombre_presences].date) == 4)
    {
        // Vérifier si la présence est pour la date spécifiée
        struct tm tm_presence;
        localtime_r(&presences[nombre_presences].date, &tm_presence);
        if (tm_presence.tm_year == tm_date.tm_year &&
            tm_presence.tm_mon == tm_date.tm_mon &&
            tm_presence.tm_mday == tm_date.tm_mday)
        {
            nombre_presences++;
        }
    }

    fclose(fichier);
    return nombre_presences;
}

void afficher_presences(Presence presences[], int nombre_presences, struct tm *tm_date, Etudiant etudiants[], int nombre_etudiants)
{
    system("clear");
    system("clear");
    printf("Liste des présences pour le %d/%d/%d :\n", tm_date->tm_mday, tm_date->tm_mon + 1, tm_date->tm_year + 1900);
    printf("ID  Prénom    Nom    Statut\n");
    for (int i = 0; i < nombre_etudiants; i++)
    {
        bool est_present = false;
        for (int j = 0; j < nombre_presences; j++)
        {
            if (presences[j].id_etudiant == etudiants[i].id)
            {
                est_present = true;
                break;
            }
        }
        printf("%d   %s %s    %s\n", etudiants[i].id, etudiants[i].prenom, etudiants[i].nom, (est_present ? "présent" : "absent"));
    }
}

void afficher_liste_presents()
{
    // Lire les classes disponibles
    Classe classes[MAX_CLASSES];
    int nombre_classes = lire_classes(classes);
    system("clear");
    printf("Classes disponibles :\n");
    for (int i = 0; i < nombre_classes; i++)
    {
        printf("%d. %s\n", i + 1, classes[i].nom_classe);
    }

    int choix_classe;
    printf("Veuillez choisir une classe (1-%d) : ", nombre_classes);
    scanf("%d", &choix_classe);
    choix_classe--; // Pour correspondre à l'index 0-based

    if (choix_classe < 0 || choix_classe >= nombre_classes)
    {
        printf("Choix invalide.\n");
        return;
    }

    // Récupérer le nom de la classe sélectionnée
    char nom_classe[MAX_LONGUEUR_NOM];
    strcpy(nom_classe, classes[choix_classe].nom_classe);

    // Lire la date actuelle
    time_t now;
    time(&now);
    struct tm *tm_now = localtime(&now);

    // Lire les étudiants de la classe
    Etudiant etudiants[MAX_ETUDIANTS_PAR_CLASSE];
    int nombre_etudiants = lire_etudiants_par_classe(nom_classe, etudiants);

    // Lire les présences pour la date actuelle
    Presence presences_today[MAX_PRESENCES];
    int nombre_presences_today = lire_presences_date(presences_today, now);

    // Trier les présences par date
    qsort(presences_today, nombre_presences_today, sizeof(Presence), comparer_dates);

    // Afficher les présences
    afficher_presences(presences_today, nombre_presences_today, tm_now, etudiants, nombre_etudiants);
}

void afficher_menu_admin()
{
    int choix;
    do
    {
        printf("\nMenu Administrateur:\n");
        printf("1. Gestion des Étudiants.\n");
        printf("2. Générer les fichiers.\n");
        printf("3. Marquer les Présences.\n");
        printf("4. Envoyer un message.\n");
        printf("5. Deconnexion.\n");
        printf("Votre choix :  ");
        scanf("%d", &choix);
        while (choix < 1 || choix > 5)
        {
            printf("Choix invalide. Veuillez entrer un choix valide : ");
            scanf("%d", &choix);
        }

        switch (choix)
        {
        case 2:
            int choix1;
            system("clear");
            printf("1. Liste des présences\n");
            printf("Votre choix:  ");
            scanf("%d", &choix1);
            if (choix1 == 1)
            {
                afficher_liste_presents();
            }
            break;

        case 3:
            system("clear");
            marquer_presence_admin();
            break;

        default:
            break;
        }
    } while (choix != 5);
}

void afficher_menu_etudiant(char *nom_utilisateur)
{
    int choix;
    do
    {
        printf("\nMenu Étudiant:\n");
        printf("1. Marquer ma présence\n");
        printf("2. Consulter le nombre de minutes d'absence\n");
        printf("3. Consulter mes messages\n");
        printf("4. Deconnexion\n");
        printf("Votre choix :  ");
        scanf("%d", &choix);
        while (choix < 1 || choix > 4)
        {
            printf("Choix invalide. Veuillez entrer un choix valide : ");
            scanf("%d", &choix);
        }

        switch (choix)
        {
        case 1:
            marquer_presence_etudiant(nom_utilisateur);
            break;
        case 2:
            // Implémentez la fonctionnalité pour consulter le nombre de minutes d'absence
            break;
        case 3:
            // Implémentez la fonctionnalité pour consulter les messages de l'étudiant
            break;
        case 4:
            printf("Merci et au revoir!\n");
            break;
        }
    } while (choix != 4);
}

void afficher_menu(Utilisateur utilisateur)
{
    if (strcmp(utilisateur.statut, "admin") == 0)
    {
        afficher_menu_admin();
    }
    else if (strcmp(utilisateur.statut, "etudiant") == 0)
    {
        afficher_menu_etudiant(utilisateur.nom); // Passer le nom d'utilisateur en argument
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

    // Lire les utilisateurs depuis le fichier
    nombre_utilisateurs = lire_utilisateurs(utilisateurs);

    char nom_utilisateur[MAX_LONGUEUR_NOM];
    char mot_de_passe[MAX_LONGUEUR_MDP];
    int indice_utilisateur = -1;

    // Demander l'authentification jusqu'à ce qu'elle soit réussie
    // Déclaration de la variable indice_utilisateur en dehors de la boucle

    // Utilisation d'une boucle while à la place de do-while pour éviter la redondance du code
while (indice_utilisateur == -1)
{
    printf("Nom d'utilisateur : ");
    fgets(nom_utilisateur, sizeof(nom_utilisateur), stdin);
    nom_utilisateur[strcspn(nom_utilisateur, "\n")] = 0; // Supprimer le caractère de retour à la ligne

    // Vérification de la validité du nom d'utilisateur
    if (strlen(nom_utilisateur) == 0 || strspn(nom_utilisateur, " ") == strlen(nom_utilisateur) || isspace((unsigned char)nom_utilisateur[0]))
    {
        printf("Le nom d'utilisateur ne peut pas être vide ou ne peut contenir que des espaces.\n");
        continue;
    }

    printf("Mot de passe : ");
    saisir_mot_de_passe(mot_de_passe);

    // Vérification de la validité du mot de passe
    if (strlen(mot_de_passe) == 0)
    {
        printf("Le mot de passe ne peut pas être vide.\n");
        continue;
    }

    indice_utilisateur = verifier_authentification(nom_utilisateur, mot_de_passe, utilisateurs, nombre_utilisateurs);

    if (indice_utilisateur == -1)
    {
        printf("Nom d'utilisateur ou mot de passe incorrect. Veuillez réessayer.\n");
    }
}
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
