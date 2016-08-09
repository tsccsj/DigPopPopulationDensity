#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "io.h"
#include "kde.h"

void kernelDensity(char * inputHousehold, bool hhOnly, char * kdeOutFile, float xMin, float yMin, float xMax, float yMax, float cellSize, float bandwidth, char * personColName, int projCode) {

	float * xCor;
	float * yCor;

	int * pCount;
	int nRow, nCol, nHH, totalCount;
	float * density;

	FILE * inputHHData;

	if(NULL == (inputHHData = fopen(inputHousehold, "r"))) {
		printf("ERROR: Can't open input household file: %s\n", inputHousehold);
		exit(1);
	}

//Cells
	nCol = ceil((xMax - xMin)/cellSize);
	nRow = ceil((yMax - yMin)/cellSize);
	xMax = xMin + cellSize * nCol;
	yMax = yMin + cellSize * nRow;

//Points
	nHH = getHHNum(inputHHData);
	if(NULL == (xCor = (float *)malloc(sizeof(float) * nHH))) {
		printf("ERROR: Out of memory in line %d!\n", __LINE__);
		exit(1);
	}
	if(NULL == (yCor = (float *)malloc(sizeof(float) * nHH))) {
		printf("ERROR: Out of memory in line %d!\n", __LINE__);
		exit(1);
	}
	if(NULL == (pCount = (int *)malloc(sizeof(int) * nHH))) {
		printf("ERROR: Out of memory in line %d!\n", __LINE__);
		exit(1);
	}

	totalCount = readPoints(inputHHData, hhOnly, nHH, xCor, yCor, pCount, personColName);

	fclose(inputHHData);
	
	if(NULL == (density = (float *) malloc(sizeof(float) * nRow * nCol))) {
		printf("ERROR: Out of memory in %d!\n", __LINE__);
		exit(1);
	}

//KDE goes here
	
	kde(density, nRow, nCol, cellSize, xMin, yMax, xCor, yCor, pCount, nHH, bandwidth);



//Write result and free resources
	free(xCor);
	free(yCor);
	free(pCount);

	writeGeoTiffF(kdeOutFile, density, nRow, nCol, xMin, yMax, cellSize, projCode);
	free(density);
}
		
//DigPopPopulationDensity OutputFileName InputDigPopNamePreNumber MinRzn NumberRzn ProjectionCode NorthBounds EastBounds SoutBound WestBounds LatticeSize KernelDiameter HouseholdOrPopulationMap [PersonsPerHousehold](if doing population "P" studies) 
int main(int argc, char ** argv) {

	if(argc != 14 && argc != 13) {
		printf("Incorrect number of input arguments:\n");
		printf("./DigPopPopulationDensity <OutputFileName> <InputDigPopNamePreNumber> <MinRzn> <NumberRzn> <ProjectionCode> <NorthBounds> <EastBounds> <SoutBound> <WestBounds> <LatticeSize> <KernelDiameter> <HouseholdOrPopulationMap> <PersonsPerHousehold>\n");
	}

	char * personColName;

	bool hhOnly;
	if(strcmp("H",argv[12]) == 0) {
		hhOnly = true;
	}
	else {
		hhOnly = false;
		personColName = argv[13];
	}

	char * outputFileName = argv[1];
	char * inputFileName = argv[2];

	int minRzn = atoi(argv[3]);
	int numRzn = atoi(argv[4]);

	float xMin = atof(argv[9]);
	float xMax = atof(argv[7]);
	float yMin = atof(argv[8]);
	float yMax = atof(argv[6]);

	int projCode = atoi(argv[5]);

	float cellSize = atof(argv[10]);
	float bandwidth = atof(argv[11]);


	char inputHousehold[200];
	char kdeOutFile[200];

	for(int i = 0; i < numRzn; i++) {
		sprintf(inputHousehold, "%s%03d-households.csv", inputFileName, (minRzn + i));
		sprintf(kdeOutFile, "%s_rzn%03d.tif", outputFileName, (minRzn + i));
		kernelDensity(inputHousehold, hhOnly, kdeOutFile, xMin, yMin, xMax, yMax, cellSize, bandwidth, personColName, projCode);
	}

	

	return 0;
	
}
