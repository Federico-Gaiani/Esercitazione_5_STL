#include <iostream>
#include "PolygonalMesh.hpp"
#include "Utils.hpp"
#include "UCDUtilities.hpp"

using namespace std;
using namespace Eigen;
using namespace PolygonalLibrary;


int main()
{
	PolygonalMesh mesh;

    if(!ImportMesh(mesh))
    {
        cerr << "file not found" << endl;
        return 1;
    }
/*	
	for (unsigned int i=0;i<mesh.NumCell0Ds;i++){
		cout<<mesh.Cell0DsId[i]<<" "<<mesh.Cell0DsCoordinates (0,i)<<" "<<mesh.Cell0DsCoordinates(1,i)<<endl;
	}

	for (auto& it : mesh.MarkerCell0Ds[5])
		cout<<it<<" ";
	cout<<endl;

	for (unsigned int i=0;i<mesh.NumCell1Ds;i++){
		cout<<mesh.Cell1DsId[i]<<" "<<mesh.Cell1DsExtrema (0,i)<<" "<<mesh.Cell1DsExtrema(1,i)<<endl;
	}

	for (auto& it : mesh.MarkerCell1Ds[5])
		cout<<it<<" ";
	cout<<endl;

	for (unsigned int i=0;i<mesh.NumCell2Ds;i++){
		int k=mesh.Cell2DsVertices[i].size();
		cout<<k<<": ";
		for (int j=0;j<k; j++){
			cout<<(mesh.Cell2DsVertices[i])[j]<<" ";
		}
		cout<<endl;
	}

	for (unsigned int i=0;i<mesh.NumCell2Ds;i++){
		int k=mesh.Cell2DsEdges[i].size();
		cout<<k<<": ";
		for (int j=0;j<k; j++){
			cout<<(mesh.Cell2DsEdges[i])[j]<<" ";
		}
		cout<<endl;
	}
	
	for (auto& it : mesh.MarkerCell2Ds[5])
		cout<<it<<" ";
	cout<<endl;
*/


	if(!test_edges(mesh))
    {
        cerr << "almeno un edge ha lunghezza nulla" << endl;
        return 1;
    }else{
		cout << "gli edges hanno lunghezza non nulla" << endl;
	}
	
	if (!test_aree(mesh))
	{
		cerr << "almeno un' area è nulla" << endl;
        return 1;
	}else{
		cout << "le aree sono tutte non nulle" << endl;
	}
	
	Gedim::UCDUtilities utilities;
    {
        vector<Gedim::UCDProperty<double>> cell0Ds_properties(1);

        cell0Ds_properties[0].Label = "Marker";
        cell0Ds_properties[0].UnitLabel = "-";
        cell0Ds_properties[0].NumComponents = 1;

        vector<double> cell0Ds_marker(mesh.NumCell0Ds, 0.0);
        for(const auto &m : mesh.MarkerCell0Ds)
            for(const unsigned int id: m.second)
                cell0Ds_marker.at(id) = m.first;

        cell0Ds_properties[0].Data = cell0Ds_marker.data();

        utilities.ExportPoints("./Cell0Ds.inp",
                               mesh.Cell0DsCoordinates,
                               cell0Ds_properties);
    }

    {

        vector<Gedim::UCDProperty<double>> cell1Ds_properties(1);

        cell1Ds_properties[0].Label = "Marker";
        cell1Ds_properties[0].UnitLabel = "-";
        cell1Ds_properties[0].NumComponents = 1;

        vector<double> cell1Ds_marker(mesh.NumCell1Ds, 0.0);
        for(const auto &m : mesh.MarkerCell1Ds)
            for(const unsigned int id: m.second)
                cell1Ds_marker.at(id) = m.first;

        cell1Ds_properties[0].Data = cell1Ds_marker.data();

        utilities.ExportSegments("./Cell1Ds.inp",
                                 mesh.Cell0DsCoordinates,
                                 mesh.Cell1DsExtrema,
                                 {},
                                 cell1Ds_properties);
    }
	
	
	
	
	
    return 0;
}
