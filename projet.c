#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <stdbool.h>


#define NOMBRE_MATIERES 10
#define TAILLE_NOM 50
#define MAX_LIGNES 10


typedef struct {
    char nom[TAILLE_NOM];
    char nom_professeur[TAILLE_NOM];
} Matiere;


typedef struct {
    char nom[TAILLE_NOM];
    char prenom[TAILLE_NOM];
    float notes[NOMBRE_MATIERES];
    Matiere matieres[NOMBRE_MATIERES];
    int nombrematieres; 
} Etudiant;

typedef struct {
    char nom[TAILLE_NOM];
    char prenom[TAILLE_NOM];
     Matiere matieres[3];
} Enseignant;


const char* nomFichier = "C:/fac/c/projet/matieres.txt";

enum {
    COL_MATIERE,
    COL_NOTE,
    NUM_COLS
};

void saisirInscriptionEnseignant(GtkWidget *widget, gpointer data);
void saisirConnexionEnseignant(GtkWidget *widget, gpointer data);
void saisirInscriptionEtudiant(GtkWidget *widget, gpointer data);
void saisirConnexionEtudiant(GtkWidget *widget, gpointer data);
void goToModifier(GtkWidget *widget, gpointer data); 
void consulter(GtkWidget *widget, gpointer data);




char nometudiantactuel[TAILLE_NOM] ; char prenometudiantactuel[TAILLE_NOM] ; char nomenseignantactuel[TAILLE_NOM] ; char prenomenseignantactuel[TAILLE_NOM] ; Matiere listematieres[NOMBRE_MATIERES]; 
GtkWidget *check_button_1, *check_button_2, *check_button_3, *check_button_4, *check_button_5, *check_button_6,*check_button_7, *check_button_8, *check_button_9,*check_button_10;
GtkWidget *radio_button_1, *radio_button_2, *radio_button_3, *radio_button_4, *radio_button_5, *radio_button_6,*radio_button_7, *radio_button_8, *radio_button_9,*radio_button_10;

// Les widgets pour la premi�re page
GtkWidget *windowAcceuilEnseignant,*vbox,*windowConnexionEnseignant,*windowConnexionEtudiant, *windowAcceuil, *windowAcceuilEtudiant, *windowIdentificationEnseignant,*windowInscriptionEnseignant, *windowIdentificationEtudiant, *windowInscriptionEtudiant; 

// Les widgets pour la deuxi�me page (Saisir)
GtkWidget *entryNomSaisir,*windowConsulter,*separator,*labelvide,*labelMatiereEnseignee, *entryPrenomSaisir,*labelNomSaisir, *labelPrenomSaisir, *entryNotes1,*entryNotes2,*entryNotes3,*entryNotes4, *windowInscriptionEnseignant=NULL, *buttonValiderSaisir, *buttonRetour;

// Les widgets pour la troisi�me page (Modifier)
GtkWidget *entryNomModifier, *entryPrenomModifier, *windowModifier=NULL, *buttonRetour;

// Les widgets pour la 4 page (Consulter)
GtkWidget *entryNomConsulter,*buttonConnexionEtudiant,*buttonConnexionEnseignant,*buttonInscriptionEtudiant,*buttonInscriptionEnseignant, *entryPrenomConsulter,*windowConsulter1=NULL, *windowConsulter=NULL,*buttonValiderConsulter, *buttonRetour;
GtkListStore *store;
GtkWidget *entryTreeView,*treeView; 


GtkWidget *entries[5][4],  *sDialog;
Etudiant etudiantactuel;Enseignant enseignantactuel; 

void viderFichier(const char *nomFichier) {
    FILE *fp = fopen(nomFichier, "w");  // Ouverture en mode de troncature pour vider le fichier

    if (fp == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    // Fermer le fichier pour le vider
    fclose(fp);
}


void on_cell_edited(GtkCellRendererText *renderer, gchar *path, gchar *text, gpointer user_data) {
    GtkTreeModel *model = gtk_tree_view_get_model(entryTreeView);
    GtkTreeModel *smodel = gtk_tree_view_get_model(treeView);
    GtkTreeIter iter;


    if (gtk_tree_model_get_iter_from_string(smodel, &iter, path)) {
        double value;
        sscanf(text, "%lf", &value);
        char output[50];
        snprintf(output, 50, "%.1f", value);

        // Mettre à jour la valeur dans le modèle 
        gtk_list_store_set(GTK_LIST_STORE(smodel), &iter, 2, output, -1);

        // Mettre à jour la note correspondante dans la structure Etudiant
        gint row = atoi(path);
        FILE *fichier = fopen("Etudiants.dat", "rb+");Etudiant etudiant;
        while (fread(&etudiant, sizeof(Etudiant), 1, fichier) == 1) {
        for (int i = 0; i < etudiant.nombrematieres; ++i) {
            if (strcmp(etudiant.nom, etudiantactuel.nom) == 0 && strcmp(etudiant.prenom, etudiantactuel.prenom) == 0) {
                // Mettre à jour la note dans le fichier des etudiants
                etudiant.notes[row] = value;
                etudiantactuel.notes[row] = value; 
            }
        }

        // Revenir au début de l'enregistrement pour écrire les modifications
            fseek(fichier, -((long)sizeof(Etudiant)), SEEK_CUR);

            // Écrire l'enregistrement modifié dans le fichier
            fwrite(&etudiant, sizeof(Etudiant), 1, fichier);

            // Aller à l'enregistrement suivant
            fseek(fichier, 0, SEEK_CUR);break; 
}
 fclose(fichier); // Fermer le fichier
    }
}





void recupmatieres(){
    FILE *fichier = fopen(nomFichier, "r");

    // Vérifier si le fichier a été ouvert avec succès
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return ;
    }

    // Lire les lignes du fichier et stocker les données dans les tableaux
    for (int i = 0; i < 10; i++) {
        // Utiliser fscanf pour lire la ligne du fichier
        if (fscanf(fichier, " %49[^:]: %49[^\n]", listematieres[i].nom, listematieres[i].nom_professeur) != 2) {
            fprintf(stderr, "Erreur de lecture de la ligne %d\n", i + 1);
            break;
        }
    }

    // Fermer le fichier après lecture
    fclose(fichier);
}




void modifierEnseignantEtudiant( const char *matiereRecherchee, const char *nouveauNomProfesseur) {

    FILE *fichier = fopen("Etudiants.dat", "rb+");Etudiant etudiant;
     while (fread(&etudiant, sizeof(Etudiant), 1, fichier) == 1) {
    for (int i = 0; i < etudiant.nombrematieres; ++i) {
        if (strcmp(etudiant.matieres[i].nom, matiereRecherchee) == 0) {
            // Mettre à jour le nom du professeur pour la matière donnée
            strncpy(etudiant.matieres[i].nom_professeur, nouveauNomProfesseur, TAILLE_NOM - 1);
        }
    }

    // Revenir au début de l'enregistrement pour écrire les modifications
        fseek(fichier, -((long)sizeof(Etudiant)), SEEK_CUR);

        // Écrire l'enregistrement modifié dans le fichier
        fwrite(&etudiant, sizeof(Etudiant), 1, fichier);

        // Aller à l'enregistrement suivant
        fseek(fichier, 0, SEEK_CUR);
}
 fclose(fichier); // Fermer le fichier
}






void modifierEnseignantFichier(const char *nomFichier, const char *matiere, const char *nouvelEnseignant) {
    // Ouvrir le fichier en mode lecture
    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    // Créer un fichier temporaire en mode écriture
    FILE *tempFichier = fopen("temp.txt", "w");
    if (tempFichier == NULL) {
        perror("Erreur lors de la création du fichier temporaire");
        fclose(fichier);
        exit(EXIT_FAILURE);
    }

    char ligne[256];

    // Parcourir le fichier ligne par ligne
    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        // Vérifier si la ligne commence par la matière recherchée
        if (strstr(ligne, matiere) != NULL) {
            // Mettre à jour la ligne avec le nouvel enseignant
            fprintf(tempFichier, "%s : %s\n", matiere, nouvelEnseignant);
        } else {
            // Copier la ligne telle quelle dans le fichier temporaire
            fputs(ligne, tempFichier);
        }
    }

    // Fermer les fichiers
    fclose(fichier);
    fclose(tempFichier);

    // Supprimer le fichier original
    if (remove(nomFichier) != 0) {
        perror("Erreur lors de la suppression du fichier original");
        exit(EXIT_FAILURE);
    }

    // Renommer le fichier temporaire pour remplacer l'original
    if (rename("temp.txt", nomFichier) != 0) {
        perror("Erreur lors du renommage du fichier temporaire");
        exit(EXIT_FAILURE);
    }
}





