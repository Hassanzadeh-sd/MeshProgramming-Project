#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include "TwoDementionalCellBaseMesh.h"
using namespace std; 

void TwoDementionalCellBaseMesh::Read_2DMeshC(string filepath){

	//-- Initialize Variables
	LineState=1;
	NCCounter=0;
	CornCheck=1;
	Nface=0;
	NPCoordinatecounter=0;
	NBECounter=0;
	NECCounter=0;
	BEPCounter=0;
	NBECount=0;
	NBE=0;

	//-- Check Exsit File in File path
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
			
			case 1: //-- Read NumberOfPoint Value
			  NumberOfPoint=IntlineNumber;
			  LineState++;
			  break;

			case 2: //-- Read NumberOfCell Value
			  NumberOfCell=IntlineNumber;
			  LineState++;
			  break;

			case 3: //-- Read NumberofBoundaryCurves Value
			  NumberofBoundaryCurves=IntlineNumber;
			  LineState++;
			  break;

			case 4: //-- Read NBE Value
			  NEC[NECCounter]=IntlineNumber;

			  //-- adding All NEC to NBE Variable
			  NBE += NEC[NECCounter];
			  NECCounter++;
			  if (NECCounter==NumberofBoundaryCurves) LineState++;
			  break;

			case 5: //-- Read BEP Array

				BEPCounter++;

				//-- %2 Mode BEPCounter for line detect
				if (BEPCounter%2==0)
				{
					//-- Add BEP Secound Value in line
					BEPval.push_back(IntlineNumber);
				    
					//-- Add BEP Values to BEP Vector
					BEP.push_back(BEPval);
					BEPval.clear();
				}
				else
				{
					//-- Add BEP First Value in line
					BEPval.push_back(IntlineNumber);
				}

				//-- All BEP Read Go to Next Read Type
				if (BEP.size()==NBE) {
					LineState++;

					//-- NumberOfCell equal zero Jump to reading Coordinate 
					if (NumberOfCell==0) LineState++;
				};

			break;

			  case 6: //-- read Corn And Neib Values

					NCCounter++;
					
					//-- Read First Number of line for detect Number Of face
					NCval.push_back(IntlineNumber);
					if(Nface==0) 
					{
						Nface=IntlineNumber+1;
						NCCounter=1;
					}

					if(Nface>5) { cout << "Mesh File Nface Number incorrect --> " << Nface <<endl;getchar();exit(1);}

					//-- Read all value in line save and go to next line
					if (NCCounter==Nface)
					{
						if (Nface==4){
							NCval.push_back(IntlineNumber);
						} 
						if (CornCheck==1) {Corn.push_back(NCval);CornCheck=0;}
						else if (CornCheck==0) {Neib.push_back(NCval);CornCheck=1;}
						NCval.clear();
						Nface=0;
					}

					//-- all Corn And Neib Read Go to next read part
					if (Corn.size()==NumberOfCell &&  Neib.size()==NumberOfCell) LineState++;
			
				  break;

			case 7: //-- Coordinate s all NumberofPoint

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

				//-- All Coordinate Read Go to next Read
				if (NPCoordinate.size()==NumberOfPoint) {

					//-- Create PLT File For Writing
					fstream PltFile ("Mesh.Plt",ios::out);
					if(!PltFile)
					{
						cerr<<"File can not open."<<endl;
						exit(1);
					}
				
					//-- PLT File Standard Command
					PltFile<<"Variables=X,Y"<<endl<<"Zone T=Grid"<<endl<<" N=  " << NumberOfPoint << " ,E= " << NumberOfCell << ", F=FEPOINT ET=QUADRILATERAL "<<endl;

					//-- Write X , Y 
					for(int i=0 ; i<=NumberOfPoint-1 ; i++)
						 {
							 for (int j = 0 ;j<=1 ; j++)
							 {
								 PltFile.precision(17);
								 PltFile<<NPCoordinate[i][j]<<"       ";
							 }
							 PltFile<<endl ;
						 }

					 //-- Write All Corn
					 for(int i=0 ; i<=NumberOfCell-1 ; i++)
						 {
							 for (int j = 1 ;j<=4 ; j++)
							 {
								 PltFile<< Corn[i][j]<<"  ";
							 }
							 PltFile<<endl ;
						 }

					 //-- Write all Curves
					 for (int Necforcounter=0; Necforcounter<NumberofBoundaryCurves; ++Necforcounter)
					 {
						 PltFile<<"Variables=X,Y"<<endl<<"Zone "<<" N=  " << NumberOfPoint << " ,E= " << NEC[Necforcounter] << ",Datapacking=Point, Zonetype=Fetriangle"<<endl;
					
						 //-- Write NumberOfPoint Coordinate
						 for(int i=0 ; i<=NumberOfPoint-1 ; i++)
						 {
							 for (int j = 0 ;j<=1 ; j++)
							 {
								 PltFile.precision(17);
								 PltFile<<NPCoordinate[i][j]<<" ";
							 }
							 PltFile<<endl ;
						 }

						 //-- Write All BEP Coordinate
						 for(int i=NBECount ; i<=NBECount+NEC[Necforcounter]-1 ; i++)
						 {
							 for (int j = 0 ;j<=1 ; j++)
							 {
								 PltFile<<BEP[i][j]<<" ";
							 }
							 PltFile<<BEP[i][1];
							 PltFile<<endl ;
						 }
						 NBECount += NEC[Necforcounter];
					 }
					 PltFile.close();
				}
			break;

			}
		}
	}

	MeshFile.close();

	}
	else {cout << "Unable to open " << filepath; getchar(); exit(1);} 

	cout << "Mesh Calculate and Create PLT File...";


};

