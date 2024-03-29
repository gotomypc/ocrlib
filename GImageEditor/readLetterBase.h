﻿void GImageEditor::readFontDataXML(string &path){
	
	xml_node col,data,resultSet,letterBaseXML, st;
	xml_document doc;
	string str0,str1;
	unsigned int dInt,in;

	if(!doc.load_file(path.c_str())){
		cout<<path<<" not loaded"<<END;exit(9);
	}
	
	resultSet = doc.child("letterBase");
	// Iterate through books
	int index=0;
	
	for (xml_node row = resultSet.child("rec"); row; row = row.next_sibling("rec")){			
		str0=row.attribute("k").value();	
		str1=row.attribute("v").value();
		if(str1=="lM")letterSign.push_back(str0);  //vowels
		if(str1=="sL")letterScale.push_back(str0);  //letters for scale detector
	}
	
	//cout<<"delimeterTib.size="<<delimeterTib.size()<<endl;
	//cout<<"grammarTib.size="<<grammarTib.size()<<endl;
	//cout<<"delimeterPali.size="<<delimeterPali.size()<<endl;
	//cout<<"grammarPali.size="<<grammarPali.size()<<endl;
	
}//_____________________________________________________________________________



void GImageEditor::readLetterBase(commandData *inputData){
	///////////////////////////////////*read letter data
//string fileName=binPath;
//fileName+="/OCRCodeTable/UniToTibetanBig.xml";
xml_node col,data,resultSet,letterBaseXML, st;
xml_document doc;
string str;
unsigned in;
int print=0;
	readFontDataXML(inputData->data["wordsDataPath"]);
    DT("//____ load UniTibTable"<<endl);
    

//(!uniTibTable.size()){ShowMessage((AnsiString)fileName.c_str()+(AnsiString)" not loaded");return;};
//cout<<"uniTibTable.size()="<<uniTibTable.size()<<END;

//fileName=binPath;
//fileName+="/OCRCodeTable/OCRLetterBase.xml";
//ShowMessage(fileName.c_str());
DT("//_  load aliKali"<<endl);
string vData;

vector<string>dataArray;
vector<string>vArray;

		if(!doc.load_file(inputData->data["OCRBasePath"].c_str())){
			cout<<inputData->data["OCRBasePath"].c_str()<<" not loaded"<<END;
			return;
		}
		resultSet = doc.child("letterBase");
		// Iterate through books
		int index=0; aliKali.textLineSize=0;
		//aliKali.base.resize(500);
		aliKali.fontName=resultSet.child("fontName").attribute("n").value();
		cout<<"aliKali.fontName="<<aliKali.fontName<<endl;


		for (xml_node row = resultSet.child("rec"); row; row = row.next_sibling("rec")){  DT(".");
				  aliKali.letterCount++;
				  glyphOCR *letter=aliKali[aliKali.letterCount-1];
				  //cout<<"aliKali.base.size()="<<aliKali.base.size();
				  //aliKali.base.push_back(letter_);
				  //glyphOCR *letter=aliKali[aliKali.base.size()-1];
			      
				  letter->idNumber=strtoul(row.attribute("id").value(),NULL,0);
			      //выполняем проверку уникальности ID 
				  int IDflag=1;
			      while(IDflag){
				      for(int i=0;i<aliKali.letterCount-1;i++){
						  if(letter->idNumber==aliKali[i]->idNumber){letter->idNumber++;IDflag=2;}
					  }
				      IDflag--;
				  }			
                  letter->letterIndex=index;index++;
				  letter->vectorCount=dataArray.size();
				  letter->name=row.attribute("n").value();   DT("letter->name="<<letter->name<<endl);
				  letter->Wylie=row.attribute("wt").value();
				  letter->letterH=atoi(row.attribute("h").value());
				  letter->letterW=atoi(row.attribute("w").value());
			      letter->dX=atoi(row.attribute("dX").value());
				  letter->dY=atoi(row.attribute("dY").value());
				  letter->lengthSum=atoi(row.attribute("s").value());
				  letter->OCRKey=atoi(row.attribute("k").value());
				  letter->stackFlag=atoi(row.attribute("f").value());
				  //letter->OCRIndex=atoi(row.attribute("in").value());
				  letter->baseLetter=atoi(row.attribute("l").value());
				  letter->Character=atoi(row.attribute("c").value());
				  letter->newLetterInBook=atoi(row.attribute("nb").value());
				  letter->y0=atoi(row.attribute("y0").value());
			      letter->y1=atoi(row.attribute("y1").value());
				  letter->cn=atoi(row.attribute("cn").value());
				  letter->cnRes=atoi(row.attribute("cnr").value());
				  letter->crRes=atoi(row.attribute("cr").value());
			      letter->selfCorrelation=atoi(row.attribute("sC").value());
			      letter->codeSpace=atoi(row.attribute("cS").value());
				  letter->recordFlag=1;
				  letter->newLetter=0;
				  col=row.child("r1");
				  vData=col.child_value();
				  dataArray=explode("\n",vData);
				  letter->vectorCount=dataArray.size();
				  aliKali.textLineSize+=(letter->y1-letter->y0);
			     
			      for(int i=0;i<letterSign.size();i++){
					  if(letterSign[i]==letter->name)letter->OCRIndex=3;
				  }	  
				  for(int i=0;i<letterScale.size();i++){
				 	  if(letterScale[i]==letter->name)letter->OCRIndex=5;
				  }	  
				  //for(int sIn=0;sIn<dataArray.size();sIn++){
					   //DM("sIn="<<sIn<<" "<<dataArray[sIn].c_str()<<END);
					   //vArray=explode(";",dataArray[sIn]);
					   //vectorOCR vector;
					   //SH(vArray.size());
					   //letter->v[sIn].vStart.x=atoi(vArray[0].c_str());
					   //letter->v[sIn].vStart.y=atoi(vArray[1].c_str());
					   //letter->v[sIn].vEnd.x=atoi(vArray[2].c_str());
					   //letter->v[sIn].vEnd.y=atoi(vArray[3].c_str());
					   //letter->v[sIn].vCenter.x=atoi(vArray[4].c_str());
					   //letter->v[sIn].vCenter.y=atoi(vArray[5].c_str());
					   //letter->v[sIn].length=atoi(vArray[6].c_str());
					   //letter->v[sIn].vectorAngle=atoi(vArray[7].c_str());
				  //}
				  letter->mask32Count=0;
				  letter->xSum=0;
				  letter->ySum=0;
				  for (col = row.child("mask32"); col; col = col.next_sibling("mask32")){
					  letter->mask32[letter->mask32Count].xMask=atoi(col.attribute("x").value());
					  letter->mask32[letter->mask32Count].yMask=atoi(col.attribute("y").value());
					  if(letter->mask32[letter->mask32Count].xMask>80||letter->mask32[letter->mask32Count].xMask<-80)continue;
					  if(letter->mask32[letter->mask32Count].yMask>80||letter->mask32[letter->mask32Count].yMask<-80)continue;
					  letter->mask32[letter->mask32Count].mW=32;
					  letter->mask32[letter->mask32Count].imgW=atoi(col.attribute("w").value()); //SH(letter->mask32[letter->mask32Count].w);
					  letter->mask32[letter->mask32Count].mH=atoi(col.attribute("h").value());
					  letter->mask32[letter->mask32Count].mWOn=atoi(col.attribute("wR").value());
					  letter->mask32[letter->mask32Count].mHOn=atoi(col.attribute("hR").value());
					  letter->mask32[letter->mask32Count].NMask=atoi(col.attribute("s").value());
					  letter->mask32[letter->mask32Count].maskType=atoi(col.attribute("t").value());
					  letter->mask32[letter->mask32Count].dlt0=atoi(col.attribute("d0").value());
					  letter->mask32[letter->mask32Count].dlt1=atoi(col.attribute("d1").value());
					  letter->mask32[letter->mask32Count].dlt2=atoi(col.attribute("d2").value());
					  letter->mask32[letter->mask32Count].status=atoi(col.attribute("st").value());
					  letter->xSum+=letter->mask32[letter->mask32Count].xMask;
					  //cout<<"letter->mask32["<<letter->mask32Count<<"].xMask="<<letter->mask32[letter->mask32Count].xMask<<END;
					  letter->ySum+=letter->mask32[letter->mask32Count].yMask;

					  data=col.child("imgOn");
					  vData=data.child_value();
					  dataArray=explode(";",vData);
					  for(int m=0;m<dataArray.size();m++){
					   sscanf(dataArray[m].c_str(),"%X",&in); //binaryPrint(in,2,c_out); DP(END);
					   letter->mask32[letter->mask32Count].imgOn[m]=in;
					  }
						  data=col.child("On");
						  vData=data.child_value();
						  dataArray=explode(";",vData);
						  for(int m=0;m<dataArray.size();m++){
						   sscanf(dataArray[m].c_str(),"%X",&in); //binaryPrint(in,2,c_out); DP(END);
						   letter->mask32[letter->mask32Count].On[m]=in;
						  }
					  data=col.child("Off");
					  vData=data.child_value();
					  dataArray=explode(";",vData);
					  for(int m=0;m<dataArray.size();m++){
					   sscanf(dataArray[m].c_str(),"%X",&in); //binaryPrint(in,2,c_out); DP(END);
					   letter->mask32[letter->mask32Count].Off[m]=in;
					  }
					  //letter->mask32[letter->mask32Count].printMask(c_out);
					  //if(letter->mask32[letter->mask32Count].mWReal<1||
					  //	 letter->mask32[letter->mask32Count].mWReal>32||
					  //	 letter->mask32[letter->mask32Count].mHReal<1||
					  //	 letter->mask32[letter->mask32Count].mHReal>64){
					  //letter->mask32[letter->mask32Count].NormalT(inputData->c_out);
					  //}
					  //if(aliKali.base.size()==5)letter->mask32[letter->mask32Count].printMask(inputData->c_out);
					  letter->mask32[letter->mask32Count].id=letter->mask32Count;
					  letter->mask32Count++;
				  }
				  if(letter->mask32Count>0){
					letter->xSum/=letter->mask32Count;
					letter->ySum/=letter->mask32Count;
				  }



				  //DT("letter->mask32Count="<<letter->mask32Count<<" letter->xSum"<<letter->xSum<<endl);
				  letter->mask128Count=0;
				  for (col = row.child("mask128"); col; col = col.next_sibling("mask128")){
					  letter->mask128[letter->mask128Count].xMask=atoi(col.attribute("x").value());
					  letter->mask128[letter->mask128Count].yMask=atoi(col.attribute("y").value());
					  letter->mask128[letter->mask128Count].mW=atoi(col.attribute("w").value()); //SH(letter->mask32[letter->mask32Count].w);
					  letter->mask128[letter->mask128Count].mH=atoi(col.attribute("h").value());
					  letter->mask128[letter->mask128Count].NMask=atoi(col.attribute("s").value());
					  letter->mask128[letter->mask128Count].status=atoi(col.attribute("st").value());

					  data=col.child("On");
					  vData=data.child_value();
					  dataArray=explode(";",vData);
					  if(letter->mask128[letter->mask128Count].status!=5){
						   for(int m=0;m<dataArray.size();m+=2){
							   sscanf(dataArray[m+1].c_str(),"%X",&in);
							   letter->mask128[letter->mask128Count].On[m]=in;
								  //binaryPrint(in,2);
							   sscanf(dataArray[m].c_str(),"%X",&in);
							   letter->mask128[letter->mask128Count].On[m+1]=in;
								  //binaryPrint(in,2);
								  //cout<<END;

						   }
					  }else{
						   for(int m=0;m<dataArray.size();m++){
							   sscanf(dataArray[m].c_str(),"%X",&in);
							   letter->mask128[letter->mask128Count].On[m]=in;
								  //binaryPrint(in,2);
						   }
					  }
					  letter->mask128Count++;
				  }
			if(letter->mask128Count){
				if(letter->idNumber<10){
			       letter->idNumber=letter->mask128[0].hashValue();
				}	
			}else{
			    cout<<"no mask"<<endl;
			}	

			
				  if(uniTibTable.find(letter->name)!=uniTibTable.end()){
					letter->baseLetter=uniTibTable[letter->name].baseLetter; 
				  }


		}
		if (aliKali.letterCount)aliKali.textLineSize/=aliKali.letterCount;
		cout<<"aliKali.textLineSize"<<aliKali.textLineSize<<END;


		//exit(0);
}//////////////////////////////////////////////////////////////////////////////////////////////////

