
// RsaTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RsaTest.h"
#include "RsaTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRsaTestDlg 对话框




CRsaTestDlg::CRsaTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRsaTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRsaTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CRsaTestDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CRsaTestDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CRsaTestDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CRsaTestDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CRsaTestDlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// CRsaTestDlg 消息处理程序

BOOL CRsaTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CRsaTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CRsaTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CRsaTestDlg::OnBnClickedButton1()
{
#ifndef WOLFSSL
	// TODO: 在此添加控件通知处理程序代码
	SetDlgItemTextW(IDC_EDIT2,L"");

	CString strInput;
	GetDlgItemTextW(IDC_EDIT1,strInput);
	
	USES_CONVERSION;
	string strValue = W2A(strInput.GetBuffer());
	strInput.ReleaseBuffer();

	string strRet = OpenSSL_RSA_Encrypt(strValue);

	strInput = A2W(strRet.c_str());
	SetDlgItemTextW(IDC_EDIT2,strInput);
#endif
}

void CRsaTestDlg::OnBnClickedButton2()
{
#ifndef WOLFSSL
	// TODO: 在此添加控件通知处理程序代码
	SetDlgItemTextW(IDC_EDIT1,L"");

	CString strInput;
	GetDlgItemTextW(IDC_EDIT2,strInput);

	USES_CONVERSION;
	string strValue = W2A(strInput.GetBuffer());
	strInput.ReleaseBuffer();

	string strRet = OpenSSL_RSA_Decrypt(strValue);

	strInput = A2W(strRet.c_str());
	SetDlgItemTextW(IDC_EDIT1,strInput);
#endif
}

#ifndef WOLFSSL
string CRsaTestDlg::OpenSSL_RSA_Decrypt(const string& strSource)
{
	if (strSource.empty())
		return "";
	if (strSource.length() < 64)
		return strSource;

	string strContent = strSource;
	string strRet;
	try
	{
		//先将16进制串转为ansi串
		unsigned char* upszContent = new unsigned char[(strContent.length() + 1) / 2];
		int i = 0;
		while (strContent.empty() == false)
		{
			wchar_t wch = 0;
			sscanf(strContent.substr(0, 2).c_str(), "%02x", &wch);
			upszContent[i] = wch;
			i++;
			strContent.erase(0, 2);
		}
		//分段解密
		BIGNUM* e = BN_new();
		BIGNUM* d = BN_new();
		BIGNUM* n = BN_new();
		RSA* ppri_key = RSA_new();
		BN_bin2bn(PUBLIC_EXPONENT_HEX, 3, e);
		BN_bin2bn(PRIVATE_EXPONENT_HEX, 128, d);
		BN_bin2bn(MODULES_HEX, 128, n);
		RSA_set0_key(ppri_key, n, e, d);

		int nfLen = 128;
		int nFenduan = (strSource.length() / 2) / 128;
		unsigned char szValue[1024] = { 0 };

		for (int nIndexDuan = 0; nIndexDuan < nFenduan; nIndexDuan++)
		{
			int nRet = RSA_private_decrypt(nfLen, (const unsigned char*)(upszContent + nfLen * nIndexDuan), szValue, ppri_key, RSA_PKCS1_PADDING/*RSA_PKCS1_OAEP_PADDING*/);//RSA_PKCS1_PADDING

			strRet += (char*)szValue;
			memset(szValue, 0, strlen((char*)szValue));
		}

		//BN_clear_free(e);
		//BN_clear_free(d);
		//BN_clear_free(n);
		//RSA_free(ppri_key);
		delete[]upszContent;
	}
	catch (...)
	{
	}
	return strRet;
}
string CRsaTestDlg::OpenSSL_RSA_Encrypt(const string& strSource)
{
	if (strSource.empty())
		return "";

	string strRet;
	try
	{
		//rsa公钥加密
		BIGNUM* e = BN_new();
		BIGNUM* n = BN_new();
		RSA* ppub_key = RSA_new();
		BN_bin2bn(PUBLIC_EXPONENT_HEX, 3, e);
		BN_bin2bn(MODULES_HEX, 128, n);
		RSA_set0_key(ppub_key, n, e, NULL);

		string strContent = strSource;
		int nfLen = 117;

		int nFenduan = (strContent.length()) / 117 + 1;
		for (int nIndexDuan = 0; nIndexDuan < nFenduan; nIndexDuan++)
		{
			if (strContent.empty() == false)
			{
				char szContent[129] = { 0 };
				unsigned char szValue[1024] = { 0 };
				sprintf(szContent, "%s", strContent.substr(0, nfLen).c_str());
				strContent.erase(0, nfLen);
				int nEnLen = RSA_public_encrypt(nfLen, (const unsigned char*)szContent, szValue, ppub_key, RSA_PKCS1_PADDING);
				for (int iPos = 0; iPos < nEnLen; iPos++)
				{
					WORD nWord = szValue[iPos];
					char szHex[8] = { 0 };
					sprintf(szHex, "%02x", nWord);
					strRet += szHex;
				}
			}
		}

		//BN_clear_free(e);
		//BN_clear_free(n);
		//RSA_free(ppub_key);
	}
	catch (...)
	{
	}

	return strRet;
}

#endif

void CRsaTestDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
#ifdef WOLFSSL
	SetDlgItemTextW(IDC_EDIT2, L"");

	CString strInput;
	GetDlgItemTextW(IDC_EDIT1, strInput);

	USES_CONVERSION;
	string strValue = W2A(strInput.GetBuffer());
	strInput.ReleaseBuffer();

	string strRet = WolfSSL_RSA_Encrypt(strValue);

	strInput = A2W(strRet.c_str());
	SetDlgItemTextW(IDC_EDIT2, strInput);
#endif // WOLFSSL
}


void CRsaTestDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
#ifdef WOLFSSL
	SetDlgItemTextW(IDC_EDIT1, L"");

	CString strInput;
	GetDlgItemTextW(IDC_EDIT2, strInput);

	USES_CONVERSION;
	string strValue = W2A(strInput.GetBuffer());
	strInput.ReleaseBuffer();

	string strRet = WolfSSL_RSA_Decrypt(strValue);

	strInput = A2W(strRet.c_str());
	SetDlgItemTextW(IDC_EDIT1, strInput);
#endif // WOLFSSL
}

#ifdef WOLFSSL
string CRsaTestDlg::WolfSSL_RSA_Decrypt(const string& strSource)
{
	if (strSource.empty())
		return "";
	if (strSource.length() < 64)
		return strSource;

	string strContent = strSource;
	string strRet;
	try
	{
		//先将16进制串转为ansi串
		unsigned char* upszContent = new unsigned char[(strContent.length() + 1) / 2];
		int i = 0;
		while (strContent.empty() == false)
		{
			wchar_t wch = 0;
			sscanf(strContent.substr(0, 2).c_str(), "%02x", &wch);
			upszContent[i] = wch;
			i++;
			strContent.erase(0, 2);
		}
		//分段解密
		WOLFSSL_BIGNUM* e = wolfSSL_BN_new();
		WOLFSSL_BIGNUM* d = wolfSSL_BN_new();
		WOLFSSL_BIGNUM* n = wolfSSL_BN_new();
		WOLFSSL_RSA* ppri_key = wolfSSL_RSA_new();
		wolfSSL_BN_bin2bn(PUBLIC_EXPONENT_HEX, 3, e);
		wolfSSL_BN_bin2bn(PRIVATE_EXPONENT_HEX, 128, d);
		wolfSSL_BN_bin2bn(MODULES_HEX, 128, n);
		wolfSSL_RSA_set0_key(ppri_key, n, e, d);

		int nfLen = 128;
		int nFenduan = (strSource.length() / 2) / 128;
		unsigned char szValue[1024] = { 0 };

		for (int nIndexDuan = 0; nIndexDuan < nFenduan; nIndexDuan++)
		{
			int nRet = wolfSSL_RSA_private_decrypt(nfLen, (const unsigned char*)(upszContent + nfLen * nIndexDuan), szValue, ppri_key, RSA_PKCS1_PADDING/*RSA_PKCS1_OAEP_PADDING*/);//RSA_PKCS1_PADDING

			strRet += (char*)szValue;
			memset(szValue, 0, strlen((char*)szValue));
		}

		//wolfSSL_BN_clear_free(e);
		//wolfSSL_BN_clear_free(d);
		//wolfSSL_BN_clear_free(n);
		//wolfSSL_RSA_free(ppri_key);
		delete[]upszContent;
	}
	catch (...)
	{
	}
	return strRet;
}
string CRsaTestDlg::WolfSSL_RSA_Encrypt(const string& strSource)
{
	if (strSource.empty())
		return "";

	string strRet;
	try
	{
		//rsa公钥加密
		WOLFSSL_BIGNUM* e = wolfSSL_BN_new();
		WOLFSSL_BIGNUM* n = wolfSSL_BN_new();
		WOLFSSL_BIGNUM* d = wolfSSL_BN_new();
		WOLFSSL_RSA* ppub_key = wolfSSL_RSA_new();
		wolfSSL_BN_bin2bn(PUBLIC_EXPONENT_HEX, 3, e);
		wolfSSL_BN_bin2bn(MODULES_HEX, 128, n);
		wolfSSL_BN_bin2bn(PRIVATE_EXPONENT_HEX, 128, d);
		wolfSSL_RSA_set0_key(ppub_key, n, e, d);

		string strContent = strSource;
		int nfLen = 117;

		int nFenduan = (strContent.length()) / 117 + 1;
		for (int nIndexDuan = 0; nIndexDuan < nFenduan; nIndexDuan++)
		{
			if (strContent.empty() == false)
			{
				char szContent[129] = { 0 };
				unsigned char szValue[1024] = { 0 };
				sprintf(szContent, "%s", strContent.substr(0, nfLen).c_str());
				strContent.erase(0, nfLen);
				int nEnLen = wolfSSL_RSA_public_encrypt(nfLen, (const unsigned char*)szContent, szValue, ppub_key, RSA_PKCS1_PADDING);
				for (int iPos = 0; iPos < nEnLen; iPos++)
				{
					WORD nWord = szValue[iPos];
					char szHex[8] = { 0 };
					sprintf(szHex, "%02x", nWord);
					strRet += szHex;
				}
			}
		}

		//wolfSSL_BN_clear_free(e);
		//wolfSSL_BN_clear_free(n);
		//wolfSSL_RSA_free(ppub_key);
	}
	catch (...)
	{
	}

	return strRet;
}
#endif //  WOLFSSL