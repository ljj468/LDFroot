#include "LDFroot.h"

int main(int argc, char *argv[]){
  // check for correct std inputs
  if(argc != 3){
    cout << "Usage: LDFroot [input file] [output file]" << endl;
    return -1;
  }
  
  // open input file
  ifstream input_file(argv[1]);
  if(!input_file.is_open()){
    cout << "Cannot open file " << argv[1] << endl;
    return -2;
  }
  
  //-------Read input file parameters-------
  
  // filename for LDF data file
  string get_directory;
  input_file >> get_directory; // input

  // output file
  string output_filename;
  input_file >> output_filename; // 4th input
  
  // maximum number of events
  UInt_t EndEvent = 0;
  input_file >> EndEvent; // 5th input

  //------------------------------------------
    
  // LDFDataReader object
  LDFDataReader *get  = new LDFDataReader();
 //------------Data------------------- 

  // Root file open
  TFile *fOut  = new TFile(argv[2], "RECREATE");

  TTree *tneuf = new TTree("AutoTree", "New tree...");

  //---------Make branches------------

get->openFile(argv[1]);
/*  if(IsOpen == -1){
    exit(1);
  }*/

    
  tneuf->Branch("data", get->data, "data[8]/i");




TFile *fOutScal  = new TFile(argv[2], "RECREATE");

  TTree *tneufScal = new TTree("AutoTree", "New tree...");

  //---------Make branches------------

get->openFile(argv[1]);
/*  if(IsOpen == -1){
    exit(1);
  }*/

    
  tneufScal->Branch("scal", get->scal, "Scal[8]/i");






int type=0;
//type=get->readOneWord();
bool ldf_eof=false;
// event loop


while(!ldf_eof){


	if(type%10==8){
		ldf_eof=true;
	}
	else if(type%10==2){
		get->readOneWord();
		bool next_event=true;
		int eventid;
		while(next_event){
			eventid=get->readDataEvent();
			if(eventid/10>0){
				
				tneuf->Fill();
			}
			next_event=(eventid%10==1);
			if(!next_event){
				type=eventid;
				}

			}
	}
	else if(type%10==5){
	
	get->readScalarEvent();
	get->parseScalars();
		}
	else if((type%10>2)&&(type%10<8)){
	
	type=get->readOneWord();	
	}
	
	else{

	type=get->readOneWord();
		
	}
	
 }

  // save ROOT file
  fOut->Write();
  fOut->Close();

  return 0;
}