void GImageEditor::readLetterStore(commandData *inputData){
	///////////////////////////////////*read letter data
	//____ load UniTibTable
	string str=inputData->data["tablePath"]+"/OCRLetterStore";
	readDirectoryToArray(inputData->fileList,str,"xml");
	//string fileName=binPath;
	//fileName+="/OCRCodeTable/UniToTibetanBig.xml";
	xml_node col,data,resultSet,letterBaseXML, st;
	xml_document doc;
	unsigned in;
	int print=1;

	cout<<" readLetterStore"<<endl;
	
	///_  load aliKali
	string vData;
	
	for(int baseIndex=0;baseIndex<inputData->fileList.size();baseIndex++){
		      
		    letterBase newBase; aliKaliStore.push_back(newBase);
		
			vector<string>dataArray;
			vector<string>vArray;
		   str=inputData->fileList[baseIndex];
		
		   if(!doc.load_file(str.c_str())){
				cout<<str<<" not loaded"<<END;
				return;
			}
			resultSet = doc.child("letterBase");
			// Iterate through books
			int index=0;
			
			for (xml_node row = resultSet.child("rec"); row; row = row.next_sibling("rec")){
				glyphOCR *letter=aliKali[aliKali.letterCount]; aliKali.letterCount++;
				//glyphOCR letter_; aliKaliStore[baseIndex]+=letter_;
				//glyphOCR *letter=aliKaliStore[baseIndex][index];
				
				letter->idNumber=strtoul(row.attribute("id").value(),NULL,0);
				letter->letterIndex=index;index++;
				letter->vectorCount=dataArray.size();
				letter->name=row.attribute("n").value();   //cout<<"letter->name="<<letter->name<<END;
				letter->Wylie=row.attribute("wt").value();
				letter->letterH=atoi(row.attribute("h").value());
				letter->letterW=atoi(row.attribute("w").value());
				letter->dX=atoi(row.attribute("dX").value());
				letter->dY=atoi(row.attribute("dY").value());
				letter->lengthSum=atoi(row.attribute("s").value());
				letter->OCRKey=atoi(row.attribute("k").value());
				letter->stackFlag=atoi(row.attribute("f").value());
				letter->OCRIndex=atoi(row.attribute("in").value());
				letter->baseLetter=atoi(row.attribute("l").value());
				letter->Character=atoi(row.attribute("c").value());
				letter->newLetterInBook=atoi(row.attribute("nb").value());
				letter->y0=atoi(row.attribute("y0").value());
				letter->y1=atoi(row.attribute("y1").value());
				letter->cn=atoi(row.attribute("cn").value());
				letter->cnRes=atoi(row.attribute("cnr").value());
				letter->crRes=atoi(row.attribute("cr").value());
				letter->recordFlag=1;
				letter->newLetter=0;
				col=row.child("r1");
				vData=col.child_value();
				dataArray=explode("\n",vData);
	         	letter->mask32Count=0;
				letter->xSum=0;
				letter->ySum=0;
				for (col = row.child("mask32"); col; col = col.next_sibling("mask32")){
					letter->mask32[letter->mask32Count].xMask=atoi(col.attribute("x").value());
					letter->mask32[letter->mask32Count].yMask=atoi(col.attribute("y").value());
					if(letter->mask32[letter->mask32Count].xMask>80||letter->mask32[letter->mask32Count].xMask<-80)continue;
					if(letter->mask32[letter->mask32Count].yMask>80||letter->mask32[letter->mask32Count].yMask<-80)continue;
					letter->mask32[letter->mask32Count].mW=32;
					letter->mask32[letter->mask32Count].imgW=atoi(col.attribute("w").value()); //SH(letter->mask32[letter->mask32Count].w);
					letter->mask32[letter->mask32Count].mH=atoi(col.attribute("h").value());
					letter->mask32[letter->mask32Count].mWOn=atoi(col.attribute("wR").value());
					letter->mask32[letter->mask32Count].mHOn=atoi(col.attribute("hR").value());
					letter->mask32[letter->mask32Count].NMask=atoi(col.attribute("s").value());
					letter->mask32[letter->mask32Count].maskType=atoi(col.attribute("t").value());
					letter->mask32[letter->mask32Count].dlt0=atoi(col.attribute("d0").value());
					letter->mask32[letter->mask32Count].dlt1=atoi(col.attribute("d1").value());
					letter->mask32[letter->mask32Count].dlt2=atoi(col.attribute("d2").value());
					letter->mask32[letter->mask32Count].status=atoi(col.attribute("st").value());
					letter->xSum+=letter->mask32[letter->mask32Count].xMask;
					//cout<<"letter->mask32["<<letter->mask32Count<<"].xMask="<<letter->mask32[letter->mask32Count].xMask<<END;
					letter->ySum+=letter->mask32[letter->mask32Count].yMask;
					
					data=col.child("imgOn");
					vData=data.child_value();
					dataArray=explode(";",vData);
					for(int m=0;m<dataArray.size();m++){
						sscanf(dataArray[m].c_str(),"%X",&in); //binaryPrint(in,2,c_out); DP(END);
						letter->mask32[letter->mask32Count].imgOn[m]=in;
					}
					data=col.child("On");
					vData=data.child_value();
					dataArray=explode(";",vData);
					for(int m=0;m<dataArray.size();m++){
						sscanf(dataArray[m].c_str(),"%X",&in); //binaryPrint(in,2,c_out); DP(END);
						letter->mask32[letter->mask32Count].On[m]=in;
					}
					data=col.child("Off");
					vData=data.child_value();
					dataArray=explode(";",vData);
					for(int m=0;m<dataArray.size();m++){
						sscanf(dataArray[m].c_str(),"%X",&in); //binaryPrint(in,2,c_out); DP(END);
						letter->mask32[letter->mask32Count].Off[m]=in;
					}
					//letter->mask32[letter->mask32Count].printMask(c_out);
					//if(letter->mask32[letter->mask32Count].mWReal<1||
					//	 letter->mask32[letter->mask32Count].mWReal>32||
					//	 letter->mask32[letter->mask32Count].mHReal<1||
					//	 letter->mask32[letter->mask32Count].mHReal>64){
					//letter->mask32[letter->mask32Count].NormalT(inputData->c_out);
					//}
					//if(aliKali.base.size()==5)letter->mask32[letter->mask32Count].printMask(inputData->c_out);
					letter->mask32[letter->mask32Count].id=letter->mask32Count;
					letter->mask32Count++;
				}
				if(letter->mask32Count>0){
					letter->xSum/=letter->mask32Count;
					letter->ySum/=letter->mask32Count;
				}
				
				//cout<<"letter->mask32Count="<<letter->mask32Count<<" letter->xSum"<<letter->xSum<<END;
				letter->mask128Count=0;
				for (col = row.child("mask128"); col; col = col.next_sibling("mask128")){
					letter->mask128[letter->mask128Count].xMask=atoi(col.attribute("x").value());
					letter->mask128[letter->mask128Count].yMask=atoi(col.attribute("y").value());
					letter->mask128[letter->mask128Count].mW=atoi(col.attribute("w").value()); //SH(letter->mask32[letter->mask32Count].w);
					letter->mask128[letter->mask128Count].mH=atoi(col.attribute("h").value());
					letter->mask128[letter->mask128Count].NMask=atoi(col.attribute("s").value());
					letter->mask128[letter->mask128Count].status=atoi(col.attribute("st").value());
					
					data=col.child("On");
					vData=data.child_value();
					dataArray=explode(";",vData);
					if(letter->mask128[letter->mask128Count].status!=5){
						for(int m=0;m<dataArray.size();m+=2){
							sscanf(dataArray[m+1].c_str(),"%X",&in);
							letter->mask128[letter->mask128Count].On[m]=in;
							//binaryPrint(in,2);
							sscanf(dataArray[m].c_str(),"%X",&in);
							letter->mask128[letter->mask128Count].On[m+1]=in;
							//binaryPrint(in,2);
							//cout<<END;
						}
					}else{
						for(int m=0;m<dataArray.size();m++){
							sscanf(dataArray[m].c_str(),"%X",&in);
							letter->mask128[letter->mask128Count].On[m]=in;
							//binaryPrint(in,2);
						}
					}
					letter->mask128Count++;
				}
				if(letter->mask128Count){
					if(letter->idNumber<10){
						letter->idNumber=letter->mask128[0].hashValue();
					}	
				}else{
					cout<<"no mask"<<endl;
				}	
				
				if(uniTibTable.find(letter->name)!=uniTibTable.end()){
					letter->baseLetter=uniTibTable[letter->name].baseLetter;
					letter->baseLetter=uniTibTable[letter->name].baseLetter;
					letter->baseLetter=uniTibTable[letter->name].baseLetter;
				}
			} cout<<"newBase.base.size()"<<aliKaliStore[aliKaliStore.size()-1].letterCount<<END;
	}//for(int indexBase=0;indexBase<inputData->fileList.size();
	

}//////////////////////////////////////////////////////////////////////////////////////////////////