void addRowToGrid(GtkWidget *grid, int row, const char *labelText, const char *valueText) {
    GtkWidget *label = gtk_label_new(labelText);
    GtkWidget *value = gtk_label_new(valueText);

    gtk_grid_attach(GTK_GRID(grid), label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), value, 1, row, 1, 1);
}



void retourIdentificationEnseignant(GtkWidget *widget, gpointer data) {
    // Cacher la fen�tre actuelle (saisie)
    gtk_widget_hide(GTK_WIDGET(data));
    // Afficher la fen�tre d'accueil
    gtk_widget_show_all(windowIdentificationEnseignant);
}

void retourIdentificationEtudiant(GtkWidget *widget, gpointer data) {
    // Cacher la fen�tre actuelle (saisie)
    gtk_widget_hide(GTK_WIDGET(data));
    // Afficher la fen�tre d'accueil
    gtk_widget_show_all(windowIdentificationEtudiant);
}

void retourAccueil(GtkWidget *widget, gpointer data) {
    // Cacher la fen�tre actuelle (saisie)
    gtk_widget_hide(GTK_WIDGET(data));
    // Afficher la fen�tre d'accueil
    gtk_widget_show_all(windowAcceuil);
}

void retourAccueilSaisir(GtkWidget *widget, gpointer data) {
    // Cacher la fen�tre actuelle (saisie)
    gtk_widget_hide(GTK_WIDGET(data));
    // Afficher la fen�tre d'accueil
    gtk_widget_show_all(windowAcceuilEnseignant);
}

void showMessageDialog(const char *message) {
    GtkWidget *messageDialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, message);
    gtk_window_set_position(GTK_WINDOW(messageDialog), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(messageDialog), 300, 100);
    gtk_dialog_run(GTK_DIALOG(messageDialog));
    gtk_widget_destroy(messageDialog);
}

void showMessageDialogechec(const char *message) {
    GtkWidget *messageDialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE, message);
    gtk_window_set_position(GTK_WINDOW(messageDialog), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(messageDialog), 300, 100);
    gtk_dialog_run(GTK_DIALOG(messageDialog));
    gtk_widget_destroy(messageDialog);
}



void goToInscrireEtudiant(GtkWidget *widget, gpointer data) {

    // Cacher la fen�tre d'accueil
    gtk_widget_hide(GTK_WIDGET(data));
    // Cr�er une nouvelle fen�tre de saisie
     windowInscriptionEtudiant = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    recupmatieres();
    gtk_window_set_position(GTK_WINDOW(windowInscriptionEtudiant), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(windowInscriptionEtudiant), 600, 500);
    gtk_window_set_title(GTK_WINDOW(windowInscriptionEtudiant), "Saisir");

    g_signal_connect(windowInscriptionEtudiant, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Créer d'autres widgets pour la saisie (labels, entry, buttons, etc.)
     labelNomSaisir = gtk_label_new("Nom :");
     entryNomSaisir = gtk_entry_new();

     labelPrenomSaisir = gtk_label_new("Prenom :");
     entryPrenomSaisir = gtk_entry_new();

    
    // Création des boutons à case à cocher
       recupmatieres();
    check_button_1 = gtk_check_button_new_with_label(listematieres[0].nom);
    check_button_2 = gtk_check_button_new_with_label(listematieres[1].nom);
    check_button_3 = gtk_check_button_new_with_label(listematieres[2].nom);
    check_button_4 = gtk_check_button_new_with_label(listematieres[3].nom);
    check_button_5 = gtk_check_button_new_with_label(listematieres[4].nom);
    check_button_6 = gtk_check_button_new_with_label(listematieres[5].nom);
    check_button_7 = gtk_check_button_new_with_label(listematieres[6].nom);
    check_button_8 = gtk_check_button_new_with_label(listematieres[7].nom);
    check_button_9 = gtk_check_button_new_with_label(listematieres[8].nom);
    check_button_10 = gtk_check_button_new_with_label(listematieres[9].nom);


    // Connexion des fonctions de rappel aux signaux "toggled" des boutons
    g_signal_connect(check_button_1,"toggled", G_CALLBACK(NULL), NULL);
    g_signal_connect(check_button_2, "toggled", G_CALLBACK(NULL), NULL);
    g_signal_connect(check_button_3, "toggled", G_CALLBACK(NULL), NULL);
    g_signal_connect(check_button_4, "toggled", G_CALLBACK(NULL), NULL);
    g_signal_connect(check_button_5, "toggled", G_CALLBACK(NULL), NULL);
    g_signal_connect(check_button_6, "toggled", G_CALLBACK(NULL), NULL);
    g_signal_connect(check_button_7, "toggled", G_CALLBACK(NULL), NULL);
    g_signal_connect(check_button_8, "toggled", G_CALLBACK(NULL), NULL);
    g_signal_connect(check_button_9, "toggled", G_CALLBACK(NULL), NULL);
    g_signal_connect(check_button_10, "toggled", G_CALLBACK(NULL), NULL);


  buttonValiderSaisir = gtk_button_new_with_label("Valider");
    g_signal_connect(buttonValiderSaisir, "clicked", G_CALLBACK(saisirInscriptionEtudiant), windowInscriptionEtudiant);
GtkWidget *buttonRetour = gtk_button_new_with_label("Retour");
    g_signal_connect(buttonRetour, "clicked", G_CALLBACK(retourIdentificationEtudiant), windowInscriptionEtudiant);

    // Agencer les widgets dans la fenêtre de saisie
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 25);

    gtk_grid_attach(GTK_GRID(grid), labelNomSaisir, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entryNomSaisir, 1, 0, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), labelPrenomSaisir, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entryPrenomSaisir, 1, 1, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), check_button_1, 0, 2, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), check_button_2, 0, 3, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), check_button_3, 0, 4, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), check_button_4, 0, 5, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), check_button_5, 0, 6, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), check_button_6, 0, 7, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), check_button_7, 0, 8, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), check_button_8, 0, 9, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), check_button_9, 0, 10, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), check_button_10, 0, 11, 2, 1);


    gtk_grid_attach(GTK_GRID(grid), buttonValiderSaisir, 0, 12, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), buttonRetour, 1, 12, 1, 1);

    // Utilisation de GtkAlignment pour centrer la grille dans la fenêtre
    GtkWidget *alignment = gtk_alignment_new(0.5, 0.5, 0, 0);
    gtk_container_add(GTK_CONTAINER(alignment), grid);

    // Afficher la fenêtre de saisie
    gtk_container_add(GTK_CONTAINER(windowInscriptionEtudiant), alignment);
    gtk_widget_show_all(windowInscriptionEtudiant);
}




void goToConnecterEtudiant(GtkWidget *widget, gpointer data) {
       gtk_widget_hide(GTK_WIDGET(data));
    windowConnexionEtudiant = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_position(GTK_WINDOW(windowConnexionEtudiant), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(windowConnexionEtudiant), 600, 500);
    gtk_window_set_title(GTK_WINDOW(windowConnexionEtudiant), "Saisir");

    g_signal_connect(windowConnexionEtudiant, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Créer d'autres widgets pour la saisie (labels, entry, buttons, etc.)
    labelNomSaisir = gtk_label_new("Nom :");
    entryNomSaisir = gtk_entry_new();

    labelPrenomSaisir = gtk_label_new("Prenom :");
    entryPrenomSaisir = gtk_entry_new();


    GtkWidget *buttonValiderSaisir = gtk_button_new_with_label("Valider");
    g_signal_connect(buttonValiderSaisir, "clicked", G_CALLBACK(saisirConnexionEtudiant), windowConnexionEtudiant);

    GtkWidget *buttonRetour = gtk_button_new_with_label("Quitter");
    g_signal_connect(buttonRetour, "clicked", G_CALLBACK(retourIdentificationEtudiant), windowConnexionEtudiant);

    // Agencer les widgets dans la fenêtre de saisie
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 25);

    gtk_grid_attach(GTK_GRID(grid), labelNomSaisir, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entryNomSaisir, 1, 0, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), labelPrenomSaisir, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entryPrenomSaisir, 1, 1, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), buttonValiderSaisir, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), buttonRetour, 1, 2, 1, 1);

    // Utilisation de GtkAlignment pour centrer la grille dans la fenêtre
    GtkWidget *alignment = gtk_alignment_new(0.5, 0.5, 0, 0);
    gtk_container_add(GTK_CONTAINER(alignment), grid);

    // Afficher la fenêtre de saisie
    gtk_container_add(GTK_CONTAINER(windowConnexionEtudiant), alignment);
    gtk_widget_show_all(windowConnexionEtudiant);
}





