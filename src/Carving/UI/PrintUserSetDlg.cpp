// PrintUserSetDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "KnifeDlg.h"
#include "PrintUserSetDlg.h"
#include "afxdialogex.h"
#include "PrintCodeDlg.h"
#include "../Misc/Barcode39.h"
#include "../../../include/QRCode/qrcode/qrencode.h"
#include "../Misc/ProgramMisc.h"
#include "../Misc/Misc.h"


// CPrintUserSetDlg dialog

IMPLEMENT_DYNAMIC(CPrintUserSetDlg, CDialogEx)

CPrintUserSetDlg::CPrintUserSetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPrintUserSetDlg::IDD, pParent)
{
	m_srcx = 20;
	m_srcy = 40;
	m_endx = m_srcx;
	m_endy = m_srcy;
	m_shift = false;
	m_move = false;
	m_fontsize = _T("");
	m_fonttype = _T("");
	m_leftstr = _T("");
	m_rightstr = _T("");
	m_value = _T("");
	m_x = _T("");
	m_y = _T("");
}

CPrintUserSetDlg::~CPrintUserSetDlg()
{
}

void CPrintUserSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_STATIC1, m_test);
	//  DDX_Control(pDX, IDC_PICTURESET, m_showview);
	DDX_Text(pDX, IDC_EDIT_FONTSIZE, m_fontsize);
	DDX_Text(pDX, IDC_EDIT_FONTTYPE, m_fonttype);
	DDX_Text(pDX, IDC_EDIT_LEFTSTR, m_leftstr);
	DDX_Text(pDX, IDC_EDIT_RIGHTSTR, m_rightstr);
	DDX_Text(pDX, IDC_EDIT_VALUE, m_value);
	DDX_Text(pDX, IDC_EDIT_X, m_x);
	DDX_Text(pDX, IDC_EDIT_Y, m_y);
	DDX_Control(pDX, IDC_COMBO_NAME, m_comname);
}


BEGIN_MESSAGE_MAP(CPrintUserSetDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CPrintUserSetDlg::OnBnClickedOk)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
//	ON_STN_CLICKED(IDC_PICTURESET, &CPrintUserSetDlg::OnStnClickedPictureset)
ON_WM_PAINT()
ON_WM_KEYDOWN()
ON_WM_KEYUP()
ON_BN_CLICKED(IDC_BUTTON2, &CPrintUserSetDlg::OnBnClickedButton2)
ON_BN_CLICKED(IDC_BUTTON1, &CPrintUserSetDlg::OnBnClickedButton1)
ON_BN_CLICKED(IDC_BUTTON9, &CPrintUserSetDlg::OnBnClickedButton9)
ON_BN_CLICKED(IDC_BUTTON3, &CPrintUserSetDlg::OnBnClickedButton3)
ON_BN_CLICKED(IDC_BUTTON5, &CPrintUserSetDlg::OnBnClickedButton5)
ON_BN_CLICKED(IDC_BUTTON8, &CPrintUserSetDlg::OnBnClickedButton8)
END_MESSAGE_MAP()


// CPrintUserSetDlg message handlers


void CPrintUserSetDlg::OnBnClickedOk()
{
	
	if (MessageBox("确定保存修改","提示",MB_OKCANCEL) == IDOK)
	{
		for (int i = 0;i < m_clickNode.size();i++)
		{
			m_allnode.push_back(m_clickNode.at(i));
		}
		m_clickNode.clear();
	}
	else
	{
		return;
	}
	CDialogEx::OnOK();
}


void CPrintUserSetDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	
	if (m_move == true)
	{
		int x1,x2,y1,y2;
		x1 = 0;
		x2 = 4000;
		y1 = 0;
		y2 = 4000;
		/*for (int i = 0;i < m_clickNode.size();i++)
		{
			if (m_clickNode.at(i)._x > x1)
			{
				x1 = m_clickNode.at(i)._x;
			}
		}
		for ()
		{
		}*/

		if (point.x < m_endx && point.y < m_endy && point.x > m_srcx && point.y > m_srcy)
		{
			int dis_x = point.x - m_oldpoint.x;
			int dis_y = point.y - m_oldpoint.y;

			for (int i = 0;i < m_clickNode.size();i++)
			{
				m_clickNode.at(i)._x = m_clickNode.at(i)._x + dis_x;
				m_clickNode.at(i)._y = m_clickNode.at(i)._y + dis_y;
			}
			m_oldpoint = point;
			Prinode();
		}
		
	}
	CDialogEx::OnMouseMove(nFlags, point);
}


void CPrintUserSetDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	
	m_oldpoint = point;
	int _x = point.x - m_srcx;
	int _y = point.y - m_srcy;
	if (m_shift == false)
	{
		for (int i = 0;i < m_clickNode.size();i++)
		{
			m_allnode.push_back(m_clickNode.at(i));
		}
		m_clickNode.clear();
	}
	if (_x > 0 && _y > 0 && _x < m_endx && _y < m_endy)
	{
		for (int i = 0;i < m_allnode.size();i++)
		{
			if (m_allnode.at(i)._name == "二维码" || m_allnode.at(i)._name == "二维码2")
			{
				char* szSourceSring = new char[m_allnode.at(i)._value.length()+1];
				for (int q = 0;q < m_allnode.at(i)._value.length();q++)
				{
					*(szSourceSring + q) = m_allnode.at(i)._value.at(q);
				}
				*(szSourceSring + m_allnode.at(i)._value.length()) = '\0';
				unsigned char*	pSourceData;
				QRcode*			pQRC;

				//int code_pixel = atoi(m_allnode.at(i)._fontsize.c_str());
				int code_pixel = 1;
				float code_plus = atof(m_allnode.at(i)._fontsize.c_str());

				int unWidth = 0;
				if (pQRC = QRcode_encodeString(szSourceSring, 0, QR_ECLEVEL_M, QR_MODE_8, 1))
				{
					unWidth = pQRC->width;
				}
				//unWidth = unWidth*code_pixel;
				unWidth = 40*code_plus;
				if (szSourceSring != NULL)
				{
					delete szSourceSring;
					szSourceSring = NULL;
				}
				int _nodex = m_allnode.at(i)._x;
				int _nodey = m_allnode.at(i)._y;
				int _endnodex = _nodex + unWidth;
				int _endnodey = _nodey + unWidth;

				if (_x >= _nodex && _y >= _nodey && _x < _endnodex && _y < _endnodey)
				{
					m_clickNode.push_back(m_allnode.at(i));
					m_move = true;
					m_oldpoint = point;
					m_allnode.erase(m_allnode.begin() + i);
					break;
				}
			}
			else if (m_allnode.at(i)._name == "一维码" || m_allnode.at(i)._name == "一维码2")
			{
				int ipen = atoi(m_allnode.at(i)._fontsize.c_str());
				CString csCode;
				csCode.Format("%s",m_allnode.at(i)._value.c_str());
				Barcode39 code;
				code.Encode39(csCode);
				int iPenW = ipen;
				int codelen = iPenW*code.GetEncodeLength();
				
				int _nodex = m_allnode.at(i)._x;
				int _nodey = m_allnode.at(i)._y;
				int _endnodex = _nodex + codelen;
				int _endnodey = _nodey + 21;

				if (_x >= _nodex && _y >= _nodey && _x < _endnodex && _y < _endnodey)
				{
					m_clickNode.push_back(m_allnode.at(i));
					m_move = true;
					m_oldpoint = point;
					m_allnode.erase(m_allnode.begin() + i);
					break;
				}
			}
			else if (m_allnode.at(i)._name == "Logo")
			{
				int _nodex = m_allnode.at(i)._x;
				int _nodey = m_allnode.at(i)._y;
				int _endnodex = _nodex + atoi(m_allnode.at(i)._fontsize.c_str())*2;
				int _endnodey = _nodey + atoi(m_allnode.at(i)._fontsize.c_str());

				if (_x >= _nodex && _y >= _nodey && _x < _endnodex && _y < _endnodey)
				{
					m_clickNode.push_back(m_allnode.at(i));
					m_move = true;
					m_oldpoint = point;
					m_allnode.erase(m_allnode.begin() + i);
					break;
				}
			}
			else if (m_allnode.at(i)._name == "板件图片")
			{
				int _nodex = m_allnode.at(i)._x - 10;
				int _nodey = m_allnode.at(i)._y - 30;
				int _endnodex = _nodex + 90;
				int _endnodey = _nodey + 90;

				if (_x >= _nodex && _y >= _nodey && _x < _endnodex && _y < _endnodey)
				{
					m_clickNode.push_back(m_allnode.at(i));
					m_move = true;
					m_oldpoint = point;
					m_allnode.erase(m_allnode.begin() + i);
					break;
				}
			}
			else if (m_allnode.at(i)._name == "排版图")
			{
				CString strValue =m_allnode.at(i)._value.c_str();
				int nXLen, nYLen;
				int nPos = strValue.Find('*');
				nXLen = atoi(strValue.Left(nPos));
				nYLen = atoi(strValue.Right(strValue.GetLength() - nPos - 1));

				int _nodex = m_allnode.at(i)._x;
				int _nodey = m_allnode.at(i)._y;
				int _endnodex = _nodex + nXLen;
				int _endnodey = _nodey + nYLen;

				if (_x >= _nodex && _y >= _nodey && _x < _endnodex && _y < _endnodey)
				{
					m_clickNode.push_back(m_allnode.at(i));
					m_move = true;
					m_oldpoint = point;
					m_allnode.erase(m_allnode.begin() + i);
					break;
				}
			}
			else
			{
				int _nodex = m_allnode.at(i)._x;
				int _nodey = m_allnode.at(i)._y;
				int _endnodey = _nodey + atoi(m_allnode.at(i)._fontsize.c_str())+1;
				int len1 = m_allnode.at(i)._leftstr.length();
				int len2 = m_allnode.at(i)._rightstr.length();
				int len3 = m_allnode.at(i)._value.length();
				int _endnodex = _nodex + atoi(m_allnode.at(i)._fontsize.c_str())*(m_allnode.at(i)._leftstr.length() + m_allnode.at(i)._rightstr.length() + m_allnode.at(i)._value.length())/2;
				if (_x >= _nodex && _y >= _nodey && _x < _endnodex && _y < _endnodey)
				{
					m_clickNode.push_back(m_allnode.at(i));
					m_move = true;
					m_oldpoint = point;
					m_allnode.erase(m_allnode.begin() + i);
					break;
				}
			}
		}
	}
	Prinode();
	UpdateData(true);
	if (m_clickNode.size() == 1)
	{
		m_fontsize.Format("%s",m_clickNode.at(0)._fontsize.c_str());
		m_fonttype.Format("%s",m_clickNode.at(0)._fontname.c_str());
		m_x.Format("%d",m_clickNode.at(0)._x);
		m_y.Format("%d",m_clickNode.at(0)._y);
		m_leftstr.Format("%s",m_clickNode.at(0)._leftstr.c_str());
		m_rightstr.Format("%s",m_clickNode.at(0)._rightstr.c_str());
		m_value.Format("%s",m_clickNode.at(0)._value.c_str());
	}
	else
	{
		m_fontsize = "";
		m_fonttype = "";
		m_x = "";
		m_y = "";
		m_leftstr = "";
		m_rightstr = "";
		m_value = "";
	}
	UpdateData(false);
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CPrintUserSetDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	
	if (m_move == true)
	{
		m_move = false;
		UpdateData(true);
		if (m_clickNode.size() == 1)
		{
			m_fontsize.Format("%s",m_clickNode.at(0)._fontsize.c_str());
			m_fonttype.Format("%s",m_clickNode.at(0)._fontname.c_str());
			m_x.Format("%d",m_clickNode.at(0)._x);
			m_y.Format("%d",m_clickNode.at(0)._y);
			m_leftstr.Format("%s",m_clickNode.at(0)._leftstr.c_str());
			m_rightstr.Format("%s",m_clickNode.at(0)._rightstr.c_str());
			m_value.Format("%s",m_clickNode.at(0)._value.c_str());
		}
		else
		{
			m_fontsize = "";
			m_fonttype = "";
			m_x = "";
			m_y = "";
			m_leftstr = "";
			m_rightstr = "";
			m_value = "";
		}
		UpdateData(false);
		Prinode();
	}
	
	CDialogEx::OnLButtonUp(nFlags, point);
}

