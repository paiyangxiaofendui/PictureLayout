
#ifndef TINYXML_EXPORT_H
#define TINYXML_EXPORT_H


#pragma warning( disable : 4251 )

#if defined(TINYXML_LIBRARY_STATIC)
		#define TINYXML_EXPORT
#else 
		#if defined( TINYXML_LIBRARY )
			#define TINYXML_EXPORT   __declspec(dllexport)
		#else
			#define TINYXML_EXPORT   __declspec(dllimport)
		#endif
#endif


#define TIXML_USE_STL

#endif