void goToInscrireEnseignant(GtkWidget *widget, gpointer data) {
    gtk_widget_hide(GTK_WIDGET(data));
    windowInscriptionEnseignant = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_position(GTK_WINDOW(windowInscriptionEnseignant), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(windowInscriptionEnseignant), 600, 500);
    gtk_window_set_title(GTK_WINDOW(windowInscriptionEnseignant), "Saisir");

    g_signal_connect(windowInscriptionEnseignant, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Créer d'autres widgets pour la saisie (labels, entry, buttons, etc.)
    labelNomSaisir = gtk_label_new("Nom :");
    entryNomSaisir = gtk_entry_new();

    labelPrenomSaisir = gtk_label_new("Prenom :");
    entryPrenomSaisir = gtk_entry_new();

    // Add a label for the question about the subject
    labelMatiereEnseignee = gtk_label_new("Quelle matiere enseignez-vous ?");


    // Add a separator line
    separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    recupmatieres();
    // Création des radio buttons
    radio_button_1 = gtk_radio_button_new_with_label(NULL, listematieres[0].nom);
    radio_button_2 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radio_button_1), listematieres[1].nom);
    radio_button_3 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radio_button_1), listematieres[2].nom);
    radio_button_4 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radio_button_1), listematieres[3].nom);
    radio_button_5 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radio_button_1), listematieres[4].nom);
    radio_button_6 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radio_button_1), listematieres[5].nom);
    radio_button_7 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radio_button_1), listematieres[6].nom);
    radio_button_8 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radio_button_1), listematieres[7].nom);
    radio_button_9 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radio_button_1), listematieres[8].nom);
    radio_button_10 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radio_button_1), listematieres[9].nom);

    // Connexion des fonctions de rappel aux signaux "toggled" des boutons
    g_signal_connect(radio_button_1, "toggled", G_CALLBACK(NULL), NULL);
    g_signal_connect(radio_button_2, "toggled", G_CALLBACK(NULL), NULL);
    g_signal_connect(radio_button_3, "toggled", G_CALLBACK(NULL), NULL);
    g_signal_connect(radio_button_4, "toggled", G_CALLBACK(NULL), NULL);
    g_signal_connect(radio_button_6, "toggled", G_CALLBACK(NULL), NULL);
    g_signal_connect(radio_button_6, "toggled", G_CALLBACK(NULL), NULL);
    g_signal_connect(radio_button_7, "toggled", G_CALLBACK(NULL), NULL);
    g_signal_connect(radio_button_8, "toggled", G_CALLBACK(NULL), NULL);
    g_signal_connect(radio_button_9, "toggled", G_CALLBACK(NULL), NULL);
    g_signal_connect(radio_button_10, "toggled", G_CALLBACK(NULL), NULL);



    GtkWidget *buttonValiderSaisir = gtk_button_new_with_label("Valider");
    g_signal_connect(buttonValiderSaisir, "clicked", G_CALLBACK(saisirInscriptionEnseignant), windowInscriptionEnseignant);

    GtkWidget *buttonRetour = gtk_button_new_with_label("Quitter");
    g_signal_connect(buttonRetour, "clicked", G_CALLBACK(retourIdentificationEnseignant), windowInscriptionEnseignant);

    // Agencer les widgets dans la fenêtre de saisie
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 25);

    gtk_grid_attach(GTK_GRID(grid), labelNomSaisir, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entryNomSaisir, 1, 0, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), labelPrenomSaisir, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entryPrenomSaisir, 1, 1, 1, 1);

    // Add the new label and separator
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new(""), 0, 2, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), labelMatiereEnseignee, 0, 3, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), separator, 0, 4, 2, 1);

    gtk_grid_attach(GTK_GRID(grid), radio_button_1, 0, 5, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), radio_button_2, 0, 6, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), radio_button_3, 0, 7, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), radio_button_4, 0, 8, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), radio_button_5, 0, 9, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), radio_button_6, 0, 10, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), radio_button_7, 0, 11, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), radio_button_8, 0, 12, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), radio_button_9, 0, 13, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), radio_button_10, 0, 14, 2, 1);



    gtk_grid_attach(GTK_GRID(grid), gtk_label_new(""), 0, 15, 2, 1);


    gtk_grid_attach(GTK_GRID(grid), buttonValiderSaisir, 0, 16, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), buttonRetour, 1, 16, 1, 1);

    // Utilisation de GtkAlignment pour centrer la grille dans la fenêtre
    GtkWidget *alignment = gtk_alignment_new(0.5, 0.5, 0, 0);
    gtk_container_add(GTK_CONTAINER(alignment), grid);

    // Afficher la fenêtre de saisie
    gtk_container_add(GTK_CONTAINER(windowInscriptionEnseignant), alignment);
    gtk_widget_show_all(windowInscriptionEnseignant);
}




void goToConnecterEnseignant(GtkWidget *widget, gpointer data) {
gtk_widget_hide(GTK_WIDGET(data));
    windowConnexionEnseignant = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_position(GTK_WINDOW(windowConnexionEnseignant), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(windowConnexionEnseignant), 600, 500);
    gtk_window_set_title(GTK_WINDOW(windowConnexionEnseignant), "Saisir");

    g_signal_connect(windowConnexionEnseignant, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Créer d'autres widgets pour la saisie (labels, entry, buttons, etc.)
    labelNomSaisir = gtk_label_new("Nom :");
    entryNomSaisir = gtk_entry_new();

    labelPrenomSaisir = gtk_label_new("Prenom :");
    entryPrenomSaisir = gtk_entry_new();


    GtkWidget *buttonValiderSaisir = gtk_button_new_with_label("Valider");
    g_signal_connect(buttonValiderSaisir, "clicked", G_CALLBACK(saisirConnexionEnseignant), windowConnexionEnseignant);

    GtkWidget *buttonRetour = gtk_button_new_with_label("Quitter");
    g_signal_connect(buttonRetour, "clicked", G_CALLBACK(retourIdentificationEnseignant), windowConnexionEnseignant);

    // Agencer les widgets dans la fenêtre de saisie
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 25);

    gtk_grid_attach(GTK_GRID(grid), labelNomSaisir, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entryNomSaisir, 1, 0, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), labelPrenomSaisir, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entryPrenomSaisir, 1, 1, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), buttonValiderSaisir, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), buttonRetour, 1, 2, 1, 1);

    // Utilisation de GtkAlignment pour centrer la grille dans la fenêtre
    GtkWidget *alignment = gtk_alignment_new(0.5, 0.5, 0, 0);
    gtk_container_add(GTK_CONTAINER(alignment), grid);

    // Afficher la fenêtre de saisie
    gtk_container_add(GTK_CONTAINER(windowConnexionEnseignant), alignment);
    gtk_widget_show_all(windowConnexionEnseignant);
}