void CPrintUserSetDlg::Prinode()
{
	std::vector<PrintNode> _printnode;
	for (int i = 0;i < m_allnode.size();i++)
	{
		_printnode.push_back(m_allnode.at(i));
	}

	HDC hdc = this->GetDC()->GetSafeHdc();
	CDC* pDC=CDC::FromHandle(hdc);

	pDC->Rectangle(m_srcx,m_srcy,m_endx,m_endy);

	for (int i = 0;i < _printnode.size();i++)
	{
		if (_printnode.at(i)._name == "二维码" || _printnode.at(i)._name == "二维码2")
		{
			int code_x = _printnode.at(i)._x + m_srcx;
			int code_y = _printnode.at(i)._y + m_srcy;
			std::string code_size = _printnode.at(i)._fontsize;

			std::string str_bar_code = _printnode.at(i)._value;

			char* szSourceSring = new char[str_bar_code.length()+1];
			for (int q = 0;q < str_bar_code.length();q++)
			{
				*(szSourceSring + q) = str_bar_code.at(q);
			}
			*(szSourceSring + str_bar_code.length()) = '\0';
			unsigned char*	pSourceData;
			QRcode*			pQRC;

			//int code_pixel = atoi(code_size.c_str());
			int code_pixel = 1;
			float code_plus = atof(code_size.c_str());

			if (pQRC = QRcode_encodeString(szSourceSring, 0, QR_ECLEVEL_M, QR_MODE_8, 1))
			{
				int unWidth = pQRC->width;

				int p_x = 0;
				int p_y = 0;

				pSourceData = pQRC->data;
				for (int c_i = 0;c_i < unWidth;c_i++)
				{
					for (int c_j = 0;c_j < unWidth;c_j++)
					{
						for (int c_k = 0;c_k < code_pixel;c_k++)
						{
							p_x = c_i * code_pixel + c_k;
							for(int c_m = 0;c_m < code_pixel;c_m++)
							{
								p_y = c_j * code_pixel + c_m;

								if (*pSourceData & 1)
								{
									pDC->SetPixel(p_y+code_x,p_x+code_y,RGB(0,0,0));
								}
							}

						}
						pSourceData++;
					}
				}
				pDC->StretchBlt(code_x,code_y,40*code_plus,40*code_plus,pDC,code_x,code_y,unWidth,unWidth,SRCCOPY);
				QRcode_free(pQRC);
			}
			if (szSourceSring != NULL)
			{
				delete szSourceSring;
				szSourceSring = NULL;
			}
		}
		else if (_printnode.at(i)._name == "一维码" || _printnode.at(i)._name == "一维码2")
		{
			int ipen = atoi(_printnode.at(i)._fontsize.c_str());
			std::string cCode = _printnode.at(i)._value;
			int x1 = _printnode.at(i)._x + m_srcx;
			int y1 = _printnode.at(i)._y + m_srcy;
			int y2 = y1 + 20;

			CString csCode;
			CString LocalBmpPath;
			csCode.Format("%s",cCode.c_str());
			Barcode39 code;
			code.Encode39(csCode);

			int iPenW = ipen;
			COLORREF clrBar		=RGB(0,0,0);
			COLORREF clrSpace	=RGB(255,255,255);


			int codelen = iPenW*code.GetEncodeLength();

			CRect m_SaveRect(x1,y1,x1 + codelen + 5,y2);
			CRect rectCodeText(x1,y2+5,x1 + codelen + 5,y2+15);
			FillRect(hdc,m_SaveRect,CBrush(clrSpace));

			code.DrawBarcode(hdc,x1,y1,y2,y2,clrBar,clrSpace,iPenW);
		}
		else if (_printnode.at(i)._name == "Logo")
		{
			int _nodex = _printnode.at(i)._x + m_srcx;
			int _nodey = _printnode.at(i)._y + m_srcy;
			std::string _nodesize = _printnode.at(i)._fontsize;

			CFont newfont;
			newfont.CreateFont(18, 0, 0, 0, FW_NORMAL,FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,   DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, "宋体");
			pDC->SelectObject(newfont);
			pDC->Rectangle(_nodex,_nodey,_nodex + 2 * atoi(_nodesize.c_str()),_nodey + atoi(_nodesize.c_str()));
			pDC->TextOut(_nodex+5,_nodey+atoi(_nodesize.c_str())/3,"Logo");
		}
		else if (_printnode.at(i)._name == "板件图片")
		{
			int _nodex = _printnode.at(i)._x + m_srcx;
			int _nodey = _printnode.at(i)._y + m_srcy;

			_nodex = _nodex - 10;
			_nodey = _nodey - 30;

			CFont newfont;
			newfont.CreateFont(18, 0, 0, 0, FW_NORMAL,FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,   DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, "宋体");
			pDC->SelectObject(newfont);
			pDC->Rectangle(_nodex,_nodey,_nodex + 90,_nodey + 90);
			pDC->TextOut(_nodex+10,_nodey+40,"板件图片");
		}
		else if (_printnode.at(i)._name == "排版图")
		{
			int nX = _printnode.at(i)._x;
			int nY = _printnode.at(i)._y;

			CString strValue = _printnode.at(i)._value.c_str();
			int nXLen = 0, nYLen = 0, nDiagramRotateAngle = 0;
			vector<CString> vSplittedString;
			vSplittedString = SplitStringByChar(strValue, '*');
			nXLen = atoi(vSplittedString[0]);
			nYLen = atoi(vSplittedString[1]);
			nDiagramRotateAngle = atoi(vSplittedString[2]);

			if((nDiagramRotateAngle/90)%2 != 0)
			{
				int nTmp = nXLen;
				nXLen = nYLen;
				nYLen = nTmp;
			}

			CRect rcDiagram;
			rcDiagram.SetRect(nX, nY, nX+nXLen, nY+nYLen);
			rcDiagram.OffsetRect(m_srcx, m_srcy);
			pDC->Rectangle(rcDiagram);

			pDC->DrawText("排版图", rcDiagram, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
		else
		{
			int _nodex = _printnode.at(i)._x;
			int _nodey = _printnode.at(i)._y;
			std::string _nodefontsize = _printnode.at(i)._fontsize;
			std::string _nodefontname = _printnode.at(i)._fontname;
			std::string _nodeleftstr = _printnode.at(i)._leftstr;
			std::string _noderightstr = _printnode.at(i)._rightstr;

			CFont newfont;
			newfont.CreateFont(atoi(_nodefontsize.c_str()), 0, 0, 0, FW_NORMAL,FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,   NONANTIALIASED_QUALITY, DEFAULT_PITCH | FF_ROMAN, (LPCSTR)_nodefontname.c_str());
			pDC->SelectObject(newfont);

			CString str_node;
			str_node.Format("%s%s%s",_printnode.at(i)._leftstr.c_str(),_printnode.at(i)._value.c_str(),_printnode.at(i)._rightstr.c_str());
			pDC->TextOut(_nodex+m_srcx,_nodey+m_srcy,str_node);
		}
	}
	_printnode.clear();
	for (int i = 0;i < m_clickNode.size();i++)
	{
		_printnode.push_back(m_clickNode.at(i));
	}
	for (int i = 0;i < _printnode.size();i++)
	{
		if (_printnode.at(i)._name == "二维码" || _printnode.at(i)._name == "二维码2")
		{
			int code_x = _printnode.at(i)._x + m_srcx;
			int code_y = _printnode.at(i)._y + m_srcy;
			std::string code_size = _printnode.at(i)._fontsize;

			std::string str_bar_code = _printnode.at(i)._value;

			char* szSourceSring = new char[str_bar_code.length()+1];
			for (int q = 0;q < str_bar_code.length();q++)
			{
				*(szSourceSring + q) = str_bar_code.at(q);
			}
			*(szSourceSring + str_bar_code.length()) = '\0';
			unsigned char*	pSourceData;
			QRcode*			pQRC;

			//int code_pixel = atoi(code_size.c_str());
			int code_pixel = 1;
			float code_plus = atof(code_size.c_str());

			if (pQRC = QRcode_encodeString(szSourceSring, 0, QR_ECLEVEL_M, QR_MODE_8, 1))
			{
				int unWidth = pQRC->width;

				int p_x = 0;
				int p_y = 0;

				pSourceData = pQRC->data;
				for (int c_i = 0;c_i < unWidth;c_i++)
				{
					for (int c_j = 0;c_j < unWidth;c_j++)
					{
						for (int c_k = 0;c_k < code_pixel;c_k++)
						{
							p_x = c_i * code_pixel + c_k;
							for(int c_m = 0;c_m < code_pixel;c_m++)
							{
								p_y = c_j * code_pixel + c_m;

								if (*pSourceData & 1)
								{
									pDC->SetPixel(p_y+code_x,p_x+code_y,RGB(255,0,0));
								}
							}

						}
						pSourceData++;
					}
				}
				pDC->StretchBlt(code_x,code_y,40*code_plus,40*code_plus,pDC,code_x,code_y,unWidth,unWidth,SRCCOPY);
				QRcode_free(pQRC);
			}
			if (szSourceSring != NULL)
			{
				delete szSourceSring;
				szSourceSring = NULL;
			}
		}
		else if (_printnode.at(i)._name == "一维码" || _printnode.at(i)._name == "一维码2")
		{
			int ipen = atoi(_printnode.at(i)._fontsize.c_str());
			std::string cCode = _printnode.at(i)._value;
			int x1 = _printnode.at(i)._x + m_srcx;
			int y1 = _printnode.at(i)._y + m_srcy;
			int y2 = y1 + 20;

			CString csCode;
			CString LocalBmpPath;
			csCode.Format("%s",cCode.c_str());
			Barcode39 code;
			code.Encode39(csCode);

			int iPenW = ipen;
			COLORREF clrBar		=RGB(255,0,0);
			COLORREF clrSpace	=RGB(255,255,255);


			int codelen = iPenW*code.GetEncodeLength();

			CRect m_SaveRect(x1,y1,x1 + codelen + 5,y2);
			CRect rectCodeText(x1,y2+5,x1 + codelen + 5,y2+15);
			FillRect(hdc,m_SaveRect,CBrush(clrSpace));

			code.DrawBarcode(hdc,x1,y1,y2,y2,clrBar,clrSpace,iPenW);
		}
		else if (_printnode.at(i)._name == "Logo")
		{
			int _nodex = _printnode.at(i)._x + m_srcx;
			int _nodey = _printnode.at(i)._y + m_srcy;
			std::string _nodesize = _printnode.at(i)._fontsize;

			pDC->SetTextColor(RGB(255,0,0));
			CFont newfont;
			newfont.CreateFont(18, 0, 0, 0, FW_NORMAL,FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,   DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, "宋体");
			pDC->SelectObject(newfont);
			pDC->Rectangle(_nodex,_nodey,_nodex + 2 * atoi(_nodesize.c_str()),_nodey + atoi(_nodesize.c_str()));
			pDC->TextOut(_nodex+5,_nodey+atoi(_nodesize.c_str())/3,"Logo");
			pDC->SetTextColor(RGB(0,0,0));

		}
		else if (_printnode.at(i)._name == "板件图片")
		{
			int _nodex = _printnode.at(i)._x + m_srcx;
			int _nodey = _printnode.at(i)._y + m_srcy;

			_nodex = _nodex - 10;
			_nodey = _nodey - 30;

			pDC->SetTextColor(RGB(255,0,0));
			CFont newfont;
			newfont.CreateFont(18, 0, 0, 0, FW_NORMAL,FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,   DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, "宋体");
			pDC->SelectObject(newfont);
			pDC->Rectangle(_nodex,_nodey,_nodex + 90,_nodey + 90);
			pDC->TextOut(_nodex+10,_nodey+40,"板件图片");
			pDC->SetTextColor(RGB(0,0,0));
		}
		else if (_printnode.at(i)._name == "排版图")
		{
			int nX = _printnode.at(i)._x;
			int nY = _printnode.at(i)._y;

			CString strValue = _printnode.at(i)._value.c_str();
			int nXLen = 0, nYLen = 0, nDiagramRotateAngle = 0;
			vector<CString> vSplittedString;
			vSplittedString = SplitStringByChar(strValue, '*');
			nXLen = atoi(vSplittedString[0]);
			nYLen = atoi(vSplittedString[1]);
			nDiagramRotateAngle = atoi(vSplittedString[2]);

			if((nDiagramRotateAngle/90)%2 != 0)
			{
				int nTmp = nXLen;
				nXLen = nYLen;
				nYLen = nTmp;
			}

			CRect rcDiagram;
			rcDiagram.SetRect(nX, nY, nX+nXLen, nY+nYLen);
			rcDiagram.OffsetRect(m_srcx, m_srcy);
			pDC->Rectangle(rcDiagram);

			pDC->SetTextColor(RGB(255,0,0));
			pDC->DrawText("排版图", rcDiagram, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			pDC->SetTextColor(RGB(0,0,0));
		}
		else
		{
			int _nodex = _printnode.at(i)._x;
			int _nodey = _printnode.at(i)._y;
			std::string _nodefontsize = _printnode.at(i)._fontsize;
			std::string _nodefontname = _printnode.at(i)._fontname;
			std::string _nodeleftstr = _printnode.at(i)._leftstr;
			std::string _noderightstr = _printnode.at(i)._rightstr;

			CFont newfont;
			newfont.CreateFont(atoi(_nodefontsize.c_str()), 0, 0, 0, FW_NORMAL,FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,   NONANTIALIASED_QUALITY, DEFAULT_PITCH | FF_ROMAN, (LPCSTR)_nodefontname.c_str());
			pDC->SelectObject(newfont);
			pDC->SetTextColor(RGB(255,0,0));

			CString str_node;
			str_node.Format("%s%s%s",_printnode.at(i)._leftstr.c_str(),_printnode.at(i)._value.c_str(),_printnode.at(i)._rightstr.c_str());
			pDC->TextOut(_nodex+m_srcx,_nodey+m_srcy,str_node);

			pDC->SetTextColor(RGB(0,0,0));
		}
	}
	

	DrawLogoTextInLabel(pDC, CRect(m_srcx,m_srcy,m_endx,m_endy));
}

BOOL CPrintUserSetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	
	m_comname.InsertString(0,"请选择");
	m_comname.InsertString(1,"Logo");
	m_comname.InsertString(2,"二维码");
	m_comname.InsertString(3,"二维码2");
	m_comname.InsertString(4,"一维码");
	m_comname.InsertString(5,"一维码2");
	m_comname.InsertString(6,"板件图片");
	m_comname.InsertString(7,"条码编号");
	m_comname.InsertString(8,"板件尺寸");
	m_comname.InsertString(9,"板件名称");
	m_comname.InsertString(10,"板件材料");
	m_comname.InsertString(11,"产品名称");
	m_comname.InsertString(12,"客户地址1");
	m_comname.InsertString(13,"客户地址2");
	m_comname.InsertString(14,"客户地址3");
	m_comname.InsertString(15,"柜号");
	m_comname.InsertString(16,"加盟店");
	m_comname.InsertString(17,"客户信息");
	m_comname.InsertString(18,"订单类型");
	m_comname.InsertString(19,"加工顺序");
	m_comname.InsertString(20,"翻转");
	m_comname.InsertString(21,"异型");
	m_comname.InsertString(22,"开槽");
	m_comname.InsertString(23,"助记号");
	m_comname.InsertString(24,"双面");
	m_comname.InsertString(25,"钻标识");
	m_comname.InsertString(26,"槽标识");
	m_comname.InsertString(27,"排版图");
	m_comname.InsertString(28,"门板标记");
	m_comname.InsertString(29,"发货日期");

	m_comname.SetCurSel(0);

	CRect rcClient;
	GetClientRect(rcClient);
	CRect rcBtn;
	GetDlgItem(IDOK)->GetWindowRect(rcBtn);
	ScreenToClient(rcBtn);
	int nLabelXLen = m_endx - m_srcx;
	int nLabelYLen = m_endy - m_srcy;
	CRect rcDrawArea;
	rcDrawArea.SetRect(20, 40, rcBtn.left - 30, rcClient.bottom);
	m_srcx = rcDrawArea.left + (rcDrawArea.Width() - nLabelXLen) / 2;
	m_srcy = rcDrawArea.top + (rcDrawArea.Height() - nLabelYLen) / 2;
	m_endx = m_srcx + nLabelXLen;
	m_endy = m_srcy + nLabelYLen;

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CPrintUserSetDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	Prinode();
	// Do not call CDialogEx::OnPaint() for painting messages
}


void CPrintUserSetDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	
	if (nChar == VK_SHIFT)
	{
		m_shift = true;
	}
	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CPrintUserSetDlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	
	if (nChar == VK_SHIFT)
	{
		m_shift = false;
	}
	CDialogEx::OnKeyUp(nChar, nRepCnt, nFlags);
}


BOOL CPrintUserSetDlg::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	

	return CDialogEx::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BOOL CPrintUserSetDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_SHIFT)  
	{
		m_shift = true;
		return TRUE;
	}
	else if (pMsg->message == WM_KEYUP && pMsg->wParam == VK_SHIFT)
	{
		m_shift = false;
		return true;
	}
	else if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_DELETE)
	{
		m_clickNode.clear();
		Prinode();
	}
	else
	{
		return FALSE;
	}
}


