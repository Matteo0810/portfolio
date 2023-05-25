/** 
*\author Mattéo Gaillard
*\version 1.0
*\date 30 octobre 2021
*
* Ce programme permet de jouer au puissance 4 sur un terminal.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
*\def NBLIG
*\brief nombre maximal de lignes d'une grille
*/
#define NBLIG 6

/**
*\def NBCOL
*\brief nombre maximal de colonnes d'une grille
*/
#define NBCOL 7

/**
*\def COLONNE_DEBUT
*\brief colonne du début
*
* elle va permettre de savoir où positionner le pion au dessus de la grille
* au début du choix de la colonne
*/
#define COLONNE_DEBUT NBCOL/2

/**
*\def COMMANDE_CLEAR
*\brief la commande pour effacer le terminal
*
* cela va permettre de savoir quel commande utiliser en fonction
*      de l'environnement de l'utilisateur:
*  - si il est sur windows (_WIN32) alors la commande est cls
*  - sinon on supposera que la commande est clear
*/
#ifdef _WIN32
    #define COMMANDE_CLEAR "cls"
#else
    #define COMMANDE_CLEAR "clear"
#endif

/**
*\def NB_PIONS_A_ALIGNER
*\brief nombre de pion à aligner (par défaut 4)
*/
const int NB_PIONS_A_ALIGNER = 4;

/**
*\def PION_A
*\brief le pion du joueur A
*/
const char PION_A = 'X';

/**
*\def PION_B
*\brief le pion du joueur B
*/
const char PION_B = 'O';

/**
*\def VIDE
*\brief représente une case vide
*/
const char VIDE = ' ';

/**
*\def VIDE
*\brief pion désignant un match nul
*/
const char INCONNU = ' ';

/**
*\def VIDE
*\brief caractère permettant de faire tomber le pion
*/
const char CODE_TOMBER_PION = ' ';

/**
*\def VIDE
*\brief caractère permettant de se déplacer vers la droite
*/
const char CODE_COLONNE_DROITE = 'd';

/**
*\def VIDE
*\brief caractère permettant de se déplacer vers la gauche
*/
const char CODE_COLONNE_GAUCHE = 'q';

/**
*\def VIDE
*\brief code à retourner si une colonne est pleine
*/
const int CODE_COLONNE_PLEINE = -1;

/**
*
*\typedef grille
*\brief type à deux dimensions de NBLIG et NBCOL caractères
*
*tableau à deux dimensions de caractères
* utilisé pour définir une grille
*
*/
typedef char grille[NBLIG][NBCOL];

void initGrille(grille g);
void afficher(grille g, char pion, int colonne);
void jouer(grille g, char pion, int *ligne, int *colonne);
void finDePartie(char pion);

void usageTouches();
void tracerTiret();
void afficherSelecteur(char pion, int colonne);

int choisirColonne(grille g, char pion, int colonne);
int trouverLigne(grille g, int colonne);

bool grillePleine(grille g);
bool estVainqueur(grille g, int ligne, int colonne);

bool verifierVerticalement(grille, int, int);
bool verifierHorizontalement(grille, int, int);

bool verifierEnDiagonaleDroite(grille, int, int);
bool verifierEnDiagonaleGauche(grille, int, int);

/**
*\fn int main()
*\brief Programme principal
*\return Code de sortie du programme (0 : sortie normale).
*
*Le programme principal constitue la boucle principale du jeu puissance 4:
*il fait jouer tour à tour les joueurs pour finir par afficher le résultat de la partie
*/
int main() {
    char vainqueur;
    int ligne, colonne;
    grille g;

    initGrille(g);
    vainqueur = INCONNU;
    afficher(g, PION_A, COLONNE_DEBUT);

    while(vainqueur == INCONNU && !grillePleine(g)) {
        jouer(g, PION_A, &ligne, &colonne);
        afficher(g, PION_B, COLONNE_DEBUT);
        if(estVainqueur(g, ligne, colonne))
            vainqueur = PION_A;
        else if(!grillePleine(g)) {
            jouer(g, PION_B, &ligne, &colonne);
            afficher(g, PION_A, COLONNE_DEBUT);
            if(estVainqueur(g, ligne, colonne))
                vainqueur = PION_B;
        }
    }

    finDePartie(vainqueur);
    return EXIT_SUCCESS;
}

