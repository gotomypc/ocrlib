﻿//C-
//C- This software is subject to, and may be distributed under, the
//C- GNU General Public License, either Version 2 of the license,
//C- or (at your option) any later version. The license should have
//C- accompanied the software or you may obtain a copy of the license
//C- from the Free Software Foundation at http://www.fsf.org .
//C-
//C- This program is distributed in the hope that it will be useful,
//C- but WITHOUT ANY WARRANTY; without even the implied warranty of
//C- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//C- GNU General Public License for more details.
//C-

//if ( ImBuf !=NULL ) farfree(ImBuf);  // освобождение памяти
//q=(тип_q *)farmalloc(n_byte);        // запрос памяти без очистки 0
//q_new=farrealloc(q_old,n_byte);;     // изменение размера блока


#ifndef _GBITMAP_H_
#define _GBITMAP_H_
#include "config.h"
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include "libtiff.h"

using namespace std;

namespace ocr{

class  GBitmap {
protected:
  GBitmap(void);
  GBitmap(int ncolumns,int nrows);
  GBitmap(const GBitmap *ref);
  GBitmap(const GBitmap* ref, int x0,int y0, int w, int h);	
  GBitmap(const GBitmap *ref, float scale, float rotation);
  GBitmap(const char *path, int mode, int invert);
  GBitmap(string &inputData,int invert);
 public:
  virtual ~GBitmap();
  void destroy(void);
  /** @name Construction. */
  //@{
  /** Constructs an empty GBitmap object.  The returned GBitmap has zero rows
      and zero columns.  Use function \Ref{init} to change the size of the
      image. */
  static GBitmap* create(void) {return new GBitmap;}

  /**\brief Constructs a GBitmap with #nrows# rows and #ncolumns# columns.  All
      pixels are initialized to white. */
  static GBitmap* create(const int ncolumns,const int nrows)
  {return new GBitmap(ncolumns,nrows); }

  /**\brief Copy constructor. Constructs a GBitmap by replicating the size, the
      border and the contents of GBitmap #ref#. */
  static GBitmap* create(const GBitmap* ref)
  {return new GBitmap(ref);}

  /**\brief Constructs a GBitmap with region of GBitmap* ref */
  static GBitmap* createRegion(const GBitmap* ref, int x0,int y0, int w, int h)
  {return new GBitmap(ref, x0, y0, w, h); }	


  /**\brief Constructs a GBitmap by copying and scale the contents of GBitmap #ref#. */
  static GBitmap* createScale(const GBitmap* ref, const float scale)
  { return new GBitmap(ref,scale,0);}
  /**\brief Constructs a GBitmap by copying and rotate the contents of GBitmap #ref#.*/

	static GBitmap* createRotate(const GBitmap* ref, const float rotation)
  { return new GBitmap(ref,0,rotation);}

  /**\brief Constructs a GBitmap by copying, scale and rotate the contents of GBitmap #ref#.*/
	static GBitmap* createScaleRotate(const GBitmap* ref, const float scale, const float rotation)
  { return new GBitmap(ref,scale,rotation);}

  /**\brief Constructs a GBitmap by copying and unpack raw bits TIFF data*/
	static GBitmap* createFromTIFF(const char *path,int invert)
	{ return new GBitmap(path, TIFF_FILE, invert);}

 /**\brief Constructs a GBitmap by copying and unpack raw bits TIFF data*/
	static GBitmap* createFromTIFFstring(string &inputData,int invert)
	{ return new GBitmap(inputData, invert);}
	
  /** @name Initialization. */
  //@{
  /**\brief Resets this GBitmap size to #nrows# rows and #ncolumns# columns and sets
	  all pixels to white. */
  void init(int ncolumns,int nrows);
  /**\brief Initializes this GBitmap with the contents of the GBitmap #ref#.  */
  void init(const GBitmap* ref);
  /**\brief Initializes a GBitmap with region of GBitmap* ref */
  void initRegion(const GBitmap* ref, int x0,int y0, int w, int h);		
  /**\brief Initializes this GBitmap with the scale contents of the GBitmap #ref#. */
  void init(const GBitmap* ref, float scale,float rotation);
  /**\brief Initializes this GBitmap with the contents of the file with Image Types mode */	  
  void init(const char *path, int mode, int invert);
  /**\brief Initializes this GBitmap with the contents of the string contained tiff file data */	  
  void init(string &inputData,int invert);

	/** copy all the GBitmap ref pixels to destination */ 
  GBitmap* operator=(const GBitmap* ref);
  /**\brief Initializes all the GBitmap pixels to value #value#. */
  void fill(unsigned char value);
   /**\brief Initializes all the GBitmap pixels to invert value */
  void invert();
  //@}

  /** @name Accessing the pixels. */
  //@{
  /**\brief Returns the number of rows (the image height). */
  unsigned int rows() const;
  /** Returns the number of columns (the image width). */
  unsigned int columns() const;
  /**\brief Returns the size the border. */
  unsigned int borderSize() const;
  /**\brief Returns a constant pointer to the first byte of row #row#.
	  This pointer can be used as an array to read the row elements. */
  const unsigned char *operator[] (int row) const;
  /**\brief Returns a pointer to the first byte of row #row#.
	  This pointer can be used as an array to read or write the row elements. */
  unsigned char *operator[] (int row);
  /**\brief Returns the size of a row in memory (in pixels).  This number is equal
	  to the difference between pointers to pixels located in the same column
	  in consecutive rows.  This difference can be larger than the number of
	  columns in the image. */
  unsigned int rowsize() const;

