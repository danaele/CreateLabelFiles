#include <iostream>
#include <fstream>
#include <string>
#include <vtkSmartPointer.h>
#include <vector>
#include <list>
#include <algorithm>
#include <map>
#include <stdlib.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataReader.h>
#include <vtkPolyData.h>
#include <vtkDataReader.h>

int main ( int argc, char *argv[] )
{
    // Ensure a filename was specified
    if(argc != 3)
    {
        std::cerr << "Not enough arguments - requires two parameters [surfaceFile] [labelFile]" << std::endl;
        return EXIT_FAILURE;
    }

    std::string surfaceName = argv[1];
    std::ifstream labelFile ( argv[2]);

    std::string labelLine;
    //Define map containing labels and a list of points associated to them
    std::map< std::string , std::list<int> > labelMap;
    //Map iterator
    std::map< std::string , std::list<int> >::const_iterator mit,mend;
    //List iteration
    std::list<int>::const_iterator lit,lend;

    int nPoint=0; //point id

    //Extract labels and points associated
    if(labelFile) //file can be open
    {
        getline(labelFile,labelLine); //format line // Need to be checked
        getline(labelFile,labelLine);    //first RGB value

        do
        {
            std::string newlabelRGB=labelLine;
            labelMap[newlabelRGB].push_back(nPoint);
            getline(labelFile,labelLine);
            nPoint++;
        }while(!labelFile.eof());
    }
    else
    {
        std::cout<<"Cannot open the label file "<<std::endl;
        return EXIT_FAILURE;
    }


    //    //Check if map is correct
    //    if(labelMap.find("")!=labelMap.end())
    //    {
    //        std::cout<<"Hi"<<std::endl;
    //    }

    //Read surface file and write one ASCII file per label
    // Read surface file
    vtkSmartPointer< vtkPolyDataReader > reader = vtkSmartPointer< vtkPolyDataReader >::New() ;
    vtkSmartPointer< vtkPolyData > polyData = vtkSmartPointer< vtkPolyData >::New() ;
    reader->SetFileName( surfaceName.c_str() ) ;
    reader->Update() ;
    polyData = reader->GetOutput() ;

    //Checked if surface file with color no containing errors
    if (reader->GetErrorCode() !=0 )
    {
        std::cerr << "Unable to open surface file: " << surfaceName.c_str() << std::endl ;
        return EXIT_FAILURE ;
    }

    for(mit=labelMap.begin(),mend=labelMap.end();mit!=mend;++mit)
    {
        std::ofstream outputFile;
        std::string labelName = mit->first;

        //Rename label file to create
        replace(labelName.begin(),labelName.end(),' ','-');
        labelName += ".asc";

        //Write the ASCII file corresponding to one surface / one label
        outputFile.open(labelName.c_str() , std::ios::out);
        if(outputFile.good())
        {
            outputFile << "#!ascii - generated by CreateLabelFiles project \n";
            outputFile << mit->second.size() << " 0\n";
            for(lit=mit->second.begin(),lend=mit->second.end();lit!=lend;++lit)
            {
                double p[3];
                polyData->GetPoint(*lit,p);
                outputFile << p[0]<< " "<< p[1]<<" "<<p[2]<< " 0 \n";
            }
        }
        else
        {
            std::cout<<"Cannot open "<< labelName<< std::endl;
        }

        outputFile.close();
    }
    //Read the map - and count number of labels
    int nb_label=0;
    int nbpoints=0;
    for(mit=labelMap.begin(),mend=labelMap.end();mit!=mend;++mit)
    {
        std::string lab=mit->first;
        std::list<int> point=mit->second;
        nb_label++;
        for(lit=mit->second.begin(),lend=mit->second.end();lit!=lend;++lit)
        {
            //   std::cout<<*lit<<" ";
        }
        //std::cout<<std::endl;
    }
   // std::cout<<nb_label<<std::endl;

    return EXIT_SUCCESS;

}
