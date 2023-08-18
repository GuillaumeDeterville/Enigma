#define _CRT_SECURE_NO_WARNINGS
#define LGMAXCHAINE 512
#define BUFFER_LENGTH 50
#define KEY_LENGTH 26
#define NO_ERROR 0
#define FILE_OPENING_ERROR_CODE -1
#define FILE_STRUCTURE_ERROR_CODE -2
#define MEMORY_ALLOCATION_ERROR_CODE -3
#define FILE_CLOSING_ERROR_CODE -4
#define KEY_LENGTH_ERROR_CODE -5
#define KEY_NO_CAPITAL_LETTER_ERROR_CODE -6
#define KEY_ELEMENTS_DIFFERENT_ERROR_CODE -7
#define KEY_NO_SYMETRICAL -8

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



/*
PROJET DE PROGRAMMATION PROCÉDURALE
Complétez :
Nom prénom étudiant 1 : Pierret Cyril
Nom prénom étudiant 2 : Deterville-françois Guillaume
*/

//You can (must) use this function as is in your program
char* Lire_Chaine_Dynamique(char invitation[])
{
	char tampon[LGMAXCHAINE];
	char* chaine;
	printf("%s", invitation);
	// gets(tampon);	// version originale de la fonction, qui n'est plus acceptee par visual studio
	gets_s(tampon, LGMAXCHAINE);	// version "securisee" de visual studio, pas acceptee par d'autres compilateurs
	chaine = (char*)malloc((strlen(tampon) + 1) * sizeof(char));
	if (chaine != NULL) strcpy(chaine, tampon);
	return chaine;
}


//Additionnals prototypes
//add your additional prototypes here
void LibererTableau(int dim, char** tab) // pour les tableaux a deux dim 
{
	for (int i = dim; i > 0; i--)
	{
		free(tab[i-1]);
	}
	  free(tab);  
}
int Lire_Entier(char invitation[], int* entier)
{
	char* chaine, fin[51];
	int temoin = 0; float reel;

	chaine = Lire_Chaine_Dynamique(invitation);
	if (!chaine) return 0;

	temoin = sscanf_s(chaine, "%f%s", &reel, fin, 51);
	if (temoin != 1) return 0;
	if (reel != (int)reel) return 0;

	*entier = (int)reel;
	free(chaine);
	return 1;

}
int LireCryptDecrypt() // on a cree une fonction pour prendre la valeur de crypt ou decrypt, 
{
	char* reponse;
	int  resultat, t;
	do
	{
		reponse = Lire_Entier("entrez 1 pour direct et -1 pour indirect : \n",&resultat);
		t = abs(resultat);
	} while (t!= 1);

	return resultat;
} 
char* ChaineMajuscule(char chaine[])
{
	unsigned int i;
	for ( i = 0; i < strlen(chaine); i++)
	{
		if (chaine[i] >= 'a' && chaine[i] <= 'z')
		{
			chaine[i] = chaine[i] - 32;
		}
	}
	return chaine;
}



// PROTOTYPES OBLIGATOIRES (NE PEUVENT PAS ETRE CHANGES)================================
// Step 1 ------------------------------------------------------------------------------
char use_key(char c, char key[], int offset, int direct);
void display_error_message_from_error_code(int code);

// Step 2 ------------------------------------------------------------------------------
int check_key(char key[]);

int read_module_configuration(char csv_row[],char key[],int* position,int* period); // partionne le fichier excel et on le range dans les tableaux

char** read_configuration(char path[],int* positions[], int* period[], int* n_mods, int* error_code); // lit le fichier excel 

// Step 3 ------------------------------------------------------------------------------

char* crypt(char message[],int n_mods,char* keys[],int positions[],int periods[],int encrypt_or_decrypt, int * error_code);

void new_positions(int n, int n_mods, int positions[], int periods[]);

// Step 4 Seconde session ---------------------------------------------------------------

char* create_key(int * error_code);

int check_symmetrical_key(char key[]);

char* create_symmetrical_key(int* error_code);

char* read_filename_csv(int* error_code);

