
// RsaTestDlg.h : ͷ�ļ�
//

#pragma once


// CRsaTestDlg �Ի���
class CRsaTestDlg : public CDialog
{
// ����
public:
	CRsaTestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_RSATEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
#ifdef WOLFSSL

	string WolfSSL_RSA_Decrypt(const string& strSource);
	string WolfSSL_RSA_Encrypt(const string& strSource);

#else

	string OpenSSL_RSA_Decrypt(const string& strSource);
	string OpenSSL_RSA_Encrypt(const string& strSource);

#endif // WOLFSSL

	

	

	// ���ɵ���Ϣӳ�亯��
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
