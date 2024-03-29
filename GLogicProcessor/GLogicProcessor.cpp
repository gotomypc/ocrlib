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
# include "GLogicProcessor.h"
# include "config.h"
#include <string.h>
#include <stdio.h>

#ifdef MACOS
#import <Cocoa/Cocoa.h>
#endif
#pragma hdrstop

namespace ocr{
#include "startConvert.h"
#include "converterPali.h"
#include "startDictionary.h"
#include "loadDictionary.h"
#include "dictReport.h"
#include "loadMap.h"
#include "LoadFontMaps.h"
#include "UniToYagpo.h"
#include "YagpoUniConverter_RTF.h"
#include "codePagesConverter.h"
#include "spellChecker.h"
#include "grammarCorrector.h";



GLogicProcessor::~GLogicProcessor(){}

void GLogicProcessor::destroy(void)
{
}

GLogicProcessor::GLogicProcessor(){

	try
	{
		init();
	}
	catch(int a)
	{
		destroy();
	}
}

// ----- initialization

void GLogicProcessor::init(){
    dictionaryReady=0;
}


};