char* scramble_message(char message[], int* error_code);


// MAIN ================================================================================
main()
{
	 // ETAPE 1 : L'UTILISATEUR CHOISIT SON MODE ET SON FICHIER 

	int mode;
	char* path = NULL;
	int erreur = 0;

	mode = LireCryptDecrypt();

	path = read_filename_csv(&erreur);
	if (erreur != NO_ERROR)
	{
		display_error_message_from_error_code(erreur);
		return NULL;
	}

	// ETAPE 2 : ON GENERE UNE CLE ALEATOIRE ET ON LA TEST + CLE SYMMETRIQUE 

	char* randomKey = NULL;
	char* symmetricalKey = NULL;

	randomKey = create_key(&erreur);
	if (erreur != NO_ERROR)
	{
		display_error_message_from_error_code(erreur);
		free(path);
		return NULL;
	}

	if (check_key(randomKey) != 0)
	{
		display_error_message_from_error_code(check_key(randomKey));
		free(path);
		free(randomKey);
		return NULL;
	}
	printf("La cle aleatoire est : %s \n", randomKey);

	symmetricalKey = create_symmetrical_key(&erreur);
	if (erreur != 0)
	{
		display_error_message_from_error_code(erreur);
		free(path);
		free(randomKey);
		return NULL;
	}
	if (check_symmetrical_key(symmetricalKey) != 0)
	{
		display_error_message_from_error_code(check_symmetrical_key(symmetricalKey));
		free(path);
		free(randomKey);
		free(symmetricalKey);
		return NULL;
	}

	printf("La cle symmetrique est : %s \n", symmetricalKey);

	
	// ETAPE 3 :  ON LIT LE FICHIER EXCEL ET ON RANGE LES DONNEES DANS DES TABLEAUX 
	
	int nb = 0;
	int* position = NULL;
	int* period = NULL;
	char** tableau_des_clés = NULL;


	tableau_des_clés=read_configuration(path, &position, &period, &nb, &erreur); 
	if (erreur != NO_ERROR)
	{
		display_error_message_from_error_code(erreur); 
		free(path);
		free(randomKey);
		free(symmetricalKey);
		return erreur;  
	}

	// ETAPE 4 : ON CRYPTE ET ON SCRAMBLE LE MESSAGE 

	char* message = NULL;
	char* messagescramble = NULL;
	char* msgcrypte = NULL;

	  message = Lire_Chaine_Dynamique("Votre message : \n");

	  message = ChaineMajuscule(message);

	  msgcrypte = crypt(message, nb, tableau_des_clés, position, period, mode, &erreur);
	  if (erreur != NO_ERROR)
	  {
		  display_error_message_from_error_code(erreur);
		  free(path);
		  free(randomKey);
		  free(symmetricalKey);
		  free(position);
		  free(period);
		  free(message);
		  LibererTableau(nb, tableau_des_clés);

		  return NULL;
	  }

	  printf("Le message crypte est : %s \n", msgcrypte);

	  messagescramble = scramble_message(msgcrypte, &erreur);
	  if (erreur != NO_ERROR)
	  {
		  display_error_message_from_error_code(erreur);
		  free(path);
		  free(randomKey);
		  free(symmetricalKey);
		  free(position);
		  free(period);
		  free(message);
		  free(msgcrypte);
		  LibererTableau(nb, tableau_des_clés);

		  return NULL;
	  }

	  printf("Le message crypte et sramble est : %s \n", messagescramble);

	  //LIBERATION DES TABLEAUX

	    free(path);
		free(randomKey);
		free(symmetricalKey);
	    free(message);
	    free(msgcrypte);
	    free(position);
	    free(period);
		free(messagescramble);
	    LibererTableau(nb, tableau_des_clés);

		return NO_ERROR;

}

// FUNCTIONS ===========================================================================
// Step 1 ------------------------------------------------------------------------------
/*
* Reminder : espacialy for this function (but for whole program in general) :
* Start with the simplest case, and progress step by step by testing your code between each step
* Don't foreget to test this function on moodle (see statement) for each step
*/

