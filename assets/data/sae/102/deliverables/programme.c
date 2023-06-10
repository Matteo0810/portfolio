    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>

    #include <stdio.h>


    // les constantes
    #define NB_LIGNES 6
    #define NB_COLONNES 7
    #define JOUEUR 'X'
    #define ADVERSAIRE 'O'
    #define VIDE ' '
    #define INCONNU ' '

    // les types
    typedef int Grille[NB_LIGNES][NB_COLONNES];

    // prototypes des fonctions
    void initGrille(Grille laGrille);
    void afficher(Grille laGrille, char pion);
    bool grillePleine(Grille laGrille);
    void jouer(Grille laGrille, char pion, int * ligne, int * colonne);
    int choisirColonne(Grille laGrille, char pion);
    int chercherLigne(Grille laGrille, int col);
    bool estVainqueur(Grille laGrille, int lig, int col);
    void finDePartie(char vainqueur);
    void copierGrille(Grille laGrille, Grille grille2);
    int ia (Grille laGrille, char pion);
    int calculVal(Grille laGrille, int lig, int col);



    // Programme principal. C'est le pion A qui commence Ã  jouer
    int partiesGagneParX = 0;
    int partiesGagneParO = 0;
    int partiesNulles = 0;
    int N = 2;
    int main()
    {
        int i;

        for(i = 0;i < N;i++) {
            Grille laGrille;
            char vainqueur=INCONNU;
            int ligne, colonne, cote;
            initGrille(laGrille);
            while (vainqueur==INCONNU && !grillePleine(laGrille)){
                jouer(laGrille,JOUEUR, &ligne, &colonne);
                if (estVainqueur(laGrille, ligne, colonne) ){
                    vainqueur = JOUEUR;
                } else if (!grillePleine(laGrille)){
                    jouer(laGrille, ADVERSAIRE, &ligne, &colonne);
                    if (estVainqueur(laGrille, ligne, colonne) ){
                        vainqueur = ADVERSAIRE;
                    }
                }
            }          

            if(vainqueur == JOUEUR) 
                partiesGagneParX++;
            else if(vainqueur == ADVERSAIRE) 
                partiesGagneParO++;
            else 
                partiesNulles++; 
        }

        printf("Résultats d'une partie: Stratégie 1 contre IA");
        printf("Nombre de parties jouées: %d\n", N);
        printf("Joueur X: %d victoires.\n", partiesGagneParX);
        printf("Joueur O: %d victoires.\n", partiesGagneParO);
        printf("Parties nulles: %d\n", partiesNulles);
        system(EXIT_SUCCESS);
    }


    void initGrille(Grille laGrille){
        int l, c;
        for (l=0 ; l<NB_LIGNES ; l++){
            for (c=0 ; c<NB_COLONNES ; c++){
                laGrille[l][c] = VIDE;
            }
        }
    }

    void afficher(Grille laGrille, char pion){
        int l, c;
        system("clear");
        printf("\t");
        printf("  %c\n", pion);
        printf("\t");
        for (c=0; c<NB_COLONNES ; c++){
            printf("----");
        }
        printf("-\n");
        for (l=0 ; l<NB_LIGNES ; l++){
            printf("\t");
            for (c=0; c<NB_COLONNES ; c++){
                printf("| %c ", laGrille[l][c]);
            }
            printf("|\n");
            printf("\t");
            for (c=0; c<NB_COLONNES ; c++){
                printf("----");
            }
            printf("-\n");
        }
        printf("\t");
        for (c=0; c<NB_COLONNES ; c++){
                printf("  %d ",c);
        }
        printf("\n\n");

    }

    bool grillePleine(Grille laGrille){
        bool pleine = true;
        int c = 0;
        while (pleine && c<NB_COLONNES){
            if (laGrille[0][c] == VIDE){
                pleine = false;
            }
            c++;
        }
        return pleine;
    }

    int stategie1(Grille grille, char pion)
    {
        int meilleureClonne = 0;

        if(grille[0][meilleureClonne] != VIDE)
            meilleureClonne++;

        return meilleureClonne;
    }

    void jouer(Grille laGrille, char pion, int * ligne, int * colonne ){
    *ligne = -1;
        do {
            *colonne = pion == JOUEUR ? stategie1(laGrille, pion) : ia(laGrille, pion);
            *ligne = chercherLigne(laGrille, *colonne);
        }while (*ligne==-1);
        laGrille[*ligne][*colonne] = pion;
    }

    int choisirColonne(Grille laGrille, char pion){
        int col;

        do{
            printf("Numero de colonne ? ");
            scanf("%d", &col);
        } while (col<0 ||col>6);
        return col;
    }

    int chercherLigne(Grille laGrille, int col){
        int ligne = -1;

        while (ligne<NB_LIGNES-1 && laGrille[ligne+1][col]==VIDE){
            ligne++;
        }
        return ligne;
    }

    bool estVainqueur(Grille laGrille, int lig, int col){
        // consiste Ã  regarder si une ligne de 4 pions s'est formÃ©e autour du pion qui vient de tomber en (lig, col)
        char lePion = laGrille[lig][col];
        int cpt,i,j;

        // regarder la verticale, en comptant le nombre de pions au Sud (inutile de regarder au Nord du pion qui vient de tomber)
        i = lig;
        cpt = 0;
        while (i<NB_LIGNES && laGrille[i][col]==lePion){
            cpt++;
            i++;
        }
        if (cpt>=4){
            return true;
        }
        // regarder l'horizontale, en comptant le nombre de pions Ã  l'Est et Ã  l'Ouest
        j = col;
        cpt = 0;
        // on regarde Ã  l'est
        while (j>=0 && laGrille[lig][j]==lePion){
            cpt++;
            j--;
        }
        j = col+1;
        // on regarde Ã  l'ouest
        while (j<NB_COLONNES && laGrille[lig][j]==lePion){
            cpt++;
            j++;
        }
        if (cpt>=4 ){
            return true;
        }
        // regarder la diagonale descendante, en comptant le nombre de pions au Nord Ouest et au Sud Est
        i = lig;
        j = col;
        cpt = 0;
        // on regarde au Nord Ouest
        while (j>=0 && i>=0 && laGrille[i][j]==lePion){
            cpt++;
            i--;
            j--;
        }
        i = lig+1;
        j = col+1;
        // on regarde au Sud Est
        while (i<NB_LIGNES && j<NB_COLONNES && laGrille[i][j]==lePion){
            cpt++;
            i++;
            j++;
        }
        if (cpt>=4 ){
            return true;
        }
        // regarder la diagonale descendante, en comptant le nombre de pions au Nord Est et au Sud Ouest
        i = lig;
        j = col;
        cpt = 0;
        // on regarde au Nord Est
        while (j<NB_COLONNES && i>=0 && laGrille[i][j]==lePion){
            cpt++;
            i--;
            j++;
        }
        i = lig+1;
        j = col-1;
        // on regarde au Sud Ouest
        while (i<NB_LIGNES && j>=0 && laGrille[i][j]==lePion){
            cpt++;
            i++;
            j--;
        }
        if (cpt>=4 ){
            return true;
        }
        return false;
    }

    void finDePartie(char vainqueur){
        if (vainqueur != INCONNU){
            printf("Joueur %c vainqueur\n", vainqueur);
        } else {
            printf("MATCH NUL");
        }
    }

    void copierGrille(Grille laGrille, Grille grille2){
        int i,j;
        i=0;
        j=0;
        for(i=0;i<NB_LIGNES;i++){
            for(j=0;j<NB_COLONNES;j++){
                grille2[i][j]=laGrille[i][j];
            }
        }
    }