/**
*\fn void initGrille(grille g).
*\brief Procédure qui initialise la grille.
*\param g: paramètre d'entrée/sortie qui représente la grille du jeu
*
* affecte une VIDE à chaque element du tableau
*/
void initGrille(grille g) {
    int i, j;
    for(i = 0;i < NBLIG;i++) {
        for(j = 0;j < NBCOL;j++) {
            g[i][j] = VIDE;
        }
    }
}

/**
*\fn void afficher(grille g, char pion, int colonne).
*\brief Procédure qui affiche la grille
*
*\param g: paramètre d'entrée qui représente la grille du jeu
*\param pion: paramètre d'entrée qui représente le pion à afficher au dessus de la grille
*\param colonne: paramètre d'entrée qui représente la colonne où mettre le pion au dessus de la grille
*/
void afficher(grille g, char pion, int colonne) {
    system(COMMANDE_CLEAR);
    int i, j;

    afficherSelecteur(pion, colonne);
    tracerTiret();
    for(i = 0;i < NBLIG;i++) {
        for(j = 0;j < NBCOL;j++) {
            printf("| %c ", g[i][j]);
        }
        printf("|\n");
    }
    tracerTiret();
}

/**
*\fn void afficherSelecteur(char pion, int colonne).
*\brief Procédure qui affiche le pion au dessus de la grille
*
*\param pion: paramètre d'entrée qui représente le pion à afficher au dessus de la grille
*\param colonne: paramètre d'entrée qui représente la colonne où mettre le pion au dessus de la grille
*/
void afficherSelecteur(char pion, int colonne) {
    int i;
    for(i = 0;i < NBCOL;i++) {
        /*
            ce printf gère les cas particuliers quand
            je suis à la première colonne ou à la dernière colonne
            ou dans n'importe quelle autre colonne.

	    (ce cas particulier a été ajouté et n'est pas présent sur la maquette mais
             il permet à l'utilisateur de savoir si quelles sont les actions possibles une fois arrivé 
             à la dernière colonne ou à la première colonne.)
        */
        if(colonne == i)
            printf(i == 0 ? "  %c> " : i ==  NBCOL-1 ? " <%c  " : " <%c> ", pion);
        else
            printf("    ");
    }
    printf("\n");
}

/**
*\fn void tracerTiret().
*\brief Trace un tiret
*/
void tracerTiret() {
    int i;
    int nbTiret;

    i = 0;
    nbTiret = (NBCOL*4)+1; // 4 = nb caractères entre chaque colonne

    while(i < nbTiret) {
        printf("-");
        i++;
    }
    printf("\n");
}

/**
*\fn bool grillePleine(grille g).
*\brief fonction qui vérifie si la grille est pleine
*
*\param g: paramètre d'entrée qui représente la grille du jeu
*
*\return true si la grille est pleine, false sinon
*/
bool grillePleine(grille g) {
    int estPleine = true;
    int i, j;

    i = 0;
    j = 0;

    while(i < NBLIG && estPleine) {
        while(j < NBCOL && estPleine) {
            if(g[i][j] == VIDE)
                estPleine = false;
            j++;
        }
        i++;
    }

    return estPleine;
}

/**
*\fn void jouer(grille g, char pion, int *ligne, int *colonne).
*\brief procédure qui joue un tour du puissance 4
*
*\param g: paramètre d'entrée qui représente la grille du jeu
*\param pion: paramètre d'entrée qui représente le pion du joueur
*\param ligne: paramètre de sortie qui représente la ligne disponible
*\param colonne: paramètre de sortie qui représente la colonne choisie par l'utilisateur
*
*le programme demande tout à bord au joueur de choisir une colonne puis une fois la colonne
*choisie trouve une ligne disponible et enfin affecte le pion du joueur la ligne de la colonne
*/
void jouer(grille g, char pion, int *ligne, int *colonne) {
    *colonne = COLONNE_DEBUT;
    int colonneChoisie;
    int ligneTrouvee;

    do {
        colonneChoisie = choisirColonne(g, pion, *colonne);
        ligneTrouvee = trouverLigne(g, colonneChoisie);
    } while(ligneTrouvee == CODE_COLONNE_PLEINE);

    *ligne = ligneTrouvee;
    *colonne = colonneChoisie;
    g[*ligne][*colonne] = pion;
}