char use_key(char c, char key[], int offset, int direct) // fonction du prof mais pas optimisé, fonction du chien oui 
{
	{
		int pos = 0;
		offset = offset % KEY_LENGTH;
		for (unsigned int i = 0; i < strlen(key); i++)
		{
			if (c == key[i])
				pos = i;
		}

		if (offset < 0 && (c + offset) < 'A')
		{
			offset = offset + 26;
		}

		if (c < 'A' || c > 'Z')
			return c;
		if (direct == 1)
		{
			return key[(c - 'A' + offset) % KEY_LENGTH];
		}
		else
		{
			return 'A' + (pos - offset + KEY_LENGTH) % KEY_LENGTH;
		}
	}
}

void display_error_message_from_error_code(int code)
{
	switch (code)
	{
	case NO_ERROR: printf("pas d'erreur \n");
		break;
	case FILE_OPENING_ERROR_CODE : printf("Erreur d ouverture de fichier ! \n");
		break;
	case FILE_STRUCTURE_ERROR_CODE: printf("Erreur de structure du fichier ! \n");
		break;
	case MEMORY_ALLOCATION_ERROR_CODE : printf("Probleme d allocation de memoire ! \n");
		break;
	case FILE_CLOSING_ERROR_CODE : printf("Probleme lors de la fermeture du ficher ! \n");
		break;
	case KEY_LENGTH_ERROR_CODE : printf("La longeur de la cle n est pas correcte \n");
		break;
	case KEY_NO_CAPITAL_LETTER_ERROR_CODE  : printf(" la cle n est pas en majuscule ! \n");
		break;
	case KEY_ELEMENTS_DIFFERENT_ERROR_CODE : printf("la cle possede des doublons ! \n");
		break;

	case KEY_NO_SYMETRICAL: printf("la cle n est pas symmetrique ! \n");
		break;
	}
}


// Step 2 ------------------------------------------------------------------------------

int read_module_configuration(char csv_row[], char key[], int* position, int* period)
{
	unsigned int i, j;

	for (j = 0; j < strlen(csv_row); j++)
	{
		if (csv_row[j] == ';')
			break;
	}
	
	for (i = 0; i < j; i++)
	{
		key[i] = csv_row[i];
	}
	key[j] = '\0';

	char cara;
	if (sscanf(&(csv_row[j+1]), "%d%c%d", position, &cara, period) != 3)
		return FILE_STRUCTURE_ERROR_CODE; // pour une erreur de structure de fichier

	return check_key(key);
}

int check_key(char key[])
{
	// on verifie la longueur
	if (strlen(key) != 26)
		return KEY_LENGTH_ERROR_CODE;
	// on verifie les majuscules 
	for (unsigned int i = 0; i < strlen(key); i++)
	{
		if (key[i] >= 'a' && key[i] <= 'z')
			return KEY_NO_CAPITAL_LETTER_ERROR_CODE;
	}
	for (unsigned int i = 0; i < strlen(key); i++)
	{
		if (key[i] > 'Z' || key[i] < 'A')
			return KEY_ELEMENTS_DIFFERENT_ERROR_CODE;
	}
	// on verifie les doublons 
	for (unsigned int i = 0; i < 25; i++)  // la longueur a deja ete verifiee 
	{
		for (unsigned int j = i + 1; j < 26; j++)
		{
			if (key[i] == key[j])
				return KEY_ELEMENTS_DIFFERENT_ERROR_CODE;
		}
	}
	return NO_ERROR; // tout c est bien passe la cle est valide 
}

