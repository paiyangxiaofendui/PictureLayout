#pragma once
#include "Export.h"
#include "XmlNode.h"


namespace TinyXml
{
    class TiXmlDocument;
}

namespace hg3d {
    class HG3D_EXPORT CXmlFile : public CXmlNode
    {
    public:
        CXmlFile(void);
        ~CXmlFile(void);

        bool Load(const TCHAR* pszFile);
        bool Save(const TCHAR* pszFile);
        bool IsValid(void) { return m_bValid; }
        void SetModified(bool flag=true) { m_bModified = flag; }
    protected:
        virtual bool OnLoad(const TCHAR* pszFile);

    private:
        std::unique_ptr<TinyXml::TiXmlDocument> m_doc;
        bool m_bValid;
        bool m_bModified;
    };
}