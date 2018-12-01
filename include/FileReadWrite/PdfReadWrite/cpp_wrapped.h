
    
/* Activate a previously created structure element or other content item. */
void
activate_item(int id)
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_activate_item(p, id);
    }
    PDFCPP_CATCH;
}

    
/* Deprecated, use  PDF_create_bookmark(). */
PDFLIB_DEPRECATED(
int
add_bookmark(const pstring& text, int parent, int open)
)
{
    int retval = 0;

    std::string text_param;
    const char *p_text_param;
    int len_text;
    param_to_utf16(text, text_param, p_text_param, len_text);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_add_bookmark2(p, p_text_param, len_text, parent, open);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Deprecated, use PDF_create_action() and PDF_create_annotation(). */
PDFLIB_DEPRECATED(
void
add_launchlink(double llx, double lly, double urx, double ury, const pstring& filename)
)
{
    std::string filename_param;
    const char *p_filename_param;
    param_to_bytes(filename, filename_param, p_filename_param);

    PDFCPP_TRY {
	m_PDFlib_api->PDF_add_launchlink(p, llx, lly, urx, ury, p_filename_param);
    }
    PDFCPP_CATCH;
}

    
/* Deprecated, use PDF_create_action() and PDF_create_annotation(). */
PDFLIB_DEPRECATED(
void
add_locallink(double llx, double lly, double urx, double ury, int page, const pstring& optlist)
)
{
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	m_PDFlib_api->PDF_add_locallink(p, llx, lly, urx, ury, page, p_optlist_param);
    }
    PDFCPP_CATCH;
}

    
/* Create a named destination on a page in the document. */
void
add_nameddest(const pstring& name, const pstring& optlist)
{
    std::string name_param;
    const char *p_name_param;
    int len_name;
    param_to_utf16(name, name_param, p_name_param, len_name);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	m_PDFlib_api->PDF_add_nameddest(p, p_name_param, len_name, p_optlist_param);
    }
    PDFCPP_CATCH;
}

    
/* Deprecated, use PDF_create_annotation(). */
PDFLIB_DEPRECATED(
void
add_note(double llx, double lly, double urx, double ury, const pstring& contents, const pstring& title, const pstring& icon, int open)
)
{
    std::string contents_param;
    const char *p_contents_param;
    int len_contents;
    param_to_utf16(contents, contents_param, p_contents_param, len_contents);
    std::string title_param;
    const char *p_title_param;
    int len_title;
    param_to_utf16(title, title_param, p_title_param, len_title);
    std::string icon_param;
    const char *p_icon_param;
    param_to_bytes(icon, icon_param, p_icon_param);

    PDFCPP_TRY {
	m_PDFlib_api->PDF_add_note2(p, llx, lly, urx, ury, p_contents_param, len_contents, p_title_param, len_title, p_icon_param, open);
    }
    PDFCPP_CATCH;
}

    
/* Add a point to a new or existing path object. */
int
add_path_point(int path, double x, double y, const pstring& type, const pstring& optlist)
{
    int retval = 0;

    std::string type_param;
    const char *p_type_param;
    param_to_bytes(type, type_param, p_type_param);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_add_path_point(p, path, x, y, p_type_param, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Deprecated, use PDF_create_action() and PDF_create_annotation(). */
PDFLIB_DEPRECATED(
void
add_pdflink(double llx, double lly, double urx, double ury, const pstring& filename, int page, const pstring& optlist)
)
{
    std::string filename_param;
    const char *p_filename_param;
    param_to_bytes(filename, filename_param, p_filename_param);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	m_PDFlib_api->PDF_add_pdflink(p, llx, lly, urx, ury, p_filename_param, page, p_optlist_param);
    }
    PDFCPP_CATCH;
}

    
/* Add a file to a portfolio folder or a package (requires PDF 1.7). */
int
add_portfolio_file(int folder, const pstring& filename, const pstring& optlist)
{
    int retval = 0;

    std::string filename_param;
    const char *p_filename_param;
    int len_filename;
    param_to_0utf16(filename, filename_param, p_filename_param, len_filename);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_add_portfolio_file(p, folder, p_filename_param, len_filename, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Add a folder to a new or existing portfolio (requires PDF 1.7ext3). */
int
add_portfolio_folder(int parent, const pstring& foldername, const pstring& optlist)
{
    int retval = 0;

    std::string foldername_param;
    const char *p_foldername_param;
    int len_foldername;
    param_to_0utf16(foldername, foldername_param, p_foldername_param, len_foldername);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_add_portfolio_folder(p, parent, p_foldername_param, len_foldername, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Add a cell to a new or existing table. */
int
add_table_cell(int table, int column, int row, const pstring& text, const pstring& optlist)
{
    int retval = 0;

    std::string text_param;
    const char *p_text_param;
    int len_text;
    param_to_utf16(text, text_param, p_text_param, len_text);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_add_table_cell(p, table, column, row, p_text_param, len_text, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Create a Textflow object, or add text and explicit options to an existing Textflow. */
int
add_textflow(int textflow, const pstring& text, const pstring& optlist)
{
    int retval = 0;

    std::string text_param;
    const char *p_text_param;
    int len_text;
    param_to_utf16(text, text_param, p_text_param, len_text);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_add_textflow(p, textflow, p_text_param, len_text, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Deprecated */
PDFLIB_DEPRECATED(
void
add_thumbnail(int image)
)
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_add_thumbnail(p, image);
    }
    PDFCPP_CATCH;
}

    
/* Deprecated, use PDF_create_action() and PDF_create_annotation(). */
PDFLIB_DEPRECATED(
void
add_weblink(double llx, double lly, double urx, double ury, const pstring& url)
)
{
    std::string url_param;
    const char *p_url_param;
    param_to_bytes(url, url_param, p_url_param);

    PDFCPP_TRY {
	m_PDFlib_api->PDF_add_weblink(p, llx, lly, urx, ury, p_url_param);
    }
    PDFCPP_CATCH;
}

    
/* Align the coordinate system with a relative vector. */
void
align(double dx, double dy)
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_align(p, dx, dy);
    }
    PDFCPP_CATCH;
}

    
/* Draw a counterclockwise circular arc segment. */
void
arc(double x, double y, double r, double alpha, double beta)
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_arc(p, x, y, r, alpha, beta);
    }
    PDFCPP_CATCH;
}

    
/* Draw a clockwise circular arc segment. */
void
arcn(double x, double y, double r, double alpha, double beta)
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_arcn(p, x, y, r, alpha, beta);
    }
    PDFCPP_CATCH;
}

    
/* Deprecated, use  PDF_create_annotation(). */
PDFLIB_DEPRECATED(
void
attach_file(double llx, double lly, double urx, double ury, const pstring& filename, const pstring& description, const pstring& author, const pstring& mimetype, const pstring& icon)
)
{
    std::string filename_param;
    const char *p_filename_param;
    int len_filename;
    param_to_0utf16(filename, filename_param, p_filename_param, len_filename);
    std::string description_param;
    const char *p_description_param;
    int len_description;
    param_to_utf16(description, description_param, p_description_param, len_description);
    std::string author_param;
    const char *p_author_param;
    int len_author;
    param_to_utf16(author, author_param, p_author_param, len_author);
    std::string mimetype_param;
    const char *p_mimetype_param;
    param_to_bytes(mimetype, mimetype_param, p_mimetype_param);
    std::string icon_param;
    const char *p_icon_param;
    param_to_bytes(icon, icon_param, p_icon_param);

    PDFCPP_TRY {
	m_PDFlib_api->PDF_attach_file2(p, llx, lly, urx, ury, p_filename_param, len_filename, p_description_param, len_description, p_author_param, len_author, p_mimetype_param, p_icon_param);
    }
    PDFCPP_CATCH;
}

    
/* Create a new PDF file subject to various options. */
int
begin_document(const pstring& filename, const pstring& optlist)
{
    int retval = 0;

    std::string filename_param;
    const char *p_filename_param;
    int len_filename;
    param_to_0utf16(filename, filename_param, p_filename_param, len_filename);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_begin_document(p, p_filename_param, len_filename, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Create a new node in the document part hierarchy (requires PDF/VT or   PDF 2.0). */
void
begin_dpart(const pstring& optlist)
{
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	m_PDFlib_api->PDF_begin_dpart(p, p_optlist_param);
    }
    PDFCPP_CATCH;
}

    
/* Start a Type 3 font definition. */
void
begin_font(const pstring& fontname, double a, double b, double c, double d, double e, double f, const pstring& optlist)
{
    std::string fontname_param;
    const char *p_fontname_param;
    int len_fontname;
    param_to_0utf16(fontname, fontname_param, p_fontname_param, len_fontname);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	m_PDFlib_api->PDF_begin_font(p, p_fontname_param, len_fontname, a, b, c, d, e, f, p_optlist_param);
    }
    PDFCPP_CATCH;
}

    
/* Deprecated, use PDF_begin_glyph_ext(). */
PDFLIB_DEPRECATED(
void
begin_glyph(const pstring& glyphname, double wx, double llx, double lly, double urx, double ury)
)
{
    std::string glyphname_param;
    const char *p_glyphname_param;
    param_to_bytes(glyphname, glyphname_param, p_glyphname_param);

    PDFCPP_TRY {
	m_PDFlib_api->PDF_begin_glyph(p, p_glyphname_param, wx, llx, lly, urx, ury);
    }
    PDFCPP_CATCH;
}

    
/* Start a glyph definition for a Type 3 font. */
void
begin_glyph_ext(int uv, const pstring& optlist)
{
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	m_PDFlib_api->PDF_begin_glyph_ext(p, uv, p_optlist_param);
    }
    PDFCPP_CATCH;
}

    
/* Open a structure element or other content item with attributes supplied as options. */
int
begin_item(const pstring& tagname, const pstring& optlist)
{
    int retval = 0;

    std::string tagname_param;
    const char *p_tagname_param;
    param_to_bytes(tagname, tagname_param, p_tagname_param);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_begin_item(p, p_tagname_param, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Start a layer for subsequent output on the page (requires PDF 1.5). */
void
begin_layer(int layer)
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_begin_layer(p, layer);
    }
    PDFCPP_CATCH;
}

    
/* Begin a marked content sequence with optional properties. */
void
begin_mc(const pstring& tagname, const pstring& optlist)
{
    std::string tagname_param;
    const char *p_tagname_param;
    param_to_bytes(tagname, tagname_param, p_tagname_param);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	m_PDFlib_api->PDF_begin_mc(p, p_tagname_param, p_optlist_param);
    }
    PDFCPP_CATCH;
}

    
/* Deprecated, use PDF_begin_page_ext(). */
PDFLIB_DEPRECATED(
void
begin_page(double width, double height)
)
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_begin_page(p, width, height);
    }
    PDFCPP_CATCH;
}

    
/* Add a new page to the document, and specify various options. */
void
begin_page_ext(double width, double height, const pstring& optlist)
{
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	m_PDFlib_api->PDF_begin_page_ext(p, width, height, p_optlist_param);
    }
    PDFCPP_CATCH;
}

    
/* Deprecated, use PDF_begin_pattern_ext(). */
PDFLIB_DEPRECATED(
int
begin_pattern(double width, double height, double xstep, double ystep, int painttype)
)
{
    int retval = 0;

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_begin_pattern(p, width, height, xstep, ystep, painttype);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Start a pattern definition with options. */
int
begin_pattern_ext(double width, double height, const pstring& optlist)
{
    int retval = 0;

    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_begin_pattern_ext(p, width, height, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Deprecated, use PDF_begin_template_ext(). */
PDFLIB_DEPRECATED(
int
begin_template(double width, double height)
)
{
    int retval = 0;

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_begin_template(p, width, height);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Start a template definition. */
int
begin_template_ext(double width, double height, const pstring& optlist)
{
    int retval = 0;

    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_begin_template_ext(p, width, height, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Draw a circle. */
void
circle(double x, double y, double r)
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_circle(p, x, y, r);
    }
    PDFCPP_CATCH;
}

    
/* Draw a circular arc segment defined by three points. */
void
circular_arc(double x1, double y1, double x2, double y2)
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_circular_arc(p, x1, y1, x2, y2);
    }
    PDFCPP_CATCH;
}

    
/* Use the current path as clipping path, and terminate the path. */
void
clip()
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_clip(p);
    }
    PDFCPP_CATCH;
}

    
/* Deprecated, use PDF_end_document(). */
PDFLIB_DEPRECATED(
void
close()
)
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_close(p);
    }
    PDFCPP_CATCH;
}

    
/* Close an open font handle which has not yet been used in the document. */
void
close_font(int font)
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_close_font(p, font);
    }
    PDFCPP_CATCH;
}

    
/* Close vector graphics. */
void
close_graphics(int graphics)
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_close_graphics(p, graphics);
    }
    PDFCPP_CATCH;
}

    
/* Close an image or template. */
void
close_image(int image)
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_close_image(p, image);
    }
    PDFCPP_CATCH;
}

    
/* Deprecated, use PDF_close_pdi_document(). */
PDFLIB_DEPRECATED(
void
close_pdi(int doc)
)
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_close_pdi(p, doc);
    }
    PDFCPP_CATCH;
}

    
/* Close all open PDI page handles, and close the input PDF document. */
void
close_pdi_document(int doc)
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_close_pdi_document(p, doc);
    }
    PDFCPP_CATCH;
}

    
/* Close the page handle and free all page-related resources. */
void
close_pdi_page(int page)
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_close_pdi_page(p, page);
    }
    PDFCPP_CATCH;
}

    
/* Close the current path. */
void
closepath()
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_closepath(p);
    }
    PDFCPP_CATCH;
}

    
/* Close the path, fill, and stroke it. */
void
closepath_fill_stroke()
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_closepath_fill_stroke(p);
    }
    PDFCPP_CATCH;
}

    
/* Close the path, and stroke it. */
void
closepath_stroke()
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_closepath_stroke(p);
    }
    PDFCPP_CATCH;
}

    
/* Apply a transformation matrix to the current coordinate system. */
void
concat(double a, double b, double c, double d, double e, double f)
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_concat(p, a, b, c, d, e, f);
    }
    PDFCPP_CATCH;
}

    
/* Same as PDF_continue_text(), but with explicit string length. */
void
continue_text(const pstring& text)
{
    std::string text_param;
    const char *p_text_param;
    int len_text;
    param_to_utf16(text, text_param, p_text_param, len_text);

    PDFCPP_TRY {
	m_PDFlib_api->PDF_continue_text2(p, p_text_param, len_text);
    }
    PDFCPP_CATCH;
}

    
/* Create a 3D view (requires PDF 1.6). */
int
create_3dview(const pstring& username, const pstring& optlist)
{
    int retval = 0;

    std::string username_param;
    const char *p_username_param;
    int len_username;
    param_to_0utf16(username, username_param, p_username_param, len_username);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_create_3dview(p, p_username_param, len_username, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Create an action which can be applied to various objects and events. */
int
create_action(const pstring& type, const pstring& optlist)
{
    int retval = 0;

    std::string type_param;
    const char *p_type_param;
    param_to_bytes(type, type_param, p_type_param);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_create_action(p, p_type_param, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Create an annotation on the current page. */
void
create_annotation(double llx, double lly, double urx, double ury, const pstring& type, const pstring& optlist)
{
    std::string type_param;
    const char *p_type_param;
    param_to_bytes(type, type_param, p_type_param);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	m_PDFlib_api->PDF_create_annotation(p, llx, lly, urx, ury, p_type_param, p_optlist_param);
    }
    PDFCPP_CATCH;
}

    
/* Create a DeviceN colorspace with an arbitrary number of color components. */
int
create_devicen(const pstring& optlist)
{
    int retval = 0;

    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_create_devicen(p, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Create a bookmark subject to various options. */
int
create_bookmark(const pstring& text, const pstring& optlist)
{
    int retval = 0;

    std::string text_param;
    const char *p_text_param;
    int len_text;
    param_to_utf16(text, text_param, p_text_param, len_text);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_create_bookmark(p, p_text_param, len_text, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Create a form field on the current page subject to various options. */
void
create_field(double llx, double lly, double urx, double ury, const pstring& name, const pstring& type, const pstring& optlist)
{
    std::string name_param;
    const char *p_name_param;
    int len_name;
    param_to_utf16(name, name_param, p_name_param, len_name);
    std::string type_param;
    const char *p_type_param;
    param_to_bytes(type, type_param, p_type_param);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	m_PDFlib_api->PDF_create_field(p, llx, lly, urx, ury, p_name_param, len_name, p_type_param, p_optlist_param);
    }
    PDFCPP_CATCH;
}

    
/* Create a form field group subject to various options. */
void
create_fieldgroup(const pstring& name, const pstring& optlist)
{
    std::string name_param;
    const char *p_name_param;
    int len_name;
    param_to_utf16(name, name_param, p_name_param, len_name);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	m_PDFlib_api->PDF_create_fieldgroup(p, p_name_param, len_name, p_optlist_param);
    }
    PDFCPP_CATCH;
}

    
/* Create a graphics state object subject to various options. */
int
create_gstate(const pstring& optlist)
{
    int retval = 0;

    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_create_gstate(p, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Create a named virtual read-only file from data provided in memory. */
void
create_pvf(const pstring& filename, const void * data, size_t data_len, const pstring& optlist)
{
    std::string filename_param;
    const char *p_filename_param;
    int len_filename;
    param_to_0utf16(filename, filename_param, p_filename_param, len_filename);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	m_PDFlib_api->PDF_create_pvf(p, p_filename_param, len_filename, data, data_len, p_optlist_param);
    }
    PDFCPP_CATCH;
}

    
/* Create a Textflow object from text contents, inline options, and explicit options. */
int
create_textflow(const pstring& text, const pstring& optlist)
{
    int retval = 0;

    std::string text_param;
    const char *p_text_param;
    int len_text;
    param_to_utf16(text, text_param, p_text_param, len_text);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_create_textflow(p, p_text_param, len_text, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Draw a Bezier curve from the current point, using 3 more control points. */
void
curveto(double x1, double y1, double x2, double y2, double x3, double y3)
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_curveto(p, x1, y1, x2, y2, x3, y3);
    }
    PDFCPP_CATCH;
}

    
/* Create a new layer definition (requires PDF 1.5). */
int
define_layer(const pstring& name, const pstring& optlist)
{
    int retval = 0;

    std::string name_param;
    const char *p_name_param;
    int len_name;
    param_to_utf16(name, name_param, p_name_param, len_name);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_define_layer(p, p_name_param, len_name, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Delete a path object. */
void
delete_path(int path)
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_delete_path(p, path);
    }
    PDFCPP_CATCH;
}

    
/* Delete a named virtual file and free its data structures (but not the contents). */
int
delete_pvf(const pstring& filename)
{
    int retval = 0;

    std::string filename_param;
    const char *p_filename_param;
    int len_filename;
    param_to_0utf16(filename, filename_param, p_filename_param, len_filename);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_delete_pvf(p, p_filename_param, len_filename);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Delete a table and all associated data structures. */
void
delete_table(int table, const pstring& optlist)
{
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	m_PDFlib_api->PDF_delete_table(p, table, p_optlist_param);
    }
    PDFCPP_CATCH;
}

    
/* Delete a textflow and all associated data structures. */
void
delete_textflow(int textflow)
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_delete_textflow(p, textflow);
    }
    PDFCPP_CATCH;
}

    
/* Draw a path object. */
void
draw_path(int path, double x, double y, const pstring& optlist)
{
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	m_PDFlib_api->PDF_draw_path(p, path, x, y, p_optlist_param);
    }
    PDFCPP_CATCH;
}

    
/* Draw an ellipse. */
void
ellipse(double x, double y, double rx, double ry)
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_ellipse(p, x, y, rx, ry);
    }
    PDFCPP_CATCH;
}

    
/* Draw an elliptical arc segment from the current point. */
void
elliptical_arc(double x, double y, double rx, double ry, const pstring& optlist)
{
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	m_PDFlib_api->PDF_elliptical_arc(p, x, y, rx, ry, p_optlist_param);
    }
    PDFCPP_CATCH;
}

    
/* Add a glyph name and/or Unicode value to a custom 8-bit encoding. */
void
encoding_set_char(const pstring& encoding, int slot, const pstring& glyphname, int uv)
{
    std::string encoding_param;
    const char *p_encoding_param;
    param_to_bytes(encoding, encoding_param, p_encoding_param);
    std::string glyphname_param;
    const char *p_glyphname_param;
    param_to_bytes(glyphname, glyphname_param, p_glyphname_param);

    PDFCPP_TRY {
	m_PDFlib_api->PDF_encoding_set_char(p, p_encoding_param, slot, p_glyphname_param, uv);
    }
    PDFCPP_CATCH;
}

    
/* Close the generated PDF document and apply various options. */
void
end_document(const pstring& optlist)
{
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	m_PDFlib_api->PDF_end_document(p, p_optlist_param);
    }
    PDFCPP_CATCH;
}

    
/* Close a node in the document part hierarchy (requires PDF/VT or PDF 2.0). */
void
end_dpart(const pstring& optlist)
{
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	m_PDFlib_api->PDF_end_dpart(p, p_optlist_param);
    }
    PDFCPP_CATCH;
}

    
/* Terminate a Type 3 font definition. */
void
end_font()
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_end_font(p);
    }
    PDFCPP_CATCH;
}

    
/* Terminate a glyph definition for a Type 3 font. */
void
end_glyph()
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_end_glyph(p);
    }
    PDFCPP_CATCH;
}

    
/* Close a structure element or other content item. */
void
end_item(int id)
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_end_item(p, id);
    }
    PDFCPP_CATCH;
}

    
/* Deactivate all active layers (requires PDF 1.5). */
void
end_layer()
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_end_layer(p);
    }
    PDFCPP_CATCH;
}

    
/* End the least recently opened marked content sequence. */
void
end_mc()
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_end_mc(p);
    }
    PDFCPP_CATCH;
}

    
/* Deprecated, use PDF_end_page_ext(). */
PDFLIB_DEPRECATED(
void
end_page()
)
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_end_page(p);
    }
    PDFCPP_CATCH;
}

    
/* Finish a page, and apply various options. */
void
end_page_ext(const pstring& optlist)
{
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	m_PDFlib_api->PDF_end_page_ext(p, p_optlist_param);
    }
    PDFCPP_CATCH;
}

    
/* Finish a pattern definition. */
void
end_pattern()
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_end_pattern(p);
    }
    PDFCPP_CATCH;
}

    
/* Deprecated, use PDF_end_template_ext(). */
PDFLIB_DEPRECATED(
void
end_template()
)
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_end_template(p);
    }
    PDFCPP_CATCH;
}

    
/* Finish a template definition. */
void
end_template_ext(double width, double height)
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_end_template_ext(p, width, height);
    }
    PDFCPP_CATCH;
}

    
/* End the current path without filling or stroking it. */
void
endpath()
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_endpath(p);
    }
    PDFCPP_CATCH;
}

    
/* Fill the interior of the path with the current fill color. */
void
fill()
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_fill(p);
    }
    PDFCPP_CATCH;
}

    
/* Fill a graphics block with variable data according to its properties. */
int
fill_graphicsblock(int page, const pstring& blockname, int graphics, const pstring& optlist)
{
    int retval = 0;

    std::string blockname_param;
    const char *p_blockname_param;
    param_to_utf8(blockname, blockname_param, p_blockname_param);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_fill_graphicsblock(p, page, p_blockname_param, graphics, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Fill an image block with variable data according to its properties. */
int
fill_imageblock(int page, const pstring& blockname, int image, const pstring& optlist)
{
    int retval = 0;

    std::string blockname_param;
    const char *p_blockname_param;
    param_to_utf8(blockname, blockname_param, p_blockname_param);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_fill_imageblock(p, page, p_blockname_param, image, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Fill a PDF block with variable data according to its properties. */
int
fill_pdfblock(int page, const pstring& blockname, int contents, const pstring& optlist)
{
    int retval = 0;

    std::string blockname_param;
    const char *p_blockname_param;
    param_to_utf8(blockname, blockname_param, p_blockname_param);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_fill_pdfblock(p, page, p_blockname_param, contents, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Fill a Textline or Textflow Block with variable data according to its properties. */
void
fill_stroke()
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_fill_stroke(p);
    }
    PDFCPP_CATCH;
}

    
/* Fill a Textline or Textflow Block with variable data according to its properties. */
int
fill_textblock(int page, const pstring& blockname, const pstring& text, const pstring& optlist)
{
    int retval = 0;

    std::string blockname_param;
    const char *p_blockname_param;
    param_to_utf8(blockname, blockname_param, p_blockname_param);
    std::string text_param;
    const char *p_text_param;
    int len_text;
    param_to_utf16(text, text_param, p_text_param, len_text);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_fill_textblock(p, page, p_blockname_param, p_text_param, len_text, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Deprecated, use  PDF_load_font(). */
PDFLIB_DEPRECATED(
int
findfont(const pstring& fontname, const pstring& encoding, int embed)
)
{
    int retval = 0;

    std::string fontname_param;
    const char *p_fontname_param;
    param_to_bytes(fontname, fontname_param, p_fontname_param);
    std::string encoding_param;
    const char *p_encoding_param;
    param_to_bytes(encoding, encoding_param, p_encoding_param);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_findfont(p, p_fontname_param, p_encoding_param, embed);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Place vector graphics on a content stream, subject to various options. */
void
fit_graphics(int graphics, double x, double y, const pstring& optlist)
{
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	m_PDFlib_api->PDF_fit_graphics(p, graphics, x, y, p_optlist_param);
    }
    PDFCPP_CATCH;
}

    
/* Place an image or template on the page, subject to various options. */
void
fit_image(int image, double x, double y, const pstring& optlist)
{
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	m_PDFlib_api->PDF_fit_image(p, image, x, y, p_optlist_param);
    }
    PDFCPP_CATCH;
}

    
/* Place an imported PDF page on the page subject to various options. */
void
fit_pdi_page(int page, double x, double y, const pstring& optlist)
{
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	m_PDFlib_api->PDF_fit_pdi_page(p, page, x, y, p_optlist_param);
    }
    PDFCPP_CATCH;
}

    
/* Fully or partially place a table on the page. */
pstring
fit_table(int table, double llx, double lly, double urx, double ury, const pstring& optlist)
{
    const char *retval = NULL;
    pstring pstring_retval;

    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_fit_table(p, table, llx, lly, urx, ury, p_optlist_param);
    }
    PDFCPP_CATCH;

    apiretval_to_pstring(retval, pstring_retval);

    return pstring_retval;
}

    
/* Format the next portion of a Textflow. */
pstring
fit_textflow(int textflow, double llx, double lly, double urx, double ury, const pstring& optlist)
{
    const char *retval = NULL;
    pstring pstring_retval;

    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_fit_textflow(p, textflow, llx, lly, urx, ury, p_optlist_param);
    }
    PDFCPP_CATCH;

    apiretval_to_pstring(retval, pstring_retval);

    return pstring_retval;
}

    
/* Place a single line of text at position (x, y) subject to various options. */
void
fit_textline(const pstring& text, double x, double y, const pstring& optlist)
{
    std::string text_param;
    const char *p_text_param;
    int len_text;
    param_to_utf16(text, text_param, p_text_param, len_text);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	m_PDFlib_api->PDF_fit_textline(p, p_text_param, len_text, x, y, p_optlist_param);
    }
    PDFCPP_CATCH;
}

    
/* Get the name of the API function which threw the last exception or failed. */
pstring
get_apiname()
{
    const char *retval = NULL;
    pstring pstring_retval;

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_get_apiname(p);
    }
    PDFCPP_CATCH;

    apiretval_to_pstring(retval, pstring_retval);

    return pstring_retval;
}

    
/* Get the contents of the PDF output buffer. */
const char *
get_buffer(long *outputlen)
{
    const char *retval = NULL;

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_get_buffer(p, outputlen);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Get the text of the last thrown exception or the reason of a failed function call. */
pstring
get_errmsg()
{
    const char *retval = NULL;
    pstring pstring_retval;

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_get_errmsg(p);
    }
    PDFCPP_CATCH;

    apiretval_to_pstring(retval, pstring_retval);

    return pstring_retval;
}

    
/* Get the number of the last thrown exception or the reason of a failed function call. */
int
get_errnum()
{
    int retval = 0;

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_get_errnum(p);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Retrieve some option or other value. */
double
get_option(const pstring& keyword, const pstring& optlist)
{
    double retval = 0;

    std::string keyword_param;
    const char *p_keyword_param;
    param_to_bytes(keyword, keyword_param, p_keyword_param);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_get_option(p, p_keyword_param, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Deprecated, use PDF_get_option() and PDF_get_string(). */
PDFLIB_DEPRECATED(
pstring
get_parameter(const pstring& key, double modifier)
)
{
    const char *retval = NULL;
    pstring pstring_retval;

    std::string key_param;
    const char *p_key_param;
    param_to_bytes(key, key_param, p_key_param);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_get_parameter(p, p_key_param, modifier);
    }
    PDFCPP_CATCH;

    apiretval_to_pstring(retval, pstring_retval);

    return pstring_retval;
}

    
/* Deprecated, use PDF_pcos_get_number(). */
PDFLIB_DEPRECATED(
double
get_pdi_value(const pstring& key, int doc, int page, int reserved)
)
{
    double retval = 0;

    std::string key_param;
    const char *p_key_param;
    param_to_bytes(key, key_param, p_key_param);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_get_pdi_value(p, p_key_param, doc, page, reserved);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Retrieve a string value. */
pstring
get_string(int idx, const pstring& optlist)
{
    const char *retval = NULL;
    pstring pstring_retval;

    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_get_string(p, idx, p_optlist_param);
    }
    PDFCPP_CATCH;

    apiretval_to_pstring(retval, pstring_retval);

    return pstring_retval;
}

    
/* Deprecated, use PDF_get_option(). */
PDFLIB_DEPRECATED(
double
get_value(const pstring& key, double modifier)
)
{
    double retval = 0;

    std::string key_param;
    const char *p_key_param;
    param_to_bytes(key, key_param, p_key_param);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_get_value(p, p_key_param, modifier);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Query detailed information about a loaded font. */
double
info_font(int font, const pstring& keyword, const pstring& optlist)
{
    double retval = 0;

    std::string keyword_param;
    const char *p_keyword_param;
    param_to_bytes(keyword, keyword_param, p_keyword_param);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_info_font(p, font, p_keyword_param, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Format vector graphics and query metrics and other properties. */
double
info_graphics(int graphics, const pstring& keyword, const pstring& optlist)
{
    double retval = 0;

    std::string keyword_param;
    const char *p_keyword_param;
    param_to_bytes(keyword, keyword_param, p_keyword_param);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_info_graphics(p, graphics, p_keyword_param, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Format an image and query metrics and other image properties. */
double
info_image(int image, const pstring& keyword, const pstring& optlist)
{
    double retval = 0;

    std::string keyword_param;
    const char *p_keyword_param;
    param_to_bytes(keyword, keyword_param, p_keyword_param);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_info_image(p, image, p_keyword_param, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Query information about a matchbox on the current page. */
double
info_matchbox(const pstring& boxname, int num, const pstring& keyword)
{
    double retval = 0;

    std::string boxname_param;
    const char *p_boxname_param;
    int len_boxname;
    param_to_0utf16(boxname, boxname_param, p_boxname_param, len_boxname);
    std::string keyword_param;
    const char *p_keyword_param;
    param_to_bytes(keyword, keyword_param, p_keyword_param);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_info_matchbox(p, p_boxname_param, len_boxname, num, p_keyword_param);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Query the results of drawing a path object without actually drawing it. */
double
info_path(int path, const pstring& keyword, const pstring& optlist)
{
    double retval = 0;

    std::string keyword_param;
    const char *p_keyword_param;
    param_to_bytes(keyword, keyword_param, p_keyword_param);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_info_path(p, path, p_keyword_param, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Perform formatting calculations for a PDI page and query the resulting metrics. */
double
info_pdi_page(int page, const pstring& keyword, const pstring& optlist)
{
    double retval = 0;

    std::string keyword_param;
    const char *p_keyword_param;
    param_to_bytes(keyword, keyword_param, p_keyword_param);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_info_pdi_page(p, page, p_keyword_param, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Query properties of a virtual file or the PDFlib Virtual Filesystem (PVF). */
double
info_pvf(const pstring& filename, const pstring& keyword)
{
    double retval = 0;

    std::string filename_param;
    const char *p_filename_param;
    int len_filename;
    param_to_0utf16(filename, filename_param, p_filename_param, len_filename);
    std::string keyword_param;
    const char *p_keyword_param;
    param_to_bytes(keyword, keyword_param, p_keyword_param);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_info_pvf(p, p_filename_param, len_filename, p_keyword_param);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Query table information related to the most recently placed table instance. */
double
info_table(int table, const pstring& keyword)
{
    double retval = 0;

    std::string keyword_param;
    const char *p_keyword_param;
    param_to_bytes(keyword, keyword_param, p_keyword_param);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_info_table(p, table, p_keyword_param);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Query the current state of a Textflow. */
double
info_textflow(int textflow, const pstring& keyword)
{
    double retval = 0;

    std::string keyword_param;
    const char *p_keyword_param;
    param_to_bytes(keyword, keyword_param, p_keyword_param);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_info_textflow(p, textflow, p_keyword_param);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Perform textline formatting without creating output and query the resulting metrics. */
double
info_textline(const pstring& text, const pstring& keyword, const pstring& optlist)
{
    double retval = 0;

    std::string text_param;
    const char *p_text_param;
    int len_text;
    param_to_utf16(text, text_param, p_text_param, len_text);
    std::string keyword_param;
    const char *p_keyword_param;
    param_to_bytes(keyword, keyword_param, p_keyword_param);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_info_textline(p, p_text_param, len_text, p_keyword_param, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Deprecated, use PDF_set_graphics_option(). */
PDFLIB_DEPRECATED(
void
initgraphics()
)
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_initgraphics(p);
    }
    PDFCPP_CATCH;
}

    
/* Draw a line from the current point to another point. */
void
lineto(double x, double y)
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_lineto(p, x, y);
    }
    PDFCPP_CATCH;
}

    
/* Load a 3D model from a disk-based or virtual file (requires PDF 1.6). */
int
load_3ddata(const pstring& filename, const pstring& optlist)
{
    int retval = 0;

    std::string filename_param;
    const char *p_filename_param;
    int len_filename;
    param_to_0utf16(filename, filename_param, p_filename_param, len_filename);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_load_3ddata(p, p_filename_param, len_filename, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Load a rich media asset or file attachment from a disk-based or virtual file. */
int
load_asset(const pstring& type, const pstring& filename, const pstring& optlist)
{
    int retval = 0;

    std::string type_param;
    const char *p_type_param;
    param_to_bytes(type, type_param, p_type_param);
    std::string filename_param;
    const char *p_filename_param;
    int len_filename;
    param_to_0utf16(filename, filename_param, p_filename_param, len_filename);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_load_asset(p, p_type_param, p_filename_param, len_filename, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Search for a font and prepare it for later use. */
int
load_font(const pstring& fontname, const pstring& encoding, const pstring& optlist)
{
    int retval = 0;

    std::string fontname_param;
    const char *p_fontname_param;
    int len_fontname;
    param_to_0utf16(fontname, fontname_param, p_fontname_param, len_fontname);
    std::string encoding_param;
    const char *p_encoding_param;
    param_to_bytes(encoding, encoding_param, p_encoding_param);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_load_font(p, p_fontname_param, len_fontname, p_encoding_param, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Open a disk-based or virtual vector graphics file subject to various options. */
int
load_graphics(const pstring& type, const pstring& filename, const pstring& optlist)
{
    int retval = 0;

    std::string type_param;
    const char *p_type_param;
    param_to_bytes(type, type_param, p_type_param);
    std::string filename_param;
    const char *p_filename_param;
    int len_filename;
    param_to_0utf16(filename, filename_param, p_filename_param, len_filename);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_load_graphics(p, p_type_param, p_filename_param, len_filename, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Search for an ICC profile, and prepare it for later use. */
int
load_iccprofile(const pstring& profilename, const pstring& optlist)
{
    int retval = 0;

    std::string profilename_param;
    const char *p_profilename_param;
    int len_profilename;
    param_to_0utf16(profilename, profilename_param, p_profilename_param, len_profilename);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_load_iccprofile(p, p_profilename_param, len_profilename, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Open a disk-based or virtual image file subject to various options. */
int
load_image(const pstring& imagetype, const pstring& filename, const pstring& optlist)
{
    int retval = 0;

    std::string imagetype_param;
    const char *p_imagetype_param;
    param_to_bytes(imagetype, imagetype_param, p_imagetype_param);
    std::string filename_param;
    const char *p_filename_param;
    int len_filename;
    param_to_0utf16(filename, filename_param, p_filename_param, len_filename);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_load_image(p, p_imagetype_param, p_filename_param, len_filename, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Find a built-in spot color name, or make a named spot color from the current fill color. */
int
makespotcolor(const pstring& spotname)
{
    int retval = 0;

    std::string spotname_param;
    const char *p_spotname_param;
    param_to_utf8(spotname, spotname_param, p_spotname_param);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_makespotcolor(p, p_spotname_param, 0);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Add a marked content point with optional properties. */
void
mc_point(const pstring& tagname, const pstring& optlist)
{
    std::string tagname_param;
    const char *p_tagname_param;
    param_to_bytes(tagname, tagname_param, p_tagname_param);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	m_PDFlib_api->PDF_mc_point(p, p_tagname_param, p_optlist_param);
    }
    PDFCPP_CATCH;
}

    
/* Set the current point for graphics output. */
void
moveto(double x, double y)
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_moveto(p, x, y);
    }
    PDFCPP_CATCH;
}

    
/* Deprecated, use PDF_load_image(). */
PDFLIB_DEPRECATED(
int
open_CCITT(const pstring& filename, int width, int height, int BitReverse, int K, int BlackIs1)
)
{
    int retval = 0;

    std::string filename_param;
    const char *p_filename_param;
    param_to_bytes(filename, filename_param, p_filename_param);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_open_CCITT(p, p_filename_param, width, height, BitReverse, K, BlackIs1);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Deprecated, use PDF_begin_document(). */
PDFLIB_DEPRECATED(
int
open_file(const pstring& filename)
)
{
    int retval = 0;

    std::string filename_param;
    const char *p_filename_param;
    param_to_bytes(filename, filename_param, p_filename_param);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_open_file(p, p_filename_param);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Deprecated, use PDF_load_image(). */
PDFLIB_DEPRECATED(
int
open_image_file(const pstring& imagetype, const pstring& filename, const pstring& stringparam, int intparam)
)
{
    int retval = 0;

    std::string imagetype_param;
    const char *p_imagetype_param;
    param_to_bytes(imagetype, imagetype_param, p_imagetype_param);
    std::string filename_param;
    const char *p_filename_param;
    param_to_bytes(filename, filename_param, p_filename_param);
    std::string stringparam_param;
    const char *p_stringparam_param;
    param_to_bytes(stringparam, stringparam_param, p_stringparam_param);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_open_image_file(p, p_imagetype_param, p_filename_param, p_stringparam_param, intparam);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Open a disk-based or virtual PDF document and prepare it for later use. */
int
open_pdi_document(const pstring& filename, const pstring& optlist)
{
    int retval = 0;

    std::string filename_param;
    const char *p_filename_param;
    int len_filename;
    param_to_0utf16(filename, filename_param, p_filename_param, len_filename);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_open_pdi_document(p, p_filename_param, len_filename, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Prepare a page for later use with PDF_fit_pdi_page(). */
int
open_pdi_page(int doc, int pagenumber, const pstring& optlist)
{
    int retval = 0;

    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_open_pdi_page(p, doc, pagenumber, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Get the value of a pCOS path with type number or boolean. */
double
pcos_get_number(int doc, const pstring& path)
{
    double retval = 0;

    std::string path_param;
    const char *p_path_param;
    param_to_utf8(path, path_param, p_path_param);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_pcos_get_number(p, doc, "%s", p_path_param);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Get the value of a pCOS path with type name, number, string, or boolean. */
pstring
pcos_get_string(int doc, const pstring& path)
{
    const char *retval = NULL;
    pstring pstring_retval;

    std::string path_param;
    const char *p_path_param;
    param_to_utf8(path, path_param, p_path_param);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_pcos_get_string(p, doc, "%s", p_path_param);
    }
    PDFCPP_CATCH;

    apiretval_to_pstring(retval, pstring_retval);

    return pstring_retval;
}

    
/* Get the contents of a pCOS path with type stream, fstream, or string. */
const unsigned char *
pcos_get_stream(int doc, int *outputlen, const pstring& optlist, const pstring& path)
{
    const unsigned char *retval = NULL;

    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);
    std::string path_param;
    const char *p_path_param;
    param_to_utf8(path, path_param, p_path_param);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_pcos_get_stream(p, doc, outputlen, p_optlist_param, "%s", p_path_param);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Deprecated, use PDF_fit_image(). */
PDFLIB_DEPRECATED(
void
place_image(int image, double x, double y, double scale)
)
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_place_image(p, image, x, y, scale);
    }
    PDFCPP_CATCH;
}

    
/* Deprecated, use PDF_fit_pdi_page(). */
PDFLIB_DEPRECATED(
void
place_pdi_page(int page, double x, double y, double sx, double sy)
)
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_place_pdi_page(p, page, x, y, sx, sy);
    }
    PDFCPP_CATCH;
}

    
/* Delete a PDF container object. */
void
poca_delete(int container, const pstring& optlist)
{
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	m_PDFlib_api->PDF_poca_delete(p, container, p_optlist_param);
    }
    PDFCPP_CATCH;
}

    
/* Insert a simple or container object in a PDF container object. */
void
poca_insert(int container, const pstring& optlist)
{
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	m_PDFlib_api->PDF_poca_insert(p, container, p_optlist_param);
    }
    PDFCPP_CATCH;
}

    
/* Create a new PDF container object of type dictionary, array, or stream and insert objects. */
int
poca_new(const pstring& optlist)
{
    int retval = 0;

    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_poca_new(p, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Remove a simple or container object from a PDF container object. */
void
poca_remove(int container, const pstring& optlist)
{
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	m_PDFlib_api->PDF_poca_remove(p, container, p_optlist_param);
    }
    PDFCPP_CATCH;
}

    
/* Process certain elements of an imported PDF document. */
int
process_pdi(int doc, int page, const pstring& optlist)
{
    int retval = 0;

    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_process_pdi(p, doc, page, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Draw a rectangle. */
void
rect(double x, double y, double width, double height)
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_rect(p, x, y, width, height);
    }
    PDFCPP_CATCH;
}

    
/* Restore the most recently saved graphics state from the stack. */
void
restore()
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_restore(p);
    }
    PDFCPP_CATCH;
}

    
/* Resume a page to add more content to it. */
void
resume_page(const pstring& optlist)
{
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	m_PDFlib_api->PDF_resume_page(p, p_optlist_param);
    }
    PDFCPP_CATCH;
}

    
/* Rotate the coordinate system. */
void
rotate(double phi)
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_rotate(p, phi);
    }
    PDFCPP_CATCH;
}

    
/* Save the current graphics state to a stack. */
void
save()
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_save(p);
    }
    PDFCPP_CATCH;
}

    
/* Scale the coordinate system. */
void
scale(double sx, double sy)
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_scale(p, sx, sy);
    }
    PDFCPP_CATCH;
}

    
/* Deprecated, use PDF_create_annotation(). */
PDFLIB_DEPRECATED(
void
set_border_color(double red, double green, double blue)
)
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_set_border_color(p, red, green, blue);
    }
    PDFCPP_CATCH;
}

    
/* Deprecated, use PDF_create_annotation(). */
PDFLIB_DEPRECATED(
void
set_border_dash(double b, double w)
)
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_set_border_dash(p, b, w);
    }
    PDFCPP_CATCH;
}

    
/* Deprecated, use PDF_create_annotation(). */
PDFLIB_DEPRECATED(
void
set_border_style(const pstring& style, double width)
)
{
    std::string style_param;
    const char *p_style_param;
    param_to_bytes(style, style_param, p_style_param);

    PDFCPP_TRY {
	m_PDFlib_api->PDF_set_border_style(p, p_style_param, width);
    }
    PDFCPP_CATCH;
}

    
/* Set one or more graphics appearance options. */
void
set_graphics_option(const pstring& optlist)
{
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	m_PDFlib_api->PDF_set_graphics_option(p, p_optlist_param);
    }
    PDFCPP_CATCH;
}

    
/* Activate a graphics state object. */
void
set_gstate(int gstate)
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_set_gstate(p, gstate);
    }
    PDFCPP_CATCH;
}

    
/* Like PDF_set_info(), but with explicit string length. */
void
set_info(const pstring& key, const pstring& value)
{
    std::string key_param;
    const char *p_key_param;
    param_to_utf8(key, key_param, p_key_param);
    std::string value_param;
    const char *p_value_param;
    int len_value;
    param_to_utf16(value, value_param, p_value_param, len_value);

    PDFCPP_TRY {
	m_PDFlib_api->PDF_set_info2(p, p_key_param, p_value_param, len_value);
    }
    PDFCPP_CATCH;
}

    
/* Define layer relationships (requires PDF 1.5). */
void
set_layer_dependency(const pstring& type, const pstring& optlist)
{
    std::string type_param;
    const char *p_type_param;
    param_to_bytes(type, type_param, p_type_param);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	m_PDFlib_api->PDF_set_layer_dependency(p, p_type_param, p_optlist_param);
    }
    PDFCPP_CATCH;
}

    
/* Set one or more global options. */
void
set_option(const pstring& optlist)
{
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	m_PDFlib_api->PDF_set_option(p, p_optlist_param);
    }
    PDFCPP_CATCH;
}

    
/* Deprecated, use PDF_set_option(), PDF_set_text_option() and PDF_set_graphics_option(). */
PDFLIB_DEPRECATED(
void
set_parameter(const pstring& key, const pstring& value)
)
{
    std::string key_param;
    const char *p_key_param;
    param_to_bytes(key, key_param, p_key_param);
    std::string value_param;
    const char *p_value_param;
    param_to_utf8(value, value_param, p_value_param);

    PDFCPP_TRY {
	m_PDFlib_api->PDF_set_parameter(p, p_key_param, p_value_param);
    }
    PDFCPP_CATCH;
}

    
/* Set one or more text filter or text appearance options for simple text output functions. */
void
set_text_option(const pstring& optlist)
{
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	m_PDFlib_api->PDF_set_text_option(p, p_optlist_param);
    }
    PDFCPP_CATCH;
}

    
/* Set the position for simple text output on the page. */
void
set_text_pos(double x, double y)
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_set_text_pos(p, x, y);
    }
    PDFCPP_CATCH;
}

    
/* Deprecated, use PDF_set_option(), PDF_set_text_option() and PDF_set_graphics_option().. */
PDFLIB_DEPRECATED(
void
set_value(const pstring& key, double value)
)
{
    std::string key_param;
    const char *p_key_param;
    param_to_bytes(key, key_param, p_key_param);

    PDFCPP_TRY {
	m_PDFlib_api->PDF_set_value(p, p_key_param, value);
    }
    PDFCPP_CATCH;
}

    
/* Set the color space and color for the graphics and text state.. */
void
setcolor(const pstring& fstype, const pstring& colorspace, double c1, double c2, double c3, double c4)
{
    std::string fstype_param;
    const char *p_fstype_param;
    param_to_bytes(fstype, fstype_param, p_fstype_param);
    std::string colorspace_param;
    const char *p_colorspace_param;
    param_to_bytes(colorspace, colorspace_param, p_colorspace_param);

    PDFCPP_TRY {
	m_PDFlib_api->PDF_setcolor(p, p_fstype_param, p_colorspace_param, c1, c2, c3, c4);
    }
    PDFCPP_CATCH;
}

    
/* Deprecated, use PDF_set_graphics_option(). */
PDFLIB_DEPRECATED(
void
setdash(double b, double w)
)
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_setdash(p, b, w);
    }
    PDFCPP_CATCH;
}

    
/* Deprecated, use PDF_set_graphics_option(). */
PDFLIB_DEPRECATED(
void
setdashpattern(const pstring& optlist)
)
{
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	m_PDFlib_api->PDF_setdashpattern(p, p_optlist_param);
    }
    PDFCPP_CATCH;
}

    
/* Deprecated, use PDF_set_graphics_option(). */
PDFLIB_DEPRECATED(
void
setflat(double flatness)
)
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_setflat(p, flatness);
    }
    PDFCPP_CATCH;
}

    
/* Set the current font in the specified size. */
void
setfont(int font, double fontsize)
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_setfont(p, font, fontsize);
    }
    PDFCPP_CATCH;
}

    
/* Deprecated, use PDF_setcolor(). */
PDFLIB_DEPRECATED(
void
setgray(double gray)
)
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_setgray(p, gray);
    }
    PDFCPP_CATCH;
}

    
/* Deprecated, use PDF_setcolor(). */
PDFLIB_DEPRECATED(
void
setgray_fill(double gray)
)
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_setgray_fill(p, gray);
    }
    PDFCPP_CATCH;
}

    
/* Deprecated, use PDF_setcolor(). */
PDFLIB_DEPRECATED(
void
setgray_stroke(double gray)
)
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_setgray_stroke(p, gray);
    }
    PDFCPP_CATCH;
}

    
/* Deprecated, use PDF_set_graphics_option(). */
PDFLIB_DEPRECATED(
void
setlinecap(int linecap)
)
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_setlinecap(p, linecap);
    }
    PDFCPP_CATCH;
}

    
/* Deprecated, use PDF_set_graphics_option(). */
PDFLIB_DEPRECATED(
void
setlinejoin(int linejoin)
)
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_setlinejoin(p, linejoin);
    }
    PDFCPP_CATCH;
}

    
/* Set the current linewidth. */
void
setlinewidth(double width)
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_setlinewidth(p, width);
    }
    PDFCPP_CATCH;
}

    
/* Explicitly set the current transformation matrix. */
void
setmatrix(double a, double b, double c, double d, double e, double f)
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_setmatrix(p, a, b, c, d, e, f);
    }
    PDFCPP_CATCH;
}

    
/* Deprecated, use PDF_set_graphics_option(). */
PDFLIB_DEPRECATED(
void
setmiterlimit(double miter)
)
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_setmiterlimit(p, miter);
    }
    PDFCPP_CATCH;
}

    
/* Deprecated, use PDF_setcolor(). */
PDFLIB_DEPRECATED(
void
setrgbcolor(double red, double green, double blue)
)
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_setrgbcolor(p, red, green, blue);
    }
    PDFCPP_CATCH;
}

    
/* Deprecated, use PDF_setcolor(). */
PDFLIB_DEPRECATED(
void
setrgbcolor_fill(double red, double green, double blue)
)
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_setrgbcolor_fill(p, red, green, blue);
    }
    PDFCPP_CATCH;
}

    
/* Deprecated, use PDF_setcolor(). */
PDFLIB_DEPRECATED(
void
setrgbcolor_stroke(double red, double green, double blue)
)
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_setrgbcolor_stroke(p, red, green, blue);
    }
    PDFCPP_CATCH;
}

    
/* Define a shading (color gradient) between two or more colors. */
int
shading(const pstring& type, double x0, double y0, double x1, double y1, double c1, double c2, double c3, double c4, const pstring& optlist)
{
    int retval = 0;

    std::string type_param;
    const char *p_type_param;
    param_to_bytes(type, type_param, p_type_param);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_shading(p, p_type_param, x0, y0, x1, y1, c1, c2, c3, c4, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Define a shading pattern using a shading object. */
int
shading_pattern(int shading, const pstring& optlist)
{
    int retval = 0;

    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_shading_pattern(p, shading, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Fill an area with a shading, based on a shading object. */
void
shfill(int shading)
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_shfill(p, shading);
    }
    PDFCPP_CATCH;
}

    
/* Same as PDF_show() but with explicit string length. */
void
show(const pstring& text)
{
    std::string text_param;
    const char *p_text_param;
    int len_text;
    param_to_utf16(text, text_param, p_text_param, len_text);

    PDFCPP_TRY {
	m_PDFlib_api->PDF_show2(p, p_text_param, len_text);
    }
    PDFCPP_CATCH;
}

    
/* Same as PDF_show_xy() but with explicit string length. */
void
show_xy(const pstring& text, double x, double y)
{
    std::string text_param;
    const char *p_text_param;
    int len_text;
    param_to_utf16(text, text_param, p_text_param, len_text);

    PDFCPP_TRY {
	m_PDFlib_api->PDF_show_xy2(p, p_text_param, len_text, x, y);
    }
    PDFCPP_CATCH;
}

    
/* Skew the coordinate system. */
void
skew(double alpha, double beta)
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_skew(p, alpha, beta);
    }
    PDFCPP_CATCH;
}

    
/* Same as PDF_stringwidth(), but with explicit string length. */
double
stringwidth(const pstring& text, int font, double fontsize)
{
    double retval = 0;

    std::string text_param;
    const char *p_text_param;
    int len_text;
    param_to_utf16(text, text_param, p_text_param, len_text);

    PDFCPP_TRY {
	retval = m_PDFlib_api->PDF_stringwidth2(p, p_text_param, len_text, font, fontsize);
    }
    PDFCPP_CATCH;

    return retval;
}

    
/* Stroke the path with the current color and line width, and clear it. */
void
stroke()
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_stroke(p);
    }
    PDFCPP_CATCH;
}

    
/* Suspend the current page so that it can later be resumed. */
void
suspend_page(const pstring& optlist)
{
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	m_PDFlib_api->PDF_suspend_page(p, p_optlist_param);
    }
    PDFCPP_CATCH;
}

    
/* Translate the origin of the coordinate system. */
void
translate(double tx, double ty)
{
    PDFCPP_TRY {
	m_PDFlib_api->PDF_translate(p, tx, ty);
    }
    PDFCPP_CATCH;
}
