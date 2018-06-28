// hgtypes.h

#if !defined(_HG3D_TYPES_H_)
#define _HG3D_TYPES_H_

#include <map>
#include <string>
#include <vector>
#include <set>
#include <tchar.h> // _T()/_TEXT()
#include <unordered_map>


namespace hg3d {

#if defined(_UNICODE) || defined(UNICODE)
typedef std::wstring  tstring;
typedef std::wstring  TString;
#else
typedef std::string   tstring;
typedef std::string   TString;
#endif

typedef std::map<TString, TString>  strstrmap;
typedef std::vector<TString>  strvector;

typedef std::vector<TString>  StringVector;
typedef std::map<TString, TString>  StringStringMap;
#if (_MSC_VER >= 1600) // vs2010
typedef std::unordered_map<TString, TString> StringUnorderedMap;
#endif
typedef std::set<TString> StringSet;

enum TEXTURE_DIRECTION {
    TEX_DIR_HORIZONTAL,
    TEX_DIR_VERTICAL
};

template<typename type>
struct Size2D_ {
    Size2D_():width(0), height(0) {}
    Size2D_(type _width, type _height):width(_width), height(_height) {}
    type width;
    type height;
};
typedef Size2D_<size_t> Size2Ds;

template <typename cT, typename traits = std::char_traits<cT> >
class HgStreamBuf_ : public std::basic_streambuf<cT, traits> {
public:
    HgStreamBuf_(cT* mem, std::streamsize sz) {
        setg(mem, mem, mem + sz);
    }
};
typedef HgStreamBuf_<char, std::char_traits<char> > HgStreamBuf;

} // namespace hg3d

#endif //#if !defined(_HG3D_TYPES_H_)