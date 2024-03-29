void GLogicProcessor::writeDictionaryTXT(commandData *inputData,  map<string, OCRDictRecord>&mainDict){
	
	ofstream srcOutput;
	string str=inputData->data["inputFile"];
	str+="_Dict.txt";
	srcOutput.open(str.c_str());
	int step=0,index=0,count=0;
	map<string, OCRDictRecord>::iterator p; 
	string mainString;
	char buff[10];
	
	for(p = mainDict.begin(); p != mainDict.end(); p++) {
		if (step==100000){
			srcOutput<<mainString; mainString="";
			cout<<"save line "<<index<<" word count "<<count<<END;step=0; 
		}
		step++;index++;
		if(p->second.wordCount>0){
			sprintf(buff,"%d",p->second.wordCount );
			mainString+=buff;
			mainString+=":|:";
			mainString+=p->first;
			mainString+="\n"; 
			count++;   
			
		}
	}	
	srcOutput<<mainString;
	srcOutput.close();				   
	cout<<"dict save done."<<END;
	
}//////////////////////////////////////////////////////////////////////////


void GLogicProcessor::loadDictLevelFile(commandData *inputData){
	ifstream srcInput;
	string str=inputData->data["inputFile"];
	srcInput.open(str.c_str());
	string srcLine;
	int index=0,step=0,count=0; 
	
	while(getline(srcInput, srcLine,'\n')){
		srcLine=str_replace("\r", "", srcLine);
		vector <string> stringItemVector;
		stringItemVector=explode(":|:", srcLine);
		if(stringItemVector.size()<2)continue;
		
		//cout<<"stringItemVector[1]="<<stringItemVector[1]<<" wordCount="<<
		//mainDict[stringItemVector[1]].wordCount<<" strVector[0]="<<atoi(stringItemVector[0].c_str())<<END;
		count=atoi(stringItemVector[0].c_str());   //cout<<"count="<<count<<END;       
		if(count>20){
			mainDict[stringItemVector[1]].wordCount+=count;
			if(step==100000){
				cout<<index<<" mainDict.size()="<<mainDict.size()<<END;
				step=0;
			}
			step++;
		}index++;
	}
	srcInput.close();
}//________________________________________________

void GLogicProcessor::writeDictionary(map<int, OCRDictRecord> &dictRecord, string &filePath){
	int i, step=0, count=0;
	
	int wordID=0;
	char cbuf[20];
	string str;
	int allDictSaved=0;
	map<int, OCRDictRecord>::iterator in=dictRecord.begin();
	string dictFlag="";
	int recordFound;
	time_t t;
	map<string, dictKey>::iterator p; 
	cout<<"start write dictionary. DictRecord size="<<dictRecord.size()<<endl;
	
	while(!allDictSaved){
		xml_document doc;
		xml_node mainNode;
		xml_node book, data, key;
		mainNode=doc.append_child();
		mainNode.set_name("dictBase");
		i=0; wordID=0;
		recordFound=0;
		
		while(i<dictRecord.size()){
			if(dictRecord[i].dict=="")dictRecord[i].dict="ND";
			if(dictRecord[i].dict.size()>5||dictRecord[i].dict.size()<2)dictRecord[i].dict="ND";
			
			if(!dictRecord[i].recordFlag){i++;continue;}
			if(dictFlag==""){
				dictFlag=dictRecord[i].dict;
				cout<<"now save "<<dictFlag<<" dict"<<endl;
			}
			if(dictRecord[i].dict==dictFlag){
				book=mainNode.append_child();
				//sprintf(cbuf,"%d",i);
				book.set_name("rec");
				t=time(NULL); while(wordID==t){
#ifdef MACOS
				sleep(1);
#endif

				t=time(NULL);}   //get timeID
				wordID=t;
				book.append_attribute("id")=(unsigned int)t;  wordID=wordID;
				book.append_attribute("dt")=dictRecord[i].dict.c_str();
				
				for(p = dictRecord[i].key.begin(); p != dictRecord[i].key.end(); p++) {
					key=book.append_child();  key.set_name("key");
					key.append_attribute("key")=p->second.key.c_str();
					key.append_attribute("ln")=p->second.ln.c_str();
					if(p->second.ln=="tb")
						key.append_attribute("tk")=p->second.tibKey.c_str();
					if(p->second.context!="")
						key.append_attribute("c")=p->second.context.c_str();
					if(p->second.grm!="")
						key.append_attribute("grm")=p->second.grm.c_str();
				}
				if(dictRecord[i].eentry.size()){
					data=book.append_child();
					data.set_name("r1");
					data.append_child(node_pcdata).set_value(dictRecord[i].eentry.c_str());
				}
				if(dictRecord[i].rentry.size()){
					data=book.append_child();
					data.set_name("r2");
					data.append_child(node_pcdata).set_value(dictRecord[i].rentry.c_str());
				}
				if(dictRecord[i].example.size()){
					data=book.append_child();
					data.set_name("r3");
					data.append_child(node_pcdata).set_value(dictRecord[i].example.c_str());
				}
				if(dictRecord[i].example.size()){
					data=book.append_child();
					data.set_name("exp");
					data.append_child(node_pcdata).set_value(dictRecord[i].exp.c_str());
				}
				dictRecord[i].recordFlag=0;
				//wordID++;
				if(step>5000){cout<<"."<<wordID<<endl; step=0;}step++;  count++;
				recordFound=1;
			} i++;
		}
		if(recordFound){
			string path=filePath+"/"+dictFlag+".xml";
			cout<<"start save"<<path<<endl;
			doc.save_file(path.c_str(),"\t", 1);
			cout<<wordID<<"records in "<<path<<" saved"<<endl;
			dictFlag="";
		}else{allDictSaved=1;cout<<"all done";};
	}
	
	
}//////////////////////////////////////////////////////////////////////////

