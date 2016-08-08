#ifndef IOH
#define IOH

int getHHNum(FILE * file);
int readPoints(FILE * file, bool hhOnly, int nHH, float * xCor, float * yCor, int * pCount, char * personColName);
void writeGeoTiffF(char * fileName, float * result, int nRow, int nCol, float xMin, float yMax, float cellSize, int epsgCode);



#endif