void saisirInscriptionEnseignant(GtkWidget *widget, gpointer data) {
    windowInscriptionEnseignant = GTK_WIDGET(data);

    // Récupé  rer les informations depuis les widgets
     Enseignant et;
    const gchar *nom = gtk_entry_get_text(GTK_ENTRY(entryNomSaisir));
    const gchar *prenom = gtk_entry_get_text(GTK_ENTRY(entryPrenomSaisir));
    strcpy(enseignantactuel.nom, nom);     strcpy(enseignantactuel.prenom, prenom); 
    FILE *fp = fopen("Enseignants.dat", "rb"); 
    
    int enseignantTrouve = 0;
    //on vérifie que l'enseignant n'est pas deja inscrit
    while (fread(&et, sizeof(Enseignant), 1, fp) == 1) {
                
                // Comparaison du nom et pr�nom
                if (strcmp(et.nom, enseignantactuel.nom) == 0 && strcmp(et.prenom, enseignantactuel.prenom) == 0) {
                // Affichage des informations de l'�tudiant
            enseignantTrouve = 1; break;
            }
            }
            fclose(fp);

    
    if(enseignantTrouve == 0){
        
    // Récupérer l'option sélectionnée
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_button_1))) {
       strcpy(enseignantactuel.matieres[0].nom , listematieres[0].nom);
       modifierEnseignantFichier(nomFichier, listematieres[0].nom, enseignantactuel.nom); //on modifie le nom du prof de cette matiere dans le fichier matieres
       modifierEnseignantEtudiant(listematieres[0].nom, enseignantactuel.nom); //on fait pareil dans le fichier etudiants pour chaque etudiant deja inscrit a la matiere
    } else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_button_2))) {
        strcpy(enseignantactuel.matieres[0].nom , listematieres[1].nom);
        modifierEnseignantFichier(nomFichier, listematieres[1].nom, enseignantactuel.nom);
        modifierEnseignantEtudiant(listematieres[1].nom, enseignantactuel.nom);
    } else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_button_3))) {
        strcpy(enseignantactuel.matieres[0].nom , listematieres[2].nom);
        modifierEnseignantFichier(nomFichier, listematieres[2].nom, enseignantactuel.nom);
        modifierEnseignantEtudiant(listematieres[2].nom, enseignantactuel.nom);
    }else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_button_4))) {
        strcpy(enseignantactuel.matieres[0].nom , listematieres[3].nom);
        modifierEnseignantFichier(nomFichier, listematieres[3].nom, enseignantactuel.nom);
        modifierEnseignantEtudiant(listematieres[3].nom, enseignantactuel.nom);
    }else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_button_5))) {
        strcpy(enseignantactuel.matieres[0].nom , listematieres[4].nom);
        modifierEnseignantFichier(nomFichier, listematieres[4].nom, enseignantactuel.nom);
        modifierEnseignantEtudiant(listematieres[4].nom, enseignantactuel.nom);
    }else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_button_6))) {
        strcpy(enseignantactuel.matieres[0].nom , listematieres[5].nom);
        modifierEnseignantFichier(nomFichier, listematieres[5].nom, enseignantactuel.nom);
        modifierEnseignantEtudiant(listematieres[5].nom, enseignantactuel.nom);
    }else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_button_7))) {
        strcpy(enseignantactuel.matieres[0].nom , listematieres[6].nom);
        modifierEnseignantFichier(nomFichier, listematieres[6].nom, enseignantactuel.nom);
        modifierEnseignantEtudiant(listematieres[6].nom, enseignantactuel.nom);
    }else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_button_8))) {
        strcpy(enseignantactuel.matieres[0].nom , listematieres[7].nom);
        modifierEnseignantFichier(nomFichier, listematieres[7].nom, enseignantactuel.nom);
        modifierEnseignantEtudiant(listematieres[7].nom, enseignantactuel.nom);
    }else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_button_9))) {
        strcpy(enseignantactuel.matieres[0].nom , listematieres[8].nom);
        modifierEnseignantFichier(nomFichier, listematieres[8].nom, enseignantactuel.nom);
        modifierEnseignantEtudiant(listematieres[8].nom, enseignantactuel.nom);
    }else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_button_10))) {
        strcpy(enseignantactuel.matieres[0].nom , listematieres[9].nom);
        modifierEnseignantFichier(nomFichier, listematieres[9].nom, enseignantactuel.nom);
        modifierEnseignantEtudiant(listematieres[9].nom, enseignantactuel.nom);
    }


 
    fp = fopen("Enseignants.dat", "ab"); 
    size_t elementsEcrits = fwrite(&enseignantactuel, sizeof(Enseignant), 1, fp);

    // Vérifier si l'écriture a réussi
    if (elementsEcrits != 1) {
        perror("Erreur lors de l'écriture dans le fichier");
        // Gérer l'erreur, peut-être retourner ou sortir de la fonction
    }
    fclose(fp);
    

    // Afficher un message de succ�s sur la fen�tre principale
    GtkWidget *successDialog = gtk_message_dialog_new(GTK_WINDOW(windowInscriptionEnseignant),
                                                    GTK_DIALOG_MODAL,
                                                    GTK_MESSAGE_INFO,
                                                    GTK_BUTTONS_OK,
                                                    "Les donnees ont ete enregistrees avec succes! Bienvenue %s %s", enseignantactuel.nom, enseignantactuel.prenom);
    gtk_dialog_run(GTK_DIALOG(successDialog));
    gtk_widget_destroy(successDialog);
     gtk_widget_hide(GTK_WIDGET(data));
       goToModifier(NULL, NULL);}
       else{
        showMessageDialogechec("Enseignant deja inscrit! Veuillez vous connecter");
        gtk_widget_hide(GTK_WIDGET(data));
        goToConnecterEnseignant(NULL, NULL); }}





void saisirConnexionEnseignant(GtkWidget *widget, gpointer data) {
    windowInscriptionEnseignant = GTK_WIDGET(data);

    Enseignant ens;
    // Récupérer les informations depuis les widgets
    
    const gchar *nom = gtk_entry_get_text(GTK_ENTRY(entryNomSaisir));
    const gchar *prenom = gtk_entry_get_text(GTK_ENTRY(entryPrenomSaisir));
    strcpy(ens.nom, nom);     strcpy(ens.prenom, prenom); 


   
    int enseignantTrouve = 0;
    //on vérifie que l'enseignant n'est pas deja inscrit
    FILE *fp = fopen("Enseignants.dat", "rb");
    while (fread(&enseignantactuel, sizeof(Enseignant), 1, fp) == 1) {
                
                // Comparaison du nom et pr�nom
                if (strcmp(ens.nom, enseignantactuel.nom) == 0 && strcmp(ens.prenom, enseignantactuel.prenom) == 0) {
                // Affichage des informations de l'�tudiant
            enseignantTrouve = 1; 
            break;}}
            fclose(fp);
            int index = 0;
    if(enseignantTrouve == 0){
        showMessageDialogechec("Enseignant non inscrit! Veuillez vous inscrire");
        gtk_widget_hide(GTK_WIDGET(data));
        goToInscrireEtudiant(NULL, NULL); }else{
            GtkWidget *successDialog = gtk_message_dialog_new(GTK_WINDOW(windowInscriptionEnseignant),
                                                    GTK_DIALOG_MODAL,
                                                    GTK_MESSAGE_INFO,
                                                    GTK_BUTTONS_OK,
                                                    " Bienvenue %s %s", enseignantactuel.nom, enseignantactuel.prenom);
    gtk_dialog_run(GTK_DIALOG(successDialog));
    gtk_widget_destroy(successDialog);
     gtk_widget_hide(GTK_WIDGET(data));
       goToModifier(NULL, NULL);
        }
}