void GLogicProcessor::readDictionary(commandData *inputData){
	xml_node col,data,resultSet,dictRecordXML, st;
	xml_document doc;
	string str,pathDict;
	int i=0,wID=0,step=0,count=0, index;
	vector<string>fileList;
	string path;
	//read source file with tibetan string punctuation mark
	//cout<<" inputData->data[wordsDataPath]="<<inputData->data["wordsDataPath"]<<endl;
	readGrammarDataXML(inputData->data["wordsDataPath"]); 
	//read all dictinary XML files in vector
	readDirectoryToArray(fileList, inputData->data["dictPath"], "xml");  
	LoadMapXML(inputData);  cout<<"load1";
	LoadYagpoToWylieMap(inputData);  cout<<"load21";
	
	for(int fileIndex=0;fileIndex<fileList.size();fileIndex++){
		xml_parse_result stat;
		cout<<" start load "; cout<<fileList[fileIndex]<<END;
		stat=doc.load_file(fileList[fileIndex].c_str());
		
		if(!stat){cout<<fileList[fileIndex]<<" not loaded";
			cout<<stat.description()<<" offset="<<stat.offset<<" line="<<stat.line<<endl;
			ifstream file (fileList[fileIndex].c_str(), ios::in|ios::binary|ios::ate);
			long offset=0; int lineIndex=0;
			ifstream::pos_type size;
			//long size_block = file.tellg();
			
			if (file.is_open())
			{
				int size = 1024;
				char *memblock = new char [size];
				//file.seekg (0, ios::beg);
				file.seekg (stat.offset, ios::beg);
				file.read (memblock, size);
				str=memblock;
				str=str_replace("<","{", str);
				str=str_replace(">","}", str);
				//file.read (memblock, size_block);
				file.close();
				cout<<"line offset="<<str<<endl;
				//c_out<<"line offset="<<memblock<<endl;
				delete memblock;     //cout<<"size_block="<<size_block<<endl;
			}
			exit(0);
		}
		
		
		// Iterate through books
		cout<<" start parsing ";
		dictRecordXML = doc.child("dictBase");
		
		
		for (xml_node row = dictRecordXML.child("rec"); row; row = row.next_sibling("rec")){
			//wID=atoi(row.attribute("id").value());
			dictRecord[i].wordID=wID;
			dictRecord[i].dict=row.attribute("dt").value();
			index=0;
			
			for (xml_node keyRecord = row.child("key"); keyRecord; keyRecord = keyRecord.next_sibling("key")){
				dictKey key;
				
				key.key=keyRecord.attribute("key").value();
				key.ln= keyRecord.attribute("ln").value();
				str=key.key;
				if(key.ln=="tb"){
					for(int m=1;m<delimeterTib.size();m++){
						str=str_replace(delimeterTib[m].c_str()," ",str);
					}
					//cout<<"  d  ";
					for(int m=0;m<grammarTib.size();m++){
						str=str_replace(grammarTib[m].c_str()," ",str);
					}
					str=str_replace("  "," ",str);
					str=str_replace("  "," ",str);
					key.tibKey=str;  //cout<<"str="<<str<<endl;
				}
				if(key.ln=="pl"||key.ln=="sk"){
					for(int m=1;m<delimeterPali.size();m++){
						str=str_replace(delimeterPali[m].c_str()," ",str);
					}
					for(int m=0;m<grammarPali.size();m++){
						str=str_replace(grammarPali[m].c_str()," ",str);
					}
					key.tibKey=str;
					//cout<<"strKey="<<str<<"/"<<endl;
				}
				key.context=keyRecord.attribute("c").value();
				key.grm= keyRecord.attribute("grm").value();
				key.level=index;
				
				//cout<<"strKey="<<str<<"/"<<endl;
				
				if(key.ln=="tb"){
					dictIndexTib[str].push_back(wID);
					//cout<<"dictIndexTib["<<str<<"]"<<dictIndexTib[str].size()<<endl;
				}
				if(key.ln=="sk"||key.ln=="pl"){
					dictIndexPali[str].push_back(wID);
				}
				dictRecord[i].key[key.ln]=key;
				dictRecord[i].reportKey.push_back(key);
				
			}
			dictRecord[i].recordFlag=1;
			
			col = row.child("r1");
			
			dictRecord[i].eentry=col.child_value();
			col = row.child("r2");
			dictRecord[i].rentry=col.child_value();
			col = row.child("r3");
			dictRecord[i].example=col.child_value();
			col = row.child("exp");
			dictRecord[i].exp=col.child_value();
			i++; wID++;
			if(step>5000){cout<<".";step=0;}step++;  count++;
		}
		
		//fileName+="1";
		//doc.save_file(fileName.c_str(),"\t", 1);
		//writeDictionary(dictRecord,fileName,c_out);
		cout<<"done read dictionary dictIndexTib.size()="<<dictIndexTib.size()<<"dictIndexPali.size()="<<dictIndexPali.size()<<END;
	}
	
	//return;
	
	vector <string> pathArray;
	string pathHash=inputData->data["root"]+HASH_PATH;  cout<<" pathHash="<<pathHash<<endl;
	readDirectoryToArray(pathArray, pathHash ,"txt");	
	unsigned int hash;
	
	cout<<"translate "<<0<<" from "<<pathArray.size()<<END;
	//readText(textBuffer,pathArray[0].c_str());
	
	if(pathArray.size()==3){
		
		cout<<"read hash index from file"<<endl;
		
		FILE * hFile;
		path=pathHash;	path+="/hashIndex.txt";	
		
		hFile = fopen(path.c_str(), "r");
		unsigned long  size;
		if (hFile == NULL){
			return; 
		}else{
			fseek(hFile, 0, SEEK_END);
			hashSize= ftell(hFile);   cout<<" size="<<size<<endl;   
			fseek(hFile, 0, SEEK_SET);
			hashData = (char *)malloc(hashSize+1);
			if(hashData==NULL){cout<<"no memory"<<endl; exit(0);} 
			cout<<" s1"<<endl;  
			if(hashData==NULL)exit(0);
			if (hashSize != fread(hashData, sizeof(char), hashSize, hFile)){
				free(hashData);
				cout<<"no read hash file"<<endl;
				exit(0); 
			}
			fclose(hFile);
		}
	    cout<<" s2"<<endl;
		path=pathHash;	path+="/hashIndexRecord.txt";	
		cout<<" s3"<<endl;
		hFile = fopen(path.c_str(), "r");
		if (hFile == NULL){
			return; 
		}else{
			cout<<" s4"<<endl;
			fseek(hFile, 0, SEEK_END);
			hashSize= ftell(hFile);   //cout<<" size="<<size<<endl;   
			fseek(hFile, 0, SEEK_SET);
			//char *memblock = (char *)malloc(hashSize+1);
			hashVector=(hashRecord*)malloc(hashSize);
			cout<<" s5"<<endl;
			if(hashData==NULL){cout<<"no memory"<<endl; exit(0);} 
			
			if (hashSize != fread(hashVector, sizeof(char), hashSize, hFile)){
				free(hashData);
				cout<<"no read hashRecord file"<<endl;
				exit(0); 
			}
			fclose(hFile);
			//cout<<" s6"<<endl;
			//char *ptr=(char*)hashVector;
			//cout<<" s7"<<endl;
			//memcpy(ptr, memblock, hashSize);
			//cout<<" s8"<<endl;
			//free(memblock);
			hashSize/=8; //Real element count of hashVector=hashSize/8(size_of(hashRecord))
			//for(int i=0;i<100;i++){cout<<".hashVector.size()="<<hashVector.size()<<END;cout<<hashVector[i].hash<<END;}
		}
		
		
		cout<<"done read hashIndex hashSize="<<hashSize<<endl;
		path=pathHash;	path+="/sortTextDictionary.txt";			
		hFile = fopen(path.c_str(), "r");
		if (hFile == NULL){
			return; 
		}else{
			fseek(hFile, 0, SEEK_END);
			size= ftell(hFile);   //cout<<" size="<<size<<endl;   
			fseek(hFile, 0, SEEK_SET);
			textBuffer = (char *)malloc(size+1);
			if(textBuffer==NULL)exit(0);
			if (size != fread(textBuffer, sizeof(char), size, hFile)){
				free(textBuffer);
				exit(0); 
			}
			fclose(hFile);
		}
		cout<<"done read textBuffer="<<size<<endl;
		
	}else{
	   if(pathArray.size()>0)
		buildDictionaryHashIndex(inputData);
	}	
	
	
	/*readText(dictRecordBase,pathArray[i]);
	 int step=0;
	 unsigned long size = dictRecordBase.size();
	 unsigned int *memblock =new unsigned int [size];
	 for(int j=0;j<dictRecordBase.size();j++){
	 
	 hash=strToHash(dictRecordBase[j].c_str(),dictRecordBase[j].length());
	 //hashDict[hash]=strVector[j];
	 memblock[j]=hash;
	 hashDict.insert(pair<int,int>(hash,1));
	 //if(hashDict[hash]>1)count++;
	 if(step==100000){cout<<j<<END;step=0;}step++;
	 }
	 cout<<"hashDict.size()="<<hashDict.size()<<END;						
	 */
	//delete[] memblock;     //cout<<"size_block="<<size_block<<endl;
	
	
	//}
	cout <<"dict loaded"<<END;
	dictionaryReady=1;
	
	
	
}//////////////////////////////////////////////////////////////////////////////////////////////////