void CPrintUserSetDlg::OnBnClickedButton2()
{
	
	if (m_clickNode.size() == 1)
	{
		UpdateData(true);

		std::string str_x = m_x.GetBuffer();
		std::string str_y = m_y.GetBuffer();

		m_clickNode.at(0)._fontsize = m_fontsize.GetBuffer();
		m_clickNode.at(0)._fontname = m_fonttype.GetBuffer();
		m_clickNode.at(0)._x = atoi(str_x.c_str());
		m_clickNode.at(0)._y = atoi(str_y.c_str());
		m_clickNode.at(0)._leftstr = m_leftstr.GetBuffer();
		m_clickNode.at(0)._rightstr = m_rightstr.GetBuffer();
		m_clickNode.at(0)._value = m_value.GetBuffer();
		UpdateData(false);
		Prinode();
	}
	else
	{
		AfxMessageBox("请选择需要修改的对象");
	}
}


void CPrintUserSetDlg::OnBnClickedButton1()
{
	
	if (m_clickNode.size() > 0)
	{
		AfxMessageBox("请不要选择修改对象");
	}
	else
	{
		UpdateData(true);
		PrintNode newnode;

		std::string str_x = m_x.GetBuffer();
		std::string str_y = m_y.GetBuffer();
		CString str_name;
		m_comname.GetLBText(m_comname.GetCurSel(),str_name);

		newnode._fontsize = m_fontsize.GetBuffer();
		newnode._fontname = m_fonttype.GetBuffer();
		newnode._x = atoi(str_x.c_str());
		newnode._y = atoi(str_y.c_str());
		newnode._leftstr = m_leftstr.GetBuffer();
		newnode._rightstr = m_rightstr.GetBuffer();
		newnode._value = m_value.GetBuffer();
		newnode._name = str_name.GetBuffer();

		for (int i = 0;i < m_allnode.size();i++)
		{
			std::string s_name1 = m_allnode.at(i)._name;
			std::string s_name2 = newnode._name;
			if (strcmp(s_name1.c_str(),s_name2.c_str()) == 0)
			{
				AfxMessageBox("该对象已存在！");
				return;
			}
		}

		m_allnode.push_back(newnode);
		UpdateData(false);
		Prinode();
	}
}


