#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include "TwoDementionalEdgeBaseMesh.h"

using namespace std; 

TwoDementionalEdgeBaseMesh::TwoDementionalEdgeBaseMesh(){
	FlagofAppendWrite=false;
}

void TwoDementionalEdgeBaseMesh::Read_2DMesh(string filepath){
	
	//-- Declare main Variable
	LineState=1;RegionCounter=0;NFCounter=0;NPCoordinatecounter=0;

	ifstream MeshFile (filepath);
	if (MeshFile.is_open())
	{
		//-- Read Line By Line from file to Line Variable
		while ( getline (MeshFile,line) )
		{
			istringstream is( line );

			//-- Get Number from Line to doubleIntlineNumber Variable
			while( is >> doubleIntlineNumber ) {
			
			//-- Convert to int type
			IntlineNumber=(int)doubleIntlineNumber;

				//-- Evaluate LineState number for Reading type 
				switch (LineState) {

				case 1: //-- Read Mesh Dimension
				  Dimension=IntlineNumber;
				  cout << "Dimension =" << IntlineNumber << endl;
				  if (Dimension!=2){cout << "Dimension Size in not Correct in " << filepath; getchar(); exit(1);}
				  LineState++;
				  break;

				case 2: //-- Read NumberOfPoint Value
				  NumberOfPoint=IntlineNumber;
				  cout << "NumberOfPoint =" << IntlineNumber << endl;
				  LineState++;
				  break;

				case 3: //-- Read NumberOfCell Value
				  NumberOfCell=IntlineNumber;
				  cout << "NumberOfCell =" << IntlineNumber << endl;
				  LineState++;
				  break;

				case 4: //-- Read NumberOfFace Value
				  NumberOfFace=IntlineNumber;
				  cout << "NumberOfFace =" << IntlineNumber << endl;
				  LineState++;
				  break;

				case 5: //-- Read NumberofRegions Value
				  NumberofRegions=IntlineNumber;
				  cout << "NumberofRegions =" << IntlineNumber << endl;
				  LineState++;
				  BC.push_back(-1);
				  NFR.push_back(-1);
				  break;

				case 6: //-- Read NumberofRegions Value

				RegionCounter ++;
				  if (RegionCounter%2==0) BC.push_back(IntlineNumber);
				  else NFR.push_back(IntlineNumber);
				  

				  if ( (NFR.size()-1)==NumberofRegions && (BC.size()-1)==NumberofRegions) {LineState++;IDSval.push_back(-1);IDS.push_back(IDSval);IDSval.clear();}
				  break;

				case 7: //-- Read all IDS Value
					NFCounter++;
					IDSval.push_back(IntlineNumber);
					if (NFCounter%5==0){
						IDS.push_back(IDSval);IDSval.clear();
					}
					if ((IDS.size()-1)==NumberOfFace) {LineState++;NPVal.push_back(-1.0);NPCoordinate.push_back(NPVal);NPVal.clear();}
				    
				 break;

				 case 8: //-- All Coordinate Read Go to next Read

						NPCoordinatecounter++;

						//-- %2 Mode NPCoordinatecounter for line detect
						if (NPCoordinatecounter%2==0)
						{
							//-- Add Y Value
							NPVal.push_back(doubleIntlineNumber);
							//-- Add Coordinate Values
							NPCoordinate.push_back(NPVal);
							NPVal.clear();
						}
						else
						{
							//-- Add X Value
							NPVal.push_back(doubleIntlineNumber);
						}

						if (NPCoordinate.size()==NumberOfPoint){
							//FiniSH
						}

				 break;


				}
			}
		}
	}else {cout << "Unable to open " << filepath; getchar(); exit(1);} 

}

void TwoDementionalEdgeBaseMesh::ReadMSH(string filepath){

	ifstream MSHFile (filepath);
	
	string FaceIndex,BCTitle,IndexChars,TempChars,CommentStr,Char;
	int Dim,NP,NC,NF,NR,MeshDim,IO,IndexCode,RowNum,SpaceCount,RegionNum,FaceCount,CellCount,BCTitleCount,MixFace,I,K,J,L,Count,M,N,CellFlag;
	int LineState=1;
	// Part 1
	if (MSHFile.is_open())
	{
		// Part 2
		FaceIndex="";BCTitle="";RowNum=100;SpaceCount=0; 
		RegionNum=0;FaceCount=1;CellCount=1;BCTitleCount=1;
		CellFlag=0;L=0;M=1;N=1; 

		// Part 3
		for(int forCounter=0;RowNum>forCounter;forCounter++)
		{
			    I=2;K=1;J=1;Count=0;MixFace=0;IndexChars="";
				TempChars="";CommentStr="";Char="";

				// Part 4
				while ( getline (MSHFile,line) )
				{
					cout << line;
					//istringstream is( line );
				}
		}
	}
}





void TwoDementionalEdgeBaseMesh::Write_2DMeshPLT(string filepath){
//-- Create PLT File For Writing and check for appending file
	
					fstream PltFile;
					if (FlagofAppendWrite)	{PltFile.open(filepath,ios::app);}			
					else {PltFile.open (filepath,ios::out);}	

					if(!PltFile)
					{
						cerr<<"File can not open."<<endl;
						exit(1);
					}
				
					//-- PLT File Standard Command
					PltFile<<" TITLE = 'Title' "<<endl<<" VARIABLES  = X , Y "<<endl<<" ZONE T='Title', N= "<< NumberOfPoint <<" , E= " << NumberOfFace << ", ET=LINESEG, F=FEBLOCK "<<endl;


					//-- Write X
					for(int i=1 ; i<=NumberOfPoint ; i++)
						 {
								 PltFile.precision(17);
								 PltFile<<NPCoordinate[i][0];
						    	 PltFile<<endl ;
						 }

					//-- Write Y
					for(int i=1 ; i<=NumberOfPoint ; i++)
						 {
								 PltFile.precision(17);
								 PltFile<<NPCoordinate[i][1];
						    	 PltFile<<endl ;
						 }

					//-- Write IDS 3,4
					for(int i=1 ; i<=NumberOfFace ; i++)
						{
							 PltFile<<IDS[i][3]<<"    ";
							 PltFile<<IDS[i][4];
						     PltFile<<endl ;	
						}

					//-- Close PltFile
					PltFile.close();

}