void GLogicProcessor::buildDictionaryHashIndex(commandData *inputData){
	
	cout<<"start build hash"<<endl;
	unsigned long size_, size_hash;
	unsigned int *hashBuffer,*p,*b; 
	vector<hashRecord> hash_Vector;
#define MEM_CHINK_INT 268435456
#define MEM_CHINK_CHAR 1073741824
	
	cout<<"start build hash index from text"<<endl;
	
	FILE * hFile;
	string pathHash=inputData->data["root"]+HASH_PATH;
	string path=pathHash;	path+="/sortTextDictionary.txt";			
	hFile = fopen(path.c_str(), "r");
	unsigned long  sizeText;
	if (hFile == NULL){
		exit(0); 
	}else{
		fseek(hFile, 0, SEEK_END);
		sizeText= ftell(hFile);   //cout<<" size="<<size<<endl;   
		fseek(hFile, 0, SEEK_SET);
		textBuffer = (char *)malloc(sizeText+1);
		if(textBuffer==NULL)exit(0);
		if (sizeText != fread(textBuffer, sizeof(char), sizeText, hFile)){
			free(textBuffer);
			exit(0); 
		}
		fclose(hFile);
	}
	cout<<"done read textBuffer="<<sizeText<<endl;
	
	
	char c;
	char *a;
	char *startIndexPtr=textBuffer;
	unsigned int startIndex,ln,offset, index=0;
	unsigned int byteAdress, adress,adressInt;
	char memBlock[2048];
	string srcStr,srcLine,str,testLine;
	vector<string>stringItemVector,keyIndex;
	int step=0,match;
	
	
	unsigned long hash_Size=536870912+512;
	hashData=(char*)malloc(hash_Size);
	if(hashData==NULL){cout<<"no memory for hash"<<END; exit(0);} 
	memset(hashData,0,hash_Size);
	cout<<"start build hash index from text"<<END;
	startIndex=0;
	startIndexPtr=textBuffer;
	
	for(int i=0;i<sizeText;i++){
		c=textBuffer[i];
		//cout<<"c="<<c<<endl;
		if(c=='\n'){
			ln=i-startIndex;
			//next we set all part of string in hash index
			//cout<<"ln="<<ln<<endl;
			memset(memBlock,0,2048);                                
			if(ln>2044){
				cout<<"too long line "<<index<<endl; 
				startIndex=i+1;
				index++;
				startIndexPtr=textBuffer+i+1;
				continue;
			}
			memcpy(memBlock,startIndexPtr,ln);
			//cout<<"mem="<<memBlock<<"/ st.size="<<ln<<endl;
			srcStr=memBlock; 
			
			//cout<<" srcStr="<<srcStr;
			stringItemVector=explode(" ",srcStr);  if(stringItemVector.size()>200){cout<<"too long line "<<index<<endl; continue;}
			
			srcStr=str_replace(" ", "", srcStr);  //remove all delimeters from key string
			//cout<<" srcStr__="<<srcStr<<endl;   
			
			adressInt=strToHash(srcStr.c_str(),srcStr.size()); 
			byteAdress=adressInt>>3; 
			adress=adressInt-(byteAdress<<3);  			
			a=hashData+byteAdress;	
			
			//put bite on adress of hash
			*a=(1<<adress)|*a;
			
			//put data in hash address table
			//cout<<" adressInt="<<adressInt<<endl;	
			hashRecord rec;
			rec.hash=adressInt;
			rec.adress=startIndex;     //if(adressInt==4114664782)cout<<"@@@match1 srcStr="<<srcStr<<"offset="<<startIndex<<endl;
			hash_Vector.push_back(rec);
			
			cout<<"start report stringItemVector.size()="<<stringItemVector.size()<<END;
			
			for (int entry=0; entry<stringItemVector.size();entry++){ //for every syllabon in List
				
				for (int k=stringItemVector.size();k>entry;k--){ //for every syllabon in List
					ln=0; testLine="";
					for (int c=entry;c<k;c++){
						testLine+=stringItemVector[c];   //all syllabons in this part of source string
						//ln++;
					}
					//cout<<"testLine=/"<<testLine<<"/"<<END; 
					adressInt=strToHash(testLine.c_str(), testLine.size()); 
					byteAdress=adressInt>>3; 
					adress=adressInt-(byteAdress<<3);  
					a=hashData+byteAdress;
					
					//read bite from adress
					match = (*a>>adress)&1;
					//cout<<"data="<<data<<endl;
					
					if(match)continue;
					
					//put bite on adress of hash
					*a=(1<<adress)|*a;
					
					//put data in hash addres table
					offset=0;
					for (int c=0;c<entry;c++){
						offset+=stringItemVector[c].size()+1;   //all syllabons in this part of source string
					}
					offset+=startIndex;
					
					//cout<<" adressInt="<<adressInt<<endl;	
					hashRecord rec;
					rec.hash=adressInt;
					rec.adress=offset;
					hash_Vector.push_back(rec);    //if(adressInt==4114664782)cout<<"@@@match srcStr="<<srcStr<<"offset="<<offset<<endl;
				}//end for (int k=StringItemList->Count
			}//end for (int entry=0;
			
			//for(int a=0;a<ln;a++){cout<<"memBlock["<<a<<"]="<<(short)memBlock[a]<<" str["<<a<<"]="<<(short)str[a]<<endl;}
			//cout<<"byteAdress="<<byteAdress<<" adress="<<dec<<adress<<" hash="<<adressInt<<endl;
			
			startIndex=i+1;
			index++;
			startIndexPtr=textBuffer+i+1;
			if(step==100000){
				cout<<"."<<index;
			step=0;}step++;
		}
	}
	//dictRecordBase.resize(index);
	//fwrite(hashBuffer0, sizeof(char), size_hash, hFile1);
	
	free(textBuffer);
	cout<<"done build index="<<index<<endl<<"start write on /hashIndex.txt"<<endl; 
	path=pathHash;	path+="/hashIndex.txt";			
	writeText(hashData,hash_Size,path.c_str());
	free(hashData);
	
	
	//hashBuffer=(unsigned int *)malloc(MEM_CHINK_CHAR);
	sort(hash_Vector.begin(),hash_Vector.end());
	
	cout<<"start write hash1 index on disk"<<endl;
	path=pathHash;	path+="/hashIndexRecord.txt";			
	hFile = fopen(path.c_str(), "w");
	hashBuffer=(unsigned int*)&hash_Vector[0];
	fwrite(hashBuffer, sizeof(hashRecord), hash_Vector.size(), hFile); 
	
	cout<<"done build hash index "<<END;
	
}//_______________________________________________________________________________________________

