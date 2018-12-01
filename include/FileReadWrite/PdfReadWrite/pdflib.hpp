/*---------------------------------------------------------------------------*
 |          Copyright (c) 1997-2010 PDFlib GmbH. All rights reserved.        |
 +---------------------------------------------------------------------------+
 |    This software may not be copied or distributed except as expressly     |
 |    authorized by PDFlib GmbH's general license agreement or a custom      |
 |    license agreement signed by PDFlib GmbH.                               |
 |    For more information about licensing please refer to www.pdflib.com.   |
 *---------------------------------------------------------------------------*/

// $Id$
//
// C++ wrapper for PDFlib
//

#ifndef PDFLIB_HPP
#define PDFLIB_HPP

#include <string>
#include <sstream>
#include <iostream>
#include <stdexcept>

/* figure out whether or not we're running on an EBCDIC-based machine */
#define PDFCPP_ASCII_A                 0x41
#define PDFCPP_PLATFORM_A              'A'
#define PDFCPP_EBCDIC_A                0xC1

#if (PDFCPP_ASCII_A != PDFCPP_PLATFORM_A \
                && PDFCPP_EBCDIC_A == PDFCPP_PLATFORM_A)
#define PDFCPP_INTERNAL_OPTLIST \
    "escapesequence=false charref=false bom=optimize outputformat=ebcdicutf8"
#else
#define PDFCPP_INTERNAL_OPTLIST \
    "escapesequence=false charref=false bom=optimize outputformat=utf8"
#endif

#include "pdflib.h"

/*
 * PDFCPP_USE_PDFLIB_NAMESPACE
 *
 * If set to a non-zero value, wrap all declarations in the namespace "pdflib".
 * Otherwise no namespace declaration is created. For backward compatibility
 * with the PDFlib 7 C++ wrapper, define this as 0.
 */
#ifndef PDFCPP_USE_PDFLIB_NAMESPACE
#define PDFCPP_USE_PDFLIB_NAMESPACE 1
#endif

/*
 * PDFCPP_PDFLIB_WSTRING
 *
 * The pdflib.hpp header declares one instantiation of the basic_PDFlib
 * template as type "PDFlib". By default this PDFlib type is std::wstring-based.
 * For backward compatibility with the PDFlib 7 std::string-based C++ wrapper,
 * define this as 0.
 */
#ifndef PDFCPP_PDFLIB_WSTRING
#define PDFCPP_PDFLIB_WSTRING 1
#endif

/*
 * PDFCPP_DL
 *
 * The pdflib.hpp header can be used for static linking against the PDFlib
 * library, or it can be configured for loading the PDFlib DLL dynamically at
 * runtime.
 *
 * The default is to compile for static linking against the PDFlib library. For
 * dynamic loading, define PDFCPP_DL as 1. In that case the resulting program
 * must not be linked against the PDFlib library. Instead the pdflibdl.c module
 * from the "bind/c" directory must be compiled and linked to the application.
 */
#ifndef PDFCPP_DL
#define PDFCPP_DL 0
#endif

#if PDFCPP_DL
#include "pdflibdl.h"
#endif

#if defined(_MSC_VER) && defined(_MANAGED)
/*
 * This definition is only here for suppressing an unnecessary warning from
 * the linker when compiling the C++ wrapper as .NET managed code. As PDF_s
 * is only used to define an opaque pointer, the .NET wrapper never needs
 * the knowledge about the actual size of PDF_s.
 */
struct PDF_s {};
#endif

#if PDFCPP_USE_PDFLIB_NAMESPACE
namespace pdflib {
#endif

template<class pstring, class conv> class basic_PDFlib;

/**
 * The "do-nothing" converter that has the effect that the basic_PDFlib class
 * behaves in the same way as the PDFlib 7.x wrapper.
 */
template<class pstring>
class NoOpConverter
{
public:
    static bool do_conversion()
    {
        return false;
    }

    static void convert_to_pdf_bytes(
        const basic_PDFlib<pstring, NoOpConverter<pstring> >&,
        const pstring&, std::string&)
    {
        throw std::logic_error(
                "NoOpConverter::convert_to_pdf_bytes: internal error: "
                "converter called although do_conversion() returns false");
    }

