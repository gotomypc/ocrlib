///диспетчер выполнения процесса распознавания страницы.
///создает все необходимые объекты, контролирует ход выполнения процедур и формирует результат

void GImageEditor::startOCR(commandData *inputData){
		   // Автоматическое масштабирование размеров картинки
		   //ShowMessage("1start");
cout<<"Start "<<inputData->data["inputFile"]<<END;
//inputData->data["inputFile"]=inputData->fileList[0];
pechaImg=LoadImageData(inputData->data["inputFile"],0);
if(!pechaDataLoaded){cout<<"no open file"<<inputData->data["inputFile"]<<endl; return;}

vector<stringOCR>strArray;  
int border;
string str;
int draw=1,print=0;
if(inputData->data["ocrData"]=="drawLetter"){
	draw=1;
	inputData->c_out<<"<html><body><div style=\"position:absolute; left:0px; top:0px;\">";
	inputData->c_out<<"<img src=\"__1draw.gif\"><br><div style=\"position:relative; left:20px;\">"<<END;
}
	cout<<"@4_1";
	              //vectorBase[0].allVectorCount=0;
	border=0; clock_t  tm_start1, tm_end1; double time1;  tm_start1=clock();
				  setBit=GBitset::createResize(pechaImg,1,1,1);
	              cout<<"@4_2";
				  setBit->pageStringDetector(strArray); // Подпрограмма выделения строк и букв ПЕЧА ( РЕЛСЫ )  
	              cout<<"@4_3";
				  border=setBit->border_size();
	              cout<<"@4_4";
	
		  //cout<<"strArray.size()="<<strArray.size()<<END;
				  //cout<<"<div style=\"position:absolute; left:0px; top:0px;\">";
				  //cout<<"<img src=\"http://localhost/"<<inputData->inputFileName<<"\">\n";
				  ///str=inputData->data["outFolder"]+"/1_out.jpg";
				  //WriteImageData(setBit->drawData,str,1);    return;
							//setContour=GBitsetContour::createResize(imgArray[0],1,1,1);
							//setContour->setData(vectorBase,strArray,iLeft,iTop);
							//vectorCount=setContour->vectorCount;
							//ShowMessage (vectorCount);
cout<<"@5";
	if(aliKali.letterCount==0)readLetterBase(inputData); cout<<"aliKali1.base.size()="<<aliKali.letterCount<<endl;

							GBitsetOCR *setOCR=GBitsetOCR::createResize(pechaImg,1,1);
							//if(NewLetterOnlyBtn->Checked==true) {mode=NEWLETTERINBOOK;}else{mode=ALL_LETTER;}
							TIME_START
													
	                        setOCR->setData(inputData,
											&aliKali,
											strArray,
											correctionTable,
											logicProcessor,
											iLeft,
											iTop,
											border,
											ALL_LETTER);
	
							TIME_PRINT_
							mainString=setOCR->mainString;
	
                             	
	                        //cout<<"time="<<time<<" mainString"<<mainString<<END;

cout<<"@6";
							//for(int a=0;a<str.size();a++)textOCRString+=(WideString)str[a];
				  //DS(setOCR->mainString.c_str());
					tm_end1=clock(); tm_end1-=tm_start1; time1=tm_end1/CLOCKS_PER_SEC;
					//DM("//______timeALL______="<<time1<<END);tm_start1=clock();
					//ShowMessage("1end");
					//DM("start_draw");
					  //setContour->destroy();
					  //delete setContour;


			  //}
			  ///Label1->Caption="Done"; Application->ProcessMessages();
			  if(setBit!=NULL){
				setBit->destroy(); 
				delete setBit;     
			  }
			  if(setOCR!=NULL){

				if(draw){
				  str="/__1draw.jpg";
#ifdef DEBUGLVL_RGB						
				    if(setOCR->setMatrix!=NULL)WriteImageData(setOCR->setMatrix->drawDataRGB,str,0);
#endif
#ifdef DEBUGLVL_GREY						
					//if(setOCR->setMatrix!=NULL)WriteImageData(setOCR->setMatrix->drawData,str,0);
#endif
					
					inputData->c_out<<"</div></body></html>";
				}
				  cout<<"start release setOCR";
				  setOCR->destroy();
				  delete setOCR; cout<<"draw2_1";
				  
				  cout<<"draw0";
				  if(inputData->data["ocrData"]=="tib"||
				     inputData->data["ocrData"]=="ocrBatch"||
					 inputData->data["ocrData"]=="letterBlock")drawStrArray(inputData,strArray,border);	
				  if(inputData->data["ocrData"]=="tibLetters")drawStrArrayDebug(inputData,strArray,border);
				  writePageXML(inputData);
				  cout<<"draw1";
				  //if(inputData->data["ocrData"]=="letterBlock")drawLettersInCorrectionTable(inputData,LETTER_ADD);	
				  cout<<"draw2";
				}
				//strArray.resize(0);
			  
	          cout<<"done"<<END;
			  //BitBtnCancelClick(this);
			  //selectMode=0; dragMode=1; pageFrameIndex=-1;
			 //ShowMessage((AnsiString)"Frame "+(AnsiString)(imageIndex+1)+(AnsiString)" is ready");

		//}

		//BitBtnDragClick(this);
		//if(count==5){SH(1);}      count++;

		pechaImg->destroy();
		delete pechaImg;
		pechaDataLoaded=0;
cout<<"all done"<<END;

//}
}//________________________________________________________________________________________________________________

