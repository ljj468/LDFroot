#include "LDFDataReader.h"

// constructor
LDFDataReader::LDFDataReader() {
	ldf_data_file=NULL;
	
    record_address = 4*8194;	
scals='\0';
	tilNext=8194;
}

// destructor
LDFDataReader::~LDFDataReader(){
};

// open raw data file
Int_t LDFDataReader::openFile(const char *filename){

  ldf_data_file = new ifstream(filename, ios::binary);
  
  if(ldf_data_file->is_open()){
    cout << "Opened file " << filename << endl;
    return 0;
  }
  else{
    cout << "File " << filename << " could not be opened." << endl;
    return -1;
  }
}
Int_t LDFDataReader::setToNextRecord(){
	//Sets record_address, used for debugging
	Int_t recordID=0;
	ULong64_t temp_word;
	while(recordID==0){
	ldf_data_file->read((char *) &temp_word, 4);
	recordID=
		2*(temp_word==DATA)+3*(temp_word==DIR)+4*(temp_word==HEAD)+
		5*(temp_word==SCAL)+6*(temp_word==DEAD)+7*(temp_word==EF);
	
	}
	return recordID;
}
// close raw data file
Int_t LDFDataReader::closeFile(){
  ldf_data_file->close();
  return 0;
}

// read a data packet for
Int_t LDFDataReader::readScalarEvent(){
	ULong64_t wordID;
	ULong64_t wordDAT;
	
	ULong64_t temp_word;
	
	int recordID=0;
	bool cont=true;
	bool done=false;
	bool keep=true;
	int count=0;
	
	ldf_data_file->read((char *) &temp_word, 4);
	while(cont){
	ldf_data_file->read((char *) &temp_word, 4);
	char c1= (char)((temp_word & 0x000000FF));
	char c2= (char)((temp_word & 0x0000FF00) >> 8);
	char c3=(char)((temp_word & 0x00FF0000) >> 16);
	char c4=(char)((temp_word & 0xFF000000) >> 24);
	
	keep=(temp_word!=0);
	if(keep){
		scals+=c1+c2+c3+c4;
		}
	recordID =	1*(temp_word==PAD)+2*(temp_word==DATA)+
				3*(temp_word==DIR)+4*(temp_word==HEAD)+
				5*(temp_word==SCAL)+6*(temp_word==DEAD)+
				7*(temp_word==EF)+8*ldf_data_file->eof()+(10*count);
				//Return variable for readEvent(), integer valued.
				//The ones digit, returns an ID of event type:
				//1: Normal Data Event or padding
				//2: DATA type record header
				//3: DIR type record header
				//4: HEAD type record header
				//5: SCAL type record header
				//6: DEAD type record header
				//7: EF type record header
				//8: End of File passed
				//9: Zeros
				//The digits before the ones digit represent the number 
				//of channels/words read
	cont=(recordID%10==0);
	if((!keep)&&(!done)){
		scals.erase(scals.find_last_not_of(" \n\r\t")+1);
		done=true;
		
		}
	count++;	
	

}
  return recordID;
}
Int_t LDFDataReader::parseScalars(){
		int ind=0;
		std::vector<std::string> scalslst;	
		std::string temp='\0';	
		while(ind<scals.length()){
			
			if(scals[ind]==' '){
			if(temp!='\0'){
			scalslst.push_back(temp);}
			cout << temp;
			temp='\0';
				}
			else{
				temp+=scals[ind];}
			
			
			
			ind++;
			}
	
	
	return 1;	

	}
Int_t LDFDataReader::readDataEvent(){
	ULong64_t wordID;
	ULong64_t wordDAT;
	
	ULong64_t temp_word;
	
	int recordID=0;
	bool cont=true;
	int count=0;
	while(cont){
	ldf_data_file->read((char *) &temp_word, 4);
	
	wordID= (temp_word & 0x000000FF);
	wordDAT= (temp_word & 0xFFFF0000) >> 16;
	
	
	recordID =	1*(temp_word==PAD)+2*(temp_word==DATA)+
				3*(temp_word==DIR)+4*(temp_word==HEAD)+
				5*(temp_word==SCAL)+6*(temp_word==DEAD)+
				7*(temp_word==EF)+8*ldf_data_file->eof()+(10*count);
				//Return variable for readEvent(), integer valued.
				//The ones digit, returns an ID of event type:
				//1: Normal Data Event or padding
				//2: DATA type record header
				//3: DIR type record header
				//4: HEAD type record header
				//5: SCAL type record header
				//6: DEAD type record header
				//7: EF type record header
				//8: End of File passed
				//9: Zeros
				//The digits before the ones digit represent the number 
				//of channels/words read
	cont=(recordID%10==0);
	if(cont){
		data[wordID]=wordDAT;
		}
	count++;	
	

}
  return recordID;
}

Int_t LDFDataReader::readOneWord(){
	//Reads one word and returns that words id
	Int_t wordID=0;
	UInt_t temp_word=0;
	ldf_data_file->read((char *)&temp_word, 4);
	wordID=
		1*(int)(temp_word==PAD)+2*(int)(temp_word==DATA)+3*(int)(temp_word==DIR)+
		4*(int)(temp_word==HEAD)+5*(int)(temp_word==SCAL)+6*(int)(temp_word==DEAD)+
		7*(int)(temp_word==EF)+8*(int)ldf_data_file->eof();
	
	return wordID;

	}


bool LDFDataReader::toNextRecord(){
	//Goes to next record in file, sets memory address to record after
	ldf_data_file->seekg(record_address);
	record_address+=4*8194;
	return record_address; 
}
