// AggregateExperRes.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <boost/filesystem.hpp>

using namespace std;
using namespace boost::filesystem;


int _tmain(int argc, _TCHAR* argv[])
{
    int expNum =0 ;
    if (argc == 1){
        expNum = 1;
    }
    else {
        expNum = _wtoi(argv[1]);
    }

    bool isResourcesFileWasFound = false;
    auto dir = current_path();

    vector <vector<double>> simple;
    vector <vector<double>> staged;
    vector <vector<double>> clustered;
    vector <vector<double>> fineSimple;
    vector <vector<double>> fineStaged;
    vector <vector<double>> fineClustered;

    vector<vector<double>> addSimple;
    vector<vector<double>> addStaged;
    vector<vector<double>> addClustered;
    vector<vector<double>> addFineSimple;
    vector<vector<double>> addFineStaged;
    vector<vector<double>> addFineClustered;

    int processedCount = 0;
    vector<double> currSimple;
    vector<double> currStaged;
    vector<double> currClustered;
    vector<double> currFineSimple;
    vector<double> currFineStaged;
    vector<double> currFineClustered;

      for (directory_iterator it(dir), end; it != end; ++it) 
      {
         std::cout << "File processed - ";
         std::cout << *it << std::endl;
         string filename = it->path().string();
         int position = filename.find("fullmetrics");
         if (position==-1) 
             continue;
         ifstream file(filename, ifstream::in);
         if (file.fail()) {
             cout << "Error while opening file " << endl;
             system("pause");
             exit(1);
         }

         int metricsTaken = 0;
         while (metricsTaken != 3){
             string s;
             getline(file,s);
             string get = "Avg reserved time (part of T): ";
             size_t found = s.find(get);
             if (found != 0) continue;
             s.erase(0,get.size());
             double fine = atof(s.c_str());
             metricsTaken++;
             bool metricWasFound = false;
             double metric = 0.0;
             while (!metricWasFound){
                getline(file,s);
                get = "Integral criterion: ";
                size_t found = s.find(get);
                if (found != 0) continue;
                metricWasFound = true;
                s.erase(0,get.size());
                metric = atof(s.c_str());
             }
             if (metricsTaken == 1){
                 currSimple.push_back(metric);
                 currFineSimple.push_back(fine);
              }
             if (metricsTaken == 2){
                 currStaged.push_back(metric);
                 currFineStaged.push_back(fine);
             }
             if (metricsTaken == 3){
                 currClustered.push_back(metric);
                 currFineClustered.push_back(fine);
             }
         }

         file.close();

         processedCount++;
         if (processedCount % expNum == 0){
             if (processedCount == 2 * expNum ||
                 processedCount == 3 * expNum ||
                 processedCount == 4 * expNum){
                     addSimple.push_back(currSimple);
                     addStaged.push_back(currStaged);
                     addClustered.push_back(currClustered);
                     addFineSimple.push_back(currFineSimple);
                     addFineStaged.push_back(currFineStaged);
                     addFineClustered.push_back(currFineClustered);

             }
             else{
                 simple.push_back(currSimple);
                 staged.push_back(currStaged);
                 clustered.push_back(currClustered);
                 fineSimple.push_back(currFineSimple);
                 fineStaged.push_back(currFineStaged);
                 fineClustered.push_back(currFineClustered);
             }
             currSimple.clear();
             currStaged.clear();
             currClustered.clear();
             currFineSimple.clear();
             currFineStaged.clear();
             currFineClustered.clear();
         }
      }

    for (int i = 0; i < addSimple.size();i++){
        simple.push_back(addSimple[i]);
        staged.push_back(addStaged[i]);
        clustered.push_back(addClustered[i]);
        fineSimple.push_back(addFineSimple[i]);
        fineStaged.push_back(addFineStaged[i]);
        fineClustered.push_back(addFineClustered[i]);
    }

    ofstream out("aggregated.txt");
    out << "Simple" << endl << endl;
    for (auto &row: simple){
        for (auto&col:row)
            out << col << " ";
        out << endl;
    }
    out << endl << endl;
    out << "Staged" << endl << endl;
    for (auto &row: staged){
        for (auto&col:row)
            out << col << " ";
        out << endl;
    }
    out << endl << endl;
    out << "Clustered" << endl << endl;
    for (auto &row: clustered){
        for (auto&col:row)
            out << col << " ";
        out << endl;
    }
    out.close();
    ofstream outFine("aggregatedFine.txt");
    outFine << "Simple" << endl << endl;
    for (auto &row: fineSimple){
        for (auto&col:row)
            outFine << col << " ";
        outFine << endl;
    }
    outFine << endl << endl;
    outFine << "Staged" << endl << endl;
    for (auto &row: fineStaged){
        for (auto&col:row)
            outFine << col << " ";
        outFine << endl;
    }
    outFine << endl << endl;
    outFine << "Clustered" << endl << endl;
    for (auto &row: fineClustered){
        for (auto&col:row)
            outFine << col << " ";
        outFine << endl;
    }
    outFine.close();

    vector<double> timeSimple;
    vector<double> timeStaged;
    vector<double> timeClustered;

    for (directory_iterator it(dir), end; it != end; ++it) 
      {
         //std::cout << "File processed - ";
         //std::cout << *it << std::endl;
         string filename = it->path().string();
         int position = filename.find("time");
         if (position==-1) 
             continue;
         ifstream file(filename, ifstream::in);
         if (file.fail()) {
             cout << "Error while opening file " << endl;
             system("pause");
             exit(1);
         }
         int variant = 0;
         while (!file.eof()){
             string s;
             getline(file,s);
             string get = "Time of executing ";
             size_t found = s.find(get);
             if (found != 0) continue;
             s.erase(0,get.size());
             string add;
             if (variant == 0)
                 add = "simple scheduling algorithm ";
             else if (variant == 1)
                 add = "reserved ordered scheme ";
             else
                 add = "clustered scheme ";
             s.erase(0,add.size());
             double metric = atof(s.c_str());
             if (variant == 0)
                 timeSimple.push_back(metric);
             else if (variant == 1)
                 timeStaged.push_back(metric);
             else
                 timeClustered.push_back(metric);
             variant++;
             if (variant == 3) variant = 0;

         }
    }

     ofstream outTime("aggregatedTime.txt");
    outTime << "Simple" << endl << endl;
    for (auto &row: timeSimple)
           outTime << row << " ";
    outTime << endl << endl;
    
     outTime << "Staged" << endl << endl;
    for (auto &row: timeStaged)
           outTime << row << " ";
    outTime << endl << endl;

     outTime << "Clustered" << endl << endl;
    for (auto &row: timeClustered)
           outTime << row << " ";
    outTime << endl << endl;
    outTime.close();
    return 0;
}