void TwoDementionalCellBaseMesh::Read_2DMeshCV2(string filepath){

	//-- Initialize Variables
	LineState=1;
	NCCounter=0;
	CornCheck=1;
	Nface=0;
	NPCoordinatecounter=0;
	NBECounter=0;
	NECCounter=0;
	BEPCounter=0;
	NBECount=0;
	NBE=0;

	//-- Check Exsit File in File path
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
			
			case 1: //-- Read NumberOfPoint Value
			  NumberOfPoint=IntlineNumber;
			  LineState++;
			  break;

			case 2: //-- Read NumberOfCell Value
			  NumberOfCell=IntlineNumber;
			  LineState++;
			  break;

			case 3: //-- Read NumberofBoundaryCurves Value
			  NumberofBoundaryCurves=IntlineNumber;
			  LineState++;
			  break;

			case 4: //-- Read NBE Value
			  NEC[NECCounter]=IntlineNumber;

			  //-- adding All NEC to NBE Variable
			  NBE += NEC[NECCounter];
			  NECCounter++;
			  if (NECCounter==NumberofBoundaryCurves) LineState++;
			  break;

			case 5: //-- Read BEP Array

				BEPCounter++;

				//-- %2 Mode BEPCounter for line detect
				if (BEPCounter%2==0)
				{
					//-- Add BEP Secound Value in line
					BEPval.push_back(IntlineNumber);
				    
					//-- Add BEP Values to BEP Vector
					BEP.push_back(BEPval);
					BEPval.clear();
				}
				else
				{
					//-- Add BEP First Value in line
					BEPval.push_back(IntlineNumber);
				}

				//-- All BEP Read Go to Next Read Type
				if (BEP.size()==NBE) {
					LineState++;

					//-- NumberOfCell equal zero Jump to reading Coordinate 
					if (NumberOfCell==0) LineState++;
				};

			break;

			  case 6: //-- read Corn And Neib Values

					NCCounter++;
					
					//-- Read First Number of line for detect Number Of face
					NCval.push_back(IntlineNumber);
					if(Nface==0) 
					{
						Nface=IntlineNumber+1;
						NCCounter=1;
					}

					if(Nface>5) { cout << "Mesh File Nface Number incorrect --> " << Nface <<endl;getchar();exit(1);}

					//-- Read all value in line save and go to next line
					if (NCCounter==Nface)
					{
						if (Nface==4){
							NCval.push_back(IntlineNumber);
						} 
						if (CornCheck==1) {Corn.push_back(NCval);CornCheck=0;}
						else if (CornCheck==0) {Neib.push_back(NCval);CornCheck=1;}
						NCval.clear();
						Nface=0;
					}

					//-- all Corn And Neib Read Go to next read part
					if (Corn.size()==NumberOfCell &&  Neib.size()==NumberOfCell) LineState++;
			
				  break;

			case 7: //-- Coordinate s all NumberofPoint

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

				//-- All Coordinate Read Go to next Read
				if (NPCoordinate.size()==NumberOfPoint) {

					//-- Create PLT File For Writing
					fstream PltFile ("Mesh.Plt",ios::out);
					if(!PltFile)
					{
						cerr<<"File can not open."<<endl;
						exit(1);
					}
				
					//-- PLT File Standard Command
					PltFile<<"Variables=X,Y"<<endl<<"Zone T=Grid"<<endl<<" N=  " << NumberOfPoint << " ,E= " << NumberOfCell << ", F=FEPOINT ET=QUADRILATERAL "<<endl;

					//-- Write X , Y 
					for(int i=0 ; i<=NumberOfPoint-1 ; i++)
						 {
							 for (int j = 0 ;j<=1 ; j++)
							 {
								 PltFile.precision(17);
								 PltFile<<NPCoordinate[i][j]<<"       ";
							 }
							 PltFile<<endl ;
						 }

					 //-- Write All Corn
					 for(int i=0 ; i<=NumberOfCell-1 ; i++)
						 {
							 for (int j = 1 ;j<=4 ; j++)
							 {
								 PltFile<< Corn[i][j]<<"  ";
							 }
							 PltFile<<endl ;
						 }

					 //-- Write all Curves
					 for (int Necforcounter=0; Necforcounter<NumberofBoundaryCurves; ++Necforcounter)
					 {
						 PltFile<<"Variables=X,Y"<<endl<<"Zone "<<" N=  " << NumberOfPoint << " ,E= " << NEC[Necforcounter] << ",Datapacking=Point, Zonetype=Fetriangle"<<endl;
					
						 //-- Write NumberOfPoint Coordinate
						 for(int i=0 ; i<=NumberOfPoint-1 ; i++)
						 {
							 for (int j = 0 ;j<=1 ; j++)
							 {
								 PltFile.precision(17);
								 PltFile<<NPCoordinate[i][j]<<" ";
							 }
							 PltFile<<endl ;
						 }

						 //-- Write All BEP Coordinate
						 for(int i=NBECount ; i<=NBECount+NEC[Necforcounter]-1 ; i++)
						 {
							 for (int j = 0 ;j<=1 ; j++)
							 {
								 PltFile<<BEP[i][j]<<" ";
							 }
							 PltFile<<BEP[i][1];
							 PltFile<<endl ;
						 }
						 NBECount += NEC[Necforcounter];
					 }
					 PltFile.close();
				}
			break;

			}
		}
	}

	MeshFile.close();

	}
	else {cout << "Unable to open " << filepath; getchar(); exit(1);} 

	cout << "Mesh Calculate and Create PLT File...";


};

