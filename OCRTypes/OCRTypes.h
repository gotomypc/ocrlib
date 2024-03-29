#ifndef HEADER_OCRTYPES
#define HEADER_OCRTYPES
//__OCR CLASSES_____________________________________
#include "config.h"
#include<string>
#include<vector>
#include<list>
#include<map>
#include "GBitmask/GBitmask.h"

using namespace std;
namespace ocr {


	const short cLimit=10;
	const short deltaLimit=13;
	const short Radius=15;
	const short kShapeCassidy=10;


	class OCRPoint {
	public:
		OCRPoint();
		short x;
		short y;
		char status;
	};

				// -- A run of pixels with the same color
struct pointLine
{
  int y;       // vertical coordinate slice
  int x1;      // first horizontal coordinate slice
  int x2;      // last horizontal coordinate slice
};


struct commandData
{
 vector<string> fileList;
 vector<string> folderList;		
 map<string,string>data;
 vector<string>fileSet;
 vector<int>lineSize;
 int imgW,imgH;
 int start;
 unsigned int idNumber;
 int x0,y0,x1,y1;
 ofstream c_out;
 ofstream stat_out;
 //Handlers for GUI
 int socketFD;  //socket file descriptor
 void *imageEditor;
 void *logicProcessor;
};




	class TsertoOCR {
	public:
		TsertoOCR();
		int y;   // vertical coordinate point
		int x0;  // first horizontal coordinate point
		int x1;  // last horizontal coordinate point

		int x;   // horizontal coordinate point
		int y0;  // first vertical coordinate point
		int y1;  // last vertical coordinate point

		int w;
		int h;

		vector<pointLine> line;
 //		char status;
	};

	class OCRMatch{
	public:
		OCRMatch(void);
		OCRPoint start;
		OCRPoint end;
		int matchCount; //count for mutch points in structure
		int centerDist;  //distance between centers of match vector
		//int contourIndexTest; //count for mutch points in structure
		//int contourIndexModel; //count for mutch points in structure
		int vectorIndexModel; //index of match vector in base
		int vectorIndexTest; //index of match vector in base
		int allMatchCount;  //in [0] element we store count for all array of mutch
		int matchSumTest; //calculated match
		int status;
		int letterIndex;
		int correlation;
		int Character;
		string name;
		string Wylie;
		int OCRIndex;
		int maxY;
        int id;
	};


int operator==(const OCRMatch &left, const OCRMatch &right);
int operator<(const OCRMatch &left, const OCRMatch &right);

	class vectorOCR {
	protected:

	public:
		vectorOCR(void);
		virtual ~vectorOCR();
		void destroy(void);
		static vectorOCR* create(void){return new vectorOCR;}
		OCRPoint vStart;
		OCRPoint vEnd;
		short length;
		string name;
		OCRPoint vCenter;
		//vector<OCRPoint>points;  //count of points in vector
		short matchSum;
		short vectorAngle;
		short vectorInd;
		short selectFlag;
		short centerDist;
		short vectorFlag;
		int contourIndex;
		int letterIndex;
		int objIndex;
		int pointCassidy;
		unsigned int allVectorCount;
		int allLetterCount;
	};
	

	class wordOCR{
	public:
		wordOCR(void);
		virtual ~wordOCR();
		void destroy(void);
		static wordOCR* create(void){return new wordOCR;}
		// wordOCR(int _x,int _y,int _x1,int _y1){x0=_x;x1=_x1;y0=_y;y1=_y1;w=_x1-_x;h=_y1-_y;};
		int x0,y0,x1,y1,xp0,yp0,xp1,yp1,xp,yp,xt0,yt0,xt1,yt1,w,h;
		int xv0,yv0,xv1,yv1;
		OCRPoint center;
		string name;
		string Wylie;
		string delimeter;
		string oldName;
		string  newLine;
		string file;
		string commentaryIndex;
		int sillablyCount;
		int letterIndex;
		vector<OCRMatch> match,match1;
		vector<TsertoOCR> TLline;
		GBitmap *img;  ///bitmap image from origimal text
		int correlation;
		int tsertoFlag;
		int spaceFlag;
		int stackFlag;
		int vowelFlag;
		int Param1,Param2;
		int selectFlag;
		int blockIndex;
		int OCRIndex;
		int fontSize;
		int checkFlag;   //flag of cheking by correction table 
		int imgFlag;
		int id;