void GImageEditor::startOCRBatch(commandData *inputData){
	
	string strHeaderHTML,srcLine,str;
	string  path=inputData->data["tablePath"]+"/header.xml";
	DIR *dir;
	int mode;
	replace(path.begin(),path.end(),'\\','/');
	//string statPath=inputData->data["tablePath"]+"/OCRTables/OCRStat.xml";

	if(!inputData->fileList.size()){
		readDirectoryToArray(inputData->fileList,inputData->data["inputFolder"],"img");
	}
	
	if(!logicProcessor->dictionaryReady)logicProcessor->readDictionary(inputData);
	
	//cout<<"if(inputData->data[\"ocrData\"]="<<inputData->data["ocrData"]<<END;
	inputData->stat_out.open(inputData->data["statPath"].c_str()); //open new stat file
	
	if(inputData->data["ocrData"]=="letterBlock"){
	   str=""; writeText(str,inputData->data["correctionTable"]);//erase old correctionTable
    }
	
	correctionTable.resize(0);  ///начинаем новую корректурную таблицу
	int stat=0;
	
	//if(inputData->data["ocrData"]=="ocrBatch")mode=1;
	//if(inputData->data["ocrData"]=="letterBlock")mode=2;
	
	int i=0;
		
#ifdef MACOS		
	
		int pid0=0,pid1=0,pid2=0,pid3=0,pid4=0,pid5=0,pid6=0,pid7=0;
		int status;
		int pid;
	
		while(i<inputData->fileList.size()){
			cout<<"NEW DIR "<<inputData->fileList[i]<<endl;
			if( ( dir=opendir(inputData->fileList[i].c_str()))!=NULL){
				readDirectoryToArray(inputData->fileList, inputData->fileList[i],"img");
				i++; continue;
			}	
			
			if(inputData->fileList.size()==1){   //if it is only one file, fork() no need;
				inputData->data["inputFile"]=inputData->fileList[0];
				str=inputData->data["inputFile"];
				str=substr(0,str.rfind("."),str);
				str+=".html";   cout<<str<<END;
				inputData->data["inputFileName"]=inputData->data["siteName"];
				inputData->data["inputFileName"]+=substr(inputData->data["siteRoot"].size(),inputData->data["inputFile"]);
				//readPageHTML(inputData);
				inputData->c_out.open(str.c_str());
				pechaDataLoaded=0;
				startOCR(inputData);
				inputData->c_out.close();
				if(stat)writeLetterStat(inputData);
				break;
			}
			
			
			
			cout<<"NEW START"<<endl;
			if(pid0==0){
				pid0 = fork();
				if (pid0 < 0)
					error("ERROR on fork");
				if (pid0 == 0)  {
					inputData->data["inputFile"]=inputData->fileList[i];
					str=inputData->data["inputFile"];
					str=substr(0,str.rfind("."),str);
					str+=".html";   cout<<str<<END;
					inputData->data["inputFileName"]=inputData->data["siteName"];
					inputData->data["inputFileName"]+=substr(inputData->data["siteRoot"].size(),inputData->data["inputFile"]);
					//readPageHTML(inputData);
					inputData->c_out.open(str.c_str());
					pechaDataLoaded=0;
					startOCR(inputData);
					inputData->c_out.close();
					if(stat)writeLetterStat(inputData);
					exit(0);
				}
				i++;if(i==inputData->fileList.size())break;
			} 	
				
			if(pid1==0){				
				pid1 = fork();
				if (pid1 < 0)
					error("ERROR on fork");
				if (pid1 == 0)  {
					inputData->data["inputFile"]=inputData->fileList[i];
					str=inputData->data["inputFile"];
					str=substr(0,str.rfind("."),str);
					str+=".html";   cout<<str<<END;
					inputData->data["inputFileName"]=inputData->data["siteName"];
					inputData->data["inputFileName"]+=substr(inputData->data["siteRoot"].size(),inputData->data["inputFile"]);
					//readPageHTML(inputData);
					inputData->c_out.open(str.c_str());
					pechaDataLoaded=0;
					startOCR(inputData);
					inputData->c_out.close();
					if(stat)writeLetterStat(inputData);
					exit(0);
				}
				i++;if(i==inputData->fileList.size())break;
			}
				
			if(pid2==0){
					pid2 = fork();
				if (pid2 < 0)
					error("ERROR on fork");
				if (pid2 == 0)  {
					inputData->data["inputFile"]=inputData->fileList[i];
					str=inputData->data["inputFile"];
					str=substr(0,str.rfind("."),str);
					str+=".html";   cout<<str<<END;
					inputData->data["inputFileName"]=inputData->data["siteName"];
					inputData->data["inputFileName"]+=substr(inputData->data["siteRoot"].size(),inputData->data["inputFile"]);
					//readPageHTML(inputData);
					inputData->c_out.open(str.c_str());
					pechaDataLoaded=0;
					startOCR(inputData);
					inputData->c_out.close();
					if(stat)writeLetterStat(inputData);
					exit(0);
				}
				i++;if(i==inputData->fileList.size())break;
			}
			if(pid3==0){		
				pid3 = fork();
				if (pid3 < 0)
					error("ERROR on fork");
				if (pid3 == 0)  {
					inputData->data["inputFile"]=inputData->fileList[i];
					str=inputData->data["inputFile"];
					str=substr(0,str.rfind("."),str);
					str+=".html";   cout<<str<<END;
					inputData->data["inputFileName"]=inputData->data["siteName"];
					inputData->data["inputFileName"]+=substr(inputData->data["siteRoot"].size(),inputData->data["inputFile"]);
					//readPageHTML(inputData);
					inputData->c_out.open(str.c_str());
					pechaDataLoaded=0;
					startOCR(inputData);
					inputData->c_out.close();
					if(stat)writeLetterStat(inputData);
					exit(0);
				}
				i++;if(i==inputData->fileList.size())break;
			}
			if(pid4==0){				
				pid4 = fork();
				if (pid4 < 0)
					error("ERROR on fork");
				if (pid4 == 0)  {
					inputData->data["inputFile"]=inputData->fileList[i];
					str=inputData->data["inputFile"];
					str=substr(0,str.rfind("."),str);
					str+=".html";   cout<<str<<END;
					inputData->data["inputFileName"]=inputData->data["siteName"];
					inputData->data["inputFileName"]+=substr(inputData->data["siteRoot"].size(),inputData->data["inputFile"]);
					//readPageHTML(inputData);
					inputData->c_out.open(str.c_str());
					pechaDataLoaded=0;
					startOCR(inputData);
					inputData->c_out.close();
					if(stat)writeLetterStat(inputData);
					exit(0);
				}
				i++;if(i==inputData->fileList.size())break;
			}
			if(pid5==0){
				pid5 = fork();
				if (pid5 < 0)
					error("ERROR on fork");
				if (pid5 == 0)  {
					inputData->data["inputFile"]=inputData->fileList[i];
					str=inputData->data["inputFile"];
					str=substr(0,str.rfind("."),str);
					str+=".html";   cout<<str<<END;
					inputData->data["inputFileName"]=inputData->data["siteName"];
					inputData->data["inputFileName"]+=substr(inputData->data["siteRoot"].size(),inputData->data["inputFile"]);
					//readPageHTML(inputData);
					inputData->c_out.open(str.c_str());
					pechaDataLoaded=0;
					startOCR(inputData);
					inputData->c_out.close();
					if(stat)writeLetterStat(inputData);
					exit(0);
				}
				i++;if(i==inputData->fileList.size())break;
			}
			if(pid6==0){		
				pid6 = fork();
				if (pid6 < 0)
					error("ERROR on fork");
				if (pid6 == 0)  {
					inputData->data["inputFile"]=inputData->fileList[i];
					str=inputData->data["inputFile"];
					str=substr(0,str.rfind("."),str);
					str+=".html";   cout<<str<<END;
					inputData->data["inputFileName"]=inputData->data["siteName"];
					inputData->data["inputFileName"]+=substr(inputData->data["siteRoot"].size(),inputData->data["inputFile"]);
					//readPageHTML(inputData);
					inputData->c_out.open(str.c_str());
					pechaDataLoaded=0;
					startOCR(inputData);
					inputData->c_out.close();
					if(stat)writeLetterStat(inputData);
					exit(0);
				}
				i++;if(i==inputData->fileList.size())break;
			}
			if(pid7==0){				
				pid7 = fork();
				if (pid7 < 0)
					error("ERROR on fork");
				if (pid7 == 0)  {
					inputData->data["inputFile"]=inputData->fileList[i];
					str=inputData->data["inputFile"];
					str=substr(0,str.rfind("."),str);
					str+=".html";   cout<<str<<END;
					inputData->data["inputFileName"]=inputData->data["siteName"];
					inputData->data["inputFileName"]+=substr(inputData->data["siteRoot"].size(),inputData->data["inputFile"]);
					//readPageHTML(inputData);
					inputData->c_out.open(str.c_str());
					pechaDataLoaded=0;
					startOCR(inputData);
					inputData->c_out.close();
					if(stat)writeLetterStat(inputData);
					exit(0);
				} 
				i++;if(i==inputData->fileList.size())break;
			}	
			cout<<"START WAIT";
			pid=wait(&status);
			if(pid==pid0)pid0=0;
			if(pid==pid1)pid1=0;
			if(pid==pid2)pid2=0;
			if(pid==pid3)pid3=0;
			if(pid==pid4)pid4=0;
			if(pid==pid5)pid5=0;
			if(pid==pid6)pid6=0;
			if(pid==pid7)pid7=0;

			cout<<"PID"<<pid0<<" "<<pid1<<" "<<pid2<<" "<<pid3<<" "<<pid4<<" "<<pid5<<" "<<pid6<<" "<<pid7<<endl;
			//if(pid0)waitpid(pid0, &status,0);
			//if(pid1)waitpid(pid1, &status,0);
			//if(pid2)waitpid(pid2, &status,0);
			//if(pid3)waitpid(pid3, &status,0);
			//if(pid4)waitpid(pid4, &status,0);
			//if(pid5)waitpid(pid5, &status,0);
			//if(pid6)waitpid(pid6, &status,0);
			//if(pid7)waitpid(pid7, &status,0);
		}
	        cout<<"START WAIT_";
	        while(pid0!=0||pid1!=0||pid2!=0||pid3!=0||pid4!=0||pid5!=0||pid6!=0||pid7!=0){
				cout<<"WAIT_PID"<<pid0<<" "<<pid1<<" "<<pid2<<" "<<pid3<<" "<<pid4<<" "<<pid5<<" "<<pid6<<" "<<pid7<<endl;
				pid=wait(&status);
				if(pid==pid0)pid0=0;
				if(pid==pid1)pid1=0;
				if(pid==pid2)pid2=0;
				if(pid==pid3)pid3=0;
				if(pid==pid4)pid4=0;
				if(pid==pid5)pid5=0;
				if(pid==pid6)pid6=0;
				if(pid==pid7)pid7=0;
            }
         	cout<<"END WAIT_" ;
#else
		while(i<inputData->fileList.size()){
			inputData->data["inputFile"]=inputData->fileList[i];
			str=inputData->data["inputFile"];
			str=substr(0,str.rfind("."),str);
			str+=".html";   //cout<<str<<END;
			inputData->data["inputFileName"]=inputData->data["siteName"];
			inputData->data["inputFileName"]+=substr(inputData->data["siteRoot"].size(),inputData->data["inputFile"]);
			//readPageHTML(inputData);
			inputData->c_out.open(str.c_str());
			pechaDataLoaded=0;
			startOCR(inputData);
			inputData->c_out.close();
		}
		
#endif		
		//drawLettersInCorrectionTable(inputData,DRAW_BASE);
	   if(inputData->data["ocrData"]=="readPageHTML"){
		
		for(int i=0;i<inputData->fileList.size();i++){
			str=inputData->fileList[i]+".html";
			//cout<<str<<END;
			
			inputData->data["inputFile"]=inputData->fileList[i];
			inputData->fileList[0]=inputData->fileList[i];
			//readPageHTML(inputData);
			//inputData->c_out.open(str.c_str());
		}
		//inputData->c_out.close();
	   }
		

	
	cout<<"COMPLETE"<<endl;
	
}//____________________________________________________________________________

