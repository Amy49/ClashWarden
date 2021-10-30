﻿// CPage_Option.cpp: 实现文件
//

#include "pch.h"
#include "ClashWarden.h"
#include "CPage_Option.h"
#include "afxdialogex.h"
#include <io.h>

// CPage_Option 对话框
CClashWardenApp* app2 = (CClashWardenApp*)AfxGetApp();
IMPLEMENT_DYNAMIC(CPage_Option, CDialogEx)

CPage_Option::CPage_Option(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_Option, pParent)
{

}


CPage_Option::~CPage_Option()
{
	CString inivalue;
	inivalue.Format(L"%d", app2->startup);
	WritePrivateProfileString(L"General", L"Startup", inivalue, app2->iniFile);
}

void CPage_Option::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CStartup, app2->startup);
}


BEGIN_MESSAGE_MAP(CPage_Option, CDialogEx)
	ON_BN_CLICKED(IDC_BTNStartup, &CPage_Option::OnBnClickedBtnstartup)
	ON_BN_CLICKED(IDC_BTNnoStartup, &CPage_Option::OnBnClickedBtnnostartup)
END_MESSAGE_MAP()


// CPage_Option 消息处理程序

BOOL CPage_Option::OnInitDialog()
{
	CDialogEx::OnInitDialog();
//	UpdateData(FALSE);
	return TRUE;
}

void CPage_Option::OnBnClickedBtnstartup()
{/*
	CRegKey SetReg;
	LPCTSTR ps = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
	LONG lResult = SetReg.Open(HKEY_LOCAL_MACHINE, ps, KEY_ALL_ACCESS);
	if (ERROR_SUCCESS == lResult)
	{
		if(app2->tunmode)
			lResult = SetReg.SetStringValue(L"Clash", app2->path + L"\\bin\\startuptun.vbs");
		else lResult = SetReg.SetStringValue(L"Clash", app2->path + L"\\bin\\startup.vbs");
		if (ERROR_SUCCESS != lResult)
		{
			AfxMessageBox(L"注册表写入失败！");
		}
		else
		{
			app2->startup = 1;
			UpdateData(FALSE);
		}
	}
	else AfxMessageBox(L"注册表打开失败！");
	SetReg.Close();
	*/
	TCHAR link[MAX_PATH];
	TCHAR link_name[MAX_PATH] = L"\\startClash.lnk";
	CString filename;
	filename = app2->tunmode ? L"\\bin\\startuptun.vbs" : L"\\bin\\startup.vbs";

	LPITEMIDLIST pidl;

	if (SUCCEEDED(SHGetSpecialFolderLocation(NULL, CSIDL_STARTUP, &pidl)))
	{
		SHGetPathFromIDList(pidl, link);
		wcscat_s(link, link_name);
		if ((_waccess(link, 0)) == -1) {
			if (SUCCEEDED(CreateLink(app2->path + filename, link, app2->path + L"\\bin")))
			{
				app2->startup = 1;
				UpdateData(FALSE);
			}
		}
		else if(app2->startup == 0)
		{
			app2->startup = 1;
			UpdateData(FALSE);
		}
	}
}


void CPage_Option::OnBnClickedBtnnostartup()
{
	/*
	CRegKey SetReg;
	LPCTSTR ps = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
	LONG lResult = SetReg.Open(HKEY_LOCAL_MACHINE, ps, KEY_ALL_ACCESS);
	if (ERROR_SUCCESS == lResult)
	{
		wchar_t chVersion[MAX_PATH] = { 0 };
		ULONG nChars = MAX_PATH;
		if (ERROR_SUCCESS == SetReg.QueryStringValue(L"Clash", chVersion, &nChars))
		{
			lResult = SetReg.DeleteValue(L"Clash");
			if (ERROR_SUCCESS != lResult)
			{
				AfxMessageBox(L"注册表改写失败！");
			}
			else
			{
				app2->startup = 0;
				UpdateData(FALSE);
			}
		}
	}
	else AfxMessageBox(L"注册表打开失败！");
	SetReg.Close();
	*/

	LPITEMIDLIST pidl;
	TCHAR link[MAX_PATH];
	TCHAR link_name[MAX_PATH] = L"\\startClash.lnk";
	BOOL ret = 0;

	if (SUCCEEDED(SHGetSpecialFolderLocation(NULL, CSIDL_STARTUP, &pidl)))
	{
		SHGetPathFromIDList(pidl, link);
		wcscat_s(link, link_name);
		if ((_waccess(link, 0)) == 0) {
			ret = DeleteFile(link);
			if(ret)
			{
				app2->startup = 0;
				UpdateData(FALSE);
			}
		}
		else if(app2->startup == 1){
			app2->startup = 0;
			UpdateData(FALSE);
		}
	}
}

HRESULT CPage_Option::CreateLink(LPCTSTR lpszPathObj, LPCTSTR lpszPathLink, LPCTSTR lpszLocation)
{
	HRESULT hres;
	IShellLink* psl;

	hres = CoInitialize(NULL);
	hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&psl);

	if (SUCCEEDED(hres)) {
		IPersistFile* ppf;

		psl->SetPath(lpszPathObj);
		psl->SetWorkingDirectory(lpszLocation);
		hres = psl->QueryInterface(IID_IPersistFile, (void**)&ppf);
		if (SUCCEEDED(hres)) {
			//WCHAR wsz[MAX_PATH];

			//MultiByteToWideChar(CP_ACP, 0, lpszPathLink, -1, wsz, MAX_PATH);
			hres = ppf->Save(lpszPathLink, TRUE);
			ppf->Release();
		}
		psl->Release();
	}

	return hres;
}