    static void convert_to_pdf_utf8(
            const basic_PDFlib<pstring, NoOpConverter<pstring> >&,
            const pstring&, std::string&)
    {
        throw std::logic_error(
                "NoOpConverter::convert_to_pdf_utf8: internal error: "
                "converter called although do_conversion() returns false");
    }

    static void convert_to_pdf_utf16(
            const basic_PDFlib<pstring, NoOpConverter<pstring> >&,
            const pstring&, std::string&)
    {
        throw std::logic_error(
                "NoOpConverter::convert_to_pdf_utf16: internal error: "
                "converter called although do_conversion() returns false");
    }

    static void convert_to_pstring(
            const basic_PDFlib<pstring, NoOpConverter<pstring> >&,
            const char *, pstring&)
    {
        throw std::logic_error(
                "NoOpConverter::convert_to_pstring: internal error: "
                "converter called although do_conversion() returns false");
    }
};

#if defined(_MSC_VER) && defined(_MANAGED)
/*
 * Switching to C++ try/catch for exception handling allows compilation
 * of the C++ wrapper with "/clr" and also with "/clr:pure".
 */
#define PDFCPP_TRY try {

#define PDFCPP_CATCH } catch (Exception &) {\
  throw;\
}

#else

#if PDFCPP_DL

#define PDFCPP_TRY      PDF_TRY_DL(m_PDFlib_api, p)

#define PDFCPP_CATCH  \
PDF_CATCH_DL(m_PDFlib_api, p) { \
    pstring message, apiname; \
    convert_exception_strings(message, apiname); \
    throw Exception(message, m_PDFlib_api->PDF_get_errnum(p), apiname, m_PDFlib_api->PDF_get_opaque(p)); \
}

#else

#define PDFCPP_TRY      PDF_TRY(p)

#define PDFCPP_CATCH  \
PDF_CATCH(p) { \
    pstring message, apiname; \
    convert_exception_strings(message, apiname); \
    throw Exception(message, PDF_get_errnum(p), apiname, PDF_get_opaque(p)); \
}

#endif // PDFCPP_DL

#endif // defined(_MSC_VER) && defined(_MANAGED)

// The C++ class wrapper for PDFlib

template<class pstring, class conv>
class basic_PDFlib
{
    friend class NoOpConverter<pstring>;

public:

    class Exception
    {
    public:
	Exception(const pstring& errmsg, int errnum, const pstring& apiname,
	    void *opaque) :
	        m_errmsg(errmsg), m_errnum(errnum), m_apiname(apiname),
	        m_opaque(opaque)
        {
        }
	pstring get_errmsg() const { return m_errmsg; }
	int get_errnum() const { return m_errnum; }
	pstring get_apiname() const { return m_apiname; }
	const void *get_opaque() const { return m_opaque; }
    private:
	pstring m_errmsg;
	int m_errnum;
	pstring m_apiname;
	void * m_opaque;
    }; // Exception

#if defined(_MSC_VER) && defined(_MANAGED)
private:
    struct opaque_wrapper_t
    {
        basic_PDFlib<pstring, conv> *this_ptr;
        void *opaque;
    };

    friend
    void
    errorhandler(PDF *p, int errortype, const char* msg)
    {
        opaque_wrapper_t *opaque_wrapper =
           static_cast<opaque_wrapper_t *>(PDF_get_opaque(p));

        pstring message, apiname;
        opaque_wrapper->this_ptr->convert_exception_strings(message, apiname);

        throw typename Exception(
                message, PDF_get_errnum(p), apiname, opaque_wrapper->opaque);
    }

    opaque_wrapper_t opaque_wrapper;

public:
    basic_PDFlib(allocproc_t allocproc = NULL,
        reallocproc_t reallocproc = NULL,
        freeproc_t freeproc = NULL,
        void *opaque = NULL) :
            m_PDFlib_api(::PDF_get_api())
    {
        check_api(opaque);

        opaque_wrapper.this_ptr = this;
        opaque_wrapper.opaque = opaque;

        p = m_PDFlib_api->PDF_new2(errorhandler, allocproc,
                                    reallocproc, freeproc, &opaque_wrapper);

        if (!p)
        {
            throw std::bad_alloc();
        }

        set_cpp_binding_options();
    }