                  // x0, x1,  y0, y1   координаты квадрата вокруг буквы
		  // xp0,xp1, yp0,yp1  координаты прямоугольника вокруг точки
		  // xt0,yt0, xt1,yt1  координаты прямоугольника вокруг стека
		  // xp,yp             координаты точки
		  // tsegFlag  флаг наличия точки в прямоугольнике
		  // spaceFlag флаг наличия пробела после точки
		  // stackFlag флаг наличия стека вместо коренной буквы.
		  // vowelFlag флаг наличия огласовки над коренной буквой.

		  //wordOCR& operator=(wordOCR &st){
		  // x0=st.x0;x1=st.x1;y0=st.y0;y1=st.y1;w=st.w;h=st.h;
		  //}
		  //int operator==(wordOCR &wt){
		  // if(x0==wt.x0&&x1==wt.x1&&y0==wt.y0&&y1==wt.y1&&name==wt.name) return 1;  //&&w==wt.w&&h==wt.h&&
		  // return 0;
		  //}

		  bool operator<(wordOCR& right){
			return correlation < right.correlation;
		  };
		  bool  operator==(vector<string> &ref);
		  bool testDelimeter(vector<string> &ref);

	};//_________________________________________________

	struct wordOCR_sort {
		bool operator ()(wordOCR const& a, wordOCR const& b) const {
			string s1,s2;
			s1=a.name+a.delimeter+a.commentaryIndex;
			s2=b.name+b.delimeter+b.commentaryIndex;
			if (s1 < s2) return true;
			if (s1 > s2) return false;
			return false;
		}
	};
	
	
	class blockOCR{
	public:
		blockOCR(void);
		virtual ~blockOCR();
		void destroy(void);
        
			int xb0,yb0,xb1,yb1;
			int blockFlag;
			int langFlag;

		  // xb0,yb0, xb1,yb1  координаты прямоугольника вокруг блоков букв
		  // blockFlag флаг принадлежности к колонке(значение флага-номер колонки)
		  // langFlag  флаг языка 0 не определено, 1 tib, 2 en.


	};//_________________________________________________
/**/
	class lineOCR{
	public:
        lineOCR(void);
       	virtual ~lineOCR();
		void destroy(void);
			int xL0,yL0,xL1,yL1;
			int lineFlag;

			// xL0,yL0, xL1,yL1  // координаты прямоугольников горизонтальных
			// блоков L, описывающих вертикальные колоноки в структурах строк.
			// lineFlag флаг принадлежности к колонке (значение флага-номер колонки)
			// флаг 0 не определено. 1,2,3,...  - номер колонки

	};//_________________________________________________
/**/


class stringOCR{
public:
	stringOCR(void);
	virtual ~stringOCR();
	void destroy(void);
	stringOCR(int _x,int _y,int _x1,int _y1){
		 x0=_x;x1=_x1;y0=_y;y1=_y1;w=_x1-_x;h=_y1-_y;
	};
		int x0,y0,x1,y1,w,h;
		int stringFlag;	/// флаг cтроки  =0 (=1), cтрока состоит из линии (букв).  //mark flag for pecha border and empty string
		int NLetter;    /// примерная длинна строки (оценка)   //letter count
		int LimY0;      /// верхний лимит строки               //upper y limit
		int LimY1;      /// нижний лимит строки                //doun y limit
		int MeanSA;     /// средее, по всей строке, расстояние между центрами масс букв  //middle letter space
		int vLim;       /// нижний лимит длинны вектора для данной строки //vector length limit for string
	    unsigned int idNumber; //uniq id number of string
		vector<int> letter;
		short count;
		string name;
	    string str; 
	    string strHTML;
	    string delimeter;
		string oldName;
	    string file;
		int selectFlag;
		int imgFlag;
	    int syllableCount;
	    int correlation; 
	    GBitmap *img;  ///bitmap image of string from origimal text
	    string path; //original text filename

		int operator==(stringOCR &wt){
			if(x0==wt.x0&&x1==wt.x1&&y0==wt.y0&&y1==wt.y1&&name==wt.name) return 1;  //&&w==wt.w&&h==wt.h&&
			return 0;
		}
		vector <wordOCR>wordArray;     //main point. here is information about every letter correlation
		vector <blockOCR>blockArray;   //language segmentetion
		vector <lineOCR>lineArray;      //maket segmentation
		int top;
	
	int length();
	int size();
	void push_back(const wordOCR &word);
	int find(const string& match,int position);
	void resize(int);
	stringOCR substr(int position,int length);
	string set_id(string &srcStr);  ///reconstract id number in source string from wordArray
	/** Return pointer on index element of wordArray
	*/ 
	wordOCR*  operator[](int index);
	void  operator+=(stringOCR& ref);
	void  operator+=(wordOCR ref);

};//________________________________________________