void GImageEditor::startOCR_socket(commandData *inputData){
	// Автоматическое масштабирование размеров картинки
	//ShowMessage("1start");
	cout<<"Start "<<inputData->data["inputFile"]<<END;
	//inputData->data["inputFile"]=inputData->fileList[0];
	pechaImg=LoadImageData(inputData->data["inputFile"],1);
	
	vector<stringOCR>strArray;  
	int border;
	string str;
	int draw=0,print=0;
	if(inputData->data["ocrData"]=="drawLetter"){
		draw=1;
		inputData->c_out<<"<html><body><div style=\"position:absolute; left:0px; top:0px;\">";
		inputData->c_out<<"<img src=\"__1draw.gif\"><br><div style=\"position:relative; left:20px;\">"<<END;
	}
	cout<<"@4_1";
	//vectorBase[0].allVectorCount=0;
	border=0; clock_t  tm_start1, tm_end1; double time1;  tm_start1=clock();
	setBit=GBitset::createResize(pechaImg,1,1,1);
	cout<<"@4_2";
	setBit->pageStringDetector(strArray); // Подпрограмма выделения строк и букв ПЕЧА ( РЕЛСЫ )  
	cout<<"@4_3";
	border=setBit->border_size();
	cout<<"@4_4";
	//cout<<"strArray.size()="<<strArray.size()<<END;
	//cout<<"<div style=\"position:absolute; left:0px; top:0px;\">";
	//cout<<"<img src=\"http://localhost/"<<inputData->inputFileName<<"\">\n";
	///str=inputData->data["outFolder"]+"/1_out.jpg";
	//WriteImageData(setBit->drawData,str,1);    return;
	//setContour=GBitsetContour::createResize(imgArray[0],1,1,1);
	//setContour->setData(vectorBase,strArray,iLeft,iTop);
	//vectorCount=setContour->vectorCount;
	//ShowMessage (vectorCount);
	cout<<"@5";
	if(aliKali.letterCount==0)readLetterBase(inputData); //cout<<"aliKali.base.size()="<<aliKali.base.size()<<endl;
	GBitsetOCR *setOCR=GBitsetOCR::createResize(pechaImg,1,1);
	//if(NewLetterOnlyBtn->Checked==true) {mode=NEWLETTERINBOOK;}else{mode=ALL_LETTER;}
	TIME_START
	setOCR->setData(inputData,
					&aliKali,
					strArray,
					correctionTable,
					logicProcessor,
					iLeft,
					iTop,
					border,
					ALL_LETTER);
	TIME_PRINT
	mainString=setOCR->mainString;
	//cout<<"time="<<time<<" mainString"<<mainString<<END;
	
	cout<<"@6";
	//for(int a=0;a<str.size();a++)textOCRString+=(WideString)str[a];
	//DS(setOCR->mainString.c_str());
	tm_end1=clock(); tm_end1-=tm_start1; time1=tm_end1/CLOCKS_PER_SEC;
	//DM("//______timeALL______="<<time1<<END);tm_start1=clock();
	//ShowMessage("1end");
	//DM("start_draw");
	//setContour->destroy();
	//delete setContour;
	
	
	//}
	///Label1->Caption="Done"; Application->ProcessMessages();
	if(setBit!=NULL){
		setBit->destroy();
		delete setBit;
	}
	if(setOCR!=NULL){
		
		if(draw){
			str=inputData->data["inputFolder"]+"/__1draw.jpg";
#ifdef DEBUGLVL_RGB			
			if(setOCR->setMatrix!=NULL)WriteImageData(setOCR->setMatrix->drawDataRGB,str,0);
#endif			
#ifdef DEBUGLVL_RGB			
			if(setOCR->setMatrix!=NULL)WriteImageData(setOCR->setMatrix->drawData,str,0);
#endif			
			
			inputData->c_out<<"</div></body></html>";
		}else{
			cout<<"draw0";
			drawStrArray(inputData,strArray,border);	
			//drawStrArrayDebug(inputData,strArray,border);
			writeStrArray(inputData,strArray);
			cout<<"draw1";
			drawLettersInCorrectionTable(inputData,LETTER_ADD);	
			cout<<"draw2";
		}
		setOCR->destroy();
		delete setOCR; cout<<"draw2_1";
		//strArray.resize(0);
	}
	cout<<"done"<<END;
	//BitBtnCancelClick(this);
	//selectMode=0; dragMode=1; pageFrameIndex=-1;
	//ShowMessage((AnsiString)"Frame "+(AnsiString)(imageIndex+1)+(AnsiString)" is ready");
	
	//}
	
	//BitBtnDragClick(this);
	//if(count==5){SH(1);}      count++;
	
	pechaImg->destroy();
	delete pechaImg;
	pechaDataLoaded=0;
	cout<<"all done"<<END;
	
	//}
}//________________________________________________________________________________________________________________