void CPrintUserSetDlg::OnBnClickedButton9()
{
	
	CFontDialog fontdlg;
	if (fontdlg.DoModal() == IDOK)
	{
		UpdateData(true);
		m_fonttype = fontdlg.GetFaceName();
		m_fontsize.Format("%d",fontdlg.GetSize()/10); 
		UpdateData(false);	
	}
}


void CPrintUserSetDlg::OnBnClickedButton3()
{
	
	if (m_clickNode.size() > 1)
	{
		int _x = m_clickNode.at(m_clickNode.size() - 1)._x;
		for (int i = 0;i < m_clickNode.size();i++)
		{
			m_clickNode.at(i)._x = _x;
		}
		Prinode();
	}
}


void CPrintUserSetDlg::OnBnClickedButton5()
{
	
	if (m_clickNode.size() > 1)
	{
		int _y = m_clickNode.at(m_clickNode.size() - 1)._y;
		for (int i = 0;i < m_clickNode.size();i++)
		{
			m_clickNode.at(i)._y = _y;
		}
		Prinode();
	}
}


void CPrintUserSetDlg::OnBnClickedButton8()
{
	
	if (m_clickNode.size() > 1)
	{
		for (int i = 1;i < m_clickNode.size();i++)
		{
			if (m_clickNode.at(i)._y < m_clickNode.at(i-1)._y)
			{
				AfxMessageBox("请按从上到下的顺序选择对象");
				return;
			}
		}
		int _beginy = m_clickNode.at(0)._y;
		int _endy = m_clickNode.at(m_clickNode.size() - 1)._y + atoi(m_clickNode.at(m_clickNode.size() - 1)._fontsize.c_str());

		int _dis_y = (_endy - _beginy)/ m_clickNode.size();

		for (int i = 0;i < m_clickNode.size();i++)
		{
			m_clickNode.at(i)._y = m_clickNode.at(0)._y + _dis_y*i;
		}
		Prinode();
	}
}
