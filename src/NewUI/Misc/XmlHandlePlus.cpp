#include "stdafx.h"
#include "XmlHandlePlus.h"

#include <vector>
#include <string>
#include <algorithm>

#ifndef XmlHandlePlus_TestProject
#define TINY_HEADING TinyXml::
namespace TinyXml{
#else
#define TINY_HEADING 
#pragma comment(lib,"tinyxmlSTLd.lib")
#endif

	//	原Handle
#if 1


XmlHandlePlus XmlHandlePlus::FirstChild() const
{
	if ( good(node) )
	{
		TiXmlNode* child = node->FirstChild();
		if ( child )
			return XmlHandlePlus( child );
	}
	return XmlHandlePlus( 0 );
}


XmlHandlePlus XmlHandlePlus::FirstChild( const char * value ) const
{
	if ( good(node) )
	{
		TiXmlNode* child = node->FirstChild( value );
		if ( child )
			return XmlHandlePlus( child );
	}
	return XmlHandlePlus( 0 );
}


XmlHandlePlus XmlHandlePlus::FirstChildElement() const
{
	if ( good(node) )
	{
		TiXmlElement* child = node->FirstChildElement();
		if ( child )
			return XmlHandlePlus( child );
	}
	return XmlHandlePlus( 0 );
}


XmlHandlePlus XmlHandlePlus::FirstChildElement( const char * value ) const
{
	if ( good(node) )
	{
		TiXmlElement* child = node->FirstChildElement( value );
		if ( child )
			return XmlHandlePlus( child );
	}
	return XmlHandlePlus( 0 );
}


XmlHandlePlus XmlHandlePlus::Child( int count ) const
{
	if ( good(node) )
	{
		int i;
		TiXmlNode* child = node->FirstChild();
		for (	i=0;
			child && i<count;
			child = child->NextSibling(), ++i )
		{
			// nothing
		}
		if ( child )
			return XmlHandlePlus( child );
	}
	return XmlHandlePlus( 0 );
}


XmlHandlePlus XmlHandlePlus::Child( const char* value, int count ) const
{
	if (good(node) )
	{
		int i;
		TiXmlNode* child = node->FirstChild( value );
		for (	i=0;
			child && i<count;
			child = child->NextSibling( value ), ++i )
		{
			// nothing
		}
		if ( child )
			return XmlHandlePlus( child );
	}
	return XmlHandlePlus( 0 );
}


XmlHandlePlus XmlHandlePlus::ChildElement( int count ) const
{
	if ( good(node) )
	{
		int i;
		TiXmlElement* child = node->FirstChildElement();
		for (	i=0;
			child && i<count;
			child = child->NextSiblingElement(), ++i )
		{
			// nothing
		}
		if ( child )
			return XmlHandlePlus( child );
	}
	return XmlHandlePlus( 0 );
}


XmlHandlePlus XmlHandlePlus::ChildElement( const char* value, int count ) const
{
	if ( good(node) )
	{
		int i;
		TiXmlElement* child = node->FirstChildElement( value );
		for (	i=0;
			child && i<count;
			child = child->NextSiblingElement( value ), ++i )
		{
			// nothing
		}
		if ( child )
			return XmlHandlePlus( child );
	}
	return XmlHandlePlus( 0 );
}

#endif