void GImageEditor::startOCRBatch_socket(commandData *inputData){
	
	string strHeaderHTML,srcLine,str;
	string  path=inputData->data["tablePath"]+"/header.xml";
	replace(path.begin(),path.end(),'\\','/');
	
	if(!inputData->fileList.size()){
		readDirectoryToArray(inputData->fileList,inputData->data["inputFolder"],"img");
	}
	
	//cout<<"if(inputData->data[\"ocrData\"]="<<inputData->data["ocrData"]<<END;
	
	if(inputData->data["ocrData"]=="ocrBatch"){
	
		for(int i=0;i<inputData->fileList.size();i++){
			str=inputData->fileList[i]+".html";   //cout<<"INPUT"<<str<<END;
			inputData->data["inputFile"]=inputData->fileList[i];

			inputData->data["inputFileName"]=inputData->data["siteName"];
			inputData->data["inputFileName"]+=substr(inputData->data["siteRoot"].size(),inputData->data["inputFile"]);
			//readPageHTML(inputData);
			
			inputData->c_out.open(str.c_str());
			pechaDataLoaded=0;
			startOCR(inputData);
			inputData->c_out.close();
			
		}
		DT("s1");
		drawLettersInCorrectionTable(inputData,DRAW_BASE);
	}else if(inputData->data["ocrData"]=="readPageHTML"){
		
		for(int i=0;i<inputData->fileList.size();i++){
			str=inputData->fileList[i]+".html";
			//cout<<str<<END;
			
			inputData->data["inputFile"]=inputData->fileList[i];
			inputData->fileList[0]=inputData->fileList[i];
			//readPageHTML(inputData);
			//inputData->c_out.open(str.c_str());
		}
		//inputData->c_out.close();
		
		
	}
	
	
}//____________________________________________________________________________