    ~basic_PDFlib() throw()
    {
        m_PDFlib_api->PDF_delete(p);
    }
#else

#if PDFCPP_DL
    class dl_load_error: public std::runtime_error
    {
    public:
        explicit dl_load_error() :
            std::runtime_error("Couldn't load PDFlib DLL")
        {
        }
    };

    /*
     * The constructor variant for dynamic loading does require the "opaque"
     * parameter to be a NULL pointer, as it is used internally. This way the
     * signature is compatible with the non-DL constructor variant.
     */
    basic_PDFlib(allocproc_t allocproc = NULL,
        reallocproc_t reallocproc = NULL,
        freeproc_t freeproc = NULL,
        void *opaque = NULL)
    {
        if (opaque)
        {
            throw std::invalid_argument(
                    "In the dynamic loading variant of the PDFlib C++ binding "
                    "the 'opaque' parameter must be NULL");
        }

        m_PDFlib_api = PDF_new2_dl(NULL, allocproc, reallocproc, freeproc, &p);

        if (!m_PDFlib_api)
        {
            throw dl_load_error();
        }

        check_api(NULL);

        set_cpp_binding_options();
    }

    ~basic_PDFlib() throw()
    {
        PDF_delete_dl(m_PDFlib_api, p);
    }
#else
    basic_PDFlib(allocproc_t allocproc = NULL,
	reallocproc_t reallocproc = NULL,
	freeproc_t freeproc = NULL,
	void *opaque = NULL) :
            m_PDFlib_api(::PDF_get_api())
    {
        check_api(opaque);

        p = m_PDFlib_api->PDF_new2(NULL, allocproc, reallocproc, freeproc,
                            opaque);

        if (!p)
        {
            throw std::bad_alloc();
        }

        set_cpp_binding_options();
    }

    ~basic_PDFlib() throw()
    {
        m_PDFlib_api->PDF_delete(p);
    }
#endif // PDFCPP_DL

#endif // defined(_MSC_VER) && defined(_MANAGED)

#include "cpp_wrapped.h"

    PDFLIB_DEPRECATED(
    pstring get_pdi_parameter(const pstring& key, int doc, int page,
            int reserved, int *len = NULL))
    {
        const char *retval = NULL;
        pstring pstring_retval;

        std::string key_param;
        const char *p_key_param;
        param_to_bytes(key, key_param, p_key_param);

        PDFCPP_TRY
            retval = m_PDFlib_api->PDF_get_pdi_parameter(p, p_key_param,
                        doc, page, reserved, len);
        PDFCPP_CATCH;

        apiretval_to_pstring(retval, pstring_retval);

        return pstring_retval;
    }

    PDFLIB_DEPRECATED(
    int open_image(const pstring& imagetype, const pstring& source,
        const char *data, long len, int width, int height, int components,
        int bpc, const pstring& params))
    {
        int retval = 0;

        std::string imagetype_param;
        const char *p_imagetype_param;
        param_to_bytes(imagetype, imagetype_param, p_imagetype_param);

        std::string source_param;
        const char *p_source_param;
        param_to_bytes(source, source_param, p_source_param);

        std::string params_param;
        const char *p_params_param;
        param_to_bytes(params, params_param, p_params_param);

        PDFCPP_TRY
        {
            retval = m_PDFlib_api->PDF_open_image(p, p_imagetype_param,
                    p_source_param, data, len, width, height, components, bpc,
                    p_params_param);
        }
        PDFCPP_CATCH;

        return retval;
    }

    PDFLIB_DEPRECATED(
    void open_mem(writeproc_t writeproc))
    {
        PDFCPP_TRY
            m_PDFlib_api->PDF_open_mem(p, writeproc);
        PDFCPP_CATCH;
    }

    PDFLIB_DEPRECATED(
    int open_pdi(const pstring& filename, const pstring& optlist, int))
    {
        int retval = 0;

        std::string filename_param;
        const char *p_filename_param;
        int len_filename;
        param_to_0utf16(filename, filename_param, p_filename_param, len_filename);

        std::string optlist_param;
        const char *p_optlist_param;
        param_to_utf8(optlist, optlist_param, p_optlist_param);

        PDFCPP_TRY
            retval = m_PDFlib_api->PDF_open_pdi(p, p_filename_param,
                    p_optlist_param, len_filename);
        PDFCPP_CATCH;

        return retval;
    }