void GLogicProcessor::buildDictionaryHashIndexUni(commandData *inputData, vector<string>&strVector){
	
	cout<<"start build hash"<<endl;
	unsigned long size_, size_hash;
	unsigned int *hashBuffer,*p,*b; 
	vector<hashRecord> hash_Vector;

	unsigned long hash_Size=536870912+512;
	static int hash_exist=0;
	if(!hash_exist){hashData=(char*)malloc(hash_Size);
		if(hashData==NULL){cout<<"no memory for hash"<<END; exit(0);} 
		memset(hashData,0,hash_Size);
		hash_exist=1;
		cout<<"start build hash in memory"<<END;
	}	
	cout<<"start build hash index from text"<<END;
	
	for(int i=0;i<strVector.size();i++){
            wstring textLine=UTF_to_Unicode(strVector[i]);
			
			//cout<<" srcStr="<<strVector[i]<<endl;
			if(textLine.size()>1024){cout<<"too long line "<<i<<endl; continue;}
			
	}
	/*
			adressInt=strToHash(srcStr.c_str(),srcStr.size()); 
			byteAdress=adressInt>>3; 
			adress=adressInt-(byteAdress<<3);  			
			a=hashData+byteAdress;	
			
			//put bite on adress of hash
			*a=(1<<adress)|*a;
			
			//put data in hash address table
			//cout<<" adressInt="<<adressInt<<endl;	
			hashRecord rec;
			rec.hash=adressInt;
			rec.adress=startIndex;     //if(adressInt==4114664782)cout<<"@@@match1 srcStr="<<srcStr<<"offset="<<startIndex<<endl;
			hash_Vector.push_back(rec);
			
			cout<<"start report stringItemVector.size()="<<stringItemVector.size()<<END;
			
			for (int entry=0; entry<stringItemVector.size();entry++){ //for every syllabon in List
				
				for (int k=stringItemVector.size();k>entry;k--){ //for every syllabon in List
					ln=0; testLine="";
					for (int c=entry;c<k;c++){
						testLine+=stringItemVector[c];   //all syllabons in this part of source string
						//ln++;
					}
					//cout<<"testLine=/"<<testLine<<"/"<<END; 
					adressInt=strToHash(testLine.c_str(), testLine.size()); 
					byteAdress=adressInt>>3; 
					adress=adressInt-(byteAdress<<3);  
					a=hashData+byteAdress;
					
					//read bite from adress
					match = (*a>>adress)&1;
					//cout<<"data="<<data<<endl;
					
					if(match)continue;
					
					//put bite on adress of hash
					*a=(1<<adress)|*a;
					
					//put data in hash addres table
					offset=0;
					for (int c=0;c<entry;c++){
						offset+=stringItemVector[c].size()+1;   //all syllabons in this part of source string
					}
					offset+=startIndex;
					
					//cout<<" adressInt="<<adressInt<<endl;	
					hashRecord rec;
					rec.hash=adressInt;
					rec.adress=offset;
					hash_Vector.push_back(rec);    //if(adressInt==4114664782)cout<<"@@@match srcStr="<<srcStr<<"offset="<<offset<<endl;
				}//end for (int k=StringItemList->Count
			}//end for (int entry=0;
			
			//for(int a=0;a<ln;a++){cout<<"memBlock["<<a<<"]="<<(short)memBlock[a]<<" str["<<a<<"]="<<(short)str[a]<<endl;}
			//cout<<"byteAdress="<<byteAdress<<" adress="<<dec<<adress<<" hash="<<adressInt<<endl;
			
			startIndex=i+1;
			index++;
			startIndexPtr=textBuffer+i+1;
			if(step==100000){
				cout<<"."<<index;
				step=0;}step++;
		}
	}
	//dictRecordBase.resize(index);
	//fwrite(hashBuffer0, sizeof(char), size_hash, hFile1);
	
	free(textBuffer);
	cout<<"done build index="<<index<<endl<<"start write on /hashIndex.txt"<<endl; 
	path=pathHash;	path+="/hashIndex.txt";			
	writeText(hashData,hash_Size,path.c_str());
	free(hashData);
	
	
	//hashBuffer=(unsigned int *)malloc(MEM_CHINK_CHAR);
	sort(hash_Vector.begin(),hash_Vector.end());
	
	cout<<"start write hash1 index on disk"<<endl;
	path=pathHash;	path+="/hashIndexRecord.txt";			
	hFile = fopen(path.c_str(), "w");
	hashBuffer=(unsigned int*)&hash_Vector[0];
	fwrite(hashBuffer, sizeof(hashRecord), hash_Vector.size(), hFile); 
*/	
	//cout<<"done build hash index "<<END;
	
}//_______________________________________________________________________________________________