/**
*\fn void usageTouches().
*\brief affiche l'usage des touches
*/
void usageTouches() {
    printf("Usage des touches:\n");
    printf("q: vers la gauche\n");
    printf("d: vers la droite\n");
    printf("espace: faire tomber le pion\n");
}

/**
*\fn int choisirColonne(grille g, char pion, int colonne).
*\brief fonction qui permet à l'utilisateur de choisir une colonne
*
*\param g: paramètre d'entrée qui représente la grille du jeu
*\param pion: paramètre d'entrée qui représente le pion du joueur
*\param colonne: paramètre d'entrée qui représente la colonne du début
*
*\return le numéro de la colonne choisir par l'utilisateur
*/
int choisirColonne(grille g, char pion, int colonne) {
    int colonneChoisie;
    char saisie;

    colonneChoisie = colonne;

    do {
        usageTouches();
        do {
            scanf("%c", &saisie);
        } while(
                saisie != CODE_COLONNE_DROITE &&
                saisie != CODE_COLONNE_GAUCHE &&
                saisie != CODE_TOMBER_PION
                );

        if(saisie != CODE_TOMBER_PION) {
            if(colonneChoisie < NBLIG && saisie == CODE_COLONNE_DROITE)
                colonneChoisie++;
            else if(colonneChoisie > 0 && saisie == CODE_COLONNE_GAUCHE)
                colonneChoisie--;
        }

        afficher(g, pion, colonneChoisie);
    } while(saisie != CODE_TOMBER_PION);

    return colonneChoisie;
}

/**
*\fn void trouverLigne(grille g, int colonne).
*\brief fonction qui permet de trouver une ligne disponible dans la colonne
*
*\param g: paramètre d'entrée qui représente la grille du jeu
*\param colonne: paramètre d'entrée qui représente la colonne choisie par l'utilisateur
*
*\return le numéro de la ligne disponible dans la colonne, retourne -1 si aucune ligne n'est disponible
*/
int trouverLigne(grille g, int colonne) {
    int ligneTrouvee;
    int i;

    i = NBLIG;
    ligneTrouvee = CODE_COLONNE_PLEINE;

    while(i >= 0 && ligneTrouvee == CODE_COLONNE_PLEINE) {
        if(g[i][colonne] == VIDE)
            ligneTrouvee = i;
        i--;
    }
    return ligneTrouvee;
}

/**
*\fn void finDePartie(char pion).
*\brief procédure qui affiche qui a gagné la fin de partie
*
*\param pion: paramètre d'entrée qui représente le pion qui a gagné : INCONNU signifie que personne n'a gagné.
*/
void finDePartie(char pion) {
    printf("Résultats de la partie:\n");
    if(pion == PION_A || pion == PION_B)
        printf("Le pion %c a gagné !", pion);
    else if(pion == VIDE)
        printf("Match nul.");
    else
        fprintf(stderr, "Impossible d'avoir autre que VIDE, PION_A ou PION_B."); /* on renvoie dans la sortie d'erreur
                                                                                    si jamais la variable "pion" n'est pas bonne */
    printf("\n");
}

/**
*\fn bool estVainqueur(grille g, int ligne, int colonne)
*\brief fonction qui permet de vérifier si le joueur est vainqueur de la partie ou non
*
*\param g: paramètre d'entrée qui représente la grille du jeu
*\param ligne: paramètre d'entrée qui représente la ligne trouvée grâce à la colonne choisie par l'utilisateur
*\param colonne: paramètre d'entrée qui représente la colonne choisie par l'utilisateur
*
*\return true si le pion dans la case est vainqueur, false sinon
*/
bool estVainqueur(grille g, int ligne, int colonne) {
    /*
        j'ai séparé chaque cas de victoire dans des fonctions
        pour améliorer la lisibilité du code.
    */
    return (
        verifierVerticalement(g, ligne, colonne) ||
        verifierHorizontalement(g, ligne, colonne) ||
 
        verifierEnDiagonaleDroite(g, ligne, colonne) ||
        verifierEnDiagonaleGauche(g, ligne, colonne)
    );
}

