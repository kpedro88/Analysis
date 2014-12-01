#include <iostream>
#include <fstream> 
#include <string>
#include <vector>
#include <map>
#include <stdlib.h>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <cmath>

#include "THnSparse.h"
#include "TFile.h"
#include "TObject.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TGraph.h"

//#include "utils/StatUtils.hh"


template < typename T > inline
unsigned int findBin(T v, vector<T> vals) {
  bool find=false;
  
  size_t bm=0;
  size_t bM=vals.size()-1;
  if(v< vals[ bm ]) return -1;
  if(v>= vals[ bM ]) return -1;
  size_t n=0;
  while(!find) {
    if(v < vals[ bm+(bM-bm)/2 ] ) {
      bM = bm+(bM-bm)/2;
    }
      else {
	bm = bm+(bM-bm)/2;
      }
    if( fabs((int)(bM-bm))==1 )  {
      return bm;
    }
    
    if(n>(size_t)vals.size()) return -1;
    n++;
  }
  return -1;
};




class DataBaseManager {

public:

  DataBaseManager(); 
  ~DataBaseManager();

  void loadDb(std::string key, std::string dbName);
  void loadDb(std::string key, std::string dbName, std::string hname);
  float getDBValue(std::string key, float v1=-1000000, float v2=-1000000,
		   float v3=-1000000, float v4=-1000000, float v5=-1000000,
		   float v6=-1000000,float v7=-1000000, float v8=-1000000, 
		   float v9=-1000000, float v10=-1000000);

  float getDBErrL(std::string key, float v1=-1000000, float v2=-1000000,
		   float v3=-1000000, float v4=-1000000, float v5=-1000000,
		   float v6=-1000000,float v7=-1000000, float v8=-1000000, 
		   float v9=-1000000, float v10=-1000000);

  float getDBErrH(std::string key, float v1=-1000000, float v2=-1000000,
		   float v3=-1000000, float v4=-1000000, float v5=-1000000,
		   float v6=-1000000,float v7=-1000000, float v8=-1000000, 
		   float v9=-1000000, float v10=-1000000);


private:
  
  void readDb(std::string key, std::string dbName);
  void readDbHisto(std::string key, std::string dbName, std::string hname);

  std::map<std::string,std::vector<std::vector<float> > > _cDbLim;
  std::map<std::string, THnSparseF*> _mDBs;
  std::map<std::string, THnSparseF*> _mDBEHs;
  std::map<std::string, THnSparseF*> _mDBELs;

};
