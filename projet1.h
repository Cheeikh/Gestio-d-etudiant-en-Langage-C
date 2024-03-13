#ifndef projet1
#define projet1

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"

typedef struct
{
    int h, mn, s;
} Heure;

typedef struct
{
    int j, m, a;
} Date;

typedef struct
{
    int id;
    char libelle[50];
    Date dateDebut;
    Date dateFin;
} Promotion;

typedef struct
{
    int id;
    char libelle[50];
} Referenciel;

typedef struct
{
    int id;
    char login[20];
    char password[20];
    char nom[20];
    char prenom[50];
    Date date_naiss;
    int type;
} Utilisateur;

typedef struct
{
    int id;
    Utilisateur u;
} Admin;

typedef struct
{
    int id;
    char mat[50];
    Utilisateur u;
    Promotion promo;
    Referenciel ref;
    int statut;
} Apprenant;

typedef struct
{
    int id;
    Apprenant apprenant;
    Date datePresence;
    Heure heure;
} Presence;

typedef struct
{
    // int id;
    Apprenant apprenant;
    char Motifs[50];
    char texte[150];
    Date date_envoi;
    Heure heure_envoi;
    int statut;
} Message;

#define TOTAL_STUDENT 2

// prototype
void chargement();
int connexion();
extern int administrateurMenu();
int menuEtudiant(int nbrMessagesNonLu);
extern int marquerPresenceMenu();
void Menu(char msg[]);
void traitementAdmin(int *result, Utilisateur *u);
void traitementApprenant(int *result, Utilisateur *u);
Heure presenceHeure(int idApprenant, Presence presences[], int nbrPresence);
int genererFichierMenu();
int verifierPresence(int idApprenant, Presence presences[], int nbrPresence);
int verifierPresenceParDate(int idApprenant, Presence presences[], int nbrPresence, Date date);
int referencielMenu();
void afficherUtilisateur(Utilisateur *u);
int testMatricule(char matricule[], Apprenant apprenants[], int nbrApp);
int lireFichierUtilisateurs(Utilisateur utilisateurs[], char *fichier);
int lireFichierAdmin(Admin admins[], char *fichier);
int lireFichierAprennant(Apprenant apprenants[], char *fichier);
int lireFichierClasse(Referenciel referenciels[], char *fichier);
int lireFichierPromo(Promotion promos[], char *fichier);
int lireFichierPresence(Presence presences[], char *fichier);
int lireFichierPresenceDate(Date dates[], char *fichier);
int lireFichierMessage(Message message[], char *fichier);
void ajouterPresence(Presence nouvellePresence, char *fichier);
void marquerMessageLu(int idApp, int numero, char *fichier);
void genererFichierPresence(Presence presences[], int nbrPresence, Apprenant apprenants[], int nbrApp, Utilisateur users[], int nbrUser, Referenciel refs[], int nbrRef, int idRef, Date date, Date dates[], int nbrDate, char *fichier);
void genererFichierPresence2(Presence presences[], int nbrPresence, Apprenant apprenants[], int nbrApp, Utilisateur users[], int nbrUser, Referenciel refs[], int nbrRef, int idRef, Date date, char *fichier);
void envoiMessageAtous(char Motifs[50], char texte[150], char *fichier);
void envoiMessageParClasse(char Motifs[50], char texte[150], int idReferenciel, char *fichier);
void envoiMessageParSelection(Apprenant apprenants[], int nbrApprenant, char Motifs[50], char texte[150], char *fichier);
Date saisieDate(void);
void disableEcho();
void enableEcho();
int saisirInt(int min, int max, char msg[], char msgerr[]);
void saisiChaine(char chaine[], char msg[], char msgerr[]);
int login(char login[], char pass[], Utilisateur utilisateurs[], int taille, Utilisateur *u);
void saisiChainePassword(char chaine[], char msg[], char msgerr[]);
void obtenirDateAujourdhui(int *jour, int *mois, int *annee, int *h, int *mn, int *s);
Date obtenirDateAujourdhui2();
extern Heure obtenirHeure();
int testSiAnneeBissextile(int annee);
int testSiDateValide(int annee, int mois, int jour);
int calculNbrJourMois(int mois, int annee);
int saisirIntDate(int min, int max, char msg[], char msgerr[]);
int saisirDate(Date *date, char msg[], char msgerr[]);
extern Message excrireMessage();
void getClasseName(int idRef, char chaine[]);
int isValidMatricule(char matricule[]);











void chargement()
{
    printf(BLUE"\n\n                      Chargement"RESET);
    fflush(stdout);
    for (int i = 0; i < 3; i++)
    {
        printf(".");
        fflush(stdout);
        usleep(500000); // Pause de 500 millisecondes
    }
    printf("\n");
}

int testMatricule(char matricule[], Apprenant apprenants[], int nbrApp)
{
    // printf("%s", apprenants[0].mat);
    printf("%s", matricule);
    for (int i = 0; i < nbrApp; i++)
    {
        // printf("%d", strcmp(apprenants[i].mat, matricule));
        if (!strcmp(apprenants[i].mat, matricule))
        {
            return apprenants[i].id;
        }
    }
    return 0;
}

int lireFichierUtilisateurs(Utilisateur utilisateurs[], char *fichier)
{
    FILE *fp;
    char ligne[100];
    char login[50], motDePasse[50];
    int id;
    char mat[50];
    char nom[20];
    char prenom[50];
    int jour, mois, annee, type;
    int nbUtilisateurs = 0;

    fp = fopen(fichier, "r");
    if (fp == NULL)
    {
        printf("Erreur d'ouverture du fichier %s.\n", fichier);
        return -1;
    }

    while (fgets(ligne, sizeof(ligne), fp))
    {
        sscanf(ligne, "%d,%[^,],%[^,],%[^,],%[^,],%d,%d,%d,%d",
               &id, login, motDePasse, nom, prenom, &jour, &mois, &annee, &type);
        utilisateurs[nbUtilisateurs].id = id;
        // strcpy(utilisateurs[nbUtilisateurs].mat, mat);
        strcpy(utilisateurs[nbUtilisateurs].login, login);
        strcpy(utilisateurs[nbUtilisateurs].password, motDePasse);
        strcpy(utilisateurs[nbUtilisateurs].nom, nom);
        strcpy(utilisateurs[nbUtilisateurs].prenom, prenom);
        utilisateurs[nbUtilisateurs].date_naiss.j = jour;
        utilisateurs[nbUtilisateurs].date_naiss.m = mois;
        utilisateurs[nbUtilisateurs].date_naiss.a = annee;
        utilisateurs[nbUtilisateurs].type = type;
        nbUtilisateurs++;
    }

    // Fermeture du fichier
    fclose(fp);

    return nbUtilisateurs;
}