/**
*\fn bool verifierVerticalement(grille g, int ligne, colonne).
*\brief fonction qui vérifie si il y a un alignement verticalement
*
*\param g: paramètre d'entrée qui représente la grille du jeu
*\param ligne: paramètre d'entrée qui représente la ligne trouvée grâce à la colonne choisie par l'utilisateur
*\param colonne: paramètre d'entrée qui représente la colonne choisie par l'utilisateur
*
*\return true si le pion est aligné verticalement, faux sinon
*/
bool verifierVerticalement(grille g, int ligne, int colonne) {
    int compteur = 0;
    int next = compteur+1;

    while(g[ligne+compteur][colonne] == g[ligne+next][colonne]) {
        compteur++;
        next = compteur+1;
    }

    return next >= NB_PIONS_A_ALIGNER;
}

/**
*\fn bool verifierHorizontalemente(grille g, int ligne, colonne).
*\brief fonction qui vérifie si il y a un alignement horizontale
*
*\param g: paramètre d'entrée qui représente la grille du jeu
*\param ligne: paramètre d'entrée qui représente la ligne trouvée grâce à la colonne choisie par l'utilisateur
*\param colonne: paramètre d'entrée qui représente la colonne choisie par l'utilisateur
*
*\return true si le pion est aligné horizontalement, faux sinon
*/
bool verifierHorizontalement(grille g, int ligne, int colonne) {
    int nbNombreAlignees = 1;
    int compteur = 0;

    while(g[ligne][colonne-compteur] == g[ligne][colonne-(compteur+1)]) {
        compteur++;
        nbNombreAlignees++;
    }
    
    compteur = 0;
    while(g[ligne][colonne+compteur] == g[ligne][colonne+(compteur+1)]) {
        compteur++;
        nbNombreAlignees++;
    }

    return nbNombreAlignees >= NB_PIONS_A_ALIGNER;
}

/**
*\fn bool verifierEnDiagonaleDroite(grille g, int ligne, colonne).
*\brief fonction qui vérifie si il y a un alignement diagonale droite
*
*\param g: paramètre d'entrée qui représente la grille du jeu
*\param ligne: paramètre d'entrée qui représente la ligne trouvée grâce à la colonne choisie par l'utilisateur
*\param colonne: paramètre d'entrée qui représente la colonne choisie par l'utilisateur
*
*\return true si le pion est aligné en diagonale droite, faux sinon
*/
bool verifierEnDiagonaleDroite(grille g, int ligne, int colonne) {
    int nbNombreAlignees = 1;
    int compteur = 0;

    while(g[ligne+compteur][colonne-compteur] == g[ligne+(compteur+1)][colonne-(compteur+1)]) {
        compteur++;
        nbNombreAlignees++;
    }

    compteur = 0;
    while(g[ligne-compteur][colonne+compteur] == g[ligne-(compteur+1)][colonne+(compteur+1)]) {
        compteur++;
        nbNombreAlignees++;
    }
    
    return nbNombreAlignees >= NB_PIONS_A_ALIGNER;
}

/**
*\fn bool verifierEnDiagonaleGauche(grille g, int ligne, colonne).
*\brief fonction qui vérifie si il y a un alignement diagonale gauche
*
*\param g: paramètre d'entrée qui représente la grille du jeu
*\param ligne: paramètre d'entrée qui représente la ligne trouvée grâce à la colonne choisie par l'utilisateur
*\param colonne: paramètre d'entrée qui représente la colonne choisie par l'utilisateur
*
*\return true si le pion est aligné en diagonale gauche, faux sinon
*/
bool verifierEnDiagonaleGauche(grille g, int ligne, int colonne) {
    int nbNombreAlignees = 1;
    int compteur = 0;

    while(g[ligne+compteur][colonne+compteur] == g[ligne+(compteur+1)][colonne+(compteur+1)]) {
        compteur++;
        nbNombreAlignees++;
    }
    
    compteur = 0;
    while(g[ligne-compteur][colonne-compteur] == g[ligne-(compteur+1)][colonne-(compteur+1)]) {
        compteur++;
        nbNombreAlignees++;
    }
    
    return nbNombreAlignees >= NB_PIONS_A_ALIGNER;
}