void GImageEditor::imageProcess(commandData *inputData){

	string strHeaderHTML,srcLine,str;
	string  path=inputData->data["tablePath"]+"/header.xml";
	replace(path.begin(),path.end(),'\\','/');
		
	if(!inputData->fileList.size()){
		readDirectoryToArray(inputData->fileList,inputData->data["inputFolder"],"img");
	}
	
#ifdef MACOS	
	int i=0;
	int pid,pid0=0,pid1=0,pid2=0,pid3=0,pid4=0,pid5=0,pid6=0,pid7=0;
	int status;
	DIR *dir;
	GBitmap *pechaImage0,*pechaImage1,*pechaImage2,*pechaImage3,*pechaImage4,*pechaImage5,*pechaImage6,*pechaImage7;
	
	pechaImage0=GBitmap::create(10,10);
	pechaImage1=GBitmap::create(10,10);
	pechaImage2=GBitmap::create(10,10);
	pechaImage3=GBitmap::create(10,10);
	pechaImage4=GBitmap::create(10,10);
	pechaImage5=GBitmap::create(10,10);
	pechaImage6=GBitmap::create(10,10);
	pechaImage7=GBitmap::create(10,10);
	
	while(i<inputData->fileList.size()){
		
		if( ( dir=opendir(inputData->fileList[i].c_str()))!=NULL){
			readDirectoryToArray(inputData->fileList, inputData->fileList[i],"img");
			inputData->fileList.erase(inputData->fileList.begin()+i);
			continue;
		}
		i++;
	}
	
		if(inputData->fileList.size()<8){
			for(int j=0;j<inputData->fileList.size();j++){
				TIME_START
				cout<<"inputData->fileList["<<j<<"]="<<inputData->fileList[j]<<END;
				inputData->data["inputFile"]=inputData->fileList[j]; pechaImage0->destroy();
				pechaImage0=LoadImageData(inputData->data["inputFile"],0); 
				if(pechaImage0==0)break;
				cout<<"@done load"; cout<<"pechaImg0="<<pechaImage0->columns()<<endl;
					proccessImage(inputData,pechaImage0);
					cout<<"end process";
				TIME_PRINT_
			}	
		}else{

			
			//int step=inputData->fileList.size()/8;
		int j=0;
		while(j<inputData->fileList.size()){
				if(pid0==0){
					pid0 = fork();
					if (pid0 < 0)error("ERROR on fork");
					if (pid0 == 0){
						//for(int j=0;j<step;j++){
							TIME_START
							inputData->data["inputFile"]=inputData->fileList[j]; pechaImage0->destroy();
							pechaImage0=LoadImageData(inputData->data["inputFile"],0); cout<<"@done load"; cout<<"pechaImg0="<<pechaImage0->columns()<<endl;
							cout<<"inputData->fileList["<<j<<"]="<<inputData->fileList[j]<<END;
							proccessImage(inputData,pechaImage0);
							TIME_PRINT_
						    exit(0);
						//}
					}	
					j++;if(j==inputData->fileList.size())break;
				}

				if(pid1==0){
					pid1 = fork();
					if (pid1 < 0)error("ERROR on fork");
					if (pid1 == 0){
						//for(int j=step;j<step*2;j++){
							inputData->data["inputFile"]=inputData->fileList[j]; pechaImage1->destroy();
							pechaImage1=LoadImageData(inputData->data["inputFile"],0); cout<<"@done load";
							cout<<"inputData->fileList["<<j<<"]="<<inputData->fileList[j]<<END;
							proccessImage(inputData,pechaImage1);
						    exit(0);
						//}
					}
					j++;if(j==inputData->fileList.size())break;
				}
				if(pid2==0){
					pid2 = fork();
					if (pid2 < 0)error("ERROR on fork");
					if (pid2 == 0){						
						//for(int j=step*2;j<step*3;j++){
							inputData->data["inputFile"]=inputData->fileList[j]; pechaImage2->destroy();
							pechaImage2=LoadImageData(inputData->data["inputFile"],0); cout<<"@done load";
							cout<<"inputData->fileList["<<j<<"]="<<inputData->fileList[j]<<END;
							proccessImage(inputData,pechaImage2);
						    exit(0);
						//}	
					}
					j++;if(j==inputData->fileList.size())break;
				}
				if(pid3==0){
					pid3 = fork();
					if (pid3 < 0)error("ERROR on fork");
					if (pid3 == 0){
						//for(int j=step*3;j<step*4;j++){
							inputData->data["inputFile"]=inputData->fileList[j]; pechaImage3->destroy();
							pechaImage3=LoadImageData(inputData->data["inputFile"],0); cout<<"@done load";
							cout<<"inputData->fileList["<<j<<"]="<<inputData->fileList[j]<<END;
							proccessImage(inputData,pechaImage3);
						    exit(0);
						//}
					}
					j++;if(j==inputData->fileList.size())break;
				}
				if(pid4==0){
					pid4 = fork();
					if (pid4 < 0)error("ERROR on fork");
					if (pid4 == 0){
						//for(int j=step*4;j<step*5;j++){
							inputData->data["inputFile"]=inputData->fileList[j]; pechaImage4->destroy();
							pechaImage4=LoadImageData(inputData->data["inputFile"],0); cout<<"@done load";
							cout<<"inputData->fileList["<<j<<"]="<<inputData->fileList[j]<<END;
							proccessImage(inputData,pechaImage4);
                            exit(0);
						//}
					}
					j++;if(j==inputData->fileList.size())break;
				}
				if(pid5==0){
					pid5 = fork();
					if (pid5 < 0)error("ERROR on fork");
					if (pid5 == 0){
						//for(int j=step*5;j<step*6;j++){
							inputData->data["inputFile"]=inputData->fileList[j]; pechaImage5->destroy();
							pechaImage5=LoadImageData(inputData->data["inputFile"],0); cout<<"@done load";
							cout<<"inputData->fileList["<<j<<"]="<<inputData->fileList[j]<<END;
							proccessImage(inputData,pechaImage5);
                            exit(0);
						//}
					}
					j++;if(j==inputData->fileList.size())break;
				}
				if(pid6 ==0){
					pid6 = fork();
					if (pid6 < 0)error("ERROR on fork");
					if (pid6 == 0){
						//for(int j=step*6;j<step*7;j++){
							inputData->data["inputFile"]=inputData->fileList[j]; pechaImage6->destroy();
							pechaImage6=LoadImageData(inputData->data["inputFile"],0); cout<<"@done load";
							cout<<"inputData->fileList["<<j<<"]="<<inputData->fileList[j]<<END;		
							proccessImage(inputData,pechaImage6);
							exit(0);
						//}
					}
					j++;if(j==inputData->fileList.size())break;
				}
				if(pid7==0){
					pid7 = fork();
					if (pid7 < 0) error("ERROR on fork");
					if (pid7 == 0){
						//for(int j=step*7;j<inputData->fileList.size();j++){
							inputData->data["inputFile"]=inputData->fileList[j]; pechaImage7->destroy();
							pechaImage7=LoadImageData(inputData->data["inputFile"],0); cout<<"@done load";
							cout<<"inputData->fileList["<<j<<"]="<<inputData->fileList[j]<<END;		
							proccessImage(inputData,pechaImage7);
							exit(0);
							
						//}
					}
					j++;if(j==inputData->fileList.size())break;
				}
		
			cout<<"START WAIT";
			pid=wait(&status);
			if(pid==pid0)pid0=0;
			if(pid==pid1)pid1=0;
			if(pid==pid2)pid2=0;
			if(pid==pid3)pid3=0;
			if(pid==pid4)pid4=0;
			if(pid==pid5)pid5=0;
			if(pid==pid6)pid6=0;
			if(pid==pid7)pid7=0;
			
			cout<<"PID"<<pid0<<" "<<pid1<<" "<<pid2<<" "<<pid3<<" "<<pid4<<" "<<pid5<<" "<<pid6<<" "<<pid7<<endl;
			
	    }
		}		

	cout<<"START WAIT_";
	while(pid0!=0||pid1!=0||pid2!=0||pid3!=0||pid4!=0||pid5!=0||pid6!=0||pid7!=0){
		cout<<"WAIT_PID"<<pid0<<" "<<pid1<<" "<<pid2<<" "<<pid3<<" "<<pid4<<" "<<pid5<<" "<<pid6<<" "<<pid7<<endl;
		pid=wait(&status);
		if(pid==pid0)pid0=0;
		if(pid==pid1)pid1=0;
		if(pid==pid2)pid2=0;
		if(pid==pid3)pid3=0;
        if(pid==pid4)pid4=0;
		if(pid==pid5)pid5=0;
		if(pid==pid6)pid6=0;
		if(pid==pid7)pid7=0;
	}
	cout<<"END WAIT_ALL_DONE" ;
	
#else
	for(int i=0;i<inputData->fileList.size();i++){
			cout<<"inputData->fileList["<<i<<"]="<<inputData->fileList[i]<<END;
			inputData->data["inputFile"]=inputData->fileList[i];

			//readPageHTML(inputData);
			//inputData->c_out.open(str.c_str());
		}
	
#endif	
			//inputData->c_out.close();
	          	

	
}//______________________________________________