void GLogicProcessor::reloadDictionary(commandData *inputData){
	xml_node col,data,resultSet,dictRecordXML, st;
	xml_document doc;
	string str;
	int i=0,wID=0,step=0,count=0, index;
	
	//cout<<fileName.c_str();
	
	ofstream c_out;  //c_out.open("/pali.txt");
	
	cout<<"erase old value from "<<inputData->data["inputFile"]<<" dictionary";
	xml_parse_result stat;
	cout<<" start load "<<inputData->data["inputFile"]<<endl;
	stat=doc.load_file(inputData->data["inputFile"].c_str());
	
	if(!stat){cout<<inputData->data["inputFile"]<<" not loaded";
		cout<<stat.description()<<" offset="<<stat.offset<<" line="<<stat.line<<endl;
		ifstream file (inputData->data["inputFile"].c_str(), ios::in|ios::binary|ios::ate);
		long offset=0; int lineIndex=0;
		ifstream::pos_type size;
		//long size_block = file.tellg();
		
		if (file.is_open())
		{
			int size = 1024;
			char *memblock = new char [size];
			//file.seekg (0, ios::beg);
			file.seekg (stat.offset, ios::beg);
			file.read (memblock, size);
			//file.read (memblock, size_block);
			file.close();
			cout<<"line offset="<<memblock<<endl;
			c_out<<"line offset="<<memblock<<endl;
			delete memblock;     //cout<<"size_block="<<size_block<<endl;
		}
		exit(0);
	}
	
	
	// Iterate through books
	cout<<" start parsing ";
	//  cd ../../YagpoOCRProgram   yagpo_dict -dt tt t1
	dictRecordXML = doc.child("dictBase");
	
	
	for (xml_node row = dictRecordXML.child("rec"); row; row = row.next_sibling("rec")){
		//wID=atoi(row.attribute("id").value());
		dictRecord[i].wordID=wID;
		dictRecord[i].dict=row.attribute("dt").value();
		index=0;
		
		for (xml_node keyRecord = row.child("key"); keyRecord; keyRecord = keyRecord.next_sibling("key")){
			dictKey key;
			
			key.key=keyRecord.attribute("key").value();
			key.ln= keyRecord.attribute("ln").value();
			if(key.ln=="tb"){
				str=key.key;
				for(int m=1;m<delimeterTib.size();m++){
					str=str_replace(delimeterTib[m].c_str()," ",str);
				}
				//cout<<"  d  ";
				for(int m=0;m<grammarTib.size();m++){
					str=str_replace(grammarTib[m].c_str()," ",str);
				}
				key.tibKey=str;
			}
			key.context=keyRecord.attribute("c").value();
			key.grm= keyRecord.attribute("grm").value();
			key.level=index;
			if(key.ln=="tb"){
				dictIndexTib[str].push_back(wID);
			}
			if(key.ln=="pl"){
				dictIndexPali[str].push_back(wID);
			}
			dictRecord[i].key[key.ln]=key;
			
		}
		dictRecord[i].recordFlag=1;
		
		col = row.child("r1");
		
		dictRecord[i].eentry=col.child_value();
		col = row.child("r2");
		dictRecord[i].rentry=col.child_value();
		col = row.child("r3");
		dictRecord[i].example=col.child_value();
		col = row.child("exp");
		dictRecord[i].exp=col.child_value();
		i++; wID++;
		if(step>5000){cout<<".";step=0;}step++;  count++;
	}
	
	//fileName+="1";
	//doc.save_file(fileName.c_str(),"\t", 1);
	//writeDictionary(dictRecord,fileName,c_out);
	cout<<"Done"<<endl;
	
	
}//////////////////////////////////////////////////////////////////////////////////////////////////

void GLogicProcessor::writeInDictionary(commandData *inputData){
	xml_node col,data,resultSet,dictRecordXML, st;
	xml_document doc;
	string str;
	int i=0,wID;
	
	
	/*
	 NAME="tibKey" TYPE="TEXT"/>
	 <NAME="engKey" TYPE="TEXT"/>
	 <NAME="rusKey" TYPE="TEXT"/>
	 <NAME="sncKey" TYPE="TEXT"/>
	 <NAME="eentry" TYPE="TEXT"/>
	 <NAME="rentry" TYPE="TEXT"/>
	 <NAME="example" TYPE="TEXT"/>
	 <NAME="dict" TYPE="TEXT"/>
	 <NAME="rus_translit" TYPE="TEXT"/>   /--
	 <NAME="Wylie" TYPE="TEXT"/>
	 <NAME="ID" TYPE="NUMBER"/>
	 <NAME="in1" TYPE="TEXT"/>
	 <NAME="input" TYPE="TEXT"/>
	 <NAME="out1" TYPE="TEXT"/>
	 <NAME="output" TYPE="TEXT"/>
	 //<NAME="search_count" TYPE="NUMBER"/>
	 //<NAME="search_id" TYPE="NUMBER"/>
	 */
	/*
	 xml_parse_result stat;
	 stat=doc.load_file(fileName.c_str());
	 
	 if(!stat){cout<<fileName.c_str()<<" not loaded";
	 cout<<stat.description()<<" offset="<<stat.offset<<" line="<<stat.line<<endl;
	 ifstream file (fileName.c_str(), ios::in|ios::binary|ios::ate);
	 long offset=0; int lineIndex=0;
	 ifstream::pos_type size;
	 //long size_block = file.tellg();
	 
	 if (file.is_open())
	 {
	 int size = 256;
	 char *memblock = new char [size];
	 //file.seekg (0, ios::beg);
	 file.seekg (stat.offset, ios::beg);
	 file.read (memblock, size);
	 //file.read (memblock, size_block);
	 file.close();
	 cout<<"line offset="<<memblock<<endl;
	 delete memblock;     //cout<<"size_block="<<size_block<<endl;
	 }
	 exit(0);
	 }
	 
	 dictRecordXML = doc.child("dictBase");
	 // Iterate through books
	 cout<<" start parsing"<<endl;
	 //  cd ../../YagpoOCRProgram   yagpo_dict -dt tt t1
	 
	 wID=0;
	 for (xml_node row = dictRecordXML.child("rec"); row; row = row.next_sibling("rec")){
	 //wID=atoi(row.attribute("id").value());
	 dictRecord[i].wordID=wID;
	 dictRecord[i].tibWord=row.attribute("tw").value();
	 str=row.attribute("tk").value();
	 for(int m=1;m<delimeter.size();m++){
	 str=str_replace(delimeter[m].c_str()," ",str);
	 }
	 //cout<<"  d  ";
	 for(int m=0;m<commonWords.size();m++){
	 str=str_replace(commonWords[m].c_str()," ",str);
	 }
	 dictIndex[str].push_back(wID);
	 dictRecord[i].tibKey=str;
	 dictRecord[i].engKey=row.attribute("ek").value();
	 dictRecord[i].rusKey=row.attribute("rk").value();
	 dictRecord[i].sncKey=row.attribute("sk").value();
	 dictRecord[i].Wylie=row.attribute("w").value();
	 dictRecord[i].dict=row.attribute("dt").value();
	 dictRecord[i].dict=row.attribute("dt").value();
	 dictRecord[i].rus_translit=row.attribute("rt").value();
	 col = row.child("r1");
	 dictRecord[i].eentry=col.child_value();
	 col = row.child("r2");
	 dictRecord[i].rentry=col.child_value();
	 col = row.child("r3");
	 dictRecord[i].example=col.child_value();
	 i++; wID++;
	 }
	 
	 fileName+="1";
	 //doc.save_file(fileName.c_str(),"\t", 1);
	 //writeDictionary(dictRecord,fileName,c_out);
	 cout<<"Done"<<endl;
	 */
	
}//////////////////////////////////////////////////////////////////////////////////////////////////