void saisirInscriptionEtudiant(GtkWidget *widget, gpointer data) {

windowInscriptionEtudiant = GTK_WIDGET(data);

    // Récupé  rer les informations depuis les widgets
     Etudiant et;
    const gchar *nom = gtk_entry_get_text(GTK_ENTRY(entryNomSaisir));
    const gchar *prenom = gtk_entry_get_text(GTK_ENTRY(entryPrenomSaisir));
    gboolean checked_1 = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_button_1));
    gboolean checked_2 = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_button_2));
    gboolean checked_3 = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_button_3));
    gboolean checked_4 = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_button_4));
    gboolean checked_5 = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_button_5));
    gboolean checked_6 = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_button_6));
    gboolean checked_7 = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_button_7));
    gboolean checked_8 = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_button_8));
    gboolean checked_9 = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_button_9));
    gboolean checked_10 = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_button_10));
    

    strcpy(etudiantactuel.nom, nom);     strcpy(etudiantactuel.prenom, prenom); 

    FILE *fp = fopen("Etudiants.dat", "rb"); 
    
    int enseignantTrouve = 0;
    //on vérifie que l'enseignant n'est pas deja inscrit
    while (fread(&et, sizeof(Etudiant), 1, fp) == 1) {
                
                // Comparaison du nom et pr�nom
                if (strcmp(et.nom, etudiantactuel.nom) == 0 && strcmp(et.prenom, etudiantactuel.prenom) == 0) {
                // Affichage des informations de l'�tudiant
            enseignantTrouve = 1; 
            break;}}
            fclose(fp);
            int index = 0;
    if(enseignantTrouve == 0){
        etudiantactuel.nombrematieres = 0; 
    // Afficher les boutons sélectionnés
    if (checked_1){
 
        strcpy(etudiantactuel.matieres[index].nom , listematieres[0].nom);strcpy(etudiantactuel.matieres[index].nom_professeur , listematieres[0].nom_professeur);etudiantactuel.notes[index] = 0;
        index++; etudiantactuel.nombrematieres++; 
         }
         if (checked_2){
 
        strcpy(etudiantactuel.matieres[index].nom , listematieres[1].nom);strcpy(etudiantactuel.matieres[index].nom_professeur , listematieres[1].nom_professeur);etudiantactuel.notes[index] = 0;
        index++; etudiantactuel.nombrematieres++; 
         }
         if (checked_3){
 
        strcpy(etudiantactuel.matieres[index].nom , listematieres[2].nom);strcpy(etudiantactuel.matieres[index].nom_professeur , listematieres[2].nom_professeur);etudiantactuel.notes[index] = 0;
        index++; etudiantactuel.nombrematieres++; 
         }
         if (checked_4){
 
        strcpy(etudiantactuel.matieres[index].nom , listematieres[3].nom);strcpy(etudiantactuel.matieres[index].nom_professeur , listematieres[3].nom_professeur);etudiantactuel.notes[index] = 0;
        index++; etudiantactuel.nombrematieres++; 
         }
         if (checked_5){
 
        strcpy(etudiantactuel.matieres[index].nom , listematieres[4].nom);strcpy(etudiantactuel.matieres[index].nom_professeur , listematieres[4].nom_professeur);etudiantactuel.notes[index] = 0;
        index++; etudiantactuel.nombrematieres++; 
         }
         if (checked_6){
 
        strcpy(etudiantactuel.matieres[index].nom , listematieres[5].nom);strcpy(etudiantactuel.matieres[index].nom_professeur , listematieres[5].nom_professeur);etudiantactuel.notes[index] = 0;
        index++; etudiantactuel.nombrematieres++; 
         }
         if (checked_7){
 
        strcpy(etudiantactuel.matieres[index].nom , listematieres[6].nom);strcpy(etudiantactuel.matieres[index].nom_professeur , listematieres[6].nom_professeur);etudiantactuel.notes[index] = 0;
        index++; etudiantactuel.nombrematieres++; 
         }
         if (checked_8){
 
        strcpy(etudiantactuel.matieres[index].nom , listematieres[7].nom);strcpy(etudiantactuel.matieres[index].nom_professeur , listematieres[7].nom_professeur);etudiantactuel.notes[index] = 0;
        index++; etudiantactuel.nombrematieres++; 
         }
         if (checked_9){
 
        strcpy(etudiantactuel.matieres[index].nom , listematieres[8].nom);strcpy(etudiantactuel.matieres[index].nom_professeur , listematieres[8].nom_professeur);etudiantactuel.notes[index] = 0;
        index++; etudiantactuel.nombrematieres++; 
         }
         if (checked_10){
 
        strcpy(etudiantactuel.matieres[index].nom , listematieres[9].nom);strcpy(etudiantactuel.matieres[index].nom_professeur , listematieres[9].nom_professeur);etudiantactuel.notes[index] = 0;
        index++; etudiantactuel.nombrematieres++; 
         }
     
   
    fp = fopen("Etudiants.dat", "ab"); 
    size_t elementsEcrits = fwrite(&etudiantactuel, sizeof(Etudiant), 1, fp);

    // Vérifier si l'écriture a réussi
    if (elementsEcrits != 1) {
        perror("Erreur lors de l'écriture dans le fichier");
        // Gérer l'erreur, peut-être retourner ou sortir de la fonction
    }
    fclose(fp);

    // Afficher un message de succ�s sur la fen�tre principale
    GtkWidget *successDialog = gtk_message_dialog_new(GTK_WINDOW(windowInscriptionEnseignant),
                                                    GTK_DIALOG_MODAL,
                                                    GTK_MESSAGE_INFO,
                                                    GTK_BUTTONS_OK,
                                                    "Les donnees ont ete enregistrees avec succes! Bienvenue %s %s", etudiantactuel.nom, etudiantactuel.prenom);
    gtk_dialog_run(GTK_DIALOG(successDialog));
    gtk_widget_destroy(successDialog);
     gtk_widget_hide(GTK_WIDGET(data));
       consulter(NULL, NULL);}else{
        showMessageDialogechec("Etudiant deja inscrit! Veuillez vous connecter");
        gtk_widget_hide(GTK_WIDGET(data));
        goToConnecterEtudiant(NULL, NULL); 
}}






void saisirConnexionEtudiant(GtkWidget *widget, gpointer data) {
    windowInscriptionEtudiant = GTK_WIDGET(data);

    Etudiant etudiant;
    // Récupérer les informations depuis les widgets
    
    const gchar *nom = gtk_entry_get_text(GTK_ENTRY(entryNomSaisir));
    const gchar *prenom = gtk_entry_get_text(GTK_ENTRY(entryPrenomSaisir));
 
     strcpy(etudiant.nom, nom);     strcpy(etudiant.prenom, prenom); 

     FILE *fp = fopen("Etudiants.dat", "rb"); 
    
    int enseignantTrouve = 0;
    //on vérifie que l'enseignant n'est pas deja inscrit
    while (fread(&etudiantactuel, sizeof(Etudiant), 1, fp) == 1) {
                
                // Comparaison du nom et pr�nom
                if (strcmp(etudiant.nom, etudiantactuel.nom) == 0 && strcmp(etudiant.prenom, etudiantactuel.prenom) == 0) {
                // Affichage des informations de l'�tudiant
            enseignantTrouve = 1; 
            break;}}
            fclose(fp);
            int index = 0;
    if(enseignantTrouve == 0){
        showMessageDialogechec("Etudiant non inscrit! Veuillez vous inscrire");
        gtk_widget_hide(GTK_WIDGET(data));
        goToInscrireEtudiant(NULL, NULL); }else{
            GtkWidget *successDialog = gtk_message_dialog_new(GTK_WINDOW(windowInscriptionEnseignant),
                                                    GTK_DIALOG_MODAL,
                                                    GTK_MESSAGE_INFO,
                                                    GTK_BUTTONS_OK,
                                                    " Bienvenue %s %s", etudiantactuel.nom, etudiantactuel.prenom);
    gtk_dialog_run(GTK_DIALOG(successDialog));
    gtk_widget_destroy(successDialog);
     gtk_widget_hide(GTK_WIDGET(data));
       consulter(NULL, NULL);
        }
}










