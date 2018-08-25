// WaiGua.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
#include <WinUser.h>
#include <stdio.h>
#include <stdlib.h>
#include <atltypes.h>
#include <string>
#include <vector>
#include <cstdio>
#include <iostream>



using namespace std;

#pragma comment (lib, "User32.lib")



#define  SLEEP_1MS			(1)
#define  SLEEP_10MS			(10)
#define  SLEEP_100MS		(10)
#define  SLEEP_1000MS		(1000)
#define  SLEEP_2000MS		(2000)
#define  SLEEP_3000MS		(3000)
#define  SLEEP_4000MS		(4000)
#define  SLEEP_5000MS		(5000)

BOOL CopyToClipboard(const char* pszData, const int nDataLen)
{
	if(::OpenClipboard(NULL))
	{
		::EmptyClipboard();
		HGLOBAL clipbuffer;
		char *buffer;
		clipbuffer = ::GlobalAlloc(GMEM_DDESHARE, nDataLen+1);
		buffer = (char *)::GlobalLock(clipbuffer);
		strcpy(buffer, pszData);
		::GlobalUnlock(clipbuffer);
		::SetClipboardData(CF_TEXT, clipbuffer);
		::CloseClipboard();
		return TRUE;
	}
	return FALSE;
}







void InputNormalString(string str)
{
	for(UINT i = 0; i < str.length(); i++)
	{
		char c = str.at(i);

		Sleep(SLEEP_1MS);

		if (c == '.')
		{
			keybd_event(VK_DECIMAL, 0, 0, 0);					// 按下
			keybd_event(VK_DECIMAL, 0, KEYEVENTF_KEYUP, 0);		// 抬起
		}
		else
		{
			keybd_event(c, 0, 0, 0);					// 按下
			keybd_event(c, 0, KEYEVENTF_KEYUP, 0);		// 抬起
		}


	}
}