void TwoDementionalCellBaseMesh::Read_3DMeshC(string filepath){

	//-- Initialize Variables
	LineState=1;
	NCCounter=0;
	CornCheck=1;
	Nface=0;
	NPCoordinatecounter=0;
	NBECounter=0;
	NECCounter=0;
	BFPCounter=0;
	NFSCounter=0;
	NBECount=0;
	NBE=0;

	//-- Check Exsit File in File path
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
			
			case 1: //-- Read NumberOfPoint Value
			  NumberOfPoint=IntlineNumber;
			  LineState++;
			  break;

			case 2: //-- Read NumberOfCell Value
			  NumberOfCell=IntlineNumber;
			  LineState++;
			  break;

			case 3: //-- Read NBS Value
			  NBS=IntlineNumber;
			  LineState++;
			  break;

			case 4: //-- Read NBE Value
			  NFS[NFSCounter]=IntlineNumber;

			  //-- adding All NEC to NBE Variable
			  NBE += NFS[NFSCounter];
			  NFSCounter++;
			  if (NFSCounter==NBS) LineState++;
			  break;

			case 5: //-- Read BFP Array

				BFPCounter++;

				//-- %2 Mode BEPCounter for line detect
				if (BFPCounter%3==0)
				{
					//-- Add BEP Secound Value in line
					BFPval.push_back(IntlineNumber);
				    
					//-- Add BEP Values to BEP Vector
					BFP.push_back(BFPval);
					BFPval.clear();
				}
				else
				{
					//-- Add BEP First Value in line
					BFPval.push_back(IntlineNumber);
				}

				//-- All BEP Read Go to Next Read Type
				if (BFP.size()==NBE) {
					LineState++;

					//-- NumberOfCell equal zero Jump to reading Coordinate 
					if (NumberOfCell==0) LineState++;
				};

			break;

			  case 6: //-- read Corn And Neib Values

					NCCounter++;
					
					//-- Read First Number of line for detect Number Of face
					NCval.push_back(IntlineNumber);
					if(Nface==0) 
					{
						Nface=IntlineNumber+1;
						NCCounter=1;
					}

					if(Nface>6) { cout << "Mesh File Nface Number incorrect --> " << Nface <<endl;getchar();exit(1);}

					//-- Read all value in line save and go to next line
					if (NCCounter==Nface)
					{
						if (Nface==6){
							NCval.push_back(IntlineNumber);
						} 
						if (CornCheck==1) {Corn.push_back(NCval);CornCheck=0;}
						else if (CornCheck==0) {Neib.push_back(NCval);CornCheck=1;}
						NCval.clear();
						Nface=0;
					}

					//-- all Corn And Neib Read Go to next read part
					if (Corn.size()==NumberOfCell &&  Neib.size()==NumberOfCell) LineState++;
			
				  break;

			case 7: //-- Coordinate s all NumberofPoint

				NPCoordinatecounter++;
				//-- %2 Mode NPCoordinatecounter for line detect
				if (NPCoordinatecounter%3==0)
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

				//-- All Coordinate Read Go to next Read
				if (NPCoordinate.size()==NumberOfPoint) {

					//-- Create PLT File For Writing
					fstream PltFile ("Mesh.Plt",ios::out);
					if(!PltFile)
					{
						cerr<<"File can not open."<<endl;
						exit(1);
					}
				
					//-- PLT File Standard Command
					PltFile<<"Variables=X,Y"<<endl<<"Zone T=Grid"<<endl<<" N=  " << NumberOfPoint << " ,E= " << NumberOfCell << ", F=FEPOINT ET=QUADRILATERAL "<<endl;

					//-- Write X , Y 
					for(int i=0 ; i<=NumberOfPoint-1 ; i++)
						 {
							 for (int j = 0 ;j<=1 ; j++)
							 {
								 PltFile.precision(17);
								 PltFile<<NPCoordinate[i][j]<<"       ";
							 }
							 PltFile<<endl ;
						 }

					 //-- Write All Corn
					 for(int i=0 ; i<=NumberOfCell-1 ; i++)
						 {
							 for (int j = 1 ;j<=4 ; j++)
							 {
								 PltFile<< Corn[i][j]<<"  ";
							 }
							 PltFile<<endl ;
						 }

					 //-- Write all Curves
					 for (int Necforcounter=0; Necforcounter<NumberofBoundaryCurves; ++Necforcounter)
					 {
						 PltFile<<"Variables=X,Y"<<endl<<"Zone "<<" N=  " << NumberOfPoint << " ,E= " << NEC[Necforcounter] << ",Datapacking=Point, Zonetype=Fetriangle"<<endl;
					
						 //-- Write NumberOfPoint Coordinate
						 for(int i=0 ; i<=NumberOfPoint-1 ; i++)
						 {
							 for (int j = 0 ;j<=1 ; j++)
							 {
								 PltFile.precision(17);
								 PltFile<<NPCoordinate[i][j]<<" ";
							 }
							 PltFile<<endl ;
						 }

						 //-- Write All BEP Coordinate
						 for(int i=NBECount ; i<=NBECount+NEC[Necforcounter]-1 ; i++)
						 {
							 for (int j = 0 ;j<=1 ; j++)
							 {
								 PltFile<<BEP[i][j]<<" ";
							 }
							 PltFile<<BEP[i][1];
							 PltFile<<endl ;
						 }
						 NBECount += NEC[Necforcounter];
					 }
					 PltFile.close();
				}
			break;

			}
		}
	}

	MeshFile.close();

	}
	else {cout << "Unable to open " << filepath; getchar(); exit(1);} 

	cout << "Mesh Calculate and Create PLT File...";


};