void GImageEditor::proccessImage(commandData *inputData,GBitmap *pechaImg_){

	cout<<"inputData->data[SplitPage]"<<inputData->data["SplitPage"]<<endl;
	
	
	vector<stringOCR>strArray;
	if(inputData->data["SplitPage"]=="CropBorder"){
		
		cout<<"cropBorder"<<END;
		setBit=GBitset::createResize(pechaImg_,1,1,1);
		setBit->AutoBorderBtn=1;
		setBit->pageStringDetector(strArray);  
		//WriteImageData(setBit->drawData,1);
		//setBit->destroy(); delete setBit;
		//return;
		//  ДЛЯ ПОКАЗА КАРТИНКИ
		//if(CheckBoxTIFF->Checked==true){ext=".tif";}
		//if(AutoRemoveBorderBtn->Checked==true){
		int pageBorder=atoi(inputData->data["pageBorder"].c_str());
		cout<<"pageBorder="<<pageBorder<<END;
		RemoveBorder(pechaImg,setBit,pageBorder, inputData->data["inputFile"]); // , strArray
		
		setBit->destroy();
		delete setBit;
		
	}
	if(inputData->data["SplitPage"]=="SplitPage"){				
		cout<<"SplitPage"<<END;
		strArray.resize(0);
		setBit=GBitset::createResize(pechaImg_,1,1,1);
		setBit->SplitPagesBtn=1;
		setBit->pageStringDetector(strArray);  
		//WriteImageData(setBit->drawData,1);
		//  ДЛЯ ПОКАЗА КАРТИНКИ
		//if(CheckBoxTIFF->Checked==true){ext=".tif";}
		//if(AutoRemoveBorderBtn->Checked==true){
		string st=inputData->data["inputFile"];
		string name=st.erase(0,st.rfind('/'));
		string ext=name;
		ext=ext.erase(0,ext.rfind('.'));
		name=name.erase(name.size()-ext.size(),ext.size());
		st=inputData->data["outFolder"];
		st+="/"; st+=name; st+="_0"; st+=ext;   //cout<<" st"<<st<<END;
		WriteSpreadData(pechaImg_,setBit,-1, st); // , strArray
		st=inputData->data["outFolder"];
		st+="/"; st+=name; st+="_1"; st+=ext;  // cout<<" st"<<st<<END;
		WriteSpreadData(pechaImg_,setBit,-2, st); // , strArray
		
		setBit->destroy();
		delete setBit;
		
	}
	//cout<<"inputData->data[\"rotationBox\"]"<<inputData->data["rotationBox"]<<END;	
	
	if(inputData->data["rotationBox"]=="autoRotate"){
		
		cout<<"autoRotate pechaImg@="<<pechaImg_->columns()<<END;
		//GBitset *set=GBitset::createResize(pechaImg_,1,1,1); cout<<"@1"; 
		//double angle=set->detectRotation(); angle*=-1;  cout<<"@2";
		pechaImg_->binarisation(80,26);
		float angle=pechaImg_->detectSkew(8);
		cout<<"angle="<<angle<<END;
		//set->destroy();
		//delete set;
		string mode="tif";
		string path=str_replace(".jpg", ".tif", inputData->data["inputFile"]);
		path=str_replace(".JPG", ".tif", path);
		path=str_replace(".jpg", ".tif", path);
		cout<<" path="<<path<<endl;
		
		 if(fabs(angle)>0.1){
			GBitmap *pechaImg0=GBitmap::createRotate(pechaImg_, angle);
			cout<<"complete rotation"<<END;
			//if(inputData->data["binary"]=="adaptive")
			WriteImageData(pechaImg0,path,0,mode);
			pechaImg0->destroy();
			delete pechaImg0; 
		}else{
			WriteImageData(pechaImg_,path,0,mode);
		}
	}


}