void GImageEditor::writeLetterBase(commandData *inputData){
int i,ind=0;
string record;
xml_document doc,docRecord;
xml_node mainNode;
xml_node book, data, key;
mainNode=doc.append_child();
mainNode.set_name("letterBase");
char cbuf[64];   //SH(aliKali.base.size());
unsigned long long in64;
unsigned int it;
	string str;	
	int print=0;
	DT("aliKali.base.size()="<<aliKali.letterCount<<endl);

	//first we read letter stat file
	record=readText(inputData->data["statPath"]);
	if(record.size()){
			char *buff=(char*)malloc(record.size()+1);
			memcpy(buff,record.c_str(),record.size());
			buff[record.size()]=0;
			record="";
			//writeText(record,inputData->data["statPath"]);
			
		xml_parse_result stat;
		stat=docRecord.parse(buff);
		
		if(!stat){cout<<"letterBase not loaded";
			cout<<stat.description()<<" offset="<<stat.offset<<" line="<<stat.line<<endl;
			ifstream file (inputData->data["statPath"].c_str(), ios::in|ios::binary|ios::ate);
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
				//str=str_replace("<","{", str);
				//str=str_replace(">","}", str);
				//file.read (memblock, size_block);
				file.close();
				cout<<"line offset="<<str<<endl;
				//c_out<<"line offset="<<memblock<<endl;
				delete memblock;     //cout<<"size_block="<<size_block<<endl;
			}
			exit(0);
		}
		
		
			//if(!docRecord.parse(buff)){
			//	cout<<"letterBase not loaded"<<END;
			//}else{
				int crRes, index;
				for (xml_node row = docRecord.child("rec"); row; row = row.next_sibling("rec")){ 
					it=strtoul(row.attribute("id").value(),NULL,0);
					index=aliKali.indexById(it);
					aliKali[index]->cn+=atoi(row.attribute("cn").value()); 
					crRes=atoi(row.attribute("cr").value());			
					//cout<<"aliKali["<<index<<"]->cn="<<aliKali[index]->cn<<" crRes="<<crRes<<"aliKali["<<index<<"]->crRes="<<aliKali[index]->crRes<<endl;
					
					   if(aliKali[index]->crRes){
						   aliKali[index]->crRes=(aliKali[index]->crRes+crRes)/2;
					   }else{
						   aliKali[index]->crRes=crRes;
					   }
					
					if(aliKali[index]->cn>1000){aliKali[index]->cn-=1000; aliKali[index]->cnRes++;}
				}
			
			}
	//}

	book=mainNode.append_child();
	book.set_name("fontName");
	book.append_attribute("n")=aliKali.fontName.c_str();

		
	for(i=0;i<aliKali.letterCount;i++){ //cout<<"aliKali.base[i].Wylie.c_str()="<<aliKali.base[i].Wylie.c_str()<<endl;
						 if(aliKali[i]->recordFlag==0){continue;}

						 book=mainNode.append_child();
						 //sprintf(cbuf,"%d",i);
						 DT("i="<<i<<" name="<<aliKali[i]->name.c_str()<<"aliKali[i].stackFlag="<<aliKali[i]->stackFlag<<END);

						 book.set_name("rec");
						//выполняем проверку уникальности ID
						int IDflag=1;
						while(IDflag){
							for(int t=0;t<aliKali.letterCount;t++){
								if(t==i)continue;
								if(aliKali[i]->idNumber==aliKali[t]->idNumber){aliKali[i]->idNumber++;IDflag=2;}
							}
							IDflag--;
						}
		
						 book.append_attribute("id")=aliKali[i]->idNumber;
						 book.append_attribute("n")=aliKali[i]->name.c_str();
						 book.append_attribute("wt")=aliKali[i]->Wylie.c_str();
						 book.append_attribute("f")=aliKali[i]->stackFlag;
						 book.append_attribute("h")=aliKali[i]->letterH;
						 book.append_attribute("w")=aliKali[i]->letterW;
					     book.append_attribute("dX")=aliKali[i]->dX;
					     book.append_attribute("dY")=aliKali[i]->dY;
						 book.append_attribute("s")=(unsigned int)aliKali[i]->lengthSum;
						 book.append_attribute("k")=aliKali[i]->OCRKey.c_str();
						 book.append_attribute("in")=aliKali[i]->OCRIndex;
						 book.append_attribute("l")=aliKali[i]->baseLetter;
						 book.append_attribute("c")=aliKali[i]->Character;
						 book.append_attribute("nb")=aliKali[i]->newLetterInBook;
						 book.append_attribute("sC")=aliKali[i]->selfCorrelation;
						 book.append_attribute("cS")=aliKali[i]->codeSpace;
						 book.append_attribute("y0")=aliKali[i]->y0;
						 book.append_attribute("y1")=aliKali[i]->y1;
		                 if(aliKali[i]->crSum&&aliKali[i]->cn){   //вычисляем статистику использования буквы;
							 aliKali[i]->crSum=aliKali[i]->crSum/aliKali[i]->cn;
							 if(aliKali[i]->crRes){
								 aliKali[i]->crRes=(aliKali[i]->crSum+aliKali[i]->crRes)/2;	 
							 }else{
								 aliKali[i]->crRes=aliKali[i]->crSum;
							 }	 
							 aliKali[i]->crSum=aliKali[i]->crSum=0;	 
						 }	 
						 book.append_attribute("cn")=aliKali[i]->cn;
						 book.append_attribute("cnr")=aliKali[i]->cnRes;
 		                 book.append_attribute("cr")=aliKali[i]->crRes;
		

								for(int in=0; in<aliKali[i]->mask32Count;in++){   
								   //if(aliKali[i].mask32[in].xMask>50||aliKali[i].mask32[in].xMask<-50)continue;
                                   //if(aliKali[i].mask32[in].yMask>50||aliKali[i].mask32[in].yMask<-50)continue;
								   data=book.append_child();
								   data.set_name("mask32");
								   data.append_attribute("x")=aliKali[i]->mask32[in].xMask;
								   data.append_attribute("y")=aliKali[i]->mask32[in].yMask;
								   data.append_attribute("w")=aliKali[i]->mask32[in].imgW;
								   data.append_attribute("wR")=aliKali[i]->mask32[in].mWOn;
								   data.append_attribute("h")=aliKali[i]->mask32[in].mH;
								   data.append_attribute("hR")=aliKali[i]->mask32[in].mHOn;
								   data.append_attribute("s")=aliKali[i]->mask32[in].NMask;
								   data.append_attribute("t")=aliKali[i]->mask32[in].maskType;
								   data.append_attribute("d0")=aliKali[i]->mask32[in].dlt0;
								   data.append_attribute("d1")=aliKali[i]->mask32[in].dlt1;
								   data.append_attribute("d2")=aliKali[i]->mask32[in].dlt2;
								   data.append_attribute("st")=aliKali[i]->mask32[in].status;

								   key=data.append_child();
								   key.set_name("imgOn");
								   record="";
								   for(int m=0;m<aliKali[i]->mask32[in].mH;m++) {
									 sprintf(cbuf,"%X",aliKali[i]->mask32[in].imgOn[m]); record+=cbuf;record+=";";
								   }key.append_child(node_pcdata).set_value(record.c_str());

									   key=data.append_child();
									   key.set_name("On");
									   record="";
									   for(int m=0;m<aliKali[i]->mask32[in].mH;m++) {
										 sprintf(cbuf,"%X",aliKali[i]->mask32[in].On[m]); record+=cbuf;record+=";";
									   }key.append_child(node_pcdata).set_value(record.c_str());
									   record="";

								   key=data.append_child();
								   key.set_name("Off");
								   for(int m=0;m<aliKali[i]->mask32[in].mH;m++) {
									 sprintf(cbuf,"%X",aliKali[i]->mask32[in].Off[m]); record+=cbuf;record+=";";
								   }key.append_child(node_pcdata).set_value(record.c_str());
								}

								   data=book.append_child();
								   data.set_name("mask128");
								   data.append_attribute("x")=aliKali[i]->mask128[0].xMask;
								   data.append_attribute("y")=aliKali[i]->mask128[0].yMask;
								   data.append_attribute("w")=aliKali[i]->mask128[0].mW;
								   data.append_attribute("h")=aliKali[i]->mask128[0].mH;
								   data.append_attribute("s")=aliKali[i]->mask128[0].NMask;
								   data.append_attribute("st")=5;
								   key=data.append_child();
								   key.set_name("On");
								   record="";
								   int t=aliKali[i]->mask128[0].mH*aliKali[i]->mask128[0].mW/32;
								   for(int m=0;m<t;m++) {
									 it=aliKali[i]->mask128[0].On[m]; 
									 sprintf(cbuf,"%X",it); record+=cbuf;record+=";";
								   } key.append_child(node_pcdata).set_value(record.c_str());
								   record="";
								   //key=data.append_child();
								   //key.set_name("Off");
								   //for(int m=0;m<aliKali[i].mask32[in].mH;m++) {
								   //	 sprintf(cbuf,"%X",aliKali[i].mask32[in].Off[m]); record+=cbuf;record+=";";
								   //}
								   //key.append_child(node_pcdata).set_value(record.c_str());



					}


 bool exitCode=doc.save_file(inputData->data["OCRBasePath"].c_str(),"\t", 1);
 if(exitCode){
	cout<<"Done. save file "<<inputData->data["OCRBasePath"].c_str()<<END;
 }else{
	cout<<"Not saved "<<inputData->data["OCRBasePath"].c_str();}

}//////////////////////////////////////////////////////////////////////////