	struct stringOCR_sort {
		bool operator ()(stringOCR const& a, stringOCR const& b) const {
			if (a.str < b.str) return true;
			if (a.str > b.str) return false;
			return false;
		}
	};
	


	class columnOCR{
	public:
		columnOCR(void);
		virtual ~columnOCR();
		void destroy(void);
		//static columnOCR* create(void){return new columnOCR;}

			int xc0,yc0,xc1,yc1;
			int Xc0,Yc0,Xc1,Yc1;
			int columnFlag;
		  // xc0,xc1, yc0,yc1  координаты блоков, вокруг которых описан
		  // прямоугольник вертикальной колонки.
		  // Xc0,Xc1, Yc0,Yc1  тоже что и  xc0,xc1, yc0,yc1 только
		  // не вычитается border ( в функции data_normalisation ).
		  // columnFlag флаг горизонтальных блоков
		  // columnFlag флаг принадлежности к колонке (значение флага-номер колонки)
		  // 0 не определено. 1, 2, 3,... номера вертикальных колонок на развороте.

	};//_________________________________________________
/**/

	class recordOCR{
	public:
		recordOCR(void);
		virtual ~recordOCR();
		void destroy(void);
		//static recordOCR* create(void){return new recordOCR;}

			int xr0,xr1;

		  // xc0,xc1  координаты блоков, вокруг которых описан
		  // прямоугольник вертикальной колонки. Тоже что и vector <recordOCR>

	};//_________________________________________________
/**/

class frameOCR{
public:
	frameOCR(void);
	virtual ~frameOCR();
	void destroy(void);
	static frameOCR* create(void){return new frameOCR;}

		int xf0,yf0,xf1,yf1;
		int Xf0,Yf0,Xf1,Yf1;

		int xfg0,yfg0,xfg1,yfg1;
		int Xfg0,Yfg0,Xfg1,Yfg1;

		int xbr0,xbr1,ybr0,ybr1;
		int frameFlag;

		vector <columnOCR>columnArray;
		vector <recordOCR>recordArray;

		// xf0,xf1, yf0,yf1;  координаты прямоугольника вокруг многоугольника
		// вертикальной колонки.
		// Xf0,Xf1, Yf0,Yf1;  тоже что и  xf0,xf1, yf0,yf1; только
		// не вычитается border ( в функции data_normalisation ).

		// xfg0,xfg1, yfg0,yfg1;  координаты прямоугольника (габариты) вокруг
		// многоугольника вертикальной колонки (с добавлением бордюра вокруг блока текста).
		// Xfg0,Xfg1, Yfg0,Yfg1;  тоже что и  xfg0,xfg1, yfg0,yfg1 только
		// не вычитается border ( в функции data_normalisation ).

		// xbr0,xbr1,ybr0,ybr1; зазор (бордюр) между многоугольниками вертикальной колонки.
		// frameFlag  флаг номера колонки (значение флага - номер колонки)
		// 0 не определено. 1, 2, 3,... номера вертикальных колонок на развороте.
};//________________________________________________

	
	
	class OCRTextData {
	public:
		OCRTextData();
		short middleH;
		short w;
		short h;
		short deltaX;
		short lineCount;
		short direction;
		short tLine[1024];
	};
	
	struct dictKey {
		string key;
		string tibKey;
		string tib;
		string srcKey;
		string destKey;
		string dict;
		string ln;
		string grm;
		string context;
		int level;
		int flag;
		int id;
	};

	class OCRDictRecord {
	public:
		OCRDictRecord();
		//char c[4048];
		//string tibKey;
		//string tibWord;
		//string engKey;
	   //	string sncKey;
		//string rusKey;
	   //	string plKey;
	  //	string gmKey;
	          //string chKey;
	          //string esKey;
		string eentry;
		string rentry;
		string example;
		string exp;
		//string Wylie;
		//string rus_translit;
		string dict;
		string searchString;
		int recordFlag;
		map<string,dictKey>key;
		vector <dictKey>reportKey;
		int wordCount;
		int wordID;
	};

	
class drawDataRecord {
public:
	drawDataRecord();
	vector<int>color;
	vector<vector<OCRPoint> >data;
	short count;
};

class GTextBufer {
public:
		GTextBufer();
		long size;
		char data;
};
ostream&operator<<(ostream& s, GTextBufer z);
	
