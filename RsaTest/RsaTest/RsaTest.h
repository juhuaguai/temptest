
// RsaTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CRsaTestApp:
// �йش����ʵ�֣������ RsaTest.cpp
//

class CRsaTestApp : public CWinAppEx
{
public:
	CRsaTestApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CRsaTestApp theApp;