    PDFLIB_DEPRECATED(
    void setpolydash(float *darray, int length))
    {
        PDFCPP_TRY
            m_PDFlib_api->PDF_setpolydash(p, darray, length);
        PDFCPP_CATCH;
    }

    PDFLIB_DEPRECATED(
    int show_boxed(const pstring& text, double left, double top, double width,
	double height, const pstring& hmode, const pstring& feature))
    {
        int retval = 0;

        std::string text_param;
        const char *p_text_param;
        int len_text;
        param_to_utf16(text, text_param, p_text_param, len_text);

        std::string hmode_param;
        const char *p_hmode_param;
        param_to_bytes(hmode, hmode_param, p_hmode_param);

        std::string feature_param;
        const char *p_feature_param;
        param_to_bytes(feature, feature_param, p_feature_param);

        PDFCPP_TRY
            retval = m_PDFlib_api->PDF_show_boxed2(p, p_text_param, len_text,
                        left, top, width, height, p_hmode_param, p_feature_param);
        PDFCPP_CATCH;

        return retval;
    }

    void begin_document_callback(writeproc_t writeproc, const pstring& optlist)
    {
        std::string optlist_param;
        const char *p_optlist_param;
        param_to_utf8(optlist, optlist_param, p_optlist_param);

        PDFCPP_TRY
            m_PDFlib_api->PDF_begin_document_callback(p, writeproc,
                                                    p_optlist_param);
        PDFCPP_CATCH;
    }

    void *get_opaque()
    {
        void *retval = NULL;

        PDFCPP_TRY
#if defined(_MSC_VER) && defined(_MANAGED)
            retval = opaque_wrapper.opaque;
#else
            retval = m_PDFlib_api->PDF_get_opaque(p);
#endif
        PDFCPP_CATCH;

        return retval;
    }

    PDFLIB_DEPRECATED(
    void xshow(const pstring& text, const double *xadvancelist))
    {
        std::string text_param;
        const char *p_text_param;
        int len_text;
        param_to_utf16(text, text_param, p_text_param, len_text);

        PDFCPP_TRY
            m_PDFlib_api->PDF_xshow(p, p_text_param, len_text, xadvancelist);
        PDFCPP_CATCH;
    }

    PDFLIB_DEPRECATED(
    std::string utf16_to_utf8(const std::string& utf16string) const)
    {
        const char *retval = NULL;

        PDFCPP_TRY
        {
            retval = m_PDFlib_api->PDF_utf16_to_utf8(p, utf16string.c_str(),
                    static_cast<int>(utf16string.length()), NULL);
        }
        PDFCPP_CATCH;

        if (retval)
            return retval;
        else
            return "";
    }

    PDFLIB_DEPRECATED(
    std::string utf8_to_utf16(const std::string& utf8string,
                                const std::string& format) const)
    {
        const char *buf;
        int size;
        std::string retval;

        PDFCPP_TRY
        {
            buf = m_PDFlib_api->PDF_utf8_to_utf16(p, utf8string.c_str(),
                                                     format.c_str(), &size);
            if (buf)
                retval.assign(buf, static_cast<size_t>(size));
        }
        PDFCPP_CATCH;

        return retval;
    }

    PDFLIB_DEPRECATED(
    std::string utf32_to_utf16(const std::string& utf32string,
                                const std::string& ordering) const)
    {
        const char *buf;
        int size;
        std::string retval;

        PDFCPP_TRY
        {
            buf = m_PDFlib_api->PDF_utf32_to_utf16(p, utf32string.data(),
                static_cast<int>(utf32string.length()), ordering.c_str(), &size);
            if (buf)
                retval.assign(buf, static_cast<size_t>(size));
        }
        PDFCPP_CATCH;

        return retval;
    }

    PDFLIB_DEPRECATED(
    std::string utf32_to_utf8(const std::string& utf32string) const)
    {
        const char *buf;
        int size;
        std::string retval;


        PDFCPP_TRY {
            buf = m_PDFlib_api->PDF_utf32_to_utf8(p, utf32string.data(),
                static_cast<int>(utf32string.length()), &size);
            if (buf)
                retval.assign(buf, static_cast<size_t>(size));
        }
        PDFCPP_CATCH;

        return retval;
    }

