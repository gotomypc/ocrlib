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


#ifndef _GSocket_H_
#define _GSocket_H_
#include "config.h"

#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>

#ifdef MACOS
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdint.h>
#endif

#ifdef WIN32
 #include <winsock2.h>
 #include <process.h>
#endif


#include "OCRTypes/OCRTypes.h"
#include "OCRString/php2stl.h"
#include "GImageEditor/GImageEditor.h"
#include "GLogicProcessor/GLogicProcessor.h"

#pragma hdrstop

#define PORT 3550
/* Port that will be opened */
#define BACKLOG 2   /* Number of allowed connections */
#define MAXDATASIZE 2048   /* Max number of bytes of data */ 
#define MAXDATASIZE_SEND 64096


using namespace std;
using namespace pugi;

namespace ocr{
	
///класс содержит методы обработки обращения по HTTP запросу по протоколу TCP со стороны Интернет браузера или сокет-клиента	
	class  GSocket {
	protected:
		GSocket(void);
		ofstream out;
	public:
		void destroy(void);
		/** @name Initialization. */
		//@{
	protected:
	public:
	    static GSocket* create(void) {return new GSocket;}
		/// @nameSocket function
		//@{
		/** @brief  Socket communication function which start socket server and handle socket connection
		 */	
		int startSocketServer(commandData *inputData);
		/** @brief  There is a separate instance of this socket communication function 
		 for each connection.  It handles all communication once a connnection has been established.
		 Client is C++ programm.
		 */	
		void pipeLine (commandData *inputData, int sockfd);
		/** @brief  There is a separate instance of this socket communication function 
		 for each connection.  It handles all communication once a connnection has been established.
		 Socket client can be C++, PHP script or Flash. Socket data is XML based
		 */	
		int  pipeLineXML (commandData *inputData, int sockfd);
		/** Open socket connection and send string by socket (wrapper)*/
		int sendTextBySocket(const char *data);
		int sendTextBySocket(string &data);


};
};

#endif