	/**
	 main structure for all codepage conversion functions
	 all data store in UTF-8 format
	 */
	typedef struct uniLetterRecord{
		map<string,string> key; //main map for store all string value
		vector<short> uniKey;  // unicode for typing
		vector<short> uniYagpo;  // unicode Yagpo
		vector<string> uniHexCode; //self Unicode in Hex in base Tibetan
		vector<string> uniHexKey;  // unicode for typing in Hex
		string utfTibKey;   //UTF in the codepage of standart Uni driver
		string utfYagpoTibKey;   //letter typing order in the Yagpo typing driver
		vector<string> uniHexTibetan;  // unicode in Hex for Tibetan letter
		vector<string> uniTibYagpo;   //unicode in the codepage of standart Uni driver
		string Wylie;
		string OCRKey; //self letter code in UTF
		string OCRKeyHex; //self letter code in hex
		string letterUTF;  //letter in Yagpo code in UTF
		string letterUTFLowerCase;  //letter in UTF in lowerCase form
		string keyUTF;  //letter in TibetanUni in UTF
		string translit;
		int stackFlag;
		int OCRIndex;
		int baseLetter;
		string lang;
	}uniRecord;

	typedef struct rootLetterOCRStruct{
		string letter;
		vector<short> lj;  //joined leter
		vector<short> lsj;  //subjoined leter
		vector<short> lssj;  //sub subjoined leter
		vector<short> lm;  //modificator
		vector<short> lbase;  //valid (2)letters
		vector<short> lbase1;  //valid (3)letters
		vector<short> lbase2;  //valid (4)letters
		vector<short> uniHexCode; //self Unicode in Hex
		vector<short> uniHexKey;  // unicode for typing in Hex
	}rootLetterOCR;
	
	
	typedef struct fontOCRMap{
		vector<uniRecord> fMap;
		string fontName;
	}fontMap;
	
	typedef struct stringMatch_{
		int matchIndex;
		int startIndex;
		int endIndex;
		int endIndexSrc;
		int count;
		int lenSum;
	}stringMatch;

	typedef struct stringEntry_{
		char *keyPtr;
		int  keySize;
		char *strPtr;
		int  strSize;
	}stringEntry;	
	
	typedef struct hashRecord_{
        unsigned int hash;
		unsigned int adress;
		bool operator < (const hashRecord_ c) const{      
			return (this->hash<c.hash);
		}
		bool operator == (const hashRecord_ c) const
		{      
			return (this->hash==c.hash);
		}
	}hashRecord;	
	
	

        enum  stackFlag{
                VOWEL = 0,
                ROOT_LETTER = 1,
                ROOT_LETTER_VOWEL = 3,
                ROOT_LETTER_DOWN_VOWEL = 5,
                ROOT_LETTER_UP_DOWN_VOWEL = 6,
                STACK_2LETTER = 7,
                STACK_2LETTER_VOWEL = 9,
                STACK_2LETTER_DOWN_VOWEL = 11,
                STACK_2LETTER_UP_DOWN_VOWEL = 12,
                STACK_3LETTER = 13,   //:)
                STACK_3LETTER_VOWEL = 15,
                STACK_3LETTER_DOWN_VOWEL = 17,
                STACK_3LETTER_UP_DOWN_VOWEL = 19,
                STACK_4LETTER = 21,
                STACK_4LETTER_VOWEL = 23,
                STACK_4LETTER_DOWN_VOWEL = 25,
				STACK_4LETTER_UP_DOWN_VOWEL = 27,
				letterKey = 0,

        };
 	
		//константы для функций обработки изображений
				enum drawConst{
				IMGFLIP=1,
				IMGNOFLIP=0,
				IMGRGB=1,
				IMGBW=0,
				ALL_LETTER=1,
				TEST_LETTER=2,
				NEWLETTERINBOOK=3,
				GET_LETTERS=4,
				BIGMASK=10,
				MIDDLEMASK=9,
				SMALLMASK=8,
				GIF=1,
				JPEG=2,
				LETTER_ADD=1,
				DRAW_BASE=0,
		};

		//константы для функций передачи данных

	enum dataConst{
		CLOSE_SOCKET=0,
		INIT_SOCKET=1,
		TIB_TO_ENG_UTF=2,
		TIB_FROM_HASH=3,
		REMAP_STRING_TIB=4,
		HASH_FIND=5,
		STRING_FIND=6,
		LOCAL_MODE=7,
		SOCKET_MODE=8
	};
	

		//////////////////////// ALGORITM ///////////////////////////

	
}

#endif //#ifndef HEADER_OCRTYPES