	//	XmlHandlePlus
#if 1

//传入的路径需要以"/"斜杠分割
//允许采用的表意字符有："/"根目录，"."当前目录，".."上一级目录
//以及一般的英文数字组合代表切换到子目录叫这个名字的节点上
//操作过程中路径不允许包含空格、非特殊表意不能随便使用"."
//最后一级节点需要外部传递判断条件以在多个同级节点中进行筛选
void XmlHandlePlus::changeDirectoryTo(const std::string& pathname,IXmlHandlePlusCompare& compareObj)
{
	std::vector<TiXmlNode*> arr = findAll(pathname,compareObj);
	if (arr.size()>0)
	{
		node = arr[0];
	}
	else
	{
		node = NULL;
	}
}


//查找所有符合条件的节点返回到集合之中
std::vector<TiXmlNode*> XmlHandlePlus::findAll(const std::string& pathname,IXmlHandlePlusCompare& compareObj)
{
	std::vector<TiXmlNode*> resultNode;

	if(good() == false) return resultNode;

	std::string path = pathname;

	//替换斜杠字符
	replace(path.begin(),path.end(),'\\','/');

	//路径塑造
	std::vector<std::string> pathList = createPathlistFromPath(path);


	//循环处理每一个元素
	TiXmlNode* tempNode = node;
	for (size_t i = 0 ; i< pathList.size() ; i++)
	{
		if (good(tempNode))
		{
			if (pathList.at(i).compare("/") == 0)
			{
				tempNode = getRoot(tempNode);
			}
			else if (pathList.at(i).compare("..") == 0)
			{
				tempNode = tempNode->Parent();
			}
			else if (pathList.at(i).compare(".") == 0)
			{
				tempNode = tempNode;
			}
			else if ( i == pathList.size() -1)	//最后一个元素
			{
				//正常情况下有名字的都会跑到这里，寻找同名的节点然后返回
				tempNode = tempNode->FirstChild(pathList.at(i));
				resultNode = getSameNameBotherNode(tempNode,pathList.at(i),compareObj,false);
				return resultNode;
			}
			else
			{
				tempNode = tempNode->FirstChild(pathList.at(i));
			}
		}
	}
	//特殊情况下，请求的目录是不可搜索字符，所以不会去到“最后一个元素那步”
	if (tempNode)
	{
		resultNode.push_back(tempNode);
	}
	return resultNode;

}

//获得这级节点所有同名且受比较函数确认的节点列表
std::vector<TiXmlNode*> XmlHandlePlus::getSameNameBotherNode(TiXmlNode* curElem,std::string& nodeName,IXmlHandlePlusCompare& compareObj,bool firstBreak)
{
	std::vector<TiXmlNode*> elements;
	if (good(curElem))
	{
		XmlNodeNameCompare nodeNameCompare(nodeName);
		for( auto child = curElem; child; child = child->NextSibling() )
		{
			if (nodeNameCompare.compare(child))
			{
				if (compareObj.compare(child))
				{
					elements.push_back(child);
					if (firstBreak)
					{
						break;
					}
				}
			}
		}
	}
	return elements;
}

//默认比较函数，返回所有都为真
bool XmlHandlePlus::defaultCompare(TiXmlNode* node)
{
	return true;
}

//默认比较对象
IXmlHandlePlusCompare& XmlHandlePlus::defaultCompareObj = XmlOuterFunctionCompare(XmlHandlePlus::defaultCompare);

//////////////////////////////////////////////////////////////////////////
//节点管理

//设置节点属性值
void XmlHandlePlus::setAttr(std::string name,std::string value)
{
	if (good())
	{
		if (node->ToElement())
		{
			node->ToElement()->SetAttribute(name,value);
		}
	}
}

//读取节点属性值，当属性值不存在时返回默认属性
std::string XmlHandlePlus::getAttr(std::string name , std::string defaultValue )
{
	if (good())
	{
		if (node->ToElement())
		{
			if (node->ToElement()->Attribute(name))
			{
				const std::string* result = node->ToElement()->Attribute(name);
				if (result)
				{
					return *result;
				}
			}
		}
	}
	return defaultValue;
}

//设置Param节点搜索结构规则，{|Name|}表示Name，{|Value|}表示Value
void XmlHandlePlus::setParamElementName(std::string paramName)
{
	m_ParamName = paramName;
}

//会在当前路径下同级子节点中搜索name匹配的内容
void XmlHandlePlus::setParam(std::string name, std::string value ,
	std::string nameOfAttr,
	void (*setName)(TiXmlElement*,std::string value) ,
	void (*setValue)(TiXmlElement*,std::string value) )
{
	if (good())
	{
		//目录向下切换一层
		std::vector<TiXmlNode*> elements = findAll(m_ParamName,XmlAttrValueCompare(nameOfAttr,name));
		for(int i = 0;i<elements.size();i++)
		{
			if (setValue)
			{
				setValue(elements.at(i)->ToElement(),value);
				return;
			}
		}

		//所有节点都没有这个元素,创建一个并添加到其中
		TiXmlElement newElement( m_ParamName );
		if (setName)
		{
			setName(&newElement,name);
		}
		if (setValue)
		{
			setValue(&newElement,value);
		}
		node->InsertEndChild(newElement);
	}
	return;
}

//获取参数节点内容
std::string XmlHandlePlus::getParam(std::string name, std::string defaultValue , 
	std::string (*findElement)(TiXmlElement*) ,
	std::string (*getValue)(TiXmlElement*) )
{
	if (good())
	{
		for(TiXmlNode* child = node->FirstChild(); child; child = child->NextSibling() )
		{
			TiXmlElement* element = child->ToElement();
			if (findElement)
			{
				if (findElement(element).compare(name) == 0)
				{
					if (getValue)
					{
						return getValue(element);
					}
				}
			}
		}
	}
	return defaultValue;
}

//获取元素Name属性值
//FIXME: 到时候替换成对象形式
std::string XmlHandlePlus::getElementName(TiXmlElement* element)
{
	if (element && element->Attribute(std::string("Name")))
	{
		return *element->Attribute(std::string("Name"));
	}
	return std::string("");
}

//设置元素Name属性值
//FIXME: 到时候替换成对象形式
void XmlHandlePlus::setElementName(TiXmlElement* element, std::string value)
{
	if (element)
	{
		element->SetAttribute(std::string("Name"),value);
	}
}

//获取元素Value属性值
//FIXME: 到时候替换成对象形式
std::string XmlHandlePlus::getElementValue(TiXmlElement* element)
{
	if (element && element->Attribute(std::string("Value")))
	{
		return *element->Attribute(std::string("Value"));
	}
	return std::string("");
}

//设置元素Value属性值
//FIXME: 到时候替换成对象形式
void XmlHandlePlus::setElementValue(TiXmlElement* element, std::string value)
{
	if (element)
	{
		element->SetAttribute(std::string("Value"),value);
	}
}

//////////////////////////////////////////////////////////////////////////
//节点操作

//把自己从树中对应的位置移除,并把节点切换到父亲处
void XmlHandlePlus::remove()
{
	if (good())
	{
		if (good(node->Parent()))
		{
			TiXmlNode* parent = node->Parent();
			parent->RemoveChild(node);
			node = parent;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//工具函数

//从当前节点返回Root节点
TiXmlNode* XmlHandlePlus::getRoot(TiXmlNode* node)
{
	if (good(node))
	{
		TiXmlNode* root = node->GetDocument();
// 		TiXmlNode* temp = node;
// 		while (temp->Parent())
// 		{
// 			root = temp->Parent();
// 			temp = temp->Parent();
// 		}
// 		TiXmlDocument* doc = root->doc;
//		return doc->RootElement();
		return root;
	}
	return NULL;
}

//返回当前节点到Root节点之间的路径（绝对路径）
std::string XmlHandlePlus::getPWD(TiXmlNode* node)
{
	if (good(node))
	{
		std::string fullpath = "/";
		TiXmlNode* root = node;
		TiXmlNode* temp = node;
		while (temp->Parent())
		{
			fullpath = "/" + temp->ValueStr() + fullpath;

			root = temp->Parent();
			temp = temp->Parent();


		}
		return fullpath;
	}
	return "";
}

//路径预处理，清除".."路径\"."路径\"/"
std::vector<std::string> XmlHandlePlus::createPathlistFromPath( std::string path )
{
	//处理path，按/转换成list
	std::vector<std::string> pathList;
	split(path,"/",pathList);

	std::vector<std::string> tempList;
	if (path.size() > 0)
	{
		//第一个节点是‘/’
		if (path.at(0) == '/')
		{
			tempList.push_back("/");
		}

		//循环处理每一个“..”“.”
		for (size_t i = 0; i<pathList.size() ; i++)
		{
			std::string temp = pathList.at(i);
			if (temp.compare("") == 0)//没用信息
			{
				if (tempList.empty())
				{
					tempList.push_back(".");
				}
			}
			else if (temp.compare(" ") == 0)//没用信息
			{
				if (tempList.empty())
				{
					tempList.push_back(".");
				}
			}
			else if(temp.compare(".") == 0)//切换到当前路径，无需响应
			{
				if (tempList.empty())
				{
					tempList.push_back(".");
				}
			}
			else if (temp.compare("..") == 0)//切换到父级操作，如果前面还有进入某个目录操作就抵消掉
			{
				if (!tempList.empty())
				{
					tempList.pop_back();
				}
				else
				{
					tempList.push_back("..");
				}
			}
			else//一般情况
			{
				tempList.push_back(temp);
			}
		}
	}
	else
	{
		tempList.push_back(".");
	}

	return tempList;
}

//把字符串分割成字符串数组
void XmlHandlePlus::split(const std::string& s,const std::string& delim,std::vector< std::string >& ret)  
{  
	size_t last = 0;  
	size_t index=s.find_first_of(delim,last);  
	while (index!=std::string::npos)  
	{  
		ret.push_back(s.substr(last,index-last));  
		last=index+1;  
		index=s.find_first_of(delim,last);  
	}  
	if (index-last>0)  
	{  
		ret.push_back(s.substr(last,index-last));  
	}  
} 

//路径切换到
TiXmlNode* XmlHandlePlus::changeDirectoryByPath( TiXmlNode* node ,std::string &path)
{
	//切换到根目录
	TiXmlNode* curElem = node;
	if (path.size() > 0)
	{
		if (path.at(0) == '/')
		{
			curElem = getRoot(node);
		}

		std::vector<std::string> pathList = createPathlistFromPath(path);



	}
	return curElem;
}


//判断节点是否为空
bool XmlHandlePlus::good(TiXmlNode* node)
{
	if (!node || (node->ToDocument() && node->ToDocument()->Error()))
	{
		return false;
	}
	return true;
}

#endif

#ifndef XmlHandlePlus_TestProject
}
#endif