int lireFichierAdmin(Admin admins[], char *fichier)
{
    FILE *fp;
    char ligne[100];
    int idAdmin, idUser, nbrAdmin = 0;

    fp = fopen(fichier, "r");
    if (fp == NULL)
    {
        printf("Erreur d'ouverture du fichier %s.\n", fichier);
        return -1;
    }

    while (fgets(ligne, sizeof(ligne), fp))
    {
        sscanf(ligne, "%d,%d",
               &idAdmin, &idUser);
        admins[nbrAdmin].id = idAdmin;
        admins[nbrAdmin].u.id = idUser;
        nbrAdmin++;
    }

    // Fermeture du fichier
    fclose(fp);

    return nbrAdmin;
}

int lireFichierAprennant(Apprenant apprenants[], char *fichier)
{
    FILE *fp;
    char ligne[100];
    char mat[50];
    int idApprenant, idUser, idPromo, idRef, status, nbrApprenant = 0;

    fp = fopen(fichier, "r");
    if (fp == NULL)
    {
        printf("Erreur d'ouverture du fichier %s.\n", fichier);
        return -1;
    }

    while (fgets(ligne, sizeof(ligne), fp))
    {
        sscanf(ligne, "%d,%[^,],%d,%d,%d,%d",
               &idApprenant, mat, &idUser, &idPromo, &idRef, &status);
        apprenants[nbrApprenant].id = idApprenant;
        strcpy(apprenants[nbrApprenant].mat, mat);
        apprenants[nbrApprenant].u.id = idUser;
        apprenants[nbrApprenant].promo.id = idPromo;
        apprenants[nbrApprenant].ref.id = idRef;
        apprenants[nbrApprenant].statut = status;
        nbrApprenant++;
    }

    // Fermeture du fichier
    fclose(fp);

    return nbrApprenant;
}

int lireFichierClasse(Referenciel referenciels[], char *fichier)
{
    FILE *fp;
    char ligne[100];
    int id, nbrRef = 0;
    char libelle[50];

    fp = fopen(fichier, "r");
    if (fp == NULL)
    {
        printf("Erreur d'ouverture du fichier %s.\n", fichier);
        return -1;
    }

    while (fgets(ligne, sizeof(ligne), fp))
    {
        sscanf(ligne, "%d,%s",
               &id, libelle);
        referenciels[nbrRef].id = id;
        strcpy(referenciels[nbrRef].libelle, libelle);
        nbrRef++;
    }

    // Fermeture du fichier
    fclose(fp);

    return nbrRef;
}

int lireFichierPromo(Promotion promos[], char *fichier)
{
    FILE *fp;
    char ligne[100];
    int idPromo, j1, m1, a1, j2, m2, a2, nbrPromo = 0;
    char libelle[50];
    fp = fopen(fichier, "r");
    if (fp == NULL)
    {
        printf("Erreur d'ouverture du fichier %s.\n", fichier);
        return -1;
    }

    while (fgets(ligne, sizeof(ligne), fp))
    {
        sscanf(ligne, "%d,%s,%d,%d,%d,%d,%d,%d",
               &idPromo, libelle, &j1, &m1, &a1, &j2, &m2, &a2);

        promos[nbrPromo].id = idPromo;
        strcpy(promos[nbrPromo].libelle, libelle);
        promos[nbrPromo].dateDebut.j = j1;
        promos[nbrPromo].dateDebut.m = m1;
        promos[nbrPromo].dateDebut.a = a1;
        promos[nbrPromo].dateFin.j = j2;
        promos[nbrPromo].dateFin.m = m2;
        promos[nbrPromo].dateFin.a = a2;
        nbrPromo++;
    }

    // Fermeture du fichier
    fclose(fp);

    return nbrPromo;
}

int lireFichierPresence(Presence presences[], char *fichier)
{
    FILE *fp;
    char ligne[100];
    int idPresence, idApprenant, j, m, a, h, mn, s, nbrPresence = 0;

    fp = fopen(fichier, "r");
    if (fp == NULL)
    {
        printf("Erreur d'ouverture du fichier %s.\n", fichier);
        return -1;
    }

    while (fgets(ligne, sizeof(ligne), fp))
    {
        sscanf(ligne, "%d,%d,%d,%d,%d,%d,%d,%d",
               &idPresence, &idApprenant, &j, &m, &a, &h, &mn, &s);
        presences[nbrPresence].id = idPresence;
        presences[nbrPresence].apprenant.id = idApprenant;
        presences[nbrPresence].datePresence.j = j;
        presences[nbrPresence].datePresence.m = m;
        presences[nbrPresence].datePresence.a = a;
        presences[nbrPresence].heure.h = h;
        presences[nbrPresence].heure.mn = mn;
        presences[nbrPresence].heure.s = s;
        nbrPresence++;
    }

    // Fermeture du fichier
    fclose(fp);

    return nbrPresence;
}

int lireFichierPresenceDate(Date dates[], char *fichier)
{
    FILE *fp;
    char ligne[100];
    int idPresence, idApprenant, j, m, a, h, mn, s, nbrDate = 0;

    fp = fopen(fichier, "r");
    if (fp == NULL)
    {
        printf("Erreur d'ouverture du fichier %s.\n", fichier);
        return -1;
    }

    while (fgets(ligne, sizeof(ligne), fp))
    {
        sscanf(ligne, "%d,%d,%d,%d,%d,%d,%d,%d",
               &idPresence, &idApprenant, &j, &m, &a, &h, &mn, &s);

        // int tailletabDate = sizeof(dates)/sizeof(dates[0]);
        int cpt = 0;
        for (int i = 0; i < nbrDate; i++)
        {
            if (dates[i].a == a && dates[i].m == m && dates[i].j == j)
            {
                cpt = 1;
                break;
            }
        }
        if (cpt == 0)
        {
            dates[nbrDate].a = a;
            dates[nbrDate].m = m;
            dates[nbrDate].j = j;
            nbrDate++;
        }
    }

    // Fermeture du fichier
    fclose(fp);

    return nbrDate;
}

int lireFichierMessage(Message message[], char *fichier)
{
    FILE *fp;
    char ligne[100];
    int idApprenant, nbrMessage = 0, statut;
    char Motifs[50], texte[150];
    int j, m, a, h, mn, s;

    fp = fopen(fichier, "r");
    if (fp == NULL)
    {
        printf("Erreur d'ouverture du fichier %s.\n", fichier);
        return -1;
    }

    while (fgets(ligne, sizeof(ligne), fp))
    {
        sscanf(ligne, "%d,%[^,],%[^,],%d,%d,%d,%d,%d,%d,%d",
               &idApprenant, Motifs, texte, &j, &m, &a, &h, &mn, &s, &statut);

        message[nbrMessage].apprenant.id = idApprenant;
        message[nbrMessage].date_envoi.j = j;
        message[nbrMessage].date_envoi.m = m;
        message[nbrMessage].date_envoi.a = a;
        message[nbrMessage].heure_envoi.h = h;
        message[nbrMessage].heure_envoi.mn = mn;
        message[nbrMessage].heure_envoi.s = s;
        strcpy(message[nbrMessage].Motifs, Motifs);
        strcpy(message[nbrMessage].texte, texte);
        message[nbrMessage].statut = statut;
        nbrMessage++;
    }

    fclose(fp);

    return nbrMessage;
}