char** read_configuration(char path[],int* positions[], int* period[], int* n_mods, int* error_code) 
{
	
	//OUVERTURE DU FICHIER 

	FILE* pfichier = fopen(path, "r");
	if (pfichier == NULL)
	{
		*error_code = FILE_OPENING_ERROR_CODE;
		return NULL;
	}

	char tab[LGMAXCHAINE];
	// on comtpe le nombre de ligne 
	int compteur = -1;
	while (fgets(tab, LGMAXCHAINE, pfichier) != NULL)
	{
		compteur++;
	}
	
	rewind(pfichier);
	fgets(tab, LGMAXCHAINE, pfichier); // on passe le header dans l'excel 

	*n_mods = compteur;

	//ALLOCATION DES TABLEAUX

	char** tabkey = NULL;
	{
		

		*positions = (int*)malloc((*n_mods) * sizeof(int));
		if (*positions == NULL)
		{
			*error_code = MEMORY_ALLOCATION_ERROR_CODE;
			fclose(pfichier);
			return NULL;
		}

		*period = (int*)malloc((*n_mods) * sizeof(int));
		if (*period == NULL)
		{
			*error_code = MEMORY_ALLOCATION_ERROR_CODE;
			free(*positions);
			fclose(pfichier);
			return NULL;
		}

		tabkey = (char**)malloc((*n_mods) * sizeof(char*));
		if (tabkey == NULL)
		{
			*error_code = MEMORY_ALLOCATION_ERROR_CODE;
			free(*positions);
			free(*period);
			fclose(pfichier);
			return NULL;
		}

		for (int i = 0; i < *n_mods; i++)
		{
			tabkey[i] = (char*)malloc((KEY_LENGTH + 1) * sizeof(char));
			if (tabkey[i] == NULL)
			{
				*error_code = MEMORY_ALLOCATION_ERROR_CODE;
				free(*positions);
				free(*period);
				LibererTableau(i - 1, tabkey);
				fclose(pfichier);
				return NULL;
			}
		}
	}
	
	int i = 0;
	while (fgets(tab, LGMAXCHAINE, pfichier) != NULL)
	{
		
		*error_code =  read_module_configuration(tab, tabkey[i], &((*positions)[i]), &((*period)[i]));
		if (*error_code != 0)
		{
			free(*positions);
			free(*period);
			LibererTableau(*n_mods, tabkey);
			fclose(pfichier);
			return NULL;
		}
		 i++;
	} 
	
	//fermer le fichier 
	rewind(pfichier); 
	fclose(pfichier);
	*error_code = NO_ERROR;
		return tabkey;
}

// Step 3 ------------------------------------------------------------------------------

char* crypt(char message[], int n_mods, char* keys[], int positions[], int periods[], int encrypt_or_decrypt, int* error_code)
{
	unsigned int i;
	int j;

	char* code;
	code = malloc(sizeof(char) * (strlen(message) + 1));
	if (code == NULL)
	{
		*error_code = MEMORY_ALLOCATION_ERROR_CODE;
		return NULL;
	}
	strcpy(code, message);
	

	for ( i = 0; i < strlen(code); i++) // boucle pour chaque caractère
	{
		for (j = 0; j < n_mods - 1; j++) // on passe dans chaque modules
			code[i] = use_key(code[i], keys[j], positions[j] % KEY_LENGTH, 1);

		j = n_mods - 1; // reflecteur mode direct en codage et indirect en decodage

		code[i] = use_key(code[i], keys[j], positions[j] % KEY_LENGTH, encrypt_or_decrypt);
		
		for (j = n_mods - 2; j >= 0; j--)
			code[i] = use_key(code[i], keys[j], positions[j] % KEY_LENGTH, -1);

		if(code[i] >= 'A' && code[i]<='Z')
			new_positions(i + 1, n_mods, positions, periods); // après le codage d'un caractère, rotation des rotors pour le caractère suivant. 

	}

	*error_code = NO_ERROR;
	return code; 
	
} 

void new_positions(int n, int n_mods, int positions[], int periods[])
{
	int j;

	for (j = 0; j < n_mods; j++)
	{
		if (periods[j] != 0)
		{
			if (n % periods[j] == 0)
			{
				positions[j]++;
			}
		}
	}
}

// Step 4 Seconde session -----------------------------------------------------------------------------------------

