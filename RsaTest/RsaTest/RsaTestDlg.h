
// RsaTestDlg.h : 头文件
//

#pragma once


// CRsaTestDlg 对话框
class CRsaTestDlg : public CDialog
{
// 构造
public:
	CRsaTestDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_RSATEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
#ifdef WOLFSSL

	string WolfSSL_RSA_Decrypt(const string& strSource);
	string WolfSSL_RSA_Encrypt(const string& strSource);

#else

	string OpenSSL_RSA_Decrypt(const string& strSource);
	string OpenSSL_RSA_Encrypt(const string& strSource);

#endif // WOLFSSL

	

	

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
};