string GLogicProcessor::sortKey(vector<dictKey>&keyArray,string &srcKey,int fullReport){
	string report="";
	string srcStr;
	int count;
	char cbuf[10];
	
	cout<<" keyArray.size()="<<keyArray.size()<<" fullReport="<<fullReport<<endl;
	count=0;
	if(fullReport){
		if(fullReport==1){
			for(int i=0;i<keyArray.size();i++){
				//cout<<"srcKey="<<srcKey<<endl;
				if(keyArray[i].ln=="tb")srcStr=keyArray[i].key;
				//if(srcKey==srcStr){
				if(keyArray[i].ln!="tb"){
					report+=keyArray[i].key;
					report+="|&|";
					report+=srcStr;
					report+="|&|";report+=keyArray[i].dict; report+="|&|";
					sprintf(cbuf,"%d",keyArray[i].id);
					report+=cbuf; report+=":|:";
					keyArray[i].flag=0;
				}
				//}
			}
		}
		if(fullReport==2){
			for(int i=0;i<keyArray.size();i++){
				//cout<<"srcKey="<<srcKey<<endl;
				if(keyArray[i].ln=="pl")srcStr=keyArray[i].key;
				//if(srcKey==srcStr){
				if(keyArray[i].ln!="tb"){
					report+=keyArray[i].key;
					report+="|&|";
					report+=srcStr;
					report+="|&|";report+=keyArray[i].dict; report+="|&|";
					sprintf(cbuf,"%d",keyArray[i].id);
					report+=cbuf; report+=":|:";
					keyArray[i].flag=0;
				}
				//}
			}
		}
		
	}else{
		
		for(int i=0;i<keyArray.size();i++){
			cout<<"srcKey="<<srcKey<<" keyArray[i].dict="<<keyArray[i].dict<<endl;
			if(keyArray[i].dict=="YGP"){
				if(keyArray[i].ln=="tb")srcStr=keyArray[i].key;
				//if(srcKey==srcStr){
				if(keyArray[i].ln!="tb"){
					report+=keyArray[i].key;
					report+="|&|";
					report+=srcStr;
					report+="|&|";report+=keyArray[i].dict; report+="|&|";
					sprintf(cbuf,"%d",keyArray[i].id);
					report+=cbuf; report+=":|:";
					keyArray[i].flag=0; count++; if(count>3)break;
				}
				//}
			}
		}
		
		
		for(int i=0;i<keyArray.size();i++){
			//cout<<"srcKey="<<srcKey<<endl;
			if(keyArray[i].ln=="tb")srcStr=keyArray[i].key;
			if(srcKey==srcStr){
				if(keyArray[i].ln!="tb"){
					report+=keyArray[i].key;
					report+="|&|";
					report+=srcStr;
					report+="|&|";report+=keyArray[i].dict; report+="|&|";
					sprintf(cbuf,"%d",keyArray[i].id);
					report+=cbuf; report+=":|:";
					keyArray[i].flag=0; count++; if(count>2)break;
				}
			}
		}
		
		count=0;
		for(int i=0;i<keyArray.size();i++){
			if(keyArray[i].dict=="MV"){
				if(keyArray[i].ln=="tb")srcStr=keyArray[i].key;
				if(keyArray[i].ln=="sk"){
					report+=keyArray[i].key;
					report+="|&|";
					report+="|&|";report+=keyArray[i].dict; report+="|&|";
					sprintf(cbuf,"%d",keyArray[i].id);
					report+=cbuf; report+=":|:";
					keyArray[i].flag=0; count++; if(count>2)break;
				}
			}
		}
		for(int i=0;i<keyArray.size();i++){
			if(keyArray[i].dict=="YO"){
				if(keyArray[i].ln=="tb")srcStr=keyArray[i].key;
				if(keyArray[i].ln=="sk"){
					report+=keyArray[i].key;
					report+="|&|";
					report+=srcStr;
					report+="|&|";report+=keyArray[i].dict; report+="|&|";
					sprintf(cbuf,"%d",keyArray[i].id);
					report+=cbuf; report+=":|:";
					keyArray[i].flag=0; count++; if(count>2)break;
				}
			}
		}
		count=0;
		for(int i=0;i<keyArray.size();i++){
			if(keyArray[i].ln=="tb")srcStr=keyArray[i].key;
			if(keyArray[i].ln=="sk"&&keyArray[i].flag){
				report+=keyArray[i].key;
				report+="|&|";
				report+=srcStr;
				report+="|&|";report+=keyArray[i].dict; report+="|&|";
				sprintf(cbuf,"%d",keyArray[i].id);
				report+=cbuf; report+=":|:";
				keyArray[i].flag=0; count++; if(count>2)break;
			}
		}
		count=0;
		for(int i=0;i<keyArray.size();i++){
			if(keyArray[i].ln=="tb")srcStr=keyArray[i].key;
			if(keyArray[i].ln=="pl"&&keyArray[i].flag){
				report+=keyArray[i].key;
				report+="|&|";
				report+=srcStr;
				report+="|&|";report+=keyArray[i].dict; report+="|&|";
				sprintf(cbuf,"%d",keyArray[i].id);
				report+=cbuf; report+=":|:";
				keyArray[i].flag=0;       count++; if(count>2)break;
			}
		}
		count=0;
		for(int i=0;i<keyArray.size();i++){
			if(keyArray[i].ln=="tb")srcStr=keyArray[i].key;
			if(keyArray[i].dict=="RHD"&&keyArray[i].flag){
				report+=keyArray[i].key;
				report+="|&|";
				report+=srcStr;
				report+="|&|";report+=keyArray[i].dict; report+="|&|";
				sprintf(cbuf,"%d",keyArray[i].id);
				report+=cbuf; report+=":|:";
				keyArray[i].flag=0;	break;
			}
		}
		count=0;
		for(int i=0;i<keyArray.size();i++){
			if(keyArray[i].ln=="tb")srcStr=keyArray[i].key;
			if(keyArray[i].dict=="TD"&&keyArray[i].flag){
				report+=keyArray[i].key;
				report+="|&|";
				report+=srcStr;
				report+="|&|";report+=keyArray[i].dict; report+="|&|";
				sprintf(cbuf,"%d",keyArray[i].id);
				report+=cbuf; report+=":|:";
				keyArray[i].flag=0;	break;
			}
		}
		count=0;
		for(int i=0;i<keyArray.size();i++){
			//cout<<"keyArray["<<i<<"].dict="<<keyArray[i].dict<<endl;
			if(keyArray[i].ln=="tb")srcStr=keyArray[i].key;
			if(keyArray[i].dict=="RYD"&&keyArray[i].flag&&keyArray[i].ln=="tb")srcStr=keyArray[i].srcKey;
			if(keyArray[i].dict=="RYD"&&keyArray[i].flag&&keyArray[i].ln=="ru"){
				report+=keyArray[i].key;
				report+="|&|";
				report+=srcStr;
				report+="|&|";report+=keyArray[i].dict; report+="|&|";
				sprintf(cbuf,"%d",keyArray[i].id);
				report+=cbuf; report+=":|:";
				keyArray[i].flag=0; count++; if(count>2)break;
			}
		}
	}
	//cout<<"report="<<report<<endl;
	
	
	return report;
}//_________________________________________________________________________________________________




