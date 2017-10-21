#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <stdlib.h>
using namespace std; 

class TwoDementionalEdgeBaseMesh
{
	private :

			//-- Declare main Variable
			string line;
			double doubleIntlineNumber;
			int IntlineNumber, LineState,RegionCounter,NFCounter,NPCoordinatecounter;
			int ME,NE,I;

			vector<int> IDSval;
			vector<double> NPVal;
			

	public:

			int NumberOfPoint,NumberOfCell,Dimension,NumberOfFace,NumberofRegions;
			vector<vector<double>> NPCoordinate;
			vector<vector<int>> IDS;
			vector<int> NFR;
			vector<int> BC;

			//-- initialize Method

			TwoDementionalEdgeBaseMesh();
			
			void Read_2DMesh(string filepath);
			void ReadMSH(string filepath);
			void Write_2DMeshPLT(string filepath);
			bool FlagofAppendWrite;

};