    PDFLIB_DEPRECATED(
    std::string utf8_to_utf32(const std::string& utf8string,
                                const std::string& ordering) const)
    {
        const char *buf;
        int size;
        std::string retval;


        PDFCPP_TRY {
            buf = m_PDFlib_api->PDF_utf8_to_utf32(p, utf8string.c_str(),
                ordering.c_str(), &size);
            if (buf)
                retval.assign(buf, static_cast<size_t>(size));
        }
        PDFCPP_CATCH;

        return retval;
    }

    PDFLIB_DEPRECATED(
    std::string utf16_to_utf32(const std::string& utf16string,
                                const std::string& ordering) const)
    {
        const char *buf;
        int size;
        std::string retval;

        PDFCPP_TRY {
            buf = m_PDFlib_api->PDF_utf16_to_utf32(p, utf16string.c_str(),
                    static_cast<int>(utf16string.length()), ordering.c_str(),
                    &size);
            if (buf)
                retval.assign(buf, static_cast<size_t>(size));
        }
        PDFCPP_CATCH;

        return retval;
    }

    /* Convert an arbitrary encoded string to a Unicode string in several formats. */
    std::string
    convert_to_unicode(const pstring& inputformat, const std::string& inputstring, const pstring& optlist) const
    {
        std::string retval;

        std::string inputformat_param;
        const char *p_inputformat_param;
        param_to_bytes(inputformat, inputformat_param, p_inputformat_param);

        std::string optlist_param;
        const char *p_optlist_param;
        param_to_utf8(optlist, optlist_param, p_optlist_param);

	PDFCPP_TRY {
            int outputlen;
            const char * const buf =
                    m_PDFlib_api->PDF_convert_to_unicode(p, p_inputformat_param,
                        inputstring.data(), static_cast<int>(inputstring.length()),
                        &outputlen, p_optlist_param);
            if (buf)
                retval.assign(buf, static_cast<size_t>(outputlen));
	}
	PDFCPP_CATCH;

	return retval;
    }

private:
    PDF *p;
    const PDFlib_api *m_PDFlib_api;

    enum
    {
        utf16_wchar_t_size = 2,
        utf32_wchar_t_size = 4
    };

    void check_api(void *opaque)
    {
        if (m_PDFlib_api->sizeof_PDFlib_api != sizeof(PDFlib_api) ||
            m_PDFlib_api->major != PDFLIB_MAJORVERSION ||
            m_PDFlib_api->minor != PDFLIB_MINORVERSION)
        {
            pstring message;
            pstring apiname; /* stays empty */

            switch (sizeof(typename pstring::value_type))
            {
            case sizeof(char):
                apiretval_to_pstring("loaded wrong version of PDFlib library", message);
                break;

            case utf16_wchar_t_size:
            case utf32_wchar_t_size:
                apiretval_to_pstring(reinterpret_cast<const char *>(L"loaded wrong version of PDFlib library"), message);
                break;

            default:
                bad_wchar_size("basic_PDFlib<pstring, conv>::check_api");
            }

            throw Exception(message, -1, apiname, opaque);
        }
    }

    void set_cpp_binding_options(void)
    {
        PDFCPP_TRY
        {
            m_PDFlib_api->PDF_set_parameter(p, "objorient", "true");
            if (conv::do_conversion())
            {
                m_PDFlib_api->PDF_set_parameter(p, "binding", "C++ conv");
                set_unicaplang_parameters();
            }
            else
            {
                switch (sizeof(typename pstring::value_type))
                {
                case sizeof(char):
                    m_PDFlib_api->PDF_set_parameter(p, "binding", "C++ legacy");
                    break;

                case utf16_wchar_t_size:
                    m_PDFlib_api->PDF_set_parameter(p, "apitextformat", "utf16");
                    m_PDFlib_api->PDF_set_parameter(p, "binding", "C++");
                    set_unicaplang_parameters();
                    break;

                case utf32_wchar_t_size:
                    m_PDFlib_api->PDF_set_parameter(p, "apitextformat", "utf32");
                    m_PDFlib_api->PDF_set_parameter(p, "binding", "C++");
                    set_unicaplang_parameters();
                    break;

                default:
                    bad_wchar_size("basic_PDFlib<pstring, conv>::basic_PDFlib");
                }
            }
        }
        PDFCPP_CATCH;
    }