void GLogicProcessor::writeDictReportXML(vector <vector <OCRDictRecord> >&dictReportPage,string &binPath){
	int i;
	xml_document doc;
	xml_node mainNode;
	xml_node book, data;
	int wordID=0;
	char cbuf[20];
	string str;
	/*
	 cout<<"dictRecord size="<<dictReportPage.size();
	 
	 mainNode=doc.append_child();
	 mainNode.set_name("dictBase");
	 
	 for(int m=0;m<dictReportPage.size();m++) {
	 
	 
	 i=0;
	 while(i<dictReportPage[m].size()){
	 book=mainNode.append_child();
	 //sprintf(cbuf,"%d",i);
	 book.set_name("rec");
	 book.append_attribute("id")=dictReportPage[m][i].wordID=wordID;
	 book.append_attribute("tw")=dictReportPage[m][i].tibWord.c_str();
	 book.append_attribute("tk")=dictReportPage[m][i].tibKey.c_str();
	 book.append_attribute("ek")=dictReportPage[m][i].engKey.c_str();
	 book.append_attribute("rk")=dictReportPage[m][i].rusKey.c_str();
	 book.append_attribute("sk")=dictReportPage[m][i].sncKey.c_str();
	 book.append_attribute("w")=dictReportPage[m][i].Wylie.c_str();
	 book.append_attribute("dt")=dictReportPage[m][i].dict.c_str();
	 book.append_attribute("rt")=dictReportPage[m][i].rus_translit.c_str();
	 data=book.append_child();
	 data.set_name("r1");
	 data.append_child(node_pcdata).set_value(dictReportPage[m][i].eentry.c_str());
	 data=book.append_child();
	 data.set_name("r2");
	 data.append_child(node_pcdata).set_value(dictReportPage[m][i].rentry.c_str());
	 data=book.append_child();
	 data.set_name("r3");
	 data.append_child(node_pcdata).set_value(dictReportPage[m][i].example.c_str());
	 
	 wordID++; i++;
	 
	 }
	 
	 }
	 
	 */
	doc.save_file(binPath.c_str(),"\t", 1);
	
}//_____________________________________________________________________________