void GImageEditor::writeLetterStat(commandData *inputData){
	ostringstream out;
	cout<<"Start write stat file"<<endl;
    
	for(int i=0;i<aliKali.letterCount;i++){
		//cout<<"0_aliKali["<<i<<"]->cn="<<aliKali[i]->cn<<" aliKali["<<i<<"]->crSum="<<aliKali[i]->crSum<<endl;
		if(aliKali[i]->cn&&aliKali[i]->crSum){   //вычисляем статистику использования буквы;
			if(aliKali[i]->crSum)aliKali[i]->crSum=aliKali[i]->crSum/aliKali[i]->cn;
			if(aliKali[i]->crRes){
				aliKali[i]->crRes=(aliKali[i]->crSum+aliKali[i]->crRes)/2;	 
			}else{
				aliKali[i]->crRes=aliKali[i]->crSum;
			}	 
			aliKali[i]->crSum=0;
        		
		out<<"<rec id=\""<<aliKali[i]->idNumber<<"\" n=\""<<aliKali[i]->name<<"\" cn=\"";
		out<<aliKali[i]->cn<<"\" cnr=\""<<aliKali[i]->cnRes<<"\" cr=\""<<aliKali[i]->crRes<<"\" />\n";		
		//cout<<"1_aliKali["<<i<<"]->cn="<<aliKali[i]->cn<<" aliKali["<<i<<"]->crRes="<<aliKali[i]->crRes<<endl;	
			
		}//else{cout<<"aliKali["<<i<<"]->crSum no data\n";}
	}

	string str=out.str();
	writeToLog(str,"/1.txt","/_1.txt");
	cout<<"Done";
	
	
};



