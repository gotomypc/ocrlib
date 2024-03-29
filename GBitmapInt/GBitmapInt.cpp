//C-
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
# include "GBitmapInt.h"
# include "config.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


namespace ocr{

GBitmapInt::~GBitmapInt(){
if(data_size)free(int_data);
data_size=0;
}

void
GBitmapInt::destroy(void)
{
  if(data_size)free(int_data);
  data_size=0;
}

GBitmapInt::GBitmapInt()
  : nrows(0), ncolumns(0), border(0),
	int_per_row(0),data_size(0)

{
}

GBitmapInt::GBitmapInt(int nrows, int ncolumns, int border)
  : nrows(0), ncolumns(0), border(0), 
	int_per_row(0), data_size(0)
{
  try
  {
    init(nrows, ncolumns, border);
  }
  catch(int a)
  {
	destroy();
  }
}


GBitmapInt::GBitmapInt(const GBitmapInt* ref)
  : nrows(0), ncolumns(0), border(0),
	int_per_row(0),data_size(0)
{
  try
  {
	init(ref, ref->border);
  }
  catch(int a)
  {
	destroy();
  }
}

GBitmapInt::GBitmapInt(const GBitmapInt *ref, int border)
  : nrows(0), ncolumns(0), border(0), 
	int_per_row(0),data_size(0)
{
  try
  {
	init(ref, border);
  }
  catch(int a)
  {
	destroy();
  }
}


// ----- initialization

void 
GBitmapInt::init(int arows, int acolumns, int aborder)
{
  destroy();
  nrows = arows;
  ncolumns = acolumns;
  border = 0;
  int_per_row = ncolumns;
  int npixels = nrows * int_per_row;
  if (npixels > 0)
	{
	  int_data=(unsigned int*)malloc((npixels+64)*4);    // запрос памяти с очисткой 0);
	}
  data_size=ncolumns*nrows;
  fill(0xffffffff);
}//_____________________________________________________________________________


void GBitmapInt::init(const GBitmapInt* ref, int aborder){
  if (this != ref)
	{
	  border=aborder;
	  init(ref->nrows, ref->ncolumns, aborder);
	  unsigned int *row = int_data+border;
	  unsigned int *rowDest;
	  for (int n=0; n<nrows; n++, row+=int_per_row){
		 rowDest=ref->int_data+int_per_row*n+border*2*n;
	     memcpy( (void*)row, (void*)rowDest,  ncolumns );
	  }
	}
  else if (aborder > border)
    {
      minborder(aborder);
	}
	 data_size=ncolumns*nrows;
}



void
GBitmapInt::minborder(int minimum)
{
/*	
  if (border < minimum)
	{
	  if (border < minimum)
		{
			  GBitmapInt tmp(this, minimum);
			  int_per_row = tmp.int_per_row;
			  int_data=(unsigned int*)farrealloc(int_data,tmp.data_size);  // изменение размера блока
			  memcpy(int_data,tmp.int_data,data_size);
			  border = minimum;
		}
	}
*/
}



// Fills a bitmap with the given value
void
GBitmapInt::fill(unsigned int value){

 memset (int_data, value, data_size*4);
//  for(unsigned int y=0; y<rows(); y++)
//	{
//	  unsigned char* bm_y = (*this)[y];
//	  for(unsigned int x=0; x<columns(); x++)
//		bm_y[x] = value;
//	}
}

}// namespace ocr