void consulter(GtkWidget *widget, gpointer data) {
    const char *path = "Etudiants.dat";;
    

    windowConsulter = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(windowConsulter), "Bulletin ");
    gtk_container_set_border_width(GTK_CONTAINER(windowConsulter), 10);
    gtk_widget_set_size_request(windowConsulter, 800, 600);
    g_signal_connect(windowConsulter, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Création d'un modèle de liste pour le tableau
    GtkListStore *store = gtk_list_store_new(3, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);

    char buffer[50];
    for (int i = 0; i < etudiantactuel.nombrematieres; i++) {
        GtkTreeIter iter;
        gtk_list_store_append(store, &iter);

        // Formatage de la note avec un seul chiffre après la virgule
        g_snprintf(buffer, sizeof(buffer), "%.1f", etudiantactuel.notes[i]);

        // Ajout des données dans le modèle de liste
        gtk_list_store_set(store, &iter, 0, etudiantactuel.matieres[i].nom, 1, etudiantactuel.matieres[i].nom_professeur, 2, buffer, -1);
    }

    // Création d'un tableau pour afficher les données
    GtkWidget *treeView = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
    g_object_unref(store);

    // Création des colonnes du tableau
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;

    // Création d'un label pour le relevé
    GtkWidget *labelReleve = gtk_label_new("Relevé pour l'année 2023-2024");
    gtk_label_set_use_markup(GTK_LABEL(labelReleve), TRUE);
    gtk_widget_set_name(labelReleve, "labelReleve");
    gtk_widget_set_halign(labelReleve, GTK_ALIGN_CENTER);

    renderer = gtk_cell_renderer_text_new();
    g_object_set(renderer, "font", "Sans Bold 10", NULL);
    column = gtk_tree_view_column_new_with_attributes("Matiere", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeView), column);

    renderer = gtk_cell_renderer_text_new();
    g_object_set(renderer, "font", "Sans Bold 10", NULL);
    column = gtk_tree_view_column_new_with_attributes("Nom Professeur", renderer, "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeView), column);

    renderer = gtk_cell_renderer_text_new();
    g_object_set(renderer, "font", "Sans Bold 10", NULL);
    column = gtk_tree_view_column_new_with_attributes("Note", renderer, "text", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeView), column);

    // Création d'un conteneur de défilement autour du tableau
    GtkWidget *scrolledWindow = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledWindow),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(scrolledWindow), treeView);

    // Création d'un bouton Quitter
    GtkWidget *quitButton = gtk_button_new_with_label("Quitter");
    g_signal_connect(quitButton, "clicked", G_CALLBACK(retourAccueil), windowConsulter);



     GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider, "C:/fac/c/projet/manny.css", NULL);

    

    // Appliquer le style au treeView (tableau)
    GtkStyleContext *contextTreeView = gtk_widget_get_style_context(treeView);
    gtk_style_context_add_provider(contextTreeView, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_widget_set_name(treeView, "treeview");  // Ajout de l'identifiant

    // Appliquer le style au bouton Quitter
    GtkStyleContext *contextQuitButton = gtk_widget_get_style_context(quitButton);
    gtk_style_context_add_provider(contextQuitButton, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_widget_set_name(quitButton, "quitButton");  // Ajout de l'identifiant

    GtkStyleContext *contextLabelReleve = gtk_widget_get_style_context(labelReleve);
    gtk_style_context_add_provider(contextLabelReleve, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    g_object_unref(provider);



    // Création d'un conteneur de boîte horizontale pour organiser le tableau et le bouton Quitter
    GtkWidget *hBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(hBox), scrolledWindow, TRUE, TRUE, 0);
   

    GtkWidget *spacer = gtk_label_new(NULL);
    gtk_widget_set_size_request(spacer, -1, 50);  
    
     GtkWidget *spacer2 = gtk_label_new(NULL);
    gtk_widget_set_size_request(spacer, -1, 50); // Hauteur de l'espace

    // Création d'un conteneur de boîte verticale pour organiser la boîte horizontale, les champs de saisie et le bouton Quitter
    GtkWidget *vBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(vBox), labelReleve, FALSE, FALSE, 0);
     gtk_box_pack_start(GTK_BOX(vBox), spacer, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vBox), hBox, TRUE, TRUE, 0);

   
    gtk_box_pack_start(GTK_BOX(vBox), spacer2, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vBox), gtk_label_new(NULL), FALSE, FALSE, 0);
     gtk_box_pack_start(GTK_BOX(vBox), gtk_label_new(NULL), FALSE, FALSE, 0);
    // Ajout du bouton Quitter dans la boîte verticale
    gtk_box_pack_start(GTK_BOX(vBox), quitButton, FALSE, FALSE, 0);

    // Ajout de la boîte verticale dans la fenêtre principale
    gtk_container_add(GTK_CONTAINER(windowConsulter), vBox);

    // Affichage de la fenêtre principale
    gtk_widget_show_all(windowConsulter);
}



void consulterEnseignant(GtkWidget *widget, gpointer data) {
    const char *path = "Etudiants.dat";;
    

    windowConsulter = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(windowConsulter), "Bulletin ");
    gtk_container_set_border_width(GTK_CONTAINER(windowConsulter), 10);
    gtk_widget_set_size_request(windowConsulter, 800, 600);
    g_signal_connect(windowConsulter, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Création d'un modèle de liste pour le tableau
    GtkListStore *store = gtk_list_store_new(3, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);

    GtkListStore *entryStore = gtk_list_store_new(1, G_TYPE_DOUBLE);

    char buffer[50];
    for (int i = 0; i < etudiantactuel.nombrematieres; i++) {
        GtkTreeIter iter;
        gtk_list_store_append(store, &iter);

        // Formatage de la note avec un seul chiffre après la virgule
        g_snprintf(buffer, sizeof(buffer), "%.1f", etudiantactuel.notes[i]);

        // Ajout des données dans le modèle de liste
        gtk_list_store_set(store, &iter, 0, etudiantactuel.matieres[i].nom, 1, etudiantactuel.matieres[i].nom_professeur, 2, buffer, -1);
        GtkTreeIter entryIter;
        gtk_list_store_append(entryStore, &entryIter);

        // Ajout des données initiales (par exemple, 0) dans le modèle de liste des champs de saisie
        gtk_list_store_set(entryStore, &entryIter, 0, 0.0, -1);  
    }

    // Création d'un tableau pour afficher les données
   treeView = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
    g_object_unref(store);

    // Création d'un tableau pour afficher les champs de saisie
    entryTreeView = gtk_tree_view_new_with_model(GTK_TREE_MODEL(entryStore));
    g_object_unref(entryStore);

    // Création des colonnes du tableau
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;

    // Création d'un label pour le relevé
    GtkWidget *labelReleve = gtk_label_new("Relevé pour l'année 2023-2024");
    gtk_label_set_use_markup(GTK_LABEL(labelReleve), TRUE);
    gtk_widget_set_name(labelReleve, "labelReleve");
    gtk_widget_set_halign(labelReleve, GTK_ALIGN_CENTER);

    renderer = gtk_cell_renderer_text_new();
    g_object_set(renderer, "font", "Sans Bold 10", NULL);
    column = gtk_tree_view_column_new_with_attributes("Matiere", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeView), column);

    renderer = gtk_cell_renderer_text_new();
    g_object_set(renderer, "font", "Sans Bold 10", NULL);
    column = gtk_tree_view_column_new_with_attributes("Nom Professeur", renderer, "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeView), column);

    renderer = gtk_cell_renderer_text_new();
    g_object_set(renderer, "font", "Sans Bold 10", NULL);
    column = gtk_tree_view_column_new_with_attributes("Note", renderer, "text", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeView), column);

    // Création d'un modèle de liste pour les champs de saisie
    GtkCellRenderer *entryRenderer = gtk_cell_renderer_spin_new();
    g_object_set(entryRenderer, "editable", TRUE, "adjustment", gtk_adjustment_new(0, 0, G_MAXDOUBLE, 1, 1, 0), NULL);

    // Création d'une nouvelle colonne pour les champs de saisie
    GtkTreeViewColumn *entryColumn = gtk_tree_view_column_new_with_attributes("Saisie", entryRenderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(entryTreeView), entryColumn);

    g_signal_connect(entryRenderer, "edited", G_CALLBACK(on_cell_edited), NULL);
    
    // Création d'un conteneur de défilement autour du tableau
    GtkWidget *scrolledWindow = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledWindow),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(scrolledWindow), treeView);

    // Création d'un bouton Quitter
    GtkWidget *quitButton = gtk_button_new_with_label("Quitter");
    g_signal_connect(quitButton, "clicked", G_CALLBACK(retourAccueil), windowConsulter);


     GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider, "C:/fac/c/projet/manny.css", NULL);

    

    // Appliquer le style au treeView (tableau)
    GtkStyleContext *contextTreeView = gtk_widget_get_style_context(treeView);
    gtk_style_context_add_provider(contextTreeView, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_widget_set_name(treeView, "treeview");  // Ajout de l'identifiant

    // Appliquer le style à entryTreeView (champs de saisie)
    GtkStyleContext *contextEntryTreeView = gtk_widget_get_style_context(entryTreeView);
    gtk_style_context_add_provider(contextEntryTreeView, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_widget_set_name(entryTreeView, "entrytreeview");  // Ajout de l'identifiant

    // Appliquer le style au bouton Quitter
    GtkStyleContext *contextQuitButton = gtk_widget_get_style_context(quitButton);
    gtk_style_context_add_provider(contextQuitButton, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_widget_set_name(quitButton, "quitButton");  // Ajout de l'identifiant

    GtkStyleContext *contextLabelReleve = gtk_widget_get_style_context(labelReleve);
    gtk_style_context_add_provider(contextLabelReleve, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    g_object_unref(provider);




    // Création d'un conteneur de boîte horizontale pour organiser le tableau, les champs de saisie et le bouton Quitter
    GtkWidget *hBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

    gtk_box_pack_start(GTK_BOX(hBox), scrolledWindow, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hBox), entryTreeView, FALSE, FALSE, 0);

        // Création d'un espace vide au-dessus du tableau
    GtkWidget *spacer = gtk_label_new(NULL);
    gtk_widget_set_size_request(spacer, -1, 50);  
    
     GtkWidget *spacer2 = gtk_label_new(NULL);
    gtk_widget_set_size_request(spacer, -1, 50); // Hauteur de l'espace

    // Création d'un conteneur de boîte verticale pour organiser la boîte horizontale, les champs de saisie et le bouton Quitter
    GtkWidget *vBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
     gtk_box_pack_start(GTK_BOX(vBox), labelReleve, FALSE, FALSE, 0);
     gtk_box_pack_start(GTK_BOX(vBox), spacer, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vBox), hBox, TRUE, TRUE, 0);

   
    gtk_box_pack_start(GTK_BOX(vBox), spacer2, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vBox), gtk_label_new(NULL), FALSE, FALSE, 0);
     gtk_box_pack_start(GTK_BOX(vBox), gtk_label_new(NULL), FALSE, FALSE, 0);
    // Ajout du bouton Quitter dans la boîte verticale
    gtk_box_pack_start(GTK_BOX(vBox), quitButton, FALSE, FALSE, 0);

    // Ajout de la boîte verticale dans la fenêtre principale
    gtk_container_add(GTK_CONTAINER(windowConsulter), vBox);

    // Affichage de la fenêtre principale
    gtk_widget_show_all(windowConsulter);

   
}