void GLogicProcessor::readDictionaryFileMakerXML(string &fileName){
	xml_node col,data,resultSet,dictRecordXML, st;
	xml_document doc;
	string str;
	int dInt=0;
	
	cout<<fileName.c_str();
	
	/*
	 <FIELD EMPTYOK="YES" MAXREPEAT="1" NAME="tibKey" TYPE="TEXT"/>
	 <FIELD EMPTYOK="YES" MAXREPEAT="1" NAME="engKey" TYPE="TEXT"/>
	 <FIELD EMPTYOK="YES" MAXREPEAT="1" NAME="rusKey" TYPE="TEXT"/>
	 <FIELD EMPTYOK="YES" MAXREPEAT="1" NAME="sncKey" TYPE="TEXT"/>
	 <FIELD EMPTYOK="YES" MAXREPEAT="1" NAME="eentry" TYPE="TEXT"/>
	 <FIELD EMPTYOK="YES" MAXREPEAT="1" NAME="rentry" TYPE="TEXT"/>
	 <FIELD EMPTYOK="YES" MAXREPEAT="1" NAME="example" TYPE="TEXT"/>
	 <FIELD EMPTYOK="YES" MAXREPEAT="1" NAME="dict" TYPE="TEXT"/>
	 <FIELD EMPTYOK="YES" MAXREPEAT="1" NAME="rus_translit" TYPE="TEXT"/>
	 <FIELD EMPTYOK="YES" MAXREPEAT="1" NAME="Wylie" TYPE="TEXT"/>
	 <FIELD EMPTYOK="YES" MAXREPEAT="1" NAME="ID" TYPE="NUMBER"/>
	 <FIELD EMPTYOK="YES" MAXREPEAT="1" NAME="in1" TYPE="TEXT"/>
	 <FIELD EMPTYOK="YES" MAXREPEAT="1" NAME="input" TYPE="TEXT"/>
	 <FIELD EMPTYOK="YES" MAXREPEAT="1" NAME="out1" TYPE="TEXT"/>
	 <FIELD EMPTYOK="YES" MAXREPEAT="1" NAME="output" TYPE="TEXT"/>
	 <FIELD EMPTYOK="YES" MAXREPEAT="1" NAME="search_count" TYPE="NUMBER"/>
	 <FIELD EMPTYOK="YES" MAXREPEAT="1" NAME="search_id" TYPE="NUMBER"/>
	 */
	/*
	 int step=0; int count=0;
	 if(!doc.load_file(fileName.c_str())){cout<<fileName.c_str()<<" not loaded";exit(0);}
	 dictRecordXML = doc.child("FMPXMLRESULT");
	 // DM("xmlns " << dictRecordXML.attribute("xmlns").value()<<END);
	 //data = dictRecordXML.child("ERRORCODE");
	 //DM("ERRORCODE" << data.child_value()<<END);
	 resultSet = dictRecordXML.child("RESULTSET");
	 //DM("FOUND " <<resultSet.attribute("FOUND").value()<<END);
	 // Iterate through books
	 cout<<"start parsing"<<endl;
	 //  cd ../../YagpoOCRProgram   yagpo_dict -dt tt t1
	 
	 int im=0;
	 for (xml_node row = resultSet.child("ROW"); row; row = row.next_sibling("ROW")){
	 //DM("RECORDID " << row.attribute("RECORDID").value()<<END);
	 OCRDictRecord record;
	 
	 col = row.child("COL");
	 data=col.child("DATA");
	 str=data.child_value();
	 //c_out<<"str1="<<str<<" -> ";
	 for(int m=1;m<delimeter.size();m++){
	 str=str_replace(delimeter[m].c_str()," ",str);
	 }
	 //cout<<"  d  ";
	 for(int m=0;m<commonWords.size();m++){
	 str=str_replace(commonWords[m].c_str()," ",str);
	 }
	 ///c_out<<str<<endl; if(im>1000)exit(0);im++;
	 //dictIndex[str].push_back(dInt);
	 dictRecord[dInt]=record;
	 dictRecord[dInt].tibWord=data.child_value();
	 dictRecord[dInt].tibKey=str;
	 if(step>5000){cout<<".";step=0;}step++;  count++;
	 //cout<<record.engKey.c_str()<<endl; if(dInt>10)exit(0); dInt++;
	 
	 col = col.next_sibling("COL");//engKey
	 data=col.child("DATA");
	 //DM("engKey="<<data.child_value());
	 dictRecord[dInt].engKey=data.child_value();
	 col = col.next_sibling("COL");//rusKey
	 data=col.child("DATA");
	 //DM("rusKey="<<data.child_value());
	 dictRecord[dInt].rusKey=data.child_value();
	 col = col.next_sibling("COL");//sncKey
	 data=col.child("DATA");
	 //DM("sncKey="<<data.child_value());
	 dictRecord[dInt].sncKey=data.child_value();
	 col = col.next_sibling("COL");//eentry
	 data=col.child("DATA");
	 //DM("eentry="<<data.child_value());
	 dictRecord[dInt].eentry=data.child_value();
	 col = col.next_sibling("COL");//rentry
	 data=col.child("DATA");
	 //DM("rentry="<<data.child_value());
	 dictRecord[dInt].rentry=data.child_value();
	 col = col.next_sibling("COL");//example
	 data=col.child("DATA");
	 //DM("example="<<data.child_value());
	 dictRecord[dInt].example=data.child_value();
	 col = col.next_sibling("COL");//dict
	 data=col.child("DATA");
	 //DM("dict="<<data.child_value());
	 dictRecord[dInt].dict=data.child_value();
	 col = col.next_sibling("COL");//rus_translit
	 data=col.child("DATA");
	 //DM("rus_translit="<<data.child_value());
	 dictRecord[dInt].rus_translit=data.child_value();
	 col = col.next_sibling("COL");//Wylie
	 data=col.child("DATA");
	 //DM("Wylie="<<data.child_value());
	 dictRecord[dInt].Wylie=data.child_value();
	 col = col.next_sibling("COL");//ID
	 col = col.next_sibling("COL");//in1
	 dictRecord[dInt].recordFlag=1;   //validate this record
	 //col.remove_child("DATA");
	 data=col.child("DATA");
	 //str=":|:"+ str;
	 data.append_child(node_pcdata).set_value(str.c_str());
	 dInt++;
	 }
	 
	 //if(!dictIndex.size()){cout<<fileName.c_str()<<" not loaded";exit(0);};
	 cout<<"dict->size()="<<dictRecord.size()<<endl;
	 fileName+="1";
	 //doc.save_file(fileName.c_str(),"\t", 1);
	 writeDictionary(dictRecord,fileName);
	 cout<<"Done";
	 */
}//////////////////////////////////////////////////////////////////////////////////////////////////


void GLogicProcessor::readGrammarDataXML(string &path){
	
	xml_node col,data,resultSet,letterBaseXML, st;
	xml_document doc;
	string str0,str1;
	unsigned int dInt,in;
	//data=doc.append_child();
	//data.set_name("rec");
	//data.append_attribute("id")=">|<|\"|\'";
	//doc.save_file("/1.xml","\t", 1);
	
	if(!doc.load_file(path.c_str())){
		cout<<path<<" not loaded"<<END;exit(9);
	}
	
	resultSet = doc.child("letterBase");
	// Iterate through books
	int index=0;
	
	for (xml_node row = resultSet.child("rec"); row; row = row.next_sibling("rec")){			
		str0=row.attribute("k").value();	
		str1=row.attribute("v").value();
		if(str1=="gT")grammarTib.push_back(str0);  //grammar partical
		if(str1=="dT")delimeterTib.push_back(str0);  //tibetan syllable delimeter
		if(str1=="iT")commentaryIndex.push_back(str0);     //commentary index in text
		if(str1=="gP")grammarPali.push_back(str0);
		if(str1=="dP")delimeterPali.push_back(str0);
	}
	
	//cout<<"delimeterTib.size="<<delimeterTib.size()<<endl;
	//cout<<"grammarTib.size="<<grammarTib.size()<<endl;
	//cout<<"delimeterPali.size="<<delimeterPali.size()<<endl;
	//cout<<"grammarPali.size="<<grammarPali.size()<<endl;
	
}//_____________________________________________________________________________