void GImageEditor::readStrArray(commandData *inputData,vector<stringOCR>&strArray){

string  path=inputData->data["inputFile"]+".xml";

xml_document doc;
xml_node mainNode;
xml_node book, data, key;
string vData;
vector<string>dataArray;
vector<string>vArray;

//strArray.resize(0);


  if(doc.load_file(path.c_str())){
		mainNode = doc.child("strArray");
		// Iterate through books
		int index=0;

		for (xml_node row = mainNode.child("rec"); row; row = row.next_sibling("rec")){
			stringOCR string;
				  string.x0=atoi(row.attribute("x0").value());
				  string.y0=atoi(row.attribute("y0").value());
				  string.x1=atoi(row.attribute("x1").value());
				  string.y1=atoi(row.attribute("y1").value());
				  string.w=atoi(row.attribute("w").value());
				  string.h=atoi(row.attribute("h").value());
				  string.stringFlag=atoi(row.attribute("s").value());
				  string.LimY0=atoi(row.attribute("lm0").value());
				  string.LimY1=atoi(row.attribute("lm1").value());
				  string.MeanSA=atoi(row.attribute("m").value());
				  string.vLim=atoi(row.attribute("v").value());
			for (data = row.child("r1"); data; data = data.next_sibling("r1")){
				wordOCR letter;
					  letter.x0=atoi(data.attribute("x0").value());
					  letter.y0=atoi(data.attribute("y0").value());
					  letter.x1=atoi(data.attribute("x1").value());
					  letter.y1=atoi(data.attribute("y1").value());
					  letter.xp0=atoi(data.attribute("xp0").value());
					  letter.yp0=atoi(data.attribute("yp0").value());
					  letter.xp1=atoi(data.attribute("xp1").value());
					  letter.yp1=atoi(data.attribute("yp1").value());
					  letter.w=atoi(data.attribute("w").value());
					  letter.h=atoi(data.attribute("h").value());
					  letter.xv0=atoi(data.attribute("xv0").value());
					  letter.yv0=atoi(data.attribute("yv0").value());
					  letter.xv1=atoi(data.attribute("xv1").value());
					  letter.yv1=atoi(data.attribute("yv1").value());
					  letter.center.x=atoi(data.attribute("cx").value());
					  letter.center.y=atoi(data.attribute("cy").value());
					  letter.letterIndex=atoi(data.attribute("l").value());
					  letter.correlation=atoi(data.attribute("c").value());
					  letter.tsertoFlag=atoi(data.attribute("t").value());
					  letter.spaceFlag=atoi(data.attribute("sp").value());
					  letter.stackFlag=atoi(data.attribute("st").value());
					  letter.vowelFlag=atoi(data.attribute("v").value());
					  letter.name=data.attribute("n").value();
					  for (key = data.child("m1"); key; key = key.next_sibling("m1")){
							OCRMatch match;
							match.correlation=atoi(data.attribute("c").value());
							match.letterIndex=atoi(data.attribute("l").value());
							match.name=key.attribute("n").value();
							letter.match.push_back(match);
					  }
					  for (key = data.child("m2"); key; key = key.next_sibling("m2")){
							OCRMatch match;
							match.correlation=atoi(data.attribute("c").value());
							match.letterIndex=atoi(data.attribute("l").value());
							match.name=key.attribute("n").value();
							letter.match1.push_back(match);
					  }



				  string.wordArray.push_back(letter);
			  }
		   strArray.push_back(string);
		}
					  data=mainNode.child("mainString");
					  mainString=data.child_value();
					  //WideString wStr1=UTF8Decode((AnsiString)mainString.c_str());
					  //textView->Lines->Text=wStr1;
					  //drawStrArray(0);
  }else{
	//ShowMessage((AnsiString)path.c_str()+(AnsiString)" not loaded");return;
  }

}//_____________________________________________________________________________

