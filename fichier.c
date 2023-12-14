#include <stdio.h>
#include <string.h>

#define TAILLE_NOM 50
#define TAILLE_MATIERE 20

typedef struct {
    char nom[TAILLE_NOM];
    char prenom[TAILLE_NOM];
    char matiere[TAILLE_MATIERE];
} Enseignant;

void viderFichier(const char *nomFichier) {
    FILE *fp = fopen(nomFichier, "w");  // Ouverture en mode de troncature pour vider le fichier

    if (fp == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    // Fermer le fichier pour le vider
    fclose(fp);
}

void afficherEnseignants(const char *nomFichier) {
    FILE *fp = fopen(nomFichier, "rb");

    if (fp == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    Enseignant enseignant;

    // Lire chaque enregistrement du fichier
    while (fread(&enseignant, sizeof(Enseignant), 1, fp) == 1) {
        // Afficher les données de l'enseignant
        printf("Nom: %s\n", enseignant.nom);
        printf("Prenom: %s\n", enseignant.prenom);
        printf("Matiere: %s\n", enseignant.matiere);
        

        if(strcmp(enseignant.nom, "lounes") == 0){
printf("\nTROUVE");
        } 
    }

    // Fermer le fichier
    fclose(fp);
}


int main() {
    // Appeler la fonction pour afficher les enseignants
    viderFichier("Enseignants.dat");
    afficherEnseignants("Enseignants.dat");

    return 0;
}
