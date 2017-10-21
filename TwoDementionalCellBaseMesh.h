#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <stdlib.h>
using namespace std; 

class TwoDementionalCellBaseMesh
{
	private :

		//-- Declare main Variable
		string line;
		double doubleIntlineNumber;
		int IntlineNumber, LineState;
		
		//-- Declare Counter Variable
		int NCCounter, CornCheck, Nface, NPCoordinatecounter, NBECounter, NECCounter ,BEPCounter, NBECount;
		
		//-- Declare Vector For Array value
		vector<int> NCval;
		vector<int> BEPval;
		vector<int> BFPval;
		vector<double> NPVal;

	public:

		int NumberOfPoint , NumberOfCell, NumberofBoundaryCurves, NBE,NBS,zcen;
		int NEC[10];
		int NFS[100];

		vector<vector<int>> BEP;
		vector<vector<int>> BFP;
		vector<vector<int>> Corn;
		vector<vector<int>> Neib;
		vector<vector<double>> NPCoordinate;
		void Read_2DMeshC(string filepath);
		void Read_2DMeshCV2(string filepath);
		void Read_3DMeshC(string filepath);
};