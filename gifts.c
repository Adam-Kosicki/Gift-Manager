#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#define MAXPLAYERS 1000 //C
#define MAXLEN 100

double amounts[MAXPLAYERS];
char names[MAXPLAYERS][MAXLEN];
int numPlayers=0;

int findIndex(char *name){
	for (int i=0; i<numPlayers; i++)
		if (strcmp(name, names[i]) == 0)
			return i;
	
	return -1;
}

int main(int argc, char *argv[]){
	if (strcmp(argv[1], "new") == 0){
		FILE *foutput = fopen("gifts.dat", "w");
		for (int i=2; i<argc; i+=2){
			int len= strlen(argv[i]) + 1;
			//write the name length
			fwrite(&len, sizeof(int), 1, foutput);
			//write the name
			fwrite(argv[i], sizeof(char), len, foutput);
			//write the amount
			double amount = atof(argv[i+1]);
			fwrite(&amount, sizeof(double), 1, foutput);
			
			printf("%s %6.2f\n", argv[i], amount);
		}
		fclose(foutput);	
	}
	else {

	//process transaction
	
	FILE *finput = fopen("gifts.dat", "r");
	int i=0, len=0;
	while (fread(&len, sizeof(int), 1, finput) > 0){
		//read that many charaters as the name
		fread(&names[i][0], sizeof(char), len, finput);
		fread(&amounts[i], sizeof(double), 1, finput);
		i++;
	}
	fclose(finput);
	numPlayers = i;

//Command: gifts Jey 100 John Miguel
//argc:5     0    1   2   3    4

	//argv[1] is the giver
	int giverIndex = findIndex(argv[1]);
	double amountToSplit = atof(argv[2]);
	amounts[giverIndex] -= amountToSplit;

	double delta = amountToSplit / (argc-3);
	for (int i=3; i<argc; i++){
		int receiverIndex = findIndex(argv[i]);
		amounts[receiverIndex] += delta;
	}
	
	FILE *foutput = fopen("gifts.dat", "w");
        for (int i=0; i<numPlayers; i++){
            int len= strlen(names[i]) + 1;
            //write the name length
            fwrite(&len, sizeof(int), 1, foutput);
            //write the name
            fwrite(names[i], sizeof(char), len, foutput);
            fwrite(&amounts[i], sizeof(double), 1, foutput);
            printf("%10s %6.2f\n", names[i], amounts[i]);
        }
              fclose(foutput);
	}
}