void rechercheEtudiant(GtkWidget *widget, gpointer data){
    Etudiant et;
    const gchar *nom = gtk_entry_get_text(GTK_ENTRY(entryNomModifier));
    const gchar *prenom = gtk_entry_get_text(GTK_ENTRY(entryPrenomModifier));

    strcpy(etudiantactuel.nom, nom);     strcpy(etudiantactuel.prenom, prenom); 
    FILE *fp = fopen("Etudiants.dat", "rb"); 
    
    int enseignantTrouve = 0;
    //on vérifie que l'enseignant n'est pas deja inscrit
    while (fread(&et, sizeof(Etudiant), 1, fp) == 1) {
                
                // Comparaison du nom et pr�nom
                if (strcmp(et.nom, etudiantactuel.nom) == 0 && strcmp(et.prenom, etudiantactuel.prenom) == 0) {
                // Affichage des informations de l'�tudiant
            enseignantTrouve = 1; 
            break;}}
            fclose(fp);
            if(enseignantTrouve == 1){
            GtkWidget *successDialog = gtk_message_dialog_new(GTK_WINDOW(windowInscriptionEnseignant),
                                                    GTK_DIALOG_MODAL,
                                                    GTK_MESSAGE_INFO,
                                                    GTK_BUTTONS_OK,
                                                    "Vous allez acceder aux notes de %s %s!", etudiantactuel.nom, etudiantactuel.prenom);
    gtk_dialog_run(GTK_DIALOG(successDialog));
    gtk_widget_destroy(successDialog);
     gtk_widget_hide(GTK_WIDGET(data));
       consulterEnseignant(NULL, NULL);}else{
        showMessageDialogechec("Etudiant non inscrit dans l'établissement");
}}






void modifierEtudiant(const char *nom, const char *prenom, double nouvelleNote1, double nouvelleNote2, double nouvelleNote3, double nouvelleNote4) {

    const char *path = "D:/c/projet/Test.dat";
    FILE *fp = fopen(path, "r+b");
    if (fp == NULL) {
        // G�rer l'erreur d'ouverture de fichier
        printf("Impossible d'ouvrir le fichier.\n");
        return; // Retourner en cas d'�chec de l'ouverture du fichier
    }

    Etudiant etud1; 
    strcpy(etud1.nom, nom); 
    strcpy(etud1.prenom, prenom); 
    etud1.notes[0] = nouvelleNote1;
    etud1.notes[1] = nouvelleNote2;
    etud1.notes[2] = nouvelleNote3;
    etud1.notes[3] = nouvelleNote4;
    
            int etudiantTrouve = 0;
            Etudiant etu;
                while (fread(&etu, sizeof(Etudiant), 1, fp) == 1) {
                     
                // Comparaison du nom et pr�nom
                if (strcmp(etu.nom, nom) == 0 && strcmp(etu.prenom, prenom) == 0) {
                etudiantTrouve = 1;
                // Positionnement du curseur de fichier � l'emplacement du dernier enregistrement lu
                fseek(fp, -((long)sizeof(Etudiant)), SEEK_CUR);

                // �criture des nouvelles informations dans le fichier
                fwrite(&etud1, sizeof( Etudiant), 1, fp);
                
                break;}

    fclose(fp);

    // Afficher un message en fonction du succ�s ou de l'�chec de la modification
    if(etudiantTrouve == 0){showMessageDialog("Etudiant non trouve.");}else {showMessageDialog("Les donnees ont ete modifiees.");}
    }}


    void validerModifier(GtkWidget *widget, gpointer data) {
        const gchar *rNom = gtk_entry_get_text(GTK_ENTRY(entryNomModifier));
        const gchar *rPrenom = gtk_entry_get_text(GTK_ENTRY(entryPrenomModifier));
        char *nom = g_strdup(rNom);
        char *prenom = g_strdup(rPrenom);

        double nouvellesNotes[4];

        // R�cup�rer les nouvelles notes � partir des champs de saisie
        for (int i = 0; i < 4; ++i) {
            const char *noteText = gtk_entry_get_text(GTK_ENTRY(entries[i][0]));
            nouvellesNotes[i] = atof(noteText);
        }

        // Appeler la fonction pour modifier l'�tudiant
        modifierEtudiant(nom, prenom, nouvellesNotes[0], nouvellesNotes[1], nouvellesNotes[2], nouvellesNotes[3]);

        // Lib�rer la m�moire allou�e avec g_strdup
        g_free(nom);
        g_free(prenom);
    }






void goToModifier(GtkWidget *widget, gpointer data) {

    gtk_widget_hide(windowAcceuilEnseignant);
    // Cr�er une nouvelle fen�tre de modification
    windowModifier = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_position(GTK_WINDOW(windowModifier), GTK_WIN_POS_CENTER);
    // D�finir une taille raisonnable pour la fen�tre (� ajuster selon vos besoins)
    gtk_window_set_default_size(GTK_WINDOW(windowModifier), 800, 600);

    gtk_window_set_title(GTK_WINDOW(windowModifier), "Modifier");
    g_signal_connect(windowModifier, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Cr�er une grille pour organiser les widgets
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);

    // Cr�er des �tiquettes et des champs de saisie pour le nom et le pr�nom
    GtkWidget *labelmodif = gtk_label_new("Entrez le nom de lélève que vous souhaitez noter :");
    GtkWidget *labelNom = gtk_label_new("Nom :");
    entryNomModifier = gtk_entry_new();

    GtkWidget *labelPrenom = gtk_label_new("Prenom :");
    entryPrenomModifier = gtk_entry_new();

    GtkWidget *buttonValiderSaisir = gtk_button_new_with_label("Valider");
    g_signal_connect(buttonValiderSaisir, "clicked", G_CALLBACK(rechercheEtudiant), windowModifier);

    GtkWidget *buttonRetour = gtk_button_new_with_label("Quitter");
    g_signal_connect(buttonRetour, "clicked", G_CALLBACK(retourIdentificationEnseignant), windowModifier);

    // Ajouter les �tiquettes et les champs de saisie � la grille
    gtk_grid_attach(GTK_GRID(grid), labelmodif, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), labelNom, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entryNomModifier, 1, 2, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), labelPrenom, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entryPrenomModifier, 1, 3, 1, 1);

    
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new(""), 0, 4, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new(""), 0, 5, 2, 1);


    gtk_grid_attach(GTK_GRID(grid), buttonValiderSaisir, 0, 6, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), buttonRetour, 1, 6, 1, 1);

    // Utilisation de GtkAlignment pour centrer la grille dans la fenêtre
    GtkWidget *alignment = gtk_alignment_new(0.5, 0.5, 0, 0);
    gtk_container_add(GTK_CONTAINER(alignment), grid);

    // Afficher la fenêtre de saisie
    gtk_container_add(GTK_CONTAINER(windowModifier), alignment);
    gtk_widget_show_all(windowModifier);

    
    gtk_widget_show_all(windowModifier);
}