void GImageEditor::pageToBase(commandData *inputData){

	readLetterBase(inputData);
	baseImg=GBitmap::create(3000,6000);
	GBitmap *img=GBitmap::create(150,150);
	int border;
	string str;
	int x0,y0,w,h,xImg=0,yImg=0;
	baseImg->fill(0);
	
	if(!inputData->fileList.size()){
		readDirectoryToArray(inputData->fileList,inputData->data["inputFolder"],"img");
	}

	for(int i=0;i<inputData->fileList.size();i++){
		str=inputData->fileList[i]+".html";   cout<<str<<END;
		inputData->data["inputFile"]=inputData->fileList[i];
		inputData->data["inputFileName"]=inputData->data["siteName"];
		inputData->data["inputFileName"]+=substr(inputData->data["siteRoot"].size(),inputData->data["inputFile"]);
		//readPageHTML(inputData);
		string str=inputData->data["inputFile"];
		pechaImg=LoadImageData(str,1);
		vector<stringOCR>strArray;
		setBit=GBitset::createResize(pechaImg,1,1,1);
				  setBit->pageStringDetector(strArray); // Подпрограмма выделения строк и букв ПЕЧА ( РЕЛСЫ )
				  border=setBit->border_size();
							GBitsetOCR *setOCR=GBitsetOCR::createResize(pechaImg,1,1);
							        setOCR->setData(inputData,
											&aliKali,
											strArray,
											correctionTable,
											logicProcessor,
											iLeft,
											iTop,
											border,
											ALL_LETTER);
							  mainString=setOCR->mainString;

			  for (int index=0;index<strArray.size();index++){
				  for (int in=0;in<strArray[index].wordArray.size();in++){
					  if(strArray[index].correlation<80&&strArray[index].correlation>0){
						 img->fill(255);
						 if(yImg>39)goto drawBasePoint;
						 if(strArray[index].wordArray[in].w<5)continue;
						 if(strArray[index].wordArray[in].OCRIndex==3)continue;
						 cout<<" . ";
						 x0=strArray[index].wordArray[in].x0-70;
						 y0=strArray[index].y0-50;
						 //LoadImageData(img,pechaImg,x0,y0, 150-1, 150-1); //deprecated
						 baseImg->drawImg(img,xImg*150,yImg*150);
						 xImg++;  if(xImg==20){xImg=0;yImg++;}
					  }
				  }
			  }

			  if(setBit!=NULL){
				setBit->destroy();
				delete setBit;
			  }
			  if(setOCR!=NULL){
				  //inputData->c_out<<"</div></body></html>";
				drawStrArray(inputData,strArray,border);
			    setOCR->destroy();
				delete setOCR;
				//drawLettersInCorrectionTable(inputData);
				  
				strArray.resize(0);
			  }

			  pechaImg->destroy();

	}
	drawBasePoint:;
	str=inputData->data["inputFolder"]+"/__letterBase.jpg";
	WriteImageData(baseImg,str,0);

}//____________________________________________________________________________


string GImageEditor::readInputAction(commandData *inputData){
	string destLine;
	GLogicProcessor *logicProcessor=(GLogicProcessor*)inputData->logicProcessor;
	int print=1;

	string str=readText("input.xml");
	str=decodeURLString(str);
	DT("INPUT str="<<str<<endl);
	readInput(inputData,str);
	//if(!inputData->fileList.size())return ""; ??

	DT("REZULT inputData->data[ocrData]="<<inputData->data["ocrData"]<<endl);
	
	if(inputData->data["ocrData"]=="setBase"){
		cout<<"INPUT set base"<<inputData->data["setBase"]<<endl;
		setBase(inputData);
	}
	if(inputData->data["ocrData"]=="setLetter"){
		cout<<"INPUT set letter"<<inputData->data["setLetter"]<<endl;
		setLetter(inputData);
	}
	if(inputData->data["ocrData"]=="saveBase"){
		writeLetterBase(inputData);
	}
	if(inputData->data["ocrData"]=="openLetterBase"){
		str="/cgi/OCRTables/OCRBase.html"; 
		destLine=readText(str);  //cout<<"destLine="<<destLine<<endl;
	}
	if(inputData->data["ocrData"]=="ocrBatch"){
		//str="/cgi/OCRTables/OCRBase.html"; 
		//destLine=readText(str);  //cout<<"destLine="<<destLine<<endl;
		startOCRBatch(inputData);
		destLine="Start OCR";
	}
	if(inputData->data["ocrData"]=="letterBlock"){
		//str="/cgi/OCRTables/OCRBase.html"; 
		//destLine=readText(str);  //cout<<"destLine="<<destLine<<endl;
		startOCRBatch(inputData);
		destLine="Start OCR";
	}
	if(inputData->data["ocrData"]=="buildLetter"){
		//str="/cgi/OCRTables/OCRBase.html"; 
		//destLine=readText(str);  //cout<<"destLine="<<destLine<<endl;
		buildLetter(inputData);
		destLine="Start OCR";
	}
	
	if(inputData->data["ocrData"]=="tib"){
		if(inputData->fileList.size()){
			inputData->data["inputFile"]=inputData->fileList[0];
			str=inputData->data["inputFile"];
			str=substr(0,str.rfind("."),str);
			str+=".html";   cout<<str<<END;
			inputData->c_out.open(str.c_str());
			if(!logicProcessor->dictionaryReady)logicProcessor->readDictionary(inputData);
			startOCR(inputData);
			inputData->c_out.close();
			destLine="Start OCR";
		}else{
		    destLine="Open file for OCR";
		}
	}
	if(inputData->data["ocrData"]=="tibLetters"){
		if(inputData->fileList.size()){
			inputData->data["inputFile"]=inputData->fileList[0];
			str=inputData->data["inputFile"];
			str=substr(0,str.rfind("."),str);
			str+=".html";   cout<<str<<END;
			inputData->c_out.open(str.c_str());
			if(!logicProcessor->dictionaryReady)logicProcessor->readDictionary(inputData);
			startOCR(inputData);
			inputData->c_out.close();
			destLine="Start OCR";
		}else{
		    destLine="Open file for OCR";
		}
	}
	if(inputData->data["ocrData"]=="ImageProcess"){
		if(inputData->fileList.size()){
			inputData->data["inputFile"]=inputData->fileList[0];
			imageProcess(inputData);
			destLine="Start Image Process";
		}else{
		    destLine="Open file for Image Process";
		}
	}
	if(inputData->data["ocrData"]=="removeLetter"){
		DT("INPUT removeLetter"<<inputData->data["setBase"]<<endl);
		setBase(inputData);
		//string data="";
		//destLine=drawBase(inputData,data);
	}
	if(inputData->data["ocrData"]=="testBase"){
          testBase(inputData);
	}

	if(inputData->data["ocrData"]=="UTF8"){
	   DT("INPUT UTF8"<<inputData->data["ocrData"]<<endl);
	   inputData->data["inputFile"]=inputData->fileList[0];
	   destLine=logicProcessor->startConvert(inputData);
	}
	if(inputData->data["ocrData"]=="RTFToYagpo"){
		DT("RTFToYagpo"<<inputData->data["ocrData"]<<endl);
		inputData->data["inputFile"]=inputData->fileList[0];
		destLine=logicProcessor->startConvert(inputData);
	}
	
	if(inputData->data["ocrData"]=="WylieToYagpo"){  //Wylie transliteration to Yagpo 
		inputData->data["inputFile"]=inputData->fileList[0];
		inputData->data["InputMethod"]="fileList";
		destLine=logicProcessor->startConvert(inputData);		
	}
	if(inputData->data["ocrData"]=="dWylieToYagpo"){  //delimited ༼Wylie༽ transliteration converter
		inputData->data["inputFile"]=inputData->fileList[0];
		inputData->data["InputMethod"]="fileList";
		destLine=logicProcessor->startConvert(inputData);		
	}
	if(inputData->data["ocrData"]=="dSinhalaASCIToYagpo"){  //delimited ༼Sinhala༽ transliteration converter
		inputData->data["inputFile"]=inputData->fileList[0];
		inputData->data["InputMethod"]="fileList";
		destLine=logicProcessor->startConvert(inputData);		
	}
	if(inputData->data["ocrData"]=="SinhalaMettaToYagpo"){  //delimited ༼Sinhala༽ transliteration converter
		inputData->data["inputFile"]=inputData->fileList[0];
		inputData->data["InputMethod"]="fileList";
		destLine=logicProcessor->startConvert(inputData);		
	}

	if(inputData->data["ocrData"]=="YagpoToUnicode"){  //Yagpo To Unicode converter
		inputData->data["inputFile"]=inputData->fileList[0];
		inputData->data["InputMethod"]="fileList";
		destLine=logicProcessor->startConvert(inputData);		
	}
	if(inputData->data["ocrData"]=="buildDict"){  //UnicodeToYagpo converter
		inputData->data["inputFile"]=inputData->fileList[0];
		inputData->data["InputMethod"]="fileList";
		destLine=logicProcessor->startDictionary(inputData);		
	}
	if(inputData->data["ocrData"]=="buildHashDict"){  //UnicodeToYagpo converter
		inputData->data["inputFile"]=inputData->fileList[0];
		inputData->data["InputMethod"]="fileList";
		destLine=logicProcessor->startDictionary(inputData);		
	}
	return destLine;
	
}//___________________________________________________________________________