void marquerMessageLu(int idApp, int numero, char *fichier)
{
    FILE *file = fopen(fichier, "r+");
    if (file == NULL)
    {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    fseek(file, 0, SEEK_SET);

    char line[100];
    int currentRow = 0;
    while (fgets(line, sizeof(line), file) != NULL)
    {
        currentRow++;

        if (currentRow == numero)
        {
            char *token = strtok(line, ",");
            int currentColumn = 0;

            while (token != NULL)
            {
                currentColumn++;

                if (currentColumn == 10)
                {
                    fseek(file, -strlen(token), SEEK_CUR);
                    fputs("1", file);
                    break;
                }

                token = strtok(NULL, ",");
            }

            fseek(file, 0, SEEK_CUR);
        }
    }

    fclose(file);
}

void ajouterPresence(Presence nouvellePresence, char *fichier)
{
    FILE *fp;

    // Open the file in append mode
    fp = fopen(fichier, "a");
    if (fp == NULL)
    {
        printf("Erreur d'ouverture du fichier %s.\n", fichier);
        return;
    }

    // Write the new presence record to the file
    fprintf(fp, "%d,%d,%d,%d,%d,%d,%d,%d\n",
            nouvellePresence.id, nouvellePresence.apprenant.id,
            nouvellePresence.datePresence.j, nouvellePresence.datePresence.m,
            nouvellePresence.datePresence.a, nouvellePresence.heure.h,
            nouvellePresence.heure.mn, nouvellePresence.heure.s);

    // Close the file
    fclose(fp);
}

void genererFichierPresence(Presence presences[], int nbrPresence,
                            Apprenant apprenants[], int nbrApp, Utilisateur users[], int nbrUser, Referenciel refs[], int nbrRef, int idRef, Date date, Date dates[], int nbrDate, char *fichier)
{
    char ligne[500], cl;
    FILE *fp;

    fp = fopen(fichier, "w");
    if (fp == NULL)
    {
        printf("Erreur d'ouverture du fichier %s.\n", fichier);
        return;
    }

    for (int indate = 0; indate < nbrDate; indate++)
    {
        char dateChaine[20];
        sprintf(dateChaine, "Date: %d - %d - %d \n", dates[indate].j, dates[indate].m, dates[indate].a);

        char ref[50];

        fprintf(fp, "%s", dateChaine);

        fprintf(fp, "o=============o=================o===============o=================o==================o=========o\n");
        fprintf(fp, "| Matricule    | Prenom         | Nom           | Classe          | Date Naissance   | Statut  |\n");
        fprintf(fp, "o=============o=================o===============o=================o==================o=========o\n");

        for (int i = 0; i < nbrApp; i++)
        {
            for (int j = 0; j < nbrUser; j++)
            {

                if (apprenants[i].u.id == users[j].id)
                {
                    if (verifierPresenceParDate(apprenants[i].id, presences, nbrPresence, dates[indate]))
                    {
                        if (apprenants[i].ref.id == 1)
                        {
                            strcpy(ref, refs[0].libelle);
                        }

                        if (apprenants[i].ref.id == 2)
                        {
                            strcpy(ref, refs[1].libelle);
                        }

                        if (apprenants[i].ref.id == 3)
                        {
                            strcpy(ref, refs[2].libelle);
                        }

                        sprintf(ligne, "| %-12s | %-14s | %-13s | %-15s | %-3d-%-3d-%-3d     | %-6s |\n",
                                apprenants[i].mat,
                                users[j].prenom,
                                users[j].nom,
                                ref,
                                users[j].date_naiss.j,
                                users[j].date_naiss.m,
                                users[j].date_naiss.a,
                                "Présent");
                        fputs(ligne, fp);
                        fprintf(fp, "o=============o=================o===============o=================o==================o=========o\n");
                        // }
                    }
                }
            }
        }
    }
    fclose(fp);

    printf("\xE2\x9C\x85              Fichier %s généré avec succès.......\n", fichier);
}

void genererFichierPresence2(Presence presences[], int nbrPresence,
                             Apprenant apprenants[], int nbrApp, Utilisateur users[], int nbrUser, Referenciel refs[], int nbrRef, int idRef, Date date, char *fichier)
{
    char ligne[500], ref[50];
    FILE *fp;

    fp = fopen(fichier, "w");
    if (fp == NULL)
    {
        printf("Erreur d'ouverture du fichier %s.\n", fichier);
        return;
    }

    fprintf(fp, "+=============o=================o===============o=================o==================o=========o\n");
    fprintf(fp, "| Matricule    | Prenom         | Nom           | Classe          | Date Naissance   | Statut  |\n");
    fprintf(fp, "+=============o=================o===============o=================o==================o=========o\n");

    for (int i = 0; i < nbrApp; i++)
    {
        for (int j = 0; j < nbrUser; j++)
        {

            if (apprenants[i].u.id == users[j].id)
            {
                if (verifierPresenceParDate(apprenants[i].id, presences, nbrPresence, date))
                {

                    if (apprenants[i].ref.id == 1)
                    {
                        strcpy(ref, refs[0].libelle);
                    }

                    if (apprenants[i].ref.id == 2)
                    {
                        strcpy(ref, refs[1].libelle);
                    }

                    if (apprenants[i].ref.id == 3)
                    {
                        strcpy(ref, refs[2].libelle);
                    }

                    //  if (apprenants[i].ref.id == refs[idRef-1].id)
                    // {
                    sprintf(ligne, "| %-12s | %-14s | %-13s | %-15s | %-3d-%-3d-%-3d | %-6s |\n",
                            apprenants[i].mat,
                            users[j].prenom,
                            users[j].nom,
                            ref,
                            users[j].date_naiss.j,
                            users[j].date_naiss.m,
                            users[j].date_naiss.a,
                            "Présent");
                    fputs(ligne, fp);
                    fprintf(fp, "+=============o=================o===============o=================o==================o=======o\n");
                    // }
                }
            }
        }
    }

    fclose(fp);

    printf("\xE2\x9C\x85              Fichier %s généré avec succès.......\n", fichier);
}

extern void envoiMessageAtous(char Motifs[50], char texte[150], char *fichier)
{

    Apprenant apprenants[100];
    int nbrApprenant = lireFichierAprennant(apprenants, "etudiants.csv");
    int j, m, a, h, mn, s;

    obtenirDateAujourdhui(&j, &m, &a, &h, &mn, &s);

    FILE *fp;

    fp = fopen(fichier, "a");
    if (fp == NULL)
    {
        printf("Erreur d'ouverture du fichier %s.\n", fichier);
        return;
    }

    for (int i = 0; i < nbrApprenant; i++)
    {
        fprintf(fp, "%d,%s,%s,%d,%d,%d,%d,%d,%d,%d\n",
                apprenants[i].id,
                Motifs,
                texte,
                j,
                m,
                a,
                h,
                mn,
                s,
                0);
    }

    // Close the file
    fclose(fp);
}

extern void envoiMessageParClasse(char Motifs[50], char texte[150], int idReferenciel, char *fichier)
{
    Apprenant apprenants[100];
    int nbrApprenant = lireFichierAprennant(apprenants, "etudiants.csv");
    int j, m, a, h, mn, s;

    obtenirDateAujourdhui(&j, &m, &a, &h, &mn, &s);

    FILE *fp;

    fp = fopen(fichier, "a");
    if (fp == NULL)
    {
        printf("Erreur d'ouverture du fichier %s.\n", fichier);
        return;
    }

    for (int i = 0; i < nbrApprenant; i++)
    {
        if (apprenants[i].ref.id == idReferenciel)
        {
            fprintf(fp, "%d,%s,%s,%d,%d,%d,%d,%d,%d,%d\n",
                    apprenants[i].id,
                    Motifs,
                    texte,
                    j,
                    m,
                    a,
                    h,
                    mn,
                    s,
                    0);
        }
    }

    // Close the file
    fclose(fp);
}

extern void envoiMessageParSelection(Apprenant apps[], int nbrApp, char Motifs[50], char texte[150], char *fichier)
{
    Apprenant apprenants[100];
    int nbrApprenant = lireFichierAprennant(apprenants, "etudiants.csv");
    int j, m, a, h, mn, s;

    obtenirDateAujourdhui(&j, &m, &a, &h, &mn, &s);

    FILE *fp;

    fp = fopen(fichier, "a");
    if (fp == NULL)
    {
        printf("Erreur d'ouverture du fichier %s.\n", fichier);
        return;
    }

    for (int i = 0; i < nbrApp; i++)
    {
        for (int j = 0; j < nbrApprenant; j++)
        {
            printf("             %d", strcmp(apps[i].mat, apprenants[j].mat));
            if (!strcmp(apps[i].mat, apprenants[j].mat))
            {
                fprintf(fp, "%d,%s,%s,%d,%d,%d,%d,%d,%d,%d\n",
                        apprenants[j].id,
                        Motifs,
                        texte,
                        j,
                        m,
                        a,
                        h,
                        mn,
                        s,
                        0);
            }
        }
    }

    // Close the file
    fclose(fp);
}

void disableEcho()
{
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void enableEcho()
{
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

int testSiAnneeBissextile(int annee)
{

    if ((annee % 4 == 0 && annee % 100 != 0) || (annee % 400 == 0))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int calculNbrJourMois(int mois, int annee)
{
    int nbrJour;

    if (mois == 2)
    {
        if (testSiAnneeBissextile(annee))
        {
            nbrJour = 29;
        }
        else
        {
            nbrJour = 28;
        }
    }
    else if (mois == 4 || mois == 6 || mois == 9 || mois == 11)
    {
        nbrJour = 30;
    }
    else
    {
        nbrJour = 31;
    }

    return nbrJour;
}

int testSiDateValide(int annee, int mois, int jour)
{
    int validite;
    int nbrJours = calculNbrJourMois(mois, annee);
    // printf("%d", nbrJours);
    if (annee > 0)
    {
        if (0 < mois && mois < 13)
        {
            if (1 <= jour && jour <= nbrJours)
            {
                validite = 1;
            }
            else
            {
                validite = 0;
            }
        }
        else
        {
            validite = 0;
        }
    }
    else
    {
        validite = 0;
    }

    return validite;
}

Date saisieDate(void)
{
    Date d;
    d.j = 0;
    d.m = 0;
    d.a = 0;
    int j, m, a;
    puts("Entrez une date [j,m,a]");
    // scanf("%d%d%d", &d.j, &d.m, &d.a);

    j = saisirIntDate(1, 31, "entrez le jour", "entrer un jour valide");
    m = saisirIntDate(1, 12, "entrez le mois", "entrer un mois valide");
    a = saisirIntDate(1, 2024, "entrez l'année", "entrer une annee valide");

    if (testSiDateValide(a, m, j))
    {
        d.j = j;
        d.m = m;
        d.a = a;
        return d;
    }
    else
    {
        return d;
    }
}

int saisirInt(int min, int max, char msg[], char msgerr[])
{
    int x;
    do
    {
        puts(msg);
        scanf("%d", &x);
        if (x < min || x > max)
        {
            printf("             %s\n", msgerr);
        }

    } while (x < min || x > max);

    return x;
}

int saisirIntDate(int min, int max, char msg[], char msgerr[])
{
    int x;
    char input[100];
    do
    {
        printf("             %s: ", msg);
        scanf("             %s", input);

        // Tente de convertir la chaîne en un entier
        if (sscanf(input, "%d", &x) != 1 || x < min || x > max)
        {
            printf("             %s\n", msgerr);
        }

    } while (x < min || x > max);

    return x;
}

int saisirDate(Date *date, char msg[], char msgerr[])
{
    char input[20]; // ajustez la taille selon vos besoins
    time_t t = time(NULL);
    struct tm *today = localtime(&t);

    do
    {
        printf("             Entrez la date (format : jour/mois/annee) : ");
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            printf("Erreur de lecture.\n");
            return -1;
        }

        size_t length = strlen(input);
        if (length > 0 && input[length - 1] == '\n')
        {
            input[length - 1] = '\0';
        }

        if (sscanf(input, "%d/%d/%d", &date->j, &date->m, &date->a) != 3 || !testSiDateValide(date->a, date->m, date->j) || (date->j > today->tm_mday && date->m >= today->tm_mon + 1 && date->a >= today->tm_year + 1900))
        {
            printf("             Date invalide. Veuillez réessayer.\n");
        }
        // (date->j > today->tm_mday && date->m > today->tm_mon + 1 && date->a > today->tm_year + 1900)
    } while (!testSiDateValide(date->a, date->m, date->j) || (date->j > today->tm_mday && date->m >= today->tm_mon + 1 && date->a >= today->tm_year + 1900));

    return 1; // Succès
}

void saisiChaine(char chaine[], char msg[], char msgerr[])
{
    char caractere;
    int i = 0, j = 0, nbrEspace = 0;

    while (1)
    {
        printf("             %s ", msg);
        caractere = getchar();
        while (caractere != '\n')
        {
            if (caractere == ' ')
            {
                nbrEspace++;
            }
            else
                chaine[i] = caractere;
            i++;

            caractere = getchar();
        }
        if (i != 0)
        {
            if (nbrEspace != strlen(chaine))
            {
                break;
            }
        }

        if (nbrEspace > 0 || caractere == '\n')
        {
            printf("             %s", msgerr);
        }
    }
    chaine[i] = '\0';
    // printf("%s", chaine);
}

void saisiMessage(char chaine[], size_t size, char msg[], char msgerr[])
{
    printf("             %s", msg);

    fgets(chaine, size, stdin);

    size_t len = strlen(chaine);
    if (len > 0 && chaine[len - 1] == '\n')
    {
        chaine[len - 1] = '\0';
    }
    else
    {
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
            ;
    }

    if (chaine[0] == '\0' || strchr(chaine, ' ') == NULL)
    {
        printf("             %s", msgerr);
        saisiChaine(chaine, msg, msgerr);
    }
}

void saisiChainePassword(char chaine[], char msg[], char msgerr[])
{
    char caractere;
    struct termios oldt, newt;
    int i = 0, j = 0, nbrEspace = 0;

    // Turn off echoing

    // Read password character by character
    // printf("Enter password: ");
    // while ((caractere = getchar()) != '\n') {
    //     putchar('*');
    //     chaine[i] = caractere;
    // }

    // Restore terminal settings

    while (1)
    {
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL | ICANON);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);

        printf("             %s ", msg);
        caractere = getchar();
        putchar('*');
        while (caractere != '\n')
        {
            if (caractere == ' ')
            {
                nbrEspace++;
            }
            else
                chaine[i] = caractere;
            i++;

            caractere = getchar();
            putchar('*');
        }
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        if (i != 0)
        {
            if (nbrEspace != strlen(chaine))
            {
                break;
            }
        }

        if (nbrEspace > 0 || caractere == '\n')
        {
            printf("             %s", msgerr);
        }

        chaine[i] = '\0';
    }
}

int login(char login[], char pass[], Utilisateur utilisateurs[], int taille, Utilisateur *u)
{

    int type = 0;
    int cpt = 0;

    for (int i = 0; i < taille; i++)
    {
        if (!strcmp(utilisateurs[i].login, login) && !strcmp(utilisateurs[i].password, pass))
        {
            u->id = utilisateurs[i].id;
            // strcpy(u->mat, utilisateurs[i].mat);
            strcpy(u->login, login);
            strcpy(u->password, pass);
            strcpy(u->nom, utilisateurs[i].nom);
            strcpy(u->prenom, utilisateurs[i].prenom);
            u->date_naiss.j = utilisateurs[i].date_naiss.j;
            u->date_naiss.m = utilisateurs[i].date_naiss.m;
            u->date_naiss.a = utilisateurs[i].date_naiss.a;
            u->type = utilisateurs[i].type;
            return 1;
        }
    }
    return 0;
}

void obtenirDateAujourdhui(int *jour, int *mois, int *annee, int *h, int *mn, int *s)
{
    time_t t = time(NULL);
    struct tm *today = localtime(&t);

    *jour = today->tm_mday;
    *mois = today->tm_mon + 1;
    *annee = today->tm_year + 1900;
    *h = today->tm_hour;
    *mn = today->tm_min;
    *s = today->tm_sec;
}

Heure obtenirHeure()
{
    Heure h;
    time_t t = time(NULL);
    struct tm *today = localtime(&t);

    h.h = today->tm_hour;
    h.mn = today->tm_min;
    h.s = today->tm_sec;

    return h;
}

Date obtenirDateAujourdhui2()
{
    Date d;
    time_t t = time(NULL);
    struct tm *today = localtime(&t);

    d.j = today->tm_mday;
    d.m = today->tm_mon + 1;
    d.a = today->tm_year + 1900;

    return d;
}

Message excrireMessage()
{
    Message message;
    char Motifs[50];
    char texte[150];
    char cl;
    while ((cl = getchar()) != '\n' && cl != EOF)
        ;
    saisiMessage(Motifs, sizeof(Motifs), "Le Motifs: ", "Ce champ est Obligatoire\n");
    saisiMessage(texte, sizeof(texte), "Le texte: ", "Ce champ est Obligatoire\n");

    strcpy(message.Motifs, Motifs);
    strcpy(message.texte, texte);

    return message;
}

void getClasseName(int idRef, char chaine[])
{
    if (idRef == 1)
    {
        strcpy(chaine, "Dev-Web");
    }
    if (idRef == 2)
    {
        strcpy(chaine, "Ref-Digital");
    }
    if (idRef == 3)
    {
        strcpy(chaine, "Dev-Data");
    }
}

int isValidMatricule(char matricule[])
{

    size_t len = strlen(matricule);
    if (len != 9)
    {
        return 0;
    }

    if (!(isalpha(matricule[0]) && isalpha(matricule[1]) &&
          isalpha(matricule[2]) && isalpha(matricule[3]) &&
          isalpha(matricule[4]) && isdigit(matricule[5]) &&
          isdigit(matricule[6]) && isdigit(matricule[7]) &&
          isdigit(matricule[8])))
    {
        return 0;
    }

    return 1;
}

int verifierPresence(int idApprenant, Presence presences[], int nbrPresence)
{
    int j, m, a, h, mn, s;
    obtenirDateAujourdhui(&j, &m, &a, &h, &mn, &s);
    for (int i = 0; i < nbrPresence; i++)
    {
        if (presences[i].apprenant.id == idApprenant && j == presences[i].datePresence.j && m == presences[i].datePresence.m && presences[i].datePresence.a == a)
        {
            return 1;
        }
    }
    return 0;
}

int verifierPresenceParDate(int idApprenant, Presence presences[], int nbrPresence, Date date)
{

    for (int i = 0; i < nbrPresence; i++)
    {
        if (presences[i].apprenant.id == idApprenant && date.j == presences[i].datePresence.j && date.m == presences[i].datePresence.m && presences[i].datePresence.a == date.a)
        {
            return 1;
        }
    }
    return 0;
}

Heure presenceHeure(int idApprenant, Presence presences[], int nbrPresence)
{
    Heure heu;
    int j, m, a, h, mn, s;
    obtenirDateAujourdhui(&j, &m, &a, &h, &mn, &s);

    for (int i = 0; i < nbrPresence; i++)
    {
        if (presences[i].apprenant.id == idApprenant && j == presences[i].datePresence.j && m == presences[i].datePresence.m && presences[i].datePresence.a == a)
        {
            heu.h = presences[i].heure.h;
            heu.mn = presences[i].heure.mn;
            heu.s = presences[i].heure.s;
            break;
        }
    }

    return heu;
}

int administrateurMenu()
{
    int choixadministrateurMenu;

#ifdef _WIN32
    system("cls"); // For Windows
#else
    system("clear"); // For Unix-like systems
#endif
    // getchar();
    do
    {
        Menu("Menu Admin");
        printf("             1 - GESTION DES ÉTUDIANTS\n");
        printf("             2 - GÉNÉRATION DE FICHIERS\n");
        printf("             3 - MARQUER LES PRÉSENCES \n");
        printf("             4 - ENVOYER UN MESSAGE \n");
        printf("             5 - Deconnection \n");

        printf("\n\n             Votre choix:   ");
        scanf("%d", &choixadministrateurMenu);

        if (choixadministrateurMenu < 1 || choixadministrateurMenu > 4)
        {
            printf("             Au revoir !!!   ");
        }

    } while (choixadministrateurMenu < 1 || choixadministrateurMenu > 5);

    return choixadministrateurMenu;
}

int menuEtudiant(int nbrMessagesNonLu)
{
    int choixmenuEtudiant;
#ifdef _WIN32
    system("cls"); // For Windows
#else
    system("clear"); // For Unix-like systems
#endif

    do
    {
        Menu("Menu Apprenant");
        printf("             1 - MARQUER MA PRÉSENCE\n");
        printf("             2 - NOMBRE DE MINUTES D’ABSENCE\n");
        printf("             3 - MES MESSAGES (%d) \n", nbrMessagesNonLu);
        printf("             4 - Déconnexion \n");

        printf("\n             Votre choix: ");
        scanf("%d", &choixmenuEtudiant);

        if (choixmenuEtudiant < 1 || choixmenuEtudiant > 4)
        {
            printf("             Votre choix entre 1, 2, 3 et 4\n");
        }

    } while (choixmenuEtudiant < 1 || choixmenuEtudiant > 4);

    return choixmenuEtudiant;
}

int genererFichierMenu()
{
    int choixMenuGF;
#ifdef _WIN32
    system("cls"); // For Windows
#else
    system("clear"); // For Unix-like systems
#endif

    do
    {
        Menu("Generer Fichier Presence");
        printf("             1 - Toutes les dates\n");
        printf("             2 - À une date\n");
        printf("             3 - Quitter \n");

        printf("\n             Votre choix: ");
        scanf("%d", &choixMenuGF);

        if (choixMenuGF < 1 || choixMenuGF > 3)
        {
            printf("Votre choix entre 1, 2 et 3\n");
        }

    } while (choixMenuGF < 1 || choixMenuGF > 3);

    return choixMenuGF;
}

int marquerPresenceMenu()
{
    int choixMarquerPresence;
    Referenciel referenciels[100];
    int nbrRef = lireFichierClasse(referenciels, "classes.csv");
#ifdef _WIN32
    system("cls"); // For Windows
#else
    system("clear"); // For Unix-like systems
#endif

    do
    {
        for (int i = 0; i < nbrRef; i++)
        {
            printf("%d - %s\n", i + 1, referenciels[i].libelle);
        }

        printf("             4 - Quitter \n");

        printf("\n             Votre choix: ");
        scanf("%d", &choixMarquerPresence);

        if (choixMarquerPresence < 1 || choixMarquerPresence > 4)
        {
            printf("             Votre choix entre 1, 2, 3 et 4\n");
        }

    } while (choixMarquerPresence < 1 || choixMarquerPresence > 4);

    return choixMarquerPresence;
}

int referencielMenu()
{
    int choixReferenciel;
    Referenciel referenciels[100];
    int nbrRef = lireFichierClasse(referenciels, "classes.csv");
#ifdef _WIN32
    system("cls"); // For Windows
#else
    system("clear"); // For Unix-like systems
#endif

    do
    {
        Menu("Référentiel");
        for (int i = 0; i < nbrRef; i++)
        {
            printf("             %d - %s\n", i + 1, referenciels[i].libelle);
        }

        // printf("2 - Ref Dig\n");
        // printf("3 - Dev Data\n");
        printf("             4 - Quitter \n");

        printf("\n             Votre choix: ");
        scanf("%d", &choixReferenciel);

        if (choixReferenciel < 1 || choixReferenciel > 4)
        {
            printf("             Votre choix entre 1, 2, 3 et 4\n");
        }

    } while (choixReferenciel < 1 || choixReferenciel > 4);

    return choixReferenciel;
}

int menuListeApprenant()
{
    int choixListeApprenant;
    Utilisateur utilisateurs[100];
    Apprenant apprenants[100];
    int nbrUser = lireFichierUtilisateurs(utilisateurs, "utilisateurs.csv");
    int nbrApprenant = lireFichierAprennant(apprenants, "etudiants.csv");
    char ref[20];
#ifdef _WIN32
    system("cls"); // For Windows
#else
    system("clear"); // For Unix-like systems
#endif

    Menu("Liste des Apprenants");
    for (int i = 0; i < nbrApprenant; i++)
    {

        for (int j = 0; j < nbrUser; j++)
        {
            if (apprenants[i].u.id == utilisateurs[j].id)
            {
                getClasseName(apprenants[i].ref.id, ref);
                printf("             %d - %s %s %s %s\n", i + 1,
                       apprenants[i].mat, utilisateurs[j].prenom,
                       utilisateurs[j].nom, ref);
            }
        }
    }
}

int menuSendMessage()
{
    int choixSendMessage;

#ifdef _WIN32
    system("cls"); // For Windows
#else
    system("clear"); // For Unix-like systems
#endif
    // getchar();
    do
    {
        Menu("Envoyer Message");
        printf("             1 - À tous\n");
        printf("             2 - À une classe\n");
        printf("             3 - Par sélectionr\n");
        printf("             4 - Quitter \n");

        printf("\n             Votre choix: ");
        scanf("%d", &choixSendMessage);

        if (choixSendMessage < 1 || choixSendMessage > 4)
        {
            printf("Votre choix entre 1, 2, 3 et 4\n");
        }

    } while (choixSendMessage < 1 || choixSendMessage > 4);

    return choixSendMessage;
}

void Menu(char msg[])
{
    printf("\n             ===============================================\n");
    printf("                             %s            \n", msg);
    printf("             ===============================================\n");
}

void traitementAdmin(int *result, Utilisateur *user)
{

    Utilisateur utilisateurs[100];
    Apprenant apprenants[100];
    Presence presences[100];
    Referenciel referenciels[100], ref;
    Date dates[100];
    int choixadministrateurMenu, choixMarquerPresence, choixApprenant, choixMenuGF, idApprenant, choixReferenciel, ChoiMenuSendMessage;
    int nbrUser = lireFichierUtilisateurs(utilisateurs, "utilisateurs.csv");
    int nbrApprenant = lireFichierAprennant(apprenants, "etudiants.csv");
    int nbrRef = lireFichierClasse(referenciels, "classes.csv");
    int nbrPresence = lireFichierPresence(presences, "presence.csv");
    int nbrDate = lireFichierPresenceDate(dates, "presence.csv");
    char matricule[50], passwordVerify[6], cl;
    int j, m, a, h, mn, s;
    obtenirDateAujourdhui(&j, &m, &a, &h, &mn, &s);

    do
    {
        choixadministrateurMenu = administrateurMenu();
        if (choixadministrateurMenu == 1)
        {
        }

        if (choixadministrateurMenu == 2)
        {

            do
            {

                choixMenuGF = genererFichierMenu();

                if (choixMenuGF == 1)
                {
                    Date d = obtenirDateAujourdhui2();
                    Heure h = obtenirHeure();
                    char dateHeureStr[50];
                    char nomFichier[150];

                    sprintf(dateHeureStr, "%04d%02d%02d_%02d%02d%02d",
                            d.a, d.m, d.j, h.h, h.mn, h.s);

                    sprintf(nomFichier, "fichier_presence_all_%d.csv", choixReferenciel);

                    genererFichierPresence(presences, nbrPresence,
                                           apprenants, nbrApprenant, utilisateurs, nbrUser,
                                           referenciels, nbrRef, choixReferenciel, d, dates, nbrDate, nomFichier);
                    // pause();
                    sleep(2);
                    continue;
                }

                if (choixMenuGF == 2)
                {
                    Date d;
                    while ((cl = getchar()) != '\n' && cl != EOF)
                        ;
                    saisirDate(&d, "Saisir la date", "La saisie n'est pas valide");

                    if (d.j != 0 && d.m != 0 && d.a != 0)
                    {
                        // Date d = obtenirDateAujourdhui2();
                        Heure h = obtenirHeure();
                        char dateHeureStr[50];
                        char nomFichier[150];

                        sprintf(dateHeureStr, "%04d%02d%02d_%02d%02d%02d",
                                d.a, d.m, d.j, h.h, h.mn, h.s);

                        sprintf(nomFichier, "fichier_presence_%04d/%02d/%02d_.csv", d.a, d.m, d.j );

                        genererFichierPresence2(presences, nbrPresence,
                                                apprenants, nbrApprenant, utilisateurs, nbrUser,
                                                referenciels, nbrRef, choixReferenciel, d, nomFichier);

                        sleep(2);
                        continue;
                    }
                    else
                    {
                        printf("             Cette date n'est pas valide\n");
                        // fflush(stdout);
                        while ((cl = getchar()) != '\n' && cl != EOF)
                            ;
                        // getchar();
                    }

                    // fflush(stdout);
                    // while ((cl = getchar()) != '\n' && cl != EOF);
                    // getchar();
                    // pause();
                }

            } while (choixMenuGF != 3);
        }

        if (choixadministrateurMenu == 3)
        {
            do
            {
                // getchar();
                while ((cl = getchar()) != '\n' && cl != EOF)
                    ;
                saisiChaine(matricule, "Donner le matricule (Q: pour quitter):", "");

                if (matricule[0] == 'q' || matricule[0] == 'Q')
                {
                    saisiChainePassword(passwordVerify, "Mot de passe :", "\n \xE2\x9D\x8CMot de passe Obligatoire\n");
                    // printf("%s", passwordVerify);
                    // printf("%d", strcmp("12345", passwordVerify));
                    if (!strcmp("12345", passwordVerify))
                    {
                        break;
                    }
                    else
                    {
                        printf("\n \xE2\x9D\x8CMot de passe invalide!!\n");
                    }
                    // while ((cl = getchar()) != '\n' && cl != EOF);
                }

                if (matricule[0] != 'q' && matricule[0] != 'Q')
                {

                    idApprenant = testMatricule(matricule, apprenants, nbrApprenant);
                    printf("%d", idApprenant);
                    if (idApprenant)
                    {
                        if (!verifierPresence(idApprenant, presences, nbrPresence))
                        {
                            Presence presence;
                            presence.apprenant.id = idApprenant;
                            presence.datePresence.a = a;
                            presence.datePresence.m = m;
                            presence.datePresence.j = j;
                            presence.heure.h = h;
                            presence.heure.mn = mn;
                            presence.heure.s = s;
                            presence.id = presences[nbrPresence - 1].id + 1;
                            ajouterPresence(presence, "presence.csv");
                            nbrPresence = lireFichierPresence(presences, "presence.csv");
                            // printf("%d", nbrPresence);
                            printf("             \xE2\x9C\x85 Vous êtes maintenant présent\n");
                        }
                        else
                        {
                            Heure heure_p = presenceHeure(idApprenant, presences, nbrPresence);
                            printf("             \xE2\x9C\x85  Cette apprenant est déjà présent\n %d:%d:%d", heure_p.h, heure_p.mn, heure_p.s);
                        }
                    }
                    else
                    {
                        printf("             \xE2\x9D\x8C Ce matricule n'est pas valide\n");
                    }
                }

            } while (1);
        }

        if (choixadministrateurMenu == 4)
        {
            do
            {
                ChoiMenuSendMessage = menuSendMessage();
                if (ChoiMenuSendMessage == 1)
                {
                    Message message = excrireMessage();

                    envoiMessageAtous(message.Motifs, message.texte, "messages.csv");
                    printf("             \xE2\x9C\x85 Message envoyé avec succès\n");
                    while ((cl = getchar()) != '\n' && cl != EOF)
                        ;
                }

                if (ChoiMenuSendMessage == 2)
                {
                    do
                    {
                        choixReferenciel = referencielMenu();
                        if (choixReferenciel == 1 || choixReferenciel == 2 || choixReferenciel == 3)
                        {
                            Message message = excrireMessage();

                            envoiMessageParClasse(message.Motifs, message.texte, choixReferenciel, "messages.csv");
                            printf("             \xE2\x9C\x85 Message envoyé avec succès.......\n");
                            while ((cl = getchar()) != '\n' && cl != EOF)
                                ;
                        }

                    } while (choixReferenciel != 4);
                }

                if (ChoiMenuSendMessage == 3)
                {
                    while ((cl = getchar()) != '\n' && cl != EOF)
                        ;
                    char input[100];
                    Apprenant apps[100];
                    int k = 0;
                    int invalidMat;

                    do
                    {
                        invalidMat = 0;
                        menuListeApprenant();
                        printf("             Donner les matricules des apprenant à envoyer des messages\n");
                        printf("             Au format [MATXX0000,MATXX0000] / (Q ou q pour quitter):\n");
                        fgets(input, sizeof(input), stdin);

                        if (input[0] == 'q' || input[0] == 'Q')
                        {
                            break;
                        }

                        char *token = strtok(input, ",");
                        while (token != NULL)
                        {

                            char trimmedMatricule[9];
                            sscanf(token, " %9s", trimmedMatricule);

                            if (isValidMatricule(trimmedMatricule))
                            {
                                strcpy(apps[k++].mat, trimmedMatricule);
                            }
                            else
                            {
                                printf("             Ce matricule n'est pas valide: %s\n", trimmedMatricule);
                                while ((cl = getchar()) != '\n' && cl != EOF)
                                    ;
                                invalidMat = 1;
                            }

                            token = strtok(NULL, ",");
                        }

                        if (invalidMat == 0)
                        {
                            Message message = excrireMessage();
                            envoiMessageParSelection(apps, k, message.Motifs, message.texte, "messages.csv");

                            printf("             \xE2\x9C\x85 Message envoyé avec succès.......\n");
                            while ((cl = getchar()) != '\n' && cl != EOF)
                                ;
                        }

                    } while (input[0] != 'q' || input[0] != 'Q');
                }

            } while (ChoiMenuSendMessage != 4);
        }
    } while (choixadministrateurMenu != 5);
#ifdef _WIN32
    system("cls"); // For Windows
#else
    system("clear"); // For Unix-like systems
#endif
    if (choixadministrateurMenu == 5)
    {
        *result = 0;
    }
}

void traitementApprenant(int *result, Utilisateur *user)
{
    Utilisateur utilisateurs[100];
    Apprenant apprenants[100];
    Presence presences[100];
    Referenciel referenciels[100], ref;
    Message messages[100];
    int choixmenuEtudiant, choixMarquerPresence, choixApprenant, choixMessage;
    int nbrUser = lireFichierUtilisateurs(utilisateurs, "utilisateurs.csv");
    int nbrApprenant = lireFichierAprennant(apprenants, "etudiants.csv");
    int nbrRef = lireFichierClasse(referenciels, "classes.csv");
    int nbrPresence = lireFichierPresence(presences, "presence.csv");
    int nbrMessage = lireFichierMessage(messages, "messages.csv");
    int j, m, a, h, mn, s, nbrMessageNonLu = 0;
    Apprenant app;
    char cl;
    int positionM[100], valPosition = 0, indiceP = 0;
    obtenirDateAujourdhui(&j, &m, &a, &h, &mn, &s);

    for (int i = 0; i < nbrApprenant; i++)
    {
        if (user->id == apprenants[i].u.id)
        {
            app.id = apprenants[i].id;
            app.promo.id = apprenants[i].promo.id;
            app.ref.id = apprenants[i].ref.id;
            app.statut = apprenants[i].statut;
            app.u.id = apprenants[i].u.id;
            break;
        }
    }

    for (int j = 0; j < nbrMessage; j++)
    {
        if (app.id == messages[j].apprenant.id && messages[j].statut == 0)
        {
            nbrMessageNonLu++;
        }
    }

    // printf("%d", app.id);

    do
    {
        choixmenuEtudiant = menuEtudiant(nbrMessageNonLu);
        if (choixmenuEtudiant == 1)
        {

            if (verifierPresence(app.id, presences, nbrPresence))
            {
                printf("\xE2\x9C\x85              Vous êtes Déjà présent !!!! ");
                // while ((cl = getchar()) != '\n' && cl != EOF);
                getchar();
                // pause();
            }
            else
            {
                Presence presence;
                presence.apprenant.id = app.id;
                presence.datePresence.a = a;
                presence.datePresence.m = m;
                presence.datePresence.j = j;
                presence.heure.h = h;
                presence.heure.mn = mn;
                presence.heure.s = s;
                presence.id = presences[nbrPresence - 1].id + 1;
                ajouterPresence(presence, "presence.csv");
                nbrPresence = lireFichierPresence(presences, "presence.csv");
                // printf("%d", nbrPresence);
                printf("\xE2\x9C\x85              Vous êtes maintenant présent\n");
            }
        }

        if (choixmenuEtudiant == 2)
        {
        }

        if (choixmenuEtudiant == 3)
        {
            int numeroMessages = 0;
            char choixQuitterMess;
            do
            {

                for (int i = nbrMessage - 1; i >= 0; i--)
                {
                    // printf("%s", messages[i].su jet);
                    valPosition++;
                    if (app.id == messages[i].apprenant.id)
                    {
                        // messages[i].statut = 1;
                        positionM[indiceP++] = valPosition;
                        printf("             %d -) %s Le %d/%d/%d à %d:%d:%d  %s\n", ++numeroMessages, messages[i].Motifs, messages[i].date_envoi.j,
                               messages[i].date_envoi.m, messages[i].date_envoi.a, messages[i].heure_envoi.h,
                               messages[i].heure_envoi.mn, messages[i].heure_envoi.s,
                               messages[i].statut == 1 ? "\xE2\x9C\x85" : "\xE2\x9D\x8C");
                    }
                }

                valPosition = 0;
                indiceP = 0;
                if (numeroMessages == 0)
                {
                    printf("             Vous n'avez pas de messages\n");
                }
                else
                {
                    int lire;
                    printf("             choisir le messages à lire:");
                    scanf("%d", &lire);

                    if (lire <= 0 || lire > numeroMessages)
                    {
                        printf("             Choix invalide\n");
                        while ((cl = getchar()) != '\n' && cl != EOF)
                            ;
                    }
                    else
                    {

                        marquerMessageLu(app.id, nbrMessage - positionM[lire - 1] + 1, "messages.csv");
                        lire--;
                        for (int i = nbrMessage - 1; i >= 0; i--)
                        {
                            if (app.id == messages[i].apprenant.id)
                            {

                                if (lire == 0)
                                {
                                    // if (messages[i].statut == 0)
                                    // {

                                    // }

                                    printf("             Motifs: %s \n", messages[i].Motifs);
                                    printf("             Texte: %s \n", messages[i].texte);
                                    break;
                                }
                                lire--;
                            }
                        }
                        numeroMessages = 0, lire = 0;
                        while ((cl = getchar()) != '\n' && cl != EOF)
                            ;
                        getchar();
                    }
                    nbrMessage = lireFichierMessage(messages, "messages.csv");
                    nbrMessageNonLu = 0;
                    for (int j = 0; j < nbrMessage; j++)
                    {
                        if (app.id == messages[j].apprenant.id && messages[j].statut == 0)
                        {
                            nbrMessageNonLu++;
                        }
                    }
                }
                printf("             voulez-vous quitter ? O(o) ou N(n) : ");
                choixQuitterMess = getchar();

                if (choixQuitterMess == 'O' || choixQuitterMess == 'o')
                {
                    break;
                }

            } while (choixQuitterMess != 'O' || choixQuitterMess != 'o');
        }
    } while (choixmenuEtudiant != 4);
#ifdef _WIN32
    system("cls"); // For Windows
#else
    system("clear"); // For Unix-like systems
#endif
    if (choixmenuEtudiant == 4)
    {
        *result = 0;
    }
}

int connexion()
#endif