  /**\brief  Предобработка битовой картинки для функция определения угла наклона текста */
  void detectContour();

  /**\brief  Убирает белый фон вокруг изображения */
  GBitmap* detectRegion(int frame,int *x0,int *x1,int *y0,int *y1);

  /**\// автоопределение количества градаций в битмапе с управляемой инверсией.
  Autodetect the number of gradations in the bitmap with a controlled inversion*/
  void AutodetectGradations(int invert);
  /**\brief  Упаковка большого битмапа в маленькй int массив. */
  void packImg32(int invert);
  /**\brief  Распаковка маленького битового массива int в большй битмап. */
  void unpackImg32(int invert);
  /**\brief  Имитация полиграфического растискивания и зашумленности */
  void dotGain(int gRateLefts, int gRateRight,
			   int gRateHighs, int gRateLower, int noiseRate);
	
	
  /**\brief   Scale GBitmap content
  масштабирование (Resample) серого (char) массива по методу "ближайших соседей" */
  void scaleFast(const GBitmap*ref, float scale);

  /**\brief  Вращение или (и) масштабирование битовой картинки */
  void rotateFast(const GBitmap*ref, float scale, float rotation);

  /**\brief функция определения угла наклона текста с помощью быстрого
	преобразования Радона аналог GBitsetRotation.h
	g параметр задающий максимально возможный определяемый угол наклона текста,
    по умолчанию g=8, с увеличением максимадьного угола наклона текста g,
    пропорционально падает быстродействие t.
    g=8 angle<7gr, t=0.125sec; g=4 angle<14gr t=0.29sec;  g=2 angle<26gr t=0.8sec.
    g=24 angle<14gr, t=0.125sec, но точность в два раза ниже чем g=4
    геометрический смысл параметра g это сумма пикселей по горизонтальной
    прямоугольной области размером 1 на 8 пикселей или 1 на 4 или 1 на 2 или 1 на 1
    или 2 на 4 (g=24) 
   */
  float detectSkew( unsigned int g);

  /**\brief функция быстрого преобразования Радона */
  void toolsRadon( int sign,
				   unsigned int sharpness[],
				   unsigned int w2,
				   unsigned int g
				  );

	
   /// Отимизация исходного массива bytes_data для определения угла наклона текста
  void optimizationSkew( unsigned int g );
	
  /**\brief функция преобразования серого массива в битовый массив (адаптивный порог)
   // Авторы = "DerekBradley and GerhardRoth".
   // www.derekbradley.ca / AdaptiveThresholding / index.html. */ 
  void binarisation(int Tr, int BASE);	
	
	
  /**\brief print GBitmap on stdout in pseudo graphic*/
  void printToScreen();
  /**\brief Save GBitmap as TIFF file with GroupFax4 compression*/
  void saveTIFF(string &path);
  /**\brief Save GBitmap as TIFF file with GroupFax4 compression*/
  void saveTIFF(const char*path);

protected:
  // bitmap components
  int nrows;
  int ncolumns;
  int border;
  int bytes_per_row;
  int grays;

public:                            ///<size(bites_data) = size(bytes_data)/8
  unsigned char  *bytes_data;      ///<буфер байтовых данных битмэпа (большой распакованный массив)
  unsigned char  *bites_data;      ///<буфер битовых данных битмэпа (маленький запакованный массив)
  unsigned char  *bytes_data_buf;  ///<промежуточный буфер обработки битмэпа
  char  *TIFF_data;
  unsigned int 	 TIFF_data_size;
  string  TIFF_string;
  unsigned int 	 TIFF_string_size;	
	
  unsigned short pack_flag;   // 1 массив bites_data запакован функцией packImg
  unsigned short pack_flag32; // 1 массив bites_data запакован функцией packImg32
  int data_size;
  unsigned short h;
  unsigned short w;
	
  void drawImg(GBitmap *img,int x0,int y0);
  void rotate(int count);

	

  void packImg(int invert);   ///< функция упаковки битмапа в битовый массив
  void unpackImg(int invert); ///< функция распаковки битового массива в битмап
  string drawToTIFFString(int invert);  ///< представление битового контента как strokn в кодировке файла TIFF GroupFax4 


};

// ---------------- IMPLEMENTATION

inline unsigned int
GBitmap::rows() const
{
  return nrows;
}

inline unsigned int
GBitmap::columns() const
{
  return ncolumns;
}

inline unsigned int
GBitmap::borderSize() const
{
  return border;
}


inline unsigned int 
GBitmap::rowsize() const
{
  return bytes_per_row;
}


inline unsigned char *
GBitmap::operator[](int row)
{
  if (row<0 || row>=nrows) {
	return  bytes_data;
  }
  return &bytes_data[row * ncolumns];
}

inline const unsigned char *
GBitmap::operator[](int row) const
{
   if (row<0 || row>=nrows) {
	return bytes_data;
  }
  return &bytes_data[row * ncolumns];
}

inline GBitmap*
GBitmap::operator=(const GBitmap*ref)
{
  init(ref);
  return this;
}




}
# endif