int _tmain(int argc, _TCHAR* argv[])
{
	// 获取当前排样方案
	


	// 填写文件名
	vector<string> file_list;
	vector<string> x_pos_list;
	vector<string> y_pos_list;


#if 0
	// 	file_list.push_back("C:\\Users\\admin\\Desktop\\tif测试图片\\001.tif");
	// 	file_list.push_back("C:\\Users\\admin\\Desktop\\tif测试图片\\002.tif");
	// 	file_list.push_back("C:\\Users\\admin\\Desktop\\tif测试图片\\003.tif");
	// 	file_list.push_back("C:\\Users\\admin\\Desktop\\tif测试图片\\001.tif");
	// 	file_list.push_back("C:\\Users\\admin\\Desktop\\tif测试图片\\002.tif");
	// 	file_list.push_back("C:\\Users\\admin\\Desktop\\tif测试图片\\003.tif");
	// 	file_list.push_back("C:\\Users\\admin\\Desktop\\tif测试图片\\001.tif");
	// 	file_list.push_back("C:\\Users\\admin\\Desktop\\tif测试图片\\002.tif");
	// 	file_list.push_back("C:\\Users\\admin\\Desktop\\tif测试图片\\003.tif");


#else

	file_list.push_back("D:\\QQPCmgr\\Desktop\\tif测试图片\\001.tif");
	file_list.push_back("D:\\QQPCmgr\\Desktop\\tif测试图片\\002.tif");
	file_list.push_back("D:\\QQPCmgr\\Desktop\\tif测试图片\\003.tif");

#endif







	x_pos_list.push_back("100");
	x_pos_list.push_back("200");
	x_pos_list.push_back("300");
	// 		x_pos_list.push_back("1500");
	// 		x_pos_list.push_back("1600");
	// 		x_pos_list.push_back("1700");
	// 		x_pos_list.push_back("3100");
	// 		x_pos_list.push_back("3200");
	// 		x_pos_list.push_back("3300");


	y_pos_list.push_back("10");
	y_pos_list.push_back("1800");
	y_pos_list.push_back("3600");
	// 		y_pos_list.push_back("10");
	// 		y_pos_list.push_back("1800");
	// 		y_pos_list.push_back("3600");
	// 		y_pos_list.push_back("10");
	// 		y_pos_list.push_back("1800");
	// 		y_pos_list.push_back("3600");








	//HWND exe_id = FindWindow(NULL, "蒙泰彩色电子出版系统 V6.0(专业版)");
	HWND exe_id = ::FindWindow(NULL, "蒙泰彩色电子出版系统 V6.0(普及版)");

	int find_exe_num = 0;

	if (exe_id == 0)
	{
		// 启动程序
#if 0

		//ShellExecute(NULL, "open", "E:\\袁梓埠个人文件夹\\代码\\MainTop\\DTP\\dtpw.exe", NULL, NULL, SW_SHOWNORMAL); 

#else

		ShellExecute(NULL, "open", "F:\\MainTop\\DTP\\dtpw.exe", NULL, NULL, SW_SHOWNORMAL); 

#endif




		while(exe_id == 0)
		{
			Sleep(100);
			exe_id = ::FindWindow(NULL, "蒙泰彩色电子出版系统 V6.0(普及版)");
			find_exe_num++;

		}

	}


	if (exe_id != NULL)
	{
		// 将窗口移到最顶层
		::SendMessage(exe_id, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
		::SetWindowPos(NULL, HWND_TOPMOST, 0,0, 1926,1446, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);

		Sleep(SLEEP_1000MS);

		bool show_coor_flag =false;																			 
		bool insert_flag =false;
		RECT exe_wnd_rect;
		::GetWindowRect(exe_id, &exe_wnd_rect);

		int x = exe_wnd_rect.left, y = exe_wnd_rect.top;


		// 窗口获取焦点
		int x2 = (exe_wnd_rect.left + exe_wnd_rect.right)/2;
		int y2 = (exe_wnd_rect.top + exe_wnd_rect.bottom)/2;
		SetCursorPos(x2, y2);
		mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,0,0,0,0);


		// 新建Ctrl+N
		keybd_event(VK_CONTROL, 0, 0, 0);				// 按下ctrl
		keybd_event('N', 0, 0, 0);						// 按下N
		keybd_event('N', 0, KEYEVENTF_KEYUP, 0);		// 抬起ctrl
		keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);	// 抬起N


		// 按键-确定 新建文件
		keybd_event(VK_RETURN, 0, 0, 0);
		keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);





		for (UINT i = 0; i < file_list.size();  i++)
		{
			// 设置焦点
			// 			HWND MainTopWnd_id = FindWindow("MainTop_TpfWindow", "");
			// 			if (MainTopWnd_id != NULL)
			// 			{
			//				RECT maintop_wnd_rect;
			//				GetWindowRect(MainTopWnd_id, &maintop_wnd_rect);

			int x = exe_wnd_rect.left + 80, y = exe_wnd_rect.top + 120;


			// 窗口获取焦点

			SetCursorPos(x, y);
			mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,0,0,0,0);




			//			}




			string file_path = file_list.at(i);
			string str_pos_x = x_pos_list.at(i);
			string str_pos_y = y_pos_list.at(i);



			// 取图片文件窗口 Ctrl+I
			keybd_event(VK_CONTROL, 0, 0, 0);				// 按下ctrl
			Sleep(SLEEP_10MS);
			keybd_event('I', 0, 0, 0);						// 按下I
			Sleep(SLEEP_10MS);
			keybd_event('I', 0, KEYEVENTF_KEYUP, 0);		// 抬起ctrl
			Sleep(SLEEP_10MS);
			keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);	// 抬起IC:\Users\admin\Desktop\tif测试图片\001.tif


			Sleep(SLEEP_1000MS);






			int  find_count = 0;
			HWND file_dlg_id;/* = ::FindWindow("#32770", "取图片文件");*/

			while(!(file_dlg_id = ::FindWindow("#32770", "取图片文件")))
			{
				Sleep(SLEEP_1000MS);
				find_count++;
			}


			if (file_dlg_id != NULL)
			{
				RECT file_dlg_rect;
				::GetWindowRect(file_dlg_id, &file_dlg_rect);

				int file_path_x = file_dlg_rect.left + 150;
				int file_path_y = file_dlg_rect.bottom - 65;

				SetCursorPos(file_path_x, file_path_y);
				mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,0,0,0,0);
				mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,0,0,0,0);

				// 删除已有数据
				keybd_event(VK_BACK, 0, 0, 0);
				keybd_event(VK_BACK, 0, KEYEVENTF_KEYUP, 0);




				// 设置到剪切板
				CopyToClipboard(file_path.c_str(), file_path.length());

				// 粘贴 Ctrl+V
				keybd_event(VK_CONTROL, 0, 0, 0);				// 按下ctrl
				keybd_event('V', 0, 0, 0);						// 按下D:\QQPCmgr\Desktop\tif测试图片\test_image.tif
				keybd_event('V', 0, KEYEVENTF_KEYUP, 0);		// 抬起ctrl
				keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);	// 抬起v




				// 按键-确定 

				keybd_event(VK_RETURN, 0, 0, 0);
				keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);


				// 按键-确定 
				keybd_event(VK_RETURN, 0, 0, 0);
				keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);




			}
			else
			{
				cout << "文件窗口未找到！" << std::endl;
				return 0;
			}

			// 显示标注
			if (show_coor_flag == false)
			{
				int show_coord_btn_x = exe_wnd_rect.left + 610;
				int show_coord_btn_y = exe_wnd_rect.top + 70;
				SetCursorPos(show_coord_btn_x, show_coord_btn_y);
				mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,0,0,0,0);

				show_coor_flag = true;
			}


			// 修改标注


			if (insert_flag == false)
			{

				Sleep(SLEEP_2000MS);
				insert_flag = true;
			}




			HWND parent_dlg_id = ::FindWindowEx(exe_id, 0,"OGL_V30_Window", "");

			HWND coor_dlg_id = ::FindWindowEx(parent_dlg_id, 0,"#32770", "");
			if (coor_dlg_id != NULL)
			{
				RECT coor_dlg_rect;
				::GetWindowRect(coor_dlg_id, &coor_dlg_rect);



				// 设置x坐标
				POINT coor_x;
				coor_x.x = coor_dlg_rect.left + 35;
				coor_x.y = coor_dlg_rect.top + 8;

				SetCursorPos(coor_x.x, coor_x.y);
				mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,0,0,0,0);
				mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,0,0,0,0);


				// 输入
				InputNormalString(str_pos_x);


				// 设置y坐标
				// 			POINT coor_y;
				// 			coor_y.x = coor_dlg_rect.left + 35;
				// 			coor_y.y = coor_dlg_rect.top + 25;
				// 
				// 			SetCursorPos(coor_y.x, coor_y.y);
				// 			mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,0,0,0,0);
				// 			mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,0,0,0,0);
				//	
				// 按下table键
				keybd_event(VK_TAB, 0, 0, 0);
				keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);




				// 输入
				InputNormalString(str_pos_y);






				// 按键-确定 
				keybd_event(VK_RETURN, 0, 0, 0);
				keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);

			}
			else
			{
				cout << "坐标窗口未找到！" << endl;
				return 0;
			}
		}
	}
	else
	{
		cout << "exe窗口未找到！" << endl;
		return 0;
	}









	

	system("pause");

	return 0;
}

