
#ifndef HG3D_EXPORT_H
#define HG3D_EXPORT_H 1


#pragma warning( disable : 4251 )

#if defined(OSG_LIBRARY_STATIC)
		#define HG3D_EXPORT
#else 
		#if defined( HG3D_LIBRARY )
			#define HG3D_EXPORT   __declspec(dllexport)
		#else
			#define HG3D_EXPORT   __declspec(dllimport)
		#endif
#endif

//一些简单参数的get和set函数的宏
#define DECLARE_PROP(type, prop) \
	inline void set##prop(const type& value) { prop = value; } \
	inline const type& get##prop() const { return (type&)prop; }


#endif