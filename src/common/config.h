#ifndef CONFIG_H
#define CONFIG_H

// WARNING! This file should be in win cp-1251 encoding for generate correct rc file on windows


// Program names
#define COMPANY_NAME "KS team"
#define COMPANY_COPYRIGHT ""
#define PRODUCT_NAME "KS client"
#define FULL_PRODUCT_NAME "KS client"
#define EXE_NAME "ks_client.exe"
#define COMPANY_DOMAIN ""
#define WEBSITE "https://github.com/Kriegspiel"

// Version
#define MAJOR 0
#define MINOR 1
#define RELEASE 1

#define MS(y) #y // make string
#define VERSIONSTRING(a,b,c) MS(a) "." MS(b) "." MS(c)
#define VERSIONSTRING_(a,b) MS(a) "." MS(b)

#define VERSION VERSIONSTRING(MAJOR,MINOR,RELEASE)
#define PRODUCT_VERSION VERSIONSTRING_(MAJOR,MINOR)

// the main part for programm
#ifndef APSTUDIO_READONLY_SYMBOLS

#define IMAGEDATA_DATA

#include <QSettings>
#include <QString>
#include <QTextCodec>
#include <QObject>

//Programm info
namespace Config {
	//const QString Encode(const char * s) {return QTextCodec::codecForName("CP1251")->toUnicode(s); } // ToDo: resolve link errors

	// about
	const QString ProgramName = QTextCodec::codecForName("CP1251")->toUnicode(PRODUCT_NAME);
	const QString CompanyName = QTextCodec::codecForName("CP1251")->toUnicode(COMPANY_NAME);
	const QString CompanyCopyright = QTextCodec::codecForName("CP1251")->toUnicode(COMPANY_COPYRIGHT);
	const QString CompanyWebSite = WEBSITE;

	const QString Version = VERSION;
	const QString Title = ProgramName;
}
#else // for *.rc info of the program on windows
	#define _STR_COMPANY_NAME COMPANY_NAME
	#define _STR_PRODUCT_NAME FULL_PRODUCT_NAME
	#define _STR_INTERNAL_NAME PRODUCT_NAME
	#define _STR_ORIGINAL_FILE_NAME EXE_NAME
	#define _STR_WEBSITE WEBSITE
	#define _PRODUCT_VERSION PRODUCT_VERSION
	#define _STR_PRODUCT_VERSION PRODUCT_VERSION
	#define _STR_FILE_DESCRIPTION PRODUCT_NAME
	#define _FILE_VERSION VERSION
	#define _STR_FILE_VERSION VERSION
	#define _STR_LEGAL_COPYRIGHT COMPANY_COPYRIGHT
	#define _STR_LEGAL_TRADE_1 ""
	#define _STR_LEGAL_TRADE_2 _STR_LEGAL_TRADE_1
#endif

#endif // CONFIG_H