void GImageEditor::writePageXML(commandData *inputData){
    int print=1;
	
	DT("start save XML file"<<END);
	string  path=inputData->data["inputFile"];
	path=substr(0,path.rfind("."),path);
	string volume=path;
	string fileIndex=fileName(path);
	path+=".xml";
	DT("path="<<path<<END);
	
	
	volume=volume.erase(volume.rfind("/"),volume.size());
	volume=substr(volume.rfind("/")+1,volume.size(),volume);   cout<<"volume="<<volume<<END;
	
	string resultText=HTMLtoTxt(mainString);
	
	xml_document doc;
	xml_node mainNode;
	xml_node book, data, key;
	mainNode=doc.append_child();
	mainNode.set_name("text:text");
	
	mainNode.append_attribute("xmlns:xsi")="http://www.w3.org/2001/XMLSchema-instance";
	mainNode.append_attribute("xmlns:text")="http://www.tbrc.org/models/text#";
	mainNode.append_attribute("RID")="Kanjur";
	mainNode.append_attribute("lang")="bo_ZH";
	mainNode.append_attribute("volume")=volume.c_str();
	mainNode.append_attribute("num")=fileIndex.c_str();
    mainNode.append_attribute("fromWork")="Kanjur";
    mainNode.append_attribute("fromVolume")=volume.c_str();
    mainNode.append_attribute("start")=0;
	mainNode.append_attribute("last")=0;
	mainNode.append_child(node_pcdata).set_value(resultText.c_str());
	
	bool exitCode=doc.save_file(path.c_str(),"\t", 1);
	
	
}


