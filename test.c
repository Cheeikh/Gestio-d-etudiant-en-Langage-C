#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LONGUEUR_NOM 50
#define MAX_CLASSES 3
#define MAX_ETUDIANTS_PAR_CLASSE 5

typedef struct
{
    char prenom[MAX_LONGUEUR_NOM];
    char nom[MAX_LONGUEUR_NOM];
    int id;
    bool est_present;
} Etudiant;

typedef struct
{
    char nom_classe[MAX_LONGUEUR_NOM];
} Classe;

int lire_classes(Classe classes[])
{
    // Simuler la lecture des classes depuis un fichier
    strcpy(classes[0].nom_classe, "DevWeb");
    strcpy(classes[1].nom_classe, "RefDig");
    strcpy(classes[2].nom_classe, "DevData");
    return MAX_CLASSES;
}

int lire_etudiants_par_classe(char nom_classe[], Etudiant etudiants[])
{
    // Simuler la lecture des étudiants depuis un fichier
    int nombre_etudiants = MAX_ETUDIANTS_PAR_CLASSE;
    for (int i = 0; i < nombre_etudiants; i++)
    {
        sprintf(etudiants[i].prenom, "Prénom%d", i+1);
        sprintf(etudiants[i].nom, "Nom%d", i+1);
        etudiants[i].id = i + 1;
        etudiants[i].est_present = false;
    }
    return nombre_etudiants;
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
                etudiants[i].est_present = true;
                id_valide = true;
                printf("Présence marquée pour l'étudiant ID %d (%s %s)\n", etudiants[i].id, etudiants[i].prenom, etudiants[i].nom);
                break;
            }
        }
        if (!id_valide)
            printf("ID étudiant invalide. Veuillez réessayer.\n");
    } while (1);
}

int main()
{
    marquer_presence_admin();
    return 0;
}
