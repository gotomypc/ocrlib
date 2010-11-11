﻿//  ----------------------------------------------------------------------------
// --- Vowel Detector --- GBitsetVowelDetector.h ---
// Подпрограмма распознования верхних огласовок в пределах строки
// Только - только начата.
//  ----------------------------------------------------------------------------

void GBitset::VowelDetector(){


TIME_START

        if( NStrok<0) { return; }

        // Запуск простой подпрограммы определения наличия верхних огласовок в пределах строки.
                      PredvVowelDetector();

/**/
     // получили маркер и координаты (в структуре) на наличие верхних огласовок.

if( GrafT[7][0] ) { DM(END); DM(77777<<" VowelDetector ");  DM(END); }

TIME_PRINT

}//_____________________________________________________________________________



//  ----------------------------------------------------------------------------
// Запуск подпрограммы предвармтельнного определения наличия верхних
// огласовок. Задача определить что в прямоугольнике заведомо что то есть
//  ----------------------------------------------------------------------------

void GBitset::PredvVowelDetector(){


  int y,x,p,d; // ,m,n
  int y0,y1,x0,x1, s0,s1,s3,s4, s5;
  int DltNstrN,DltNstrNKh,nrow;
  int NWord,res,maxres,Prg1,Prg2,LimY1,LimY0;



       // вычисление максимального размера вертикального массива BufS
       nrow=hStrMax*2+1;
       vector<int> BufW(nrow);


////////////////////////////////////////////////////////////////////////////////


   int NStrok = strArray->size();
   for ( Nstr=0; Nstr < NStrok; Nstr++ ){ // Цикл по количеству строк NStrok
       // получение оптимального коэффициента вертикальной фильтрации для каждой строки
       DltNstrN = strArray[0][Nstr].y1 - strArray[0][Nstr].y0;
       DltNstrNKh = DltNstrN*DeltaKh/100;  //=DltNstrN*ncolumns/srcW*srcH/nrows;

         NWord = strArray[0][Nstr].wordArray.size();
         for ( Nwrd=0; Nwrd < NWord; Nwrd++ ){ // Цикл по количеству квадратиков NWord

             /// получение координат прямоугольников и точек из структуры по x по y
             wordOCR *wP=&strArray[0][Nstr].wordArray[Nwrd];
             // x0,x1, y0,y1 координаты квадрата вокруг буквы
             x0 = wP->x0;    x1 = wP->x1;    y0 = wP->y0;    y1 = wP->y1;

             // получение области поиска стеков - прямоугольников
             s0=y1+(y1-y0)/8;    s1=y1+(y1-y0)*3/5;
             s3=x0+(x1-x0)/10;   s4=x1-(x1-x0)/10;

             // Lim верхний лимит строки
             LimY1=strArray[0][Nstr].LimY1;
             if ( s1 < LimY1 ) { s5=LimY1; }
             if ( s1+(s1-s0)/2 < LimY1 ) { s5=s1+(s1-s0)/2; }
             if ( s1 > LimY1 ) { s5=s0; }
             s0=s5;
/**/
             // проверка выхода области поиска стеков за граници массива
             if ( s0 <  1 ) { s0=s1=1; }    if ( s1 > nrows-2 ) { s0=s1=nrows-1; }
             if ( s3 <  1 ) { s3=s4=1; }    if ( s4 > ncolumns-2) { s3=s4=ncolumns-1; }

             // запись области поиска стеков - прямоугольников в структуру
             wP->xv0 = s3;   wP->xv1 = s4;   wP->yv0 = s0;   wP->yv1 = s1;

             // обнуление флага стека
             wP->vowelFlag=0;

             /// заполнение массова стека BufS горизонтальными суммами
             p=maxres=0;  BufW.assign(nrow,0);
             for ( y=s0; y < s1; y++ ) {        // вертикальный цикл
                 res=0; d=(y*ncolumns);
                 for ( x=s3; x < s4; x++ ) {    // горизонтальный цикл
                       res+=bits_data[d + x];
                 // визуализация (серым) области поиска стеков
#ifdef DEBUGLVL
if( GrafT[7][1] ) { drawData[0][y][x]*=0.8; }   // визуализация (серым) ////
#endif
                     } // for x
                 BufW[p]=res<<6;   p++;      // *64 копирование в BufW
                 if ( p > nrow-2 ) { p=nrow-2; }
                 // получение максимума BufW[p]
                 if( res > maxres ) { maxres=res; }
             } // for y

             /// первичный отсев прямоугольников в которых заведомо нет стеков
             /// те практически отсев пустых прямоугольников

             // для улучшения быстродействия переходим к обработке следующей буквы Nwrd
             Prg1=(s1-s0)*(s4-s3)/200 + 1; // порог по нескольким пикселам (s1-s0)/12 + 1;
             Prg2=DeltaKh*(y1-y0)/(abs(x1-x0)+1)/10; // порог по прямоугольности области поиска
             if ( maxres < Prg1 || Prg2 > 15) { goto NwrdEND; }


             /// вторичный отсев прямоугольников те выдение прямоугольников
             /// которые имеет смысл более сложно обрабатывать (сглаживание BufW)

             //  сглаживание BufW ФИЛЬТР НИЗКИХ ЧАСТОТ
			 SP->filterHigh(BufW,DltNstrN/3);   //  /2 /4


             // получение максимума сглаживанной (SP->filterHigh) функции BufW[у]
             maxres=0;
             for ( y=0; y < p; y++ ) {
                 if( BufW[y] > maxres ) { maxres=BufW[y]; }
             } // for y


             // проверка порога и установка флага в единицу
             Prg1=32*(s4-s3)/4 + 1;  // /6 порог по ширине области поиска стеков (s1-s0)*(s4-s3)/5 + 1;
             // vowelFlag флаг наличия огласовки над коренной буквой.
             if ( maxres > Prg1 ) { wP->vowelFlag=1; }

#ifdef DEBUGLVL
if( GrafT[7][2] ) {
// функции рисования вертикальных сглаженных графиков BufW точками
for (y=0; y < p; y++) { drawData[0][y+s0][(BufW[y]/32+s3)]*=0.1; }  ///
// функции рисования порога Prg1 вертикальной пунктирной чертой
for (y=0; y < p; y+=2) { drawData[0][y+s0][(Prg1/32+s3)]*=0.4; }    ///
// DM(maxres<<" maxres "); DM(Prg1<<" Prg1 "); DM(prg<<" prg ");
}
#endif
/**/
             // переход к обработке следующей буквы Nwrd  // {break;}
                                 NwrdEND:;

         } // for Nwrd  // Цикл по количеству букв Nwrd

   } // for Nstr // Цикл по количеству строк NStrok


////////////////////////////////////////////////////////////////////////////////


// x0,y0,x1,y1
// x0,y0 координаты левого нижнего угла квадратика строки (слога, буквы).
// x1,y1 координаты правого верхнего угла квадратика строки (слога, буквы).

       // получили маркер (в структуре) на наличие верхних огласовок.

/**/
       // Визуализация букв темно синими квадратиками из структур //
             if( GrafT[7][1] ) { vowelDraw(0x00AA0011 ); }

}//__________________________________________________________________________