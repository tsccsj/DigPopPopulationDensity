#include <stdio.h>
#include <string.h>
#include <gdal.h>
#include <ogr_srs_api.h>
#include <ogr_api.h>
#include <cpl_conv.h>

int getHHNum(FILE * file) {
	int hhCount = 0;

	rewind(file);
	char line[4000];
	fgets(line, 4000, file);
	
	while(NULL != fgets(line, 4000, file)) {
		hhCount ++;
	}
	
	return hhCount;
}


int readPoints(FILE * file, bool hhOnly, int nHH, float * xCor, float * yCor, int * pCount, char * personColName) {
	char * att[1000];
	int numHHAtt;

	int personColID = -1; 

	rewind(file);
	char line[8000];

	if(NULL == (fgets(line, 8000, file))) {
		printf("ERROR: Corrupted household file at line 1.\n");
		return false;
	} 
	
	if(!hhOnly) {
		char * cName = strtok(line, ",");
		
		for(int i = 1;; i++) {
			cName = strtok(NULL, ",");

			if(cName == NULL) {
				break;
			}
			
			if(strcmp(personColName, cName) == 0)	{
				personColID = i;
			}
			
		}

		if(personColID < 0) {
			printf("ERROR: incorrect population-per-houshold column name: %s.\n", personColName);
			return false;
		}
	}

	int totalCount = 0;

	for(int i = 0; i < nHH; i++) {
		if(NULL == (fgets(line, 8000, file))) {
			printf("ERROR: Corrupted household file at line %d.\n", i + 2);
			return false;
		}

		xCor[i] = atof(strtok(line, ","));
		yCor[i] = atof(strtok(NULL, ","));

		if(hhOnly) {
			pCount[i] = 1;
			totalCount ++;
		}
		else {
			for(int j = 2; j < personColID; j++) {
				strtok(NULL, ",");
			}
			pCount[i] = atoi(strtok(NULL, ","));
			totalCount += pCount[i];
		}
	}

	return totalCount;
}

void writeGeoTiffF(char * fileName, float * result, int nRow, int nCol, float xMin, float yMax, float cellSize, int epsgCode) {
	
	GDALAllRegister();
	OGRRegisterAll();

	GDALDatasetH hDstDS;
	GDALDriverH hDriver;
	GDALRasterBandH hBand;
	OGRSpatialReferenceH hSRS;
	char *pszSRS_WKT = NULL;
	double adfGeoTransform[6];

	char *papszOptions[] = {"COMPRESS=LZW",NULL};
	const char *pszFormat="GTiff";

	if(NULL == (hDriver = GDALGetDriverByName(pszFormat))) {
		printf("ERROR: hDriver is null cannot output using GDAL\n");
		exit(1);
	}
	
	hDstDS = GDALCreate(hDriver, fileName, nCol, nRow, 1, GDT_Float32, papszOptions);

	adfGeoTransform[0] = xMin;
	adfGeoTransform[1] = cellSize;
	adfGeoTransform[2] = 0;
	adfGeoTransform[3] = yMax;
	adfGeoTransform[4] = 0;
	adfGeoTransform[5] = -cellSize;

	GDALSetGeoTransform(hDstDS,adfGeoTransform);

	hSRS=OSRNewSpatialReference(NULL);
	OSRImportFromEPSG(hSRS,epsgCode);
	OSRExportToWkt(hSRS,&pszSRS_WKT);
	GDALSetProjection(hDstDS,pszSRS_WKT);
	OSRDestroySpatialReference(hSRS);
	CPLFree(pszSRS_WKT);

	hBand=GDALGetRasterBand(hDstDS,1);
	GDALSetRasterNoDataValue(hBand,-1);
	GDALRasterIO(hBand, GF_Write, 0, 0, nCol, nRow, result, nCol, nRow, GDT_Float32, 0, 0 );
	
	GDALClose(hDstDS);

	return;
}

