#include <stdio.h>
#include <math.h>

void kde(float * density, int nRow, int nCol, float cellSize, float xMin, float yMax, float * xCor, float * yCor, int * pCount, int nHH, float bandwidth) {
	int searchWindow = (int)(ceil(bandwidth / cellSize));
	int wXMin, wYMin, wXMax, wYMax;

	float cellX, cellY;
	float dist2;
	float bandwidth2 = bandwidth * bandwidth;

	int rowID, colID;
	
	for(int i = 0; i < nRow * nCol; i++) {
		density[i] = 0.0f;
	}

	if(bandwidth < 1) {
		for(int k = 0; k < nHH; k++) {
			rowID = (int)((yMax - yCor[k]) / cellSize);
			colID = (int)((xCor[k] - xMin) / cellSize);
			density[rowID * nCol + colID] += pCount[k];
		}
	}
	else {
		for(int k = 0; k < nHH; k++) {
			rowID = (int)((yMax - yCor[k]) / cellSize);
			colID = (int)((xCor[k] - xMin) / cellSize);

			wXMin = colID - searchWindow;
			wXMax = colID + searchWindow + 1;
			wYMin = rowID - searchWindow;
			wYMax = rowID + searchWindow + 1;

			if(wXMin < 0) {
				wXMin = 0;
			}
			if(wXMax > nCol) {
				wXMax = nCol;
			}
			if(wYMin < 0) {
				wYMin = 0;
			}
			if(wYMax > nRow) {
				wYMax = nRow;
			}

			for(int m = wYMin; m < wYMax; m++) {
				for(int n = wXMin; n < wXMax; n++) {
					cellX = xMin + cellSize * (n + 0.5);
					cellY = yMax - cellSize * (m + 0.5);
					dist2 = (cellX - xCor[k]) * (cellX - xCor[k]) + (cellY - yCor[k]) * (cellY - yCor[k]);
					if(dist2 < bandwidth2) {
						density[m * nCol + n] += pCount[k] + (1 - dist2/bandwidth2) * (1 - dist2/bandwidth2);
					}
				}
			}		
		}
	}
}