int ia(Grille laGrille, char pion){

    int ligne, colonne, i, j, k, score,l, m, n, scoreMax;
    ligne=0;
    colonne=0;
    score=0;
    scoreMax=0;
    char pionadv = pion == JOUEUR ? ADVERSAIRE : JOUEUR;
    j=0;
  
    Grille grille2;
    copierGrille(laGrille, grille2);

    bool vainqueur;
    vainqueur=false;


    while((!vainqueur)&&(j<NB_COLONNES)){
        i=chercherLigne(grille2, j);
        grille2[i][j]=pion;
        vainqueur=estVainqueur(grille2, i, j);
        grille2[i][j]=VIDE;
        if(!vainqueur){
            j++;
        }
        if(vainqueur){
            return j;
        }
    }
    j=0;
    while((!vainqueur)&&(j<NB_COLONNES)){
        i=chercherLigne(grille2, j);
        grille2[i][j]=pionadv;
        vainqueur=estVainqueur(grille2, i, j);
        grille2[i][j]=VIDE;
        if(!vainqueur){
            j++;
        }
        if(vainqueur){
            return j;
        }
        
    }

    if(!vainqueur){

        i=0;
        while((i<NB_COLONNES)&&(!vainqueur)){
            l=chercherLigne(grille2, i);
            grille2[l][i]=pion;
            score=score+calculVal(grille2, l, i);
            j=0;
            while((j<NB_COLONNES)&&(!vainqueur)){
                m=chercherLigne(grille2, j);
                grille2[m][j]=pionadv;
                score=score-calculVal(grille2, m, j);

                k=0;
                while((k<NB_COLONNES)&&(!vainqueur)){
                    n=chercherLigne(grille2, k);
                    grille2[n][k]=pion;
                    score=score+calculVal(grille2, n, k);

                    if(score>scoreMax){
                        scoreMax=score;
                        colonne=i;
                    }
                    score=score-calculVal(grille2, n, k);

                    grille2[n][k]=VIDE;
                    k++;
                }
                
                score=score+calculVal(grille2, m, j);
                j++;
            }
            
            score=score-calculVal(grille2, l, i);
            i++;
        }
        
    }
    return colonne;

}

int calculVal(Grille grille2, int lig, int col){
    char lePion = grille2[lig][col];
    int cpt,i,j;
    j = col;
    i = lig;
    cpt = 0;
    while (i<NB_LIGNES && grille2[i][col]==lePion){
        cpt++;
        i++;
    }
    while (j>=0 && grille2[lig][j]==lePion){
        cpt++;
        j--;
    }
    j = col+1;
    while (j<NB_COLONNES && grille2[lig][j]==lePion){
        cpt++;
        j++;
    }
    i = lig;
    j = col;
    while (j>=0 && i>=0 && grille2[i][j]==lePion){
        cpt++;
        i--;
        j--;
    }
    i = lig+1;
    j = col+1;
    while (i<NB_LIGNES && j<NB_COLONNES && grille2[i][j]==lePion){
        cpt++;
        i++;
        j++;
    }
    i = lig;
    j = col;
    while (j<NB_COLONNES && i>=0 && grille2[i][j]==lePion){
        cpt++;
        i--;
        j++;
    }
    i = lig+1;
    j = col-1;
    while (i<NB_LIGNES && j>=0 && grille2[i][j]==lePion){
        cpt++;
        i++;
        j--;
    }
    return cpt;
}