char* create_key(int* error_code)
{
	srand(time(NULL));
	int var = KEY_LENGTH;
	int lettre = 0;
	char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char tampon[KEY_LENGTH+1];

	for (unsigned int i = 0; i < KEY_LENGTH; i++)
	{
		lettre = rand() % var;
		tampon[i] = alphabet[lettre];
		for ( unsigned int j = lettre; j < strlen(alphabet) - 1; j++)
		{
			alphabet[j] = alphabet[j + 1];
		}
		var--;
	}
	tampon[26] = '\0';
	char* chaine = (char*)malloc(sizeof(char) * (strlen(tampon)+1));
	if (chaine == NULL)
	{
		*error_code = MEMORY_ALLOCATION_ERROR_CODE;
		return NULL;
	}
	strcpy(chaine, tampon);
	*error_code = NO_ERROR;
	return chaine;
}

char* read_filename_csv(int* error_code)
{
	char temp[100];
	char* fin = NULL;
	char* mot = Lire_Chaine_Dynamique("entrez un fichier : \n");

	strcpy(temp, mot);
	fin = strchr(temp, '.');
	if (fin != NULL)
	{
		if (strcmp(temp, ".csv") != -1 )
			return mot;
	}

	char tampon[100];
	char* fichier;

	strcpy(tampon, mot);
	strcat(tampon, ".csv");

	fichier = (char*)malloc(sizeof(char) * (strlen(tampon) + 1));
	if (fichier == NULL)
	{
		*error_code = MEMORY_ALLOCATION_ERROR_CODE;
		free(mot);
		return NULL;
	}

	strcpy(fichier, tampon);

	free(mot);
	*error_code = NO_ERROR;
	return fichier;

}

char* create_symmetrical_key(int* error_code)
{
	srand(time(NULL));
	int var = KEY_LENGTH;
	int lettre = 0;
	char alphabet1[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char alphabet2[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char tampon[] = "__________________________";
	char* key = NULL;
	int a;

	for (unsigned int i = 0; i < KEY_LENGTH; i++)
	{
		if (tampon[i] == '_')
		{
			lettre = rand() % var;
			tampon[i] = alphabet2[lettre];

			for (unsigned int j = lettre; j < strlen(alphabet2) - 1; j++)
				alphabet2[j] = alphabet2[j + 1];
			if (tampon[i] == alphabet1[i])
				var = var - 1;
			else
			{
				for (unsigned int j = 0; j < strlen(alphabet2) - 1; j++)
				{
					alphabet2[j] = alphabet2[j + 1];
				}
				var = var - 2;
				for (a = 0; a < KEY_LENGTH; a++)
				{
					if (tampon[i] == alphabet1[a])
						tampon[a] = alphabet1[i];
				}
			}
		}
	}

	key = (char*)malloc(sizeof(char) * (strlen(tampon) + 1));
	if (key == NULL)
	{
		*error_code = MEMORY_ALLOCATION_ERROR_CODE;
		return NULL;
	}
	strcpy(key, tampon);
	*error_code = NO_ERROR;
	return key;
}

int check_symmetrical_key(char key[])
{

	for (unsigned int i = 0; i < strlen(key); i++)
	{
		if (use_key(key[i], key, 0, 1) != use_key(key[i], key, 0, -1))
		{
			return KEY_NO_SYMETRICAL;
		}
	}

	return NO_ERROR;
}

char* scramble_message(char message[], int* error_code)
{
	char tampon[100];
	int i = 0;
	int j = 0;
	while (message[i] != '\0')
	{
		if ((j % 6 == 0))
		{
			tampon[j] = ' ';
			j++;
		}
		if ((message[i] < '0' || message[i] > '9') != 1)
		{
			tampon[j] = message[i];
			j++;
		}
		if ((message[i] < 'A' || message[i] > 'Z')!=1) // si compris entre A et Z, alors on entre dans la condition
		{
			tampon[j] = message[i];
			j++;
		}
		i++;
	}
	tampon[j] = '\0';

	char* chaine = (char*)malloc(sizeof(char) * (strlen(tampon) + 1));
	if (chaine == NULL)
	{
		*error_code = MEMORY_ALLOCATION_ERROR_CODE;
		return NULL;
	}

	strcpy(chaine, tampon);
	*error_code = NO_ERROR;
	return chaine;

}