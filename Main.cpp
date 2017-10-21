// SampleMesh.cpp : Defines the entry point for the console application.//
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include "TwoDementionalCellBaseMesh.h"
#include "TwoDementionalEdgeBaseMesh.h"

using namespace std; 

int _tmain(int argc, _TCHAR* argv[])
{
	//TwoDementionalCellBaseMesh t1;
	//t1.InitialiseMesh("Mesh.Txt");

	TwoDementionalEdgeBaseMesh t2;

	//t2.Read_2DMesh("MeshE.txt");

	//t2.Write_2DMeshPLT("test.Plt");
	t2.ReadMSH("Test.msh");
	getchar();
	return 0;
}