void identificationEnseignant(GtkWidget *widget, gpointer data){
    gtk_widget_hide(windowAcceuil);
    windowIdentificationEnseignant = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(windowIdentificationEnseignant), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(windowIdentificationEnseignant), 600, 500);

    gtk_window_set_title(GTK_WINDOW(windowIdentificationEnseignant), "S'identifier en tant qu'enseignant");
    g_signal_connect(windowIdentificationEnseignant, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Création de la boîte verticale
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(windowIdentificationEnseignant), vbox);

    GtkWidget *buttonConnexionEnseignant = gtk_button_new_with_label("Se Connecter");
    g_signal_connect(buttonConnexionEnseignant, "clicked", G_CALLBACK(goToConnecterEnseignant), windowIdentificationEnseignant);

    GtkWidget *buttonInscriptionEnseignant = gtk_button_new_with_label("S'inscrire");
    g_signal_connect(buttonInscriptionEnseignant, "clicked", G_CALLBACK(goToInscrireEnseignant), windowIdentificationEnseignant);

    
     GtkWidget *buttonRetour = gtk_button_new_with_label("Retour");
    g_signal_connect(buttonRetour, "clicked", G_CALLBACK(retourAccueil), windowIdentificationEnseignant);

    // Ajout des boutons à la boîte verticale
    gtk_box_pack_start(GTK_BOX(vbox), buttonConnexionEnseignant, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), buttonInscriptionEnseignant, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), buttonRetour, TRUE, TRUE, 0);


    // Charger le style CSS à partir d'un fichier externe
    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(cssProvider, "C:/fac/c/projet/manny.css", NULL);
    // Appliquer le fournisseur de style aux contextes de style des widgets
    gtk_style_context_add_provider(gtk_widget_get_style_context(buttonConnexionEnseignant), GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    gtk_style_context_add_provider(gtk_widget_get_style_context(buttonInscriptionEnseignant), GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        gtk_style_context_add_provider(gtk_widget_get_style_context(buttonRetour), GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);


    // Affichage de la fenêtre et démarrage de la boucle principale
    gtk_widget_show_all(windowIdentificationEnseignant);
}



void identificationEtudiant(GtkWidget *widget, gpointer data){
    gtk_widget_hide(windowAcceuil);
    windowIdentificationEtudiant = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(windowIdentificationEtudiant), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(windowIdentificationEtudiant), 600, 500);

    gtk_window_set_title(GTK_WINDOW(windowIdentificationEtudiant), "S'identifier");
    g_signal_connect(windowIdentificationEtudiant, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Création de la boîte verticale
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(windowIdentificationEtudiant), vbox);

    buttonConnexionEtudiant = gtk_button_new_with_label("Se Connecter");
    g_signal_connect(buttonConnexionEtudiant, "clicked", G_CALLBACK(goToConnecterEtudiant), windowIdentificationEtudiant);

    buttonInscriptionEtudiant = gtk_button_new_with_label("S'inscrire");
    g_signal_connect(buttonInscriptionEtudiant, "clicked", G_CALLBACK(goToInscrireEtudiant), windowIdentificationEtudiant);

    buttonRetour = gtk_button_new_with_label("Retour");
    g_signal_connect(buttonRetour, "clicked", G_CALLBACK(retourAccueil), windowIdentificationEtudiant);


    // Ajout des boutons à la boîte verticale
    gtk_box_pack_start(GTK_BOX(vbox), buttonConnexionEtudiant, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), buttonInscriptionEtudiant, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), buttonRetour, TRUE, TRUE, 0);

    // Charger le style CSS à partir d'un fichier externe
    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(cssProvider, "C:/fac/c/projet/manny.css", NULL);
    // Appliquer le fournisseur de style aux contextes de style des widgets
    gtk_style_context_add_provider(gtk_widget_get_style_context(buttonConnexionEtudiant), GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    gtk_style_context_add_provider(gtk_widget_get_style_context(buttonInscriptionEtudiant), GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    gtk_style_context_add_provider(gtk_widget_get_style_context(buttonRetour), GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    // Affichage de la fenêtre et démarrage de la boucle principale
    gtk_widget_show_all(windowIdentificationEtudiant);
}





int main(int argc, char *argv[]) {

    viderFichier("Enseignants.dat");viderFichier("Etudiants.dat");recupmatieres();
    // Initialisation de GTK
    gtk_init(&argc, &argv);

    // Création de la fenêtre principale
    windowAcceuil = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(windowAcceuil), GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(windowAcceuil), "Interface GTK");
    gtk_window_set_default_size(GTK_WINDOW(windowAcceuil), 600, 500);
    g_signal_connect(windowAcceuil, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Création de la boîte verticale
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(windowAcceuil), vbox);

    // Création du label de bienvenue
    GtkWidget *label = gtk_label_new("Bienvenue dans ManiniManinotes, vous êtes ?");
    gtk_box_pack_start(GTK_BOX(vbox), label, TRUE, TRUE, 0);

    // Obtenir le contexte de style du label
    GtkStyleContext *labelStyleContext = gtk_widget_get_style_context(label);

    // Appliquer un style CSS au label
    gtk_style_context_add_class(labelStyleContext, "welcome-label");

    // Création du bouton Enseignant
    GtkWidget *buttonEnseignant = gtk_button_new();
    GtkWidget *buttonEnseignantBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *buttonEnseignantImage = gtk_image_new_from_file("C:/fac/c/projet/prof.png");
    GtkWidget *buttonEnseignantLabel = gtk_label_new("Enseignant");
    gtk_box_pack_start(GTK_BOX(buttonEnseignantBox), buttonEnseignantImage, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(buttonEnseignantBox), buttonEnseignantLabel, TRUE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(buttonEnseignant), buttonEnseignantBox);
    g_signal_connect(buttonEnseignant, "clicked", G_CALLBACK(identificationEnseignant), NULL);

    // Création du bouton Etudiant
    GtkWidget *buttonEleve = gtk_button_new();
    GtkWidget *buttonEleveBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *buttonEleveImage = gtk_image_new_from_file("C:/fac/c/projet/etudiant.png");
    GtkWidget *buttonEleveLabel = gtk_label_new("Etudiant");
    gtk_box_pack_start(GTK_BOX(buttonEleveBox), buttonEleveImage, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(buttonEleveBox), buttonEleveLabel, TRUE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(buttonEleve), buttonEleveBox);
    g_signal_connect(buttonEleve, "clicked", G_CALLBACK(identificationEtudiant), NULL);

    // Ajout des boutons à la boîte verticale
    gtk_box_pack_start(GTK_BOX(vbox), buttonEnseignant, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), buttonEleve, TRUE, TRUE, 0);

    // Charger le style CSS à partir d'un fichier externe
    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(cssProvider, "C:/fac/c/projet/manny.css", NULL);

    // Appliquer le fournisseur de style aux contextes de style des widgets
    gtk_style_context_add_provider(labelStyleContext, GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    gtk_style_context_add_provider(gtk_widget_get_style_context(buttonEnseignant), GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    gtk_style_context_add_provider(gtk_widget_get_style_context(buttonEleve), GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    // Affichage de la fenêtre et démarrage de la boucle principale
    gtk_widget_show_all(windowAcceuil);
    gtk_main();


    return 0;
}