    void set_unicaplang_parameters() const
    {
        m_PDFlib_api->PDF_set_parameter(p, "unicaplang", "true");
        m_PDFlib_api->PDF_set_parameter(p, "textformat", "auto2");
        m_PDFlib_api->PDF_set_parameter(p, "hypertextformat", "auto2");
        m_PDFlib_api->PDF_set_parameter(p, "hypertextencoding", "");
    }

    void bad_wchar_size(const char *apiname) const
    {
        std::ostringstream exception_text;
        exception_text << apiname << ": unsupported wchar_t size: "
                        << sizeof(typename pstring::value_type);

        throw std::logic_error(exception_text.str());
    }

    void param_to_utf8(const pstring& param, std::string& pdflib_param,
                        const char *& pdflib_ptr) const
    {
        if (conv::do_conversion())
        {
            conv::convert_to_pdf_utf8(*this, param, pdflib_param);
            pdflib_ptr = pdflib_param.c_str();
        }
        else
        {
            const char * const s = reinterpret_cast<const char *>(param.c_str());
            int outputlen;
            const size_t param_length = param.length();

            switch (sizeof(typename pstring::value_type))
            {
            case sizeof(char):
                /*
                 * Legacy case: Pass through user-supplied string.
                 */
                pdflib_ptr = s;
                break;

            case utf16_wchar_t_size:
                pdflib_ptr = param_length > 0
                   ? m_PDFlib_api->PDF_convert_to_unicode(p, "utf16",
                        s, static_cast<int>(param_length * sizeof(wchar_t)),
                        &outputlen, PDFCPP_INTERNAL_OPTLIST)
                   : "";
                break;

            case utf32_wchar_t_size:
                pdflib_ptr = param_length > 0
                    ? m_PDFlib_api->PDF_convert_to_unicode(p, "utf32",
                        s, static_cast<int>(param_length * sizeof(wchar_t)),
                        &outputlen, PDFCPP_INTERNAL_OPTLIST)
                    : "";
                break;

            default:
                bad_wchar_size("basic_PDFlib<pstring, conv>::param_to_utf8");
            }
        }
    }

    void param_to_0utf16(const pstring& param, std::string& pdflib_param,
                        const char *& pdflib_ptr, int& len) const
    {
        if (conv::do_conversion())
        {
            conv::convert_to_pdf_utf16(*this, param, pdflib_param);
            pdflib_ptr = pdflib_param.c_str();
            len = static_cast<int>(pdflib_param.length());
        }
        else
        {
            const char * const s = reinterpret_cast<const char *>(param.c_str());
            const size_t param_length = param.length();

            switch (sizeof(typename pstring::value_type))
            {
            case sizeof(char):
                /*
                 * Legacy case: Pass through user-supplied string with length 0,
                 * string must not contain 0 bytes.
                 */
                pdflib_ptr = s;
                len = 0;
                break;

            case utf16_wchar_t_size:
                /*
                 * UTF-16 can also be passed through directly
                 */
                pdflib_ptr = s;
                len = static_cast<int>(param_length * utf16_wchar_t_size);
                break;

            case utf32_wchar_t_size:
                if (param_length > 0)
                {
                    pdflib_ptr =
                        m_PDFlib_api->PDF_convert_to_unicode(p, "utf32",
                            s, static_cast<int>(param_length * utf32_wchar_t_size),
                            &len, "escapesequence=false charref=false outputformat=utf16");
                }
                else
                {
                    static const char utf16_eos[] = { 0, 0 };

                    pdflib_ptr = utf16_eos;
                    len = 0;
                }
                break;

            default:
                bad_wchar_size("basic_PDFlib<pstring, conv>::param_to_utf16");
            }
        }
    }

