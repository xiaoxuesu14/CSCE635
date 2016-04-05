#ifndef __COMMPARSER_H_DEFINED__
#define __COMMPARSER_H_DEFINED__

class commParser
{
	public:
		/** Class constructor
		  *
		  */
		commParser();
		/** Feed new bytes received to the parser
		  * 
		  */
		void newBytes(char*bytes);
	private:
		/** 
		  * 
		  */
		void handleMsg();
}

#endif