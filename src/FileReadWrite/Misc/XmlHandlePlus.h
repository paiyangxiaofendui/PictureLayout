#pragma once


#include <string>
#include <list>
#include <vector>

//#define XmlHandlePlus_TestProject
#ifndef XmlHandlePlus_TestProject
#include "../../../include/TinyXml/tinyxml.h"
namespace TinyXml{
#else
#include "XmlHandlePlus/tinyxml.h"
#endif

class IXmlHandlePlusCompare
{
public:
	virtual bool compare(TiXmlNode* element) = 0;
};


typedef bool (*FuncCompare)(TiXmlNode*);
class XmlOuterFunctionCompare;
class XmlHandlePlus
{
	/************************************************************************/
	/* 拷贝自TiXmlHandle                                                                     */
	/************************************************************************/
#if 1
public:

	XmlHandlePlus operator=( const XmlHandlePlus& ref ) { if ( &ref != this ) this->node = ref.node; return *this; }

	/// Return a handle to the first child node.
	XmlHandlePlus FirstChild() const;
	/// Return a handle to the first child node with the given name.
	XmlHandlePlus FirstChild( const char * value ) const;
	/// Return a handle to the first child element.
	XmlHandlePlus FirstChildElement() const;
	/// Return a handle to the first child element with the given name.
	XmlHandlePlus FirstChildElement( const char * value ) const;

	/** Return a handle to the "index" child with the given name. 
		The first child is 0, the second 1, etc.
	*/
	XmlHandlePlus Child( const char* value, int index ) const;
	/** Return a handle to the "index" child. 
		The first child is 0, the second 1, etc.
	*/
	XmlHandlePlus Child( int index ) const;
	/** Return a handle to the "index" child element with the given name. 
		The first child element is 0, the second 1, etc. Note that only TiXmlElements
		are indexed: other types are not counted.
	*/
	XmlHandlePlus ChildElement( const char* value, int index ) const;
	/** Return a handle to the "index" child element. 
		The first child element is 0, the second 1, etc. Note that only TiXmlElements
		are indexed: other types are not counted.
	*/
	XmlHandlePlus ChildElement( int index ) const;

	#ifdef TIXML_USE_STL
	XmlHandlePlus FirstChild( const std::string& _value ) const				{ return FirstChild( _value.c_str() ); }
	XmlHandlePlus FirstChildElement( const std::string& _value ) const		{ return FirstChildElement( _value.c_str() ); }

	XmlHandlePlus Child( const std::string& _value, int index ) const			{ return Child( _value.c_str(), index ); }
	XmlHandlePlus ChildElement( const std::string& _value, int index ) const	{ return ChildElement( _value.c_str(), index ); }
	#endif

	/** Return the handle as a TiXmlNode. This may return null.
	*/
	TiXmlNode* ToNode() const			{ return node; } 
	/** Return the handle as a TiXmlElement. This may return null.
	*/
	TiXmlElement* ToElement() const		{ return ( ( node && node->ToElement() ) ? node->ToElement() : 0 ); }
	/**	Return the handle as a TiXmlText. This may return null.
	*/
	TiXmlText* ToText() const			{ return ( ( node && node->ToText() ) ? node->ToText() : 0 ); }
	/** Return the handle as a TiXmlUnknown. This may return null.
	*/
	TiXmlUnknown* ToUnknown() const		{ return ( ( node && node->ToUnknown() ) ? node->ToUnknown() : 0 ); }

	/** @deprecated use ToNode. 
		Return the handle as a TiXmlNode. This may return null.
	*/
	TiXmlNode* Node() const			{ return ToNode(); } 
	/** @deprecated use ToElement. 
		Return the handle as a TiXmlElement. This may return null.
	*/
	TiXmlElement* Element() const	{ return ToElement(); }
	/**	@deprecated use ToText()
		Return the handle as a TiXmlText. This may return null.
	*/
	TiXmlText* Text() const			{ return ToText(); }
	/** @deprecated use ToUnknown()
		Return the handle as a TiXmlUnknown. This may return null.
	*/
	TiXmlUnknown* Unknown() const	{ return ToUnknown(); }

private:
	TiXmlNode* node;
#endif


	/************************************************************************/
	/* 自己的实现                                                                     */
	/************************************************************************/
#if 1
public:
	/// 从节点创建帮助句柄.
	XmlHandlePlus(TiXmlNode& _rnode) {
		TiXmlNode* _node = &_rnode;
		if (_node->ToDocument())
		{
			if (_node->ToDocument()->Error())
			{
				node = NULL;
			}
		}
		m_ParamName = "Param";
		node = _node;
	}

	/// 从节点创建帮助句柄.
	XmlHandlePlus(TiXmlNode* _node) {
		if (_node->ToDocument())
		{
			if (_node->ToDocument()->Error())
			{
				node = NULL;
			}
		}
		m_ParamName = "Param";
		node = _node;
	}
	/// 拷贝构造函数
	XmlHandlePlus(const XmlHandlePlus& refr , std::string path = "/") {
		m_ParamName = "Param";
		node = refr.node;
	}

	/// 析构函数
	~XmlHandlePlus() {}