void GImageEditor::writeStrArray(commandData *inputData,vector<stringOCR>&strArray){

				string  path=inputData->data["inputFile"]+".xml";

				int i,ind=0;
				string record;
				xml_document doc;
				xml_node mainNode;
				xml_node book, data, key;
				mainNode=doc.append_child();
				mainNode.set_name("strArray");
				char cbuf[10];   //SH(aliKali.base.size());
				for (int index=0;index<strArray.size()-1; index++){ //SH(aliKali[i].Wylie.c_str());

						 book=mainNode.append_child();
						 //sprintf(cbuf,"%d",i);
						 //DP("index="<<index<<END);
						 book.set_name("rec");

						 book.append_attribute("id")=index;
						 book.append_attribute("x0")=strArray[index].x0;
						 book.append_attribute("y0")=strArray[index].y0;
						 book.append_attribute("x1")=strArray[index].x1;
						 book.append_attribute("y1")=strArray[index].y1;
						 book.append_attribute("w")=strArray[index].w;
						 book.append_attribute("h")=strArray[index].h;
						 book.append_attribute("s")=strArray[index].stringFlag;
						 book.append_attribute("lm0")=strArray[index].LimY0;
						 book.append_attribute("lm1")=strArray[index].LimY1;
						 book.append_attribute("m")=strArray[index].MeanSA;
						 book.append_attribute("v")=strArray[index].vLim;

  /*	int x0,y0,x1,y1,xp0,yp0,xp1,yp1,xp,yp,xt0,yt0,xt1,yt1,w,h;
		int xv0,yv0,xv1,yv1;
		OCRPoint center;
		string name;
		int letterIndex;
		vector<OCRMatch> match,match1;
		vector<TsertoOCR> TLline;
		int correlation;
		int tsertoFlag;
		int spaceFlag;
		int stackFlag;
		int vowelFlag;
		int Param1,Param2;
		int selectFlag;
		int blockIndex;
  */
							for(int in=0; in<strArray[index].wordArray.size();in++){
								//DP("in="<<in<<END);
								data=book.append_child();
								data.set_name("r1");
								data.append_attribute("x0")=strArray[index].wordArray[in].x0;
								data.append_attribute("y0")=strArray[index].wordArray[in].y0;
								data.append_attribute("x1")=strArray[index].wordArray[in].x1;
								data.append_attribute("y1")=strArray[index].wordArray[in].y1;
								data.append_attribute("xp0")=strArray[index].wordArray[in].xp0;
								data.append_attribute("yp0")=strArray[index].wordArray[in].yp0;
								data.append_attribute("xp1")=strArray[index].wordArray[in].xp1;
								data.append_attribute("yp1")=strArray[index].wordArray[in].yp1;
								//data.append_attribute("xp")=strArray[index].wordArray[in].xp;
								//data.append_attribute("yp")=strArray[index].wordArray[in].yp;
								//data.append_attribute("xt0")=strArray[index].wordArray[in].xt0;
								//data.append_attribute("yt0")=strArray[index].wordArray[in].yt0;
								//data.append_attribute("xt1")=strArray[index].wordArray[in].xt1;
								//data.append_attribute("yt1")=strArray[index].wordArray[in].yt1;
								data.append_attribute("w")=strArray[index].wordArray[in].w;
								data.append_attribute("h")=strArray[index].wordArray[in].h;
								data.append_attribute("xv0")=strArray[index].wordArray[in].xv0;
								data.append_attribute("yv0")=strArray[index].wordArray[in].yv0;
								data.append_attribute("xv1")=strArray[index].wordArray[in].xv1;
								data.append_attribute("yv1")=strArray[index].wordArray[in].yv1;
								data.append_attribute("cx")=strArray[index].wordArray[in].center.x;
								data.append_attribute("cy")=strArray[index].wordArray[in].center.y;
								data.append_attribute("n")=strArray[index].wordArray[in].name.c_str();
								//data.append_attribute("l")=strArray[index].wordArray[in].letterIndex;
								data.append_attribute("c")=strArray[index].wordArray[in].correlation;
								data.append_attribute("t")=strArray[index].wordArray[in].tsertoFlag;
								data.append_attribute("sp")=strArray[index].wordArray[in].spaceFlag;
								data.append_attribute("st")=strArray[index].wordArray[in].stackFlag;
								data.append_attribute("v")=strArray[index].wordArray[in].vowelFlag;

								for(int i=0; i<strArray[index].wordArray[in].match.size()&&i<1;i++){
									 //DP("m="<<i);
									 key=data.append_child();
									 key.set_name("m1");
									 key.append_attribute("n")=strArray[index].wordArray[in].match[i].name.c_str();
									 key.append_attribute("l")=strArray[index].wordArray[in].match[i].letterIndex;
									 key.append_attribute("c")=strArray[index].wordArray[in].match[i].correlation;
								}
								for(int i=0; i<strArray[index].wordArray[in].match1.size()&&i<1;i++){
									 //DP("m1="<<i);
									 key=data.append_child();
									 key.set_name("m2");
									 key.append_attribute("n")=strArray[index].wordArray[in].match1[i].name.c_str();
									 key.append_attribute("l")=strArray[index].wordArray[in].match1[i].letterIndex;
									 key.append_attribute("c")=strArray[index].wordArray[in].match1[i].correlation;
								}
								//DP(END);


							}

					}
				   /*	mainNode=doc.append_child();
					mainNode.set_name("vectorBase");
							record="";
							for(int in=0; in<vectorBase[0].allVectorCount;in++){
							   sprintf(cbuf,"%d",vectorBase[in].vStart.x);    record+=cbuf;record+=";";
							   sprintf(cbuf,"%d",vectorBase[in].vStart.y);    record+=cbuf;record+=";";
							   sprintf(cbuf,"%d",vectorBase[in].vEnd.x);      record+=cbuf;record+=";";
							   sprintf(cbuf,"%d",vectorBase[in].vEnd.y);      record+=cbuf;record+=";";
							   sprintf(cbuf,"%d",vectorBase[in].vCenter.x);   record+=cbuf;record+=";";
							   sprintf(cbuf,"%d",vectorBase[in].vCenter.y);   record+=cbuf;record+=";";
							   sprintf(cbuf,"%d",vectorBase[in].length);      record+=cbuf;record+=";";
							   sprintf(cbuf,"%d",vectorBase[in].vectorAngle); record+=cbuf;record+=";";
							   record+="\n";
							}
							data=book.append_child();
							data.set_name("vector");
							data.append_child(node_pcdata).set_value(record.c_str());

				  */

					data=mainNode.append_child();
					data.set_name("mainString");
					data.append_child(node_pcdata).set_value(mainString.c_str());

 bool exitCode=doc.save_file(path.c_str(),"\t", 1);

//if(exitCode){
//	cout<<"Done. save file "<<path<<END;
// }else{
//	cout<<"Not saved "<<path<<END;
// }


}//_____________________________________________________________________________


void GImageEditor::readPageHTML(commandData *inputData,vector<stringOCR>&strArray){

xml_node col,data,body,letterBaseXML, st,div1;
xml_document doc;
string str,srcLine,strHTML;
//struct stat attrib;

	   //str=inputData->data["inputFile"]+".html";
	   //if(stat(str.c_str(), &attrib)<0)return;
	   //inputData->c_out.open(str.c_str());
	   //inputData->c_out<<strHeaderHTML;
				//string strHeaderHTML,strBaseHTML;
				string  path=inputData->data["inputFile"]+".html";
				ifstream htmlData(path.c_str());

				if( !htmlData ){
					cout<<"Source path "<<path<<" not exist"<<endl;
					return;
				}
				int flag=0,count=0;
				while (getline(htmlData, srcLine,'\n')){
				  if(flag==0){
					 str=substr(0,5,srcLine);
					 if(str=="<body")flag=1;
				  }
				  if(flag){strHTML+=srcLine; strHTML+="\n";count++;}
				}


char *cbuf=(char*)malloc(strHTML.size()+1);
memcpy(cbuf,strHTML.c_str(),strHTML.size());
cbuf[strHTML.size()]=0;
		xml_parse_result stat;
		stat=doc.parse(cbuf);

		if(!stat){cout<<path<<" not loaded";
			str="";
			cout<<stat.description()<<" offset="<<stat.offset<<" line="<<stat.line<<"<br>"<<endl;
				for(int i=stat.offset;i<stat.offset+1024&&i<strHTML.size();i++)str+=cbuf[i];
				cout<<str<<endl;
		}

		// Iterate through books
		//cout<<" start parsing "<<flag<<"; "<<count<<END;
		count=0;
		body=doc.child("body");
		xml_node mainDiv=body.child("div");
		//cout<<mainDiv.attribute("style").value()<<END;

		//xml_node div = mainDiv.first_child();
		//cout<<" /__/ "<<div.attribute("style").value()<<END;
	int x0, x1, y0, y1;
	vector<string> list,list1;
	stringOCR strLine;
	wordOCR wP;
	int stringCount=-1;

	for (xml_node div = mainDiv.first_child(); div; div = div.next_sibling()){
		 //xml_node div = body.next_sibling();
		 //cout<<count<<END;count++;
			//str=div.attribute("style").value();
			//str=str_replace("px", "", str);
			//cout<<"   "<<str<<END;

		 str=div.attribute("class").value();
		 //cout<<" class1= "<<str<<END;


			if(str=="sT"){
				//div1 = div.first_child();
				str=div.attribute("style").value();
				str=str_replace("px", "", str);
				list=explode(";",str);
				//cout<<"list[0]="<<list[0];
				list1=explode(":",list[0]);
				//cout<<"list1[0]="<<list1[0];
				x0=atoi(list1[1].c_str()); //cout<<" x0="<<x0;
				//cout<<"list[1]="<<list[1];
				list1=explode(":",list[1]);
				y0=atoi(list1[1].c_str()); //cout<<" y0="<<y0;
				//cout<<"list[2]="<<list[2];
				list1=explode(":",list[2]);
				x1=atoi(list1[1].c_str()); //cout<<" x1="<<x1;
				//cout<<"list[3]="<<list[3];
				list1=explode(":",list[3]);
				y1=atoi(list1[1].c_str());  y1+=y0;	//cout<<" y1="<<y1<<END;
				strLine.x0=x0; strLine.x1=x1; strLine.y0=y0; strLine.y1=y1;
				strArray.push_back(strLine); stringCount++;
			}

		   if(str=="dE"){
			   //div1 = div.first_child();
			   //cout<<"/___"<<div.attribute("class").value()<<" ";
			   wP.correlation=atoi(div.attribute("c").value()); //cout<<"correlation="<<wP.correlation<<END;
			   str=div.attribute("style").value();
			   str=str_replace("px", "", str);
			   list=explode(";",str);
			   //cout<<"list[0]="<<list[0];
			   list1=explode(":",list[0]);
			   //cout<<"list1[0]="<<list1[0];
			   x0=atoi(list1[1].c_str()); //cout<<" x0="<<x0;
			   //cout<<"list[1]="<<list[1];
			   list1=explode(":",list[1]);
			   y0=atoi(list1[1].c_str()); //cout<<" y0="<<y0;
			   //cout<<"list[2]="<<list[2];
			   list1=explode(":",list[2]);
			   x1=atoi(list1[1].c_str()); x1+=x0; //cout<<" x1="<<x1;
			   //cout<<"list[3]="<<list[3];
			   list1=explode(":",list[3]);
			   y1=atoi(list1[1].c_str());  y1+=y0;	//cout<<" y1="<<y1<<END;
			   wP.x0=x0; wP.x1=x1; wP.y0=y0; wP.y1=y1;
			   div1 = div.first_child();
			   div1 = div1.next_sibling();
			   //cout<<" class="<<div1.attribute("class").value()<<END;
			   wP.name=div1.child_value();  //cout<<" name="<<wP.name<<END;
			   //strArray[stringCount].wordArray.push_back(wP);
		   }
		   if(str=="nT"){
			 cout<<div.child_value()<<END;
		   }
		}

		//xml_node::iterator it = body.begin();
		//cout << "class " << it->attribute("style").value();

		//it++;
		//cout << "class " << it->attribute("style").value();

		//exit(0);

}//_____________________________________________________________________________