string GImageEditor::letterBlockCorrelation(commandData *inputData,unsigned int in){
   
	cout<<"letterBlockCorrelation"<<endl;
	
	int indexLetter=indexById(correctionTable,in);
	//if(pechaDataLoaded)pechaImg->destroy();
    //pechaImg=	
	vector<stringOCR>strArray;
	string str;
	if(correctionTable[indexLetter].img->TIFF_string.size()){
		correctionTable[indexLetter].img->unpackImg(0);
	}	

	GBitset *setBit=GBitset::createResize(correctionTable[indexLetter].img,1,1,1);
	cout<<"@4_2";
	setBit->pageStringDetector(strArray); // Подпрограмма выделения строк и букв ПЕЧА ( РЕЛСЫ )  
	cout<<"@4_3";
	if(strArray.size()>1)strArray[0].wordArray.resize(0);
	
	cout<<" setBit->border_size()="<<setBit->border_size()<<endl;
	
	
	if(aliKali.letterCount==0)readLetterBase(inputData); //cout<<"aliKali1.base.size()="<<aliKali.base.size()<<endl;
	GBitsetOCR *setOCR=GBitsetOCR::createResize(correctionTable[indexLetter].img,1,1);
	setOCR->setData(inputData,
					&aliKali,
					strArray,
					correctionTable,
					logicProcessor,
					0,
					0,
					setBit->border_size(),
					ALL_LETTER);
	
	mainString=setOCR->mainString;
	cout<<mainString<<"strArray="<<strArray[0].wordArray.size();
	
str="/__1draw.jpg";
#ifdef DEBUGLVL_RGB						
	if(setOCR->setMatrix!=NULL)WriteImageData(setOCR->setMatrix->drawDataRGB,str,0);
#endif
#ifdef DEBUGLVL_GREY						
	if(setOCR->setMatrix!=NULL)WriteImageData(setOCR->setMatrix->drawData,str,0);
#endif
	
	
	ostringstream c_out1;
    int x0,y0,x1,y1,gY,index=0;
	
	// заполнение массива координат прямоугольника
	for (int strInd=strArray.size()-1; strInd >=0; strInd-- ){
		x0=strArray[strInd].x0;   x1=strArray[strInd].x1;
		y0=strArray[strInd].y0;   y1=strArray[strInd].y1;
		c_out1<<"<div class=\"sT\" id=\"s"<<strInd<<"\" style=\"left:";
		c_out1<<x0<<"px; top:"<<y0<<"px; width:"<<x1-x0<<"px; height:";
		c_out1<<y1-y0<<"px;\" ";
		c_out1<<"LimY0=\""<<strArray[strInd].LimY0<<"\" ";
		c_out1<<"LimY1=\""<<strArray[strInd].LimY0<<"\" ";
		c_out1<<"y0=\""<<strArray[strInd].y0<<"\" ";
		c_out1<<"y1=\""<<strArray[strInd].y1<<"\" ";
		c_out1<<"x1=\""<<strArray[strInd].x1<<"\" ></div>\n";
		
		for (int m=0; m < strArray[strInd].wordArray.size(); m++ ){
			wordOCR *wP=&strArray[strInd].wordArray[m];
			if(wP->correlation<0)continue;  //word marked as text frame etc. 
			x0=wP->x0;   x1=wP->x1;   y0=wP->y0;   y1=wP->y1;
			//gY=strArray[strInd].y1-y0+10;
			gY=0;
			//DM("y0="<<y0<<" y1="<<y1<<" x0="<<x0<<" x1="<<x1<<" LimY0="<<strArray[strInd].LimY0<<" LimY1="<<strArray[strInd].LimY1<<END);
			if(x1-x0>0){
				c_out1<<"<div class=\"dE\" id=\"g"<<index<<"\" c=\""<<wP->correlation;
				c_out1<<"\" style=\"left:"<<x0<<"px; top:"<<y0<<"px; width:"<<x1-x0<<"px; height:";
				c_out1<<y1-y0<<"px; ";
				if(wP->correlation<70){c_out1<<" border-color:#ff0000; border-width:3px;";}
				if(wP->correlation>70&&wP->correlation<80){c_out1<<" border-color:#cc0033; border-width:2px;";}
				
				c_out1<<"\">\n<div class=\"dM\" style=\"width: "<<x1-x0<<"px; height:3px;\"></div>\n";
				//c_out1<<wP->OCRIndex;
				c_out1<<"<div class=\"nG\" id=\""<<index<<"\" style=\" top:"<<gY<<"px\" onClick=\"edit(";
				c_out1<<index<<")\" onBlur=\"setText("<<index<<")\">"<<wP->name<<"</div></div>";
				index++;
			}
			DT("n="<<wP->name.c_str()<<" c="<<wP->correlation<<END);
			
		} // m
		
	} // strInd
	
	str=c_out1.str();
	
	string destStr=drawEditLetterBlock(inputData,str,in);
	return destStr;

}


















