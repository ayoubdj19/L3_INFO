#include <stdio.h>
#include <stdlib.h>

int OracleParenthesage(FILE * input, int reponse) {
  
  /* A completer */
  return reponse;
}

int main(int argc, char * argv[]) {
  FILE * input;
  FILE * output;
  char output_string[25];
  char *s;
  /* Valeur booleenne de la sortie (vrai pour "Bon parenthesage") */
  int output_value;

  if (argc < 3) {
    printf("Usage: %s <input_file> <output_file>\n", argv[0]);
    return 1;
  }

  input = fopen(argv[1], "r");
  if (input == NULL) {
    fprintf(stderr, "Erreur lors de l'ouverture du fichier %s\n", argv[1]);
    return 1;
  }

  output = fopen(argv[2], "r");
  if (output == NULL) {
    fprintf(stderr, "Erreur lors de l'ouverture du fichier %s\n", argv[2]);
    fclose(input);
    return 1;
  }

  s = fgets(output_string, 25, output);
  if (s==NULL) {
    fprintf(stderr, "Erreur lors de la lecture dans le fichier %s\n", argv[2]);
    fclose(input);
    fclose(output);
    return 1;
  }
  
  if (strcmp(output_string,"Bon parenthesage\n") == 0) {
    output_value = 1;
  } else if (strcmp(output_string,"Mauvais parenthesage\n") == 0) {
    output_value = 0;
  } else {
    printf("ERREUR\n");
    fclose(input);
    fclose(output);
    return 1;
  }

  if (OracleParenthesage(input,output_value)) {
    printf("OK\n");
    fclose(input);
    fclose(output);
    return 0;
  } else {
    printf("ERREUR\n");
    fclose(input);
    fclose(output);
    return 1;
  }
}
