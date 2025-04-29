#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

namespace PolygonalLibrary
{
bool ImportMesh(PolygonalMesh& mesh)
{

    if(!ImportCell0Ds(mesh))
        return false;

    if(!ImportCell1Ds(mesh))
        return false;

    if(!ImportCell2Ds(mesh))
        return false;

    return true;

}
// ***************************************************************************
bool ImportCell0Ds(PolygonalMesh& mesh)
{
    ifstream file("./Cell0Ds.csv");

    if(file.fail())
        return false;

    list<string> listLines;

    string line;
    while (getline(file, line))
        listLines.push_back(line);

    file.close();

    // remove header
    listLines.pop_front();

    mesh.NumCell0Ds = listLines.size();

    if (mesh.NumCell0Ds == 0)
    {
        cerr << "There is no cell 0D" << endl;
        return false;
    }

    mesh.Cell0DsId.reserve(mesh.NumCell0Ds);
    mesh.Cell0DsCoordinates = Eigen::MatrixXd::Zero(3, mesh.NumCell0Ds);

    for (const string& line : listLines)
    {
        istringstream converter(line);

        unsigned int id;
        unsigned int marker;
        Vector2d coord;
		char delim;

        converter >>  id >> delim
		>> marker >> delim
		>>mesh.Cell0DsCoordinates(0, id) >> delim
		>> mesh.Cell0DsCoordinates(1, id);

        mesh.Cell0DsId.push_back(id);

        /// Memorizza i marker
        if(marker != 0)
        {
            const auto it = mesh.MarkerCell0Ds.find(marker);
            if(it == mesh.MarkerCell0Ds.end())
            {
                mesh.MarkerCell0Ds.insert({marker, {id}});
            }
            else
            {
                // mesh.MarkerCell0Ds[marker].push_back(id);
                it->second.push_back(id);
            }
        }

    }

    return true;
}
// ***************************************************************************

bool ImportCell1Ds(PolygonalMesh& mesh)
{
    ifstream file("./Cell1Ds.csv");

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);

    file.close();

    // remove header
    listLines.pop_front();

    mesh.NumCell1Ds = listLines.size();

    if (mesh.NumCell1Ds == 0)
    {
        cerr << "There is no cell 1D" << endl;
        return false;
    }

    mesh.Cell1DsId.reserve(mesh.NumCell1Ds);
    mesh.Cell1DsExtrema = Eigen::MatrixXi(2, mesh.NumCell1Ds);
	// OSS gli estremi sono gli id delle cell0D

    for (const string& line : listLines)
    {
        istringstream converter(line);

        unsigned int id;
        unsigned int marker;
        Vector2i vertices;
		char delim;

        converter >>  id >> delim
		>> marker >> delim 
		>>  mesh.Cell1DsExtrema(0, id) >> delim
		>>  mesh.Cell1DsExtrema(1, id);
        mesh.Cell1DsId.push_back(id);

        /// Memorizza i marker
        if(marker != 0)
        {
            const auto it = mesh.MarkerCell1Ds.find(marker);
            if(it == mesh.MarkerCell1Ds.end())
            {
                mesh.MarkerCell1Ds.insert({marker, {id}});
            }
            else
            {
                // mesh.MarkerCell1Ds[marker].push_back(id);
                it->second.push_back(id);
            }
        }
    }

    return true;
}
// ***************************************************************************


