/** This function build letter base from text file of alphabet
*/ 
void GImageEditor::buildLetter(commandData *inputData){
	if(!inputData->fileList.size())return;
	string data=readText(inputData->fileList[0]);
	if(!data.size())return;
    //cout<<"data="<<data<<endl;
	data=str_replace("\r", "", data);
	vector<string>fontData=explode("\n",data);
	string fontName=fontData[0];
	//cout<<"font="<<fontName;

#ifdef MACOS
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	short index;
	short wScale=400;//[recordField intValue]*3; cout <<"Start font table building. wScale="<<wScale<<endl;
	//if(wScale<90)wScale=90;
	unsigned char *p1;
	int y0=0, y1=0;  //text line coordinate
	NSImage *stringOCRImage;
	fontName=str_replace("\r","", fontName);
	NSString *fontNameStr=[[NSString alloc] initWithUTF8String:fontName.c_str()]; NSLog(fontNameStr);
	NSMutableDictionary *attrs = [NSMutableDictionary dictionary];
	[attrs setObject:[NSColor blackColor] forKey: NSForegroundColorAttributeName];
	[attrs setObject:[NSFont fontWithName:fontNameStr size:wScale/6] forKey: NSFontAttributeName];
	
	stringOCRImage = [[NSImage alloc]initWithSize:NSMakeSize(wScale*4, wScale)];
    fontData[0]="";
	string textLine=implode("",fontData);
		
	[stringOCRImage lockFocus];
		[[NSColor whiteColor] set];
		[NSBezierPath fillRect:NSMakeRect(0, 0, wScale*4,wScale)];
		[[NSColor blackColor] set];
		NSString *str=[[NSString alloc] initWithUTF8String:textLine.c_str()];
		[str  drawAtPoint: NSMakePoint(wScale/3,wScale/4) withAttributes: attrs];	
	[stringOCRImage unlockFocus];
	
	NSBitmapImageRep *srcImageRep = [NSBitmapImageRep imageRepWithData:[stringOCRImage TIFFRepresentation]]; 
	unsigned char *srcData = [srcImageRep bitmapData];
	
	GBitmap *textLineImg=GBitmap::create(wScale*4,wScale); 
	for(int x = 0; x <wScale*4; x++) {
		for(int y = 1; y< wScale; y++){
			p1=srcData+(y)*wScale*16+x*4;
			textLineImg[0][y][x]=*p1;
			//if(bSet[0][y][x]==255){cout<<"0";}else{cout<<"1";}//draw on screen
		} //cout<<endl;
	}
	[srcImageRep release];
	[stringOCRImage release];
	[str release];
	WriteImageData(textLineImg,"/_1.jpg",0); //exit(0);
	///set line coordinates in page	
    GBitset *setBit=GBitset::createResize(textLineImg,1,1,1);
	cout<<"@4_2";
	vector<stringOCR>strArray; 
	setBit->pageStringDetector(strArray); // Подпрограмма выделения строк и букв ПЕЧА ( РЕЛСЫ )  
	cout<<"@4_3 strArray.size()="<<strArray.size()<<endl;
	int border=setBit->border_size();
	setBit->destroy();
	if(strArray.size()){
		y0=strArray[0].y0;
		y1=strArray[0].y1;
	}	
    if(y1-y0<0){cout<<"no string"; return;}
	
	stringOCRImage = [[NSImage alloc]initWithSize:NSMakeSize(wScale, wScale)];
	
	GBitset *set;
	
	for(index=1;index<fontData.size()&&index<2000;index++){   ///// for every string we build OCR Table Record
		
			cout<<"index="<<index<<endl;
			if(fontData[index]=="")continue;
			[stringOCRImage lockFocus];
			
			[[NSColor whiteColor] set];
			[NSBezierPath fillRect:NSMakeRect(0, 0, wScale,wScale)];
			[[NSColor blackColor] set];
			str=[[NSString alloc] initWithUTF8String:fontData[index].c_str()];
			[str  drawAtPoint: NSMakePoint(wScale/3,wScale/4) withAttributes: attrs];	
			
			[stringOCRImage unlockFocus];
			
			srcImageRep = [NSBitmapImageRep imageRepWithData:[stringOCRImage TIFFRepresentation]]; 
			unsigned char *srcData = [srcImageRep bitmapData];
			
			GBitmap *bMap=GBitmap::create(wScale,wScale); 
			for(int x = 0; x <wScale; x++) {
				for(int y = 1; y< wScale; y++){
					p1=srcData+(y)*wScale*4+x*4;
					bMap[0][y][x]=*p1;
					//if(bSet[0][y][x]==255){cout<<"0";}else{cout<<"1";}//draw on screen
				} //cout<<endl;
			}
		    int x0_,x1_,y0_,y1_;
			GBitmap *img128=bMap->detectRegion(5,&x0_,&x1_,&y0_,&y1_);
			WriteImageData(img128,"/_1.jpg",0); //exit(0);
		    
		    cout<<"y0="<<y0<<" y1="<<y1<<"y0_="<<y0_<<" y1_="<<y1_<<" bMap->rows()="<<bMap->rows()<<endl;
		    y1_=y1-y0_;
		    y0_=y0-y0_;
		
			short letterIndex;			//letterIndex=letterBase[letterBase.size()-1]->contourIndex;
			
			GBitsetContour *vSet=GBitsetContour::createResize(img128,1,1,0);
		    aliKali.letterCount++;
		    glyphOCR *letter=aliKali[aliKali.letterCount-1];
			vSet->textLineSize=(y1-y0);
			letter->textLineSize=(y1-y0);
			
			vectorBase[0].allVectorCount=0;
			vSet->setFontData(vectorBase,letter,fontData[index],wScale);
			cout<<"vectorBase[0].allVectorCount="<<vectorBase[0].allVectorCount<<endl;

			letter->maskFromVector();
			cout<<"m1";
			string draw=letter->drawToSVG();
			writeText(draw,"/1.svg");
			
			//WriteImageData(vSet->drawData, "/_112.jpg",0);
			//_//_DM("DONE1");
			
		
			//cout<<"START_PACK img128.columns()="<<img128->columns()<<END;
			letter->mask128[0].packBitMask128(img128);  
			//glyphNew.mask128[0].printMask();
			//exit(0);
			letter->mask128[0].xMask=-(letter->mask128[0].mW/2);
			cout<<"m1_1";
			//normalisation on slice size
			int s= img128->columns(); if(s>128)s=128;
			int dl=letter->mask128[0].mW-s;
			letter->xMin=500;
			letter->xMax=0;
			for(int a=0;a<letter->mask32Count;a++){
				letter->mask32[a].xMask-=dl/2;
				if(letter->mask32[a].xMask<letter->xMin)letter->xMin=letter->mask32[a].xMask;
				if(letter->mask32[a].xMask+letter->mask32[a].imgW>letter->xMax)letter->xMax=letter->mask32[a].xMask+letter->mask32[a].imgW;
			}
			//cout<<" letter->xMin="<<letter->xMin<<endl;
			letter->dX=-(letter->mask128[0].mW/2-letter->letterW/2-(letter->mask128[0].mW/2+letter->xMin));
			letter->dY=-(letter->mask128[0].mH/2-letter->letterH/2-(letter->mask128[0].mH/2+letter->yMin));
			
			letter->mask128[0].yMask=-(letter->mask128[0].mH/2);
			letter->mask128Count=1;
			letter->idNumber=letter->mask128[0].hashValue();
			cout<<"m1_2";
			letter->reloadMask();
		    
		    letter->y0=y0_;
			letter->y0=-(letter->mask128[0].mH/2-letter->y0);
			letter->y1=y1_;
		    letter->y1-=letter->mask128[0].mH/2;
		
		    cout<<"m1_3";
		    //cout<<" allVectorCount in letter "<<set->vectorCount<<endl;
			vSet->destroy();
			bMap->destroy();
			img128->destroy();
		    [str release];
		    [srcImageRep release];
			cout<<"m2";
			//if(index==0){	
				//NSData *TIFFData = [stringOCRImage TIFFRepresentation];   //write file on disk
				//[[stringOCRImage TIFFRepresentation] writeToFile:@"/_1111.tiff" atomically:YES];
			//}
			//cout<<"main index="<<index<<endl;
	}//for(index=0;index<[f
	
	
	//set textLineSize in base
	aliKali.textLineSize=0;
	for(int i=0;i<aliKali.letterCount;i++){
		aliKali.textLineSize+=aliKali[i]->y1-aliKali[i]->y0;
	}aliKali.textLineSize/=aliKali.letterCount;
	
	
	cout<<"m3";
	
	//[pool release];
	
	cout<<"m4";
	
#endif	
	
	
	//writeLetterBase(inputData);
	
	
}//__________________________________________________________________________










