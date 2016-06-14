#ifndef LDFDATAREADER_H
#define LDFDATAREADER_H
#include <iostream>
#include <fstream>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <math.h>
#include <vector>
// ROOT headers
#include "Rtypes.h"
#include "TTree.h"

using namespace std;
/*	
#define DATA 0x44415441
#define	DIR  0x44495220
#define	HEAD 0x48454144
#define	PAC  0x50414320
#define	SCAL 0x5343414C
#define	DEAD 0x44454144
#define	EF   0x454F4620
#define	PAD  0xFFFFFFFF
*/
#define DATA 0x41544144
#define	DIR  0x20524944
#define	HEAD 0x44414548
#define	PAC  0x20434150
#define	SCAL 0x4C414353
#define	DEAD 0x44414544
#define	EF   0x20464F45
#define	PAD  0xFFFFFFFF

class LDFDataReader{
  	ifstream *ldf_data_file; // raw data file

  	ULong64_t record_address;
    
  	// LDF data frame data

	UInt_t tilNext;
public:
  UInt_t ID[8];
  UInt_t data[8];
  UInt_t scal[8];  	
std::string scals;  
LDFDataReader();
  ~LDFDataReader();

  Int_t openFile(const char *filename); 
  Int_t closeFile();

  bool get_end_of_file() {return ldf_data_file->eof();}

  // LDF data
  UInt_t* get_ID() {return ID;}
  UInt_t* get_data() {return data;}
  
  
  //Sets memory address and current location to beginning of next record
  Int_t setToNextRecord();
  // functions for reading raw data
  Int_t readDataEvent();
  Int_t readScalarEvent();
  Int_t parseScalars();
  // reads one word of file
  Int_t readOneWord();
  //Goes to next record
  bool toNextRecord();
};
#endif
