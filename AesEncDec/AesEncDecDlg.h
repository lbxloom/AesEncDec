
// AesEncDecDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "MyAes.h"
#include "afxcmn.h"


// CAesEncDecDlg �Ի���
class CAesEncDecDlg : public CDialogEx
{
// ����
public:
	CAesEncDecDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_AESENCDEC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
public:
    CFile m_FileToEnc;
    CFile m_FileToDec;
    CFileStatus m_FileStatus;
    DWORD m_currentProgress;
    char bufKey[17];
    char bufIV[17];

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnDropFiles(HDROP hDropInfo);
//    afx_msg void OnEnChangeFilename();
    CEdit m_edtFileName;
    afx_msg void OnBnClickedEnc();
    CEdit m_edtKey;
//    afx_msg void OnBnClickedQuit();
    afx_msg void OnBnClickedDec();
    int m_nMode;
    afx_msg void OnBnClickedEbc();
    afx_msg void OnBnClickedCbc();
    CEdit m_edtIV;
    CProgressCtrl m_prgProgress;
    afx_msg void OnTimer(UINT_PTR nIDEvent);
};