void GImageEditor::readCorrectionTable(commandData *inputData){

	cout<<"Start read inputData->data[\"correctionTable\"]"<<inputData->data["correctionTable"]<<END;
	string str,vData;
	vector<string>dataArray;
	xml_node data;
	xml_document doc;
	unsigned int in;
	int w,h, index;
	char buff[2];
	char c,*p;
	setHex(); //set hex code for char string to byte conversion
	
	xml_parse_result stat;
	stat=doc.load_file(inputData->data["correctionTable"].c_str());
	
	if(!stat){cout<<"correctionTable not loaded";
		if(stat.offset==0)return;
		cout<<stat.description()<<" offset="<<stat.offset<<" line="<<stat.line<<endl;
		ifstream file (inputData->data["correctionTable"].c_str(), ios::in|ios::binary|ios::ate);
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
			//str=str_replace("<","{", str);
			//str=str_replace(">","}", str);
			//file.read (memblock, size_block);
			file.close();
			cout<<"line offset="<<str<<endl;
			//c_out<<"line offset="<<memblock<<endl;
			delete memblock;     //cout<<"size_block="<<size_block<<endl;
		}
		exit(0);
	}
	
	
	correctionTable.resize(0);
	
	for (xml_node row = doc.child("rec"); row; row = row.next_sibling("rec")){
		stringOCR wp;
		wp.file=row.attribute("f").value();
		wp.name=row.attribute("n").value();
		wp.idNumber=strtoul(row.attribute("id").value(),NULL,0); //cout<<" wp.idNumber="<<wp.idNumber<<endl;
		//wp.x0=atoi(row.attribute("x0").value());
		//wp.y0=atoi(row.attribute("y0").value());
		//wp.x1=atoi(row.attribute("x1").value());
		//wp.y1=atoi(row.attribute("y1").value());
		//wp.checkFlag=1; //string checked
		wp.correlation=atoi(row.attribute("c").value());
		data=row.child("img");
		w=atoi(data.attribute("w").value());
		h=atoi(data.attribute("h").value());  //cout<<"open w="<<w<<" h="<<h<<endl;
		vData=data.child_value();
		//wp.img=GBitmap::create(w,h);
		//wp.img->bites_data=(unsigned char*)malloc(vData.size());
		//p=wp.img->bites_data;
		
		//wp.img=GBitmap::create(loadFromTiffString(data,1));
		wp.img=GBitmap::create();
		wp.img->h=h;
		wp.img->w=w;
		
		char *cBuff=(char*)malloc(vData.size()/2);
		int i=0; unsigned short *s;
			for(int m=0;m<vData.size();m+=2){
				s=(unsigned short*)&vData[m];
				cBuff[i]=hexCode[(*s)]; 
				//cout<<buff[0]<<buff[1]<<" "<<*s<<" cBuff["<<i<<"]="<<(short)cBuff[i]<<endl;
				i++;
			}
		wp.img->TIFF_string.resize(vData.size()/2);
		p=&wp.img->TIFF_string[0];
		memcpy(p, cBuff, vData.size()/2); free(cBuff);
		wp.img->pack_flag=1;
		
		//writeText(wp.img->TIFF_string,"/2.tiff"); //exit(0);
		
		//unsigned char c; string record;
		//for(int m=0;m<wp.img->TIFF_string.size();m++) {
		//	c=(unsigned char)wp.img->TIFF_string[m];
		//	record+=charToHex(c);
		//}	
		//writeText(record,"/2.txt"); exit(0);

		
		//wp.img=GBitmap::create(loadFromTiffString(data,1));
		//wp.img=GBitmap::create();
		//wp.img->TIFF_string_size=vData.size();
		//wp.img->TIFF_string=(char*)malloc(wp.img->TIFF_string_size);
		//if(wp.img->TIFF_string==NULL){cout<<"NO MEMORY"; continue;}
		//char *p=&vData[0];
		//memcpy(wp.img->TIFF_string, p, wp.img->TIFF_string_size);
		//cout<<".";
		//cout<<"w="<<wp.img->columns();
		//wp.img->pack_flag=1;	
		//wp.img->data_size=vData.size()*8;
		//writeText(data,"/1.tiff");
		correctionTable.push_back(wp);
		
	}	
	
	
	cout<<"done read correctionTable.size()="<<correctionTable.size()<<endl;
	//WriteImageData(correctionTable[0].img,"/1.jpg",0);

}
