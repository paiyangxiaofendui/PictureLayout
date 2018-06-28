#pragma once

#include <vector>
#include "../TinyXml/tinyxml.h"
#include "hgtypes.h"
#include "Export.h"


namespace hg3d {
    class CXmlNode;
    typedef std::vector<CXmlNode*> XmlNodeVec;

    class HG3D_EXPORT CXmlNode
    {
    public:
        typedef CXmlNode MyType;
        typedef CXmlNode NodeType;
        typedef std::vector<CXmlNode*> NodeSet;

        CXmlNode(void);
        CXmlNode(const CXmlNode&);
        virtual ~CXmlNode(void);

        static MyType* NewElementNode(const hg3d::TString& strNodeName);

        //
        CXmlNode& operator=(const CXmlNode& src);

        // overwrite
        virtual bool Parse(TinyXml::TiXmlNode* pNode);
        virtual bool Parse(const TCHAR* pszString);
        virtual TinyXml::TiXmlNode* ToNode();


        // attribute
        const tstring& GetAttributeValue(const TCHAR* pszName) const;
        const tstring& GetName() const;
        const XmlNodeVec& GetChildNodes() const;
        const bool IsTextNode() const;
        StringStringMap& GetAttributes() { return m_mapAttrs; }
        
        bool SetAttributeValue(const TCHAR* pszName, const TCHAR* pszValue);
        bool SetAttributeValue(const tstring& strName, const tstring& strValue);
        bool AddAttribute(const TCHAR* pszName, const TCHAR* pszValue);
        bool AddAttribute(const tstring& strName, const tstring& strValue);
        void SetName(const TCHAR* pszName);
        void SetName(const tstring& strName);


        MyType* GetParent(void) const { return m_parent; }

        /**
        * @brief 返回第一个符合指定属性的子节点
        * @param pszAttributeName 属性名称
        * @param pszAttributeValue 属性值
        * @retval NULL 符合条件的节点不存在
        * @retval !NULL 子节点指针
        */
        const NodeType* GetChildByAttribute(const TCHAR* pszAttributeName, const TCHAR* pszAttributeValue) const;
        NodeType* GetChildByAttribute(const TCHAR* pszAttributeName, const TCHAR* pszAttributeValue);

        /**
        * @brief 返回路径指定的节点
        * @param pszPath 从当前节点开始的绝对路径，如“/users/user/name"
        * @retval NULL 路径指定的节点不存在
        * @retval !NULL 路径指定的节点指针
        */
        CXmlNode* SelectChild(const TCHAR* pszPath);

        /**
        * @brief 返回指定路径的所有子节点
        * @param pszPath 从当前节点开始的绝对路径，如“/users"
        * @return 子节点集合的引用
        */
        NodeSet& SelectChildren(const TCHAR* pszPath);

        /**
        * @brief 返回指定路径下匹配指定属性的所有子节点
        * @param pszPath 从当前节点开始的绝对路径，如“/users"
        * @param pszAttribute 节点属性的名称
        * @param pszAttributeValue 节点属性的值
        * @return 子节点集合
        */
        XmlNodeVec SelectChildren(const TCHAR* pszPath, const TCHAR* pszAttribute, const TCHAR* pszAttributeValue);

        /**
        * @breif 添加子节点
        * @param pNode 子节点指针
        * @retval true 成功
        * @retval false 失败
        */
        bool AddChild(CXmlNode *pNode);

        /**
        * @breif 删除子节点
        * @param attributeName 子节点属性名
        * @param attributeValue 子节点属性值
        * @param recursive 是否递归访问子节点
        * @retval true 成功
        * @retval false 失败
        */
        bool DeleteChildWidthAttribute(const TCHAR* attributeName, const TCHAR* attributeValue, bool recursive=false);
        bool RemoveChild(MyType* pChild);

        /**
        * @breif 修改所有节点属性值
        * @param attributeName 节点属性名
        * @param valueOld 属性旧值
        * @param valueNew 属性新值
        * @retval 被修改的节点数
        */
        int AlterAttributeRecursively(const hg3d::TString& attributeName,
            const hg3d::TString& valueOld, const hg3d::TString& valueNew);

    protected:
        TinyXml::TiXmlNode* ToXmlNode(void);
        TinyXml::TiXmlText* ToTextNode(void);
        TinyXml::TiXmlElement* ToElementNode(void);
        void AlterAttributeRecursively(const hg3d::TString& attributeName,
                                       const hg3d::TString& valueOld,
                                       const hg3d::TString& valueNew,
                                       int& count);
    protected:
        int             m_type;         ///< 节点类型
        TString         m_strName;      ///< 节点名称，如果是Text节点则是节点Text
        StringStringMap m_mapAttrs;     ///< 节点属性集合
        NodeSet         m_children;     ///< 子节点集合
        MyType*         m_parent;       ///< 父节点
    };
}