    void param_to_utf16(const pstring& param, std::string& pdflib_param,
                        const char *& pdflib_ptr, int& len) const
    {
        if (conv::do_conversion())
        {
            conv::convert_to_pdf_utf16(*this, param, pdflib_param);
            pdflib_ptr = pdflib_param.c_str();
            len = static_cast<int>(pdflib_param.length());
        }
        else
        {
            const char *s = reinterpret_cast<const char *>(param.c_str());
            const size_t param_length = param.length();

            switch (sizeof(typename pstring::value_type))
            {
            case sizeof(char):
                /*
                 * Legacy case: Pass through user-supplied string including
                 * explicit length, string may contain 0 bytes.
                 */
                pdflib_ptr = s;
                len = static_cast<int>(param_length);
                break;

            case utf16_wchar_t_size:
                /*
                 * UTF-16 can also be passed through directly
                 */
                pdflib_ptr = s;
                len = static_cast<int>(param_length * utf16_wchar_t_size);
                break;

            case utf32_wchar_t_size:
                if (param_length > 0)
                {
                    pdflib_ptr =
                        m_PDFlib_api->PDF_convert_to_unicode(p, "utf32",
                            s, static_cast<int>(param_length * utf32_wchar_t_size),
                            &len, "escapesequence=false charref=false outputformat=utf16");
                }
                else
                {
                    static const char utf16_eos[] = { 0, 0 };

                    pdflib_ptr = utf16_eos;
                    len = 0;
                }
                break;

            default:
                bad_wchar_size("basic_PDFlib<pstring, conv>::param_to_utf16");
            }
        }
    }

    void param_to_bytes(const pstring& param, std::string& pdflib_param,
                        const char *& pdflib_ptr) const
    {
        if (conv::do_conversion())
        {
            conv::convert_to_pdf_bytes(*this, param, pdflib_param);
            pdflib_ptr = pdflib_param.c_str();
        }
        else
        {
            const size_t size = sizeof(typename pstring::value_type);
            const char *s = reinterpret_cast<const char *>(param.c_str());

            switch (size)
            {
            case sizeof(char):
                pdflib_ptr = s;
                break;

            case utf16_wchar_t_size:
            case utf32_wchar_t_size:
                {
                    const size_t param_length = param.length();

                    if (param_length > 0)
                    {
                        int highchar;

                        const char * const deflated =
                            m_PDFlib_api->PDF_deflate_unicode(p, s,
                                    static_cast<int>(param_length * size),
                                    size, &highchar);

                        if (!deflated)
                        {
                            std::ostringstream exception_text;

                            exception_text
                                << "basic_PDFlib::param_to_bytes: high "
                                    "Unicode character '0x"
                                << std::hex << highchar
                                << "' is not supported in this character string";

                            throw std::runtime_error(exception_text.str().c_str());
                        }

                        pdflib_ptr = deflated;
                    }
                    else
                    {
                        pdflib_ptr = "";
                    }
                }
                break;

            default:
                bad_wchar_size("basic_PDFlib<pstring, conv>::param_to_bytes");
            }
        }
    }

    void apiretval_to_pstring(const char * const pdflib_retval,
                        pstring& cpp_retval) const
    {
        if (conv::do_conversion())
        {
            if (pdflib_retval)
            {
                conv::convert_to_pstring(*this, pdflib_retval, cpp_retval);
            }
            else
            {
                cpp_retval.erase();
            }
        }
        else
        {
            if (pdflib_retval)
            {
                cpp_retval.assign(reinterpret_cast
                                    <const typename pstring::value_type *>
                                        (pdflib_retval));
            }
            else
            {
                cpp_retval.erase();
            }
        }
    }

    void
    convert_exception_strings(pstring& message, pstring& apiname) const
    {
        if (conv::do_conversion())
        {
            conv::convert_to_pstring(*this, m_PDFlib_api->PDF_get_errmsg(p),
                                        message);
            conv::convert_to_pstring(*this, m_PDFlib_api->PDF_get_apiname(p),
                                        apiname);
        }
        else
        {
            /*
             * Without custom converter the PDFlib API returns the error message
             * and the API name in the expected encoding, so just put this
             * string into the output string.
             */
            message = reinterpret_cast<const typename pstring::value_type *>
                                        (m_PDFlib_api->PDF_get_errmsg(p));
            apiname = reinterpret_cast<const typename pstring::value_type *>
                                        (m_PDFlib_api->PDF_get_apiname(p));
        }
    }

    /*
     * Prevent copy construction and assignment.
     */
    basic_PDFlib(const basic_PDFlib&);
    basic_PDFlib& operator=(const basic_PDFlib&);
};

#if PDFCPP_PDFLIB_WSTRING
typedef basic_PDFlib<std::wstring, NoOpConverter<std::wstring> > PDFlib;
#else
typedef basic_PDFlib<std::string, NoOpConverter<std::string> > PDFlib;
#endif

#if PDFCPP_USE_PDFLIB_NAMESPACE
} // end of PDFlib namespace
#endif

#endif	// PDFLIB_HPP