	//////////////////////////////////////////////////////////////////////////
	//节点搜索类
public:
	//传入的路径需要以"/"斜杠分割
	//允许采用的表意字符有："/"根目录，"."当前目录，".."上一级目录
	//以及一般的英文数字组合代表切换到子目录叫这个名字的节点上
	//操作过程中路径不允许包含空格、非特殊表意不能随便使用"."

	//切换工作目录，传入实现筛选接口类
	void changeDirectoryTo(const std::string& path,IXmlHandlePlusCompare& compareObj = defaultCompareObj);

	//查找所有符合条件的节点返回到集合之中
	std::vector<TiXmlNode*> findAll(const std::string& pathname,IXmlHandlePlusCompare& compareObj = defaultCompareObj);

protected:

	//获得这级节点所有同名且受比较函数确认的节点列表,可以设置一遇上马上返回
	static std::vector<TiXmlNode*> getSameNameBotherNode(TiXmlNode* curElem,std::string& name,IXmlHandlePlusCompare& compareObj,bool firstBreak = false);

	//节点路径切换
	static TiXmlNode* changeDirectoryByPath( TiXmlNode* curElem ,std::string &pathList);

	//默认比较函数，返回所有都为真
	static bool defaultCompare(TiXmlNode* node);

	//默认比较对象,所有对象返回true
	static IXmlHandlePlusCompare& defaultCompareObj;


private:
	std::string m_ParamName;
	//////////////////////////////////////////////////////////////////////////
	//节点管理
public:
	//设置节点属性值
	void setAttr(std::string name,std::string value);
	//读取节点属性值，当属性值不存在时返回默认属性
	std::string getAttr(std::string name , std::string defaultValue = "" );

	//设置Param节点搜索结构规则，{|Name|}表示Name，{|Value|}表示Value
	void setParamElementName(std::string paramName);

	//会在当前路径下同级子节点中搜索name匹配的内容
	void setParam(std::string name, std::string value, std::string nameOfAttr = "Name" ,void (*setName)(TiXmlElement*,std::string value) = setElementName,void (*setValue)(TiXmlElement*,std::string value) = setElementValue);
	
	//获取参数节点内容
	std::string getParam(std::string name, std::string defaultValue = "" , std::string (*findElement)(TiXmlElement*) = getElementName ,std::string (*getValue)(TiXmlElement*) = getElementValue);

	//获取元素Name属性值
	static std::string getElementName(TiXmlElement* element);

	//设置元素Name属性值
	static void setElementName(TiXmlElement* element, std::string value);

	//获取元素Value属性值
	static std::string getElementValue(TiXmlElement* element);

	//设置元素Value属性值
	static void setElementValue(TiXmlElement* element, std::string value);

	//获取handle封装
	XmlHandlePlus getHandle(){	return XmlHandlePlus(node); }

	//////////////////////////////////////////////////////////////////////////
	//节点操作
public:
	//把自己从树中对应的位置移除,并把节点切换到父亲处
	void remove();

	TiXmlNode* RawData() { return node; }

	//////////////////////////////////////////////////////////////////////////
	//工具函数
public:
	//从当前节点返回Root节点
	static TiXmlNode* getRoot(TiXmlNode* node);

	//返回当前节点到Root节点之间的路径（绝对路径）
	static std::string getPWD(TiXmlNode* node);

	//路径预处理，清除".."路径\"."路径\"/"
	static std::vector<std::string> createPathlistFromPath( std::string path );

	//返回当前节点到Root节点之间的路径（绝对路径）
	std::string getPWD(){	return getPWD(node);}

	//判断节点是否为空
	bool good() const{	return good(node);	}

	//判断节点是否为空
	static bool good(TiXmlNode* node);

	//把字符串分割成字符串数组
	static void split(const std::string& s,const std::string& delim,std::vector< std::string >& ret);
#endif
};

/************************************************************************/
/*  比较器                                                                     */
/************************************************************************/
#if 1

//比较类，用于设置外部比较函数
class XmlOuterFunctionCompare : public IXmlHandlePlusCompare
{
private:
	FuncCompare m_lastCompare;
public:
	XmlOuterFunctionCompare(FuncCompare lastCompare) :m_lastCompare(lastCompare) {}
	virtual bool compare( TiXmlNode* node ) {
		if (m_lastCompare)
		{
			if (m_lastCompare(node))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return true;
		}
	}
};

//比较类，用于筛选 属性名 和 值 匹配的节点
class XmlAttrValueCompare : public IXmlHandlePlusCompare
{
private:
	std::string m_name;
	std::string m_value;
public:
	XmlAttrValueCompare(std::string name,std::string val) :m_name(name),m_value(val){}
	virtual bool compare( TiXmlNode* node ) {
		XmlHandlePlus han(node);
		return han.getAttr(m_name).compare(m_value) == 0 ? true : false;
	}
};

//比较类，用于节点名称匹配Node
class XmlNodeNameCompare : public IXmlHandlePlusCompare
{
private:
	std::string m_nodeName;
public:
	XmlNodeNameCompare(std::string nodeName) :m_nodeName(nodeName){}
	virtual bool compare( TiXmlNode* node ) {
		return node->ValueStr() == m_nodeName ? true : false;
	}
};

#endif

#ifndef XmlHandlePlus_TestProject
}
#endif