bool ImportCell2Ds(PolygonalMesh& mesh)
{
    ifstream file;
    file.open("./Cell2Ds.csv");

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);

    file.close();

    // remove header
    listLines.pop_front();

    mesh.NumCell2Ds = listLines.size();

    if (mesh.NumCell2Ds == 0)
    {
        cerr << "There is no cell 2D" << endl;
        return false;
    }

    mesh.Cell2DsId.reserve(mesh.NumCell2Ds);
	
	mesh.Cell2DsNumVert.reserve(mesh.NumCell2Ds);
	mesh.Cell2DsNumEdg.reserve(mesh.NumCell2Ds);
	
    mesh.Cell2DsVertices.reserve(mesh.NumCell2Ds);
    mesh.Cell2DsEdges.reserve(mesh.NumCell2Ds);

    for (const string& line : listLines)
    {
        istringstream converter(line);

        unsigned int id;
		unsigned int marker;
		unsigned int NumVert;
		unsigned int NumEdg;
		char delim;
		unsigned int Vert;
		
		//todo memorizzare nell'apposito vettore NumVert e NumEdg

        converter >>  id >> delim >> marker >> delim >> NumVert >> delim;
		
		vector<unsigned int> v = {};
		v.reserve(NumVert);
		
        for(unsigned int i = 0; i < NumVert; i++){
            converter >> Vert >> delim;
			v.push_back(Vert);
		}
		converter >> NumEdg >> delim;
		
		vector<unsigned int> e = {};
		e.reserve(NumEdg);
		
        for(unsigned int i = 0; i < NumEdg; i++){
            converter >> Vert;
			
			e.push_back(Vert);
			if (i < NumEdg - 1) converter >> delim;

		}
		
        mesh.Cell2DsId.push_back(id);
		mesh.Cell2DsNumVert.push_back(NumVert);
		mesh.Cell2DsNumEdg.push_back(NumEdg);
		mesh.Cell2DsVertices.push_back(v);
		mesh.Cell2DsEdges.push_back(e);
		
		
        
		if(marker != 0)
        {
            const auto it = mesh.MarkerCell2Ds.find(marker);
            if(it == mesh.MarkerCell2Ds.end())
            {
                mesh.MarkerCell2Ds.insert({marker, {id}});
            }
            else
            {
                // mesh.MarkerCell1Ds[marker].push_back(id);
                it->second.push_back(id);
            }
        }
		
    }

    return true;
}

bool test_edges(const PolygonalMesh& mesh){
	const double eps_1 = 10e-8;
	for (unsigned int i=0;i<mesh.NumCell1Ds;i++){
		
		unsigned int idInizio= mesh.Cell1DsExtrema(0,i);
		unsigned int idFine = mesh.Cell1DsExtrema(1,i);
		
		double x1 =mesh.Cell0DsCoordinates(0,idInizio);
		double y1 =mesh.Cell0DsCoordinates(1,idInizio);
		double x2 =mesh.Cell0DsCoordinates(0,idFine);
		double y2 =mesh.Cell0DsCoordinates(1,idFine);
		
		double dist = sqrt(pow((x2-x1),2)+pow((y2-y1),2));
		//cout<<dist<<endl;
		if (dist <= eps_1)
		return false;
		
	}
	
	
	
		return true;
	
}

bool test_aree(const PolygonalMesh& mesh){
	const double eps_2 = 10e-16;
	for (unsigned int i=0;i<mesh.NumCell2Ds;i++){
		
		double area =0.0;
		
		for (unsigned int j=0; j<mesh.Cell2DsNumVert[i]; j++){
			unsigned int idPoint=(mesh.Cell2DsVertices[i])[j];
			unsigned int idPointSucc=(mesh.Cell2DsVertices[i])[(j+1)%mesh.Cell2DsNumVert[i]];
			//cout<<j<<" "<<idPoint<<" "<<idPointSucc<<endl;
			double x1 = mesh.Cell0DsCoordinates(0, idPoint);
            double y1 = mesh.Cell0DsCoordinates(1, idPoint);
            double x2 = mesh.Cell0DsCoordinates(0, idPointSucc);
            double y2 = mesh.Cell0DsCoordinates(1, idPointSucc);

            area += x1 * y2 - x2 * y1;
		}
		area = abs(area)/2.0;
		if (area<= eps_2)
			return false;
	}
	return true;
}

bool test_marker(const PolygonalMesh& mesh){
	map<unsigned int, list<unsigned int>> marker_0_corretto = {
		{1,{0}},
		{2,{1}},
		{3,{2}},
		{4,{3}},
		{5,{6,16,24}},
		{6,{7,17,22,78}},
		{7,{8,20,23,52,59}},
		{8,{5,15,21,26,92}},	
	};
	map<unsigned int, list<unsigned int>> marker_1_corretto = {
		{5,{8,19,22,28}},
		{6,{6,23,26,126,127}},
		{7,{14,17,24,79,92,93}},
		{8,{11,25,29,30,159,160}},	
	};
	map<unsigned int, list<unsigned int>> marker_2_corretto = {};
	if (!(mesh.MarkerCell0Ds==marker_0_corretto)){
		cout<<"errore marker 0Ds"<<endl;
		return false;
	}
	if (!(mesh.MarkerCell1Ds==marker_1_corretto)){
		cout<<"errore marker 1Ds"<<endl;
		return false;
	}
	if (!(mesh.MarkerCell2Ds==marker_2_corretto)){
		cout<<"errore marker 2Ds"<<endl;
		return false;
	}
	return true;
}


}
