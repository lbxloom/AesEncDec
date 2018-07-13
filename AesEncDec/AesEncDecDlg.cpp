
// AesEncDecDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AesEncDec.h"
#include "AesEncDecDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAesEncDecDlg �Ի���



CAesEncDecDlg::CAesEncDecDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CAesEncDecDlg::IDD, pParent)
, m_nMode(0)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

void CAesEncDecDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, EDT_FILENAME, m_edtFileName);
    DDX_Control(pDX, EDT_KEY, m_edtKey);
    DDX_Radio(pDX, RD_EBC, m_nMode);
    DDX_Control(pDX, EDT_IV, m_edtIV);
    DDX_Control(pDX, IDC_PROGRESS1, m_prgProgress);
}

BEGIN_MESSAGE_MAP(CAesEncDecDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_WM_DROPFILES()
//    ON_EN_CHANGE(EDT_FILENAME, &CAesEncDecDlg::OnEnChangeFilename)
    ON_BN_CLICKED(BTN_ENC, &CAesEncDecDlg::OnBnClickedEnc)
//    ON_BN_CLICKED(BTN_QUIT, &CAesEncDecDlg::OnBnClickedQuit)
    ON_BN_CLICKED(BTN_DEC, &CAesEncDecDlg::OnBnClickedDec)
    ON_BN_CLICKED(RD_EBC, &CAesEncDecDlg::OnBnClickedEbc)
    ON_BN_CLICKED(RD_CBC, &CAesEncDecDlg::OnBnClickedCbc)
    ON_WM_TIMER()
END_MESSAGE_MAP()


// CAesEncDecDlg ��Ϣ�������

BOOL CAesEncDecDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
    m_edtKey.SetLimitText(16);
    m_edtIV.SetLimitText(16);
    m_edtIV.EnableWindow(FALSE);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CAesEncDecDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CAesEncDecDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CAesEncDecDlg::OnDropFiles(HDROP hDropInfo)
{
    // TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
    TCHAR fileName[MAXBYTE] = { 0 };
    DragQueryFile(hDropInfo, 0, fileName, sizeof(fileName));
    SetDlgItemText(EDT_FILENAME, fileName);
    
    
    CString strFileSize;
    CString strFileName;

    if (CFile::GetStatus(fileName, m_FileStatus))
    {
        strFileSize.Format(_T("%d"), m_FileStatus.m_size);
        SetDlgItemText(TXT_FILESIZE, strFileSize);
    }

    if (m_FileToEnc.m_hFile != CFile::hFileNull)
    {
        m_FileToEnc.Close();
    }
    
    if (!m_FileToEnc.Open(fileName, 
        CFile::modeReadWrite | CFile::typeBinary))
    {
        AfxMessageBox(_T("�ļ���ʧ��"));
        return;
    }
    
    CDialogEx::OnDropFiles(hDropInfo);
}

DWORD WINAPI AesEncProc(
    _In_ LPVOID lpParameter
    )
{
    CAesEncDecDlg* pThis = (CAesEncDecDlg*)lpParameter;
    CMyAes aes;
    char bufRead[16] = { 0 };

    //������Կ
    aes.SetKey(pThis->bufKey);
    aes.ExtendKey();

    //����IV
    if (pThis->m_nMode == 1)
    {
        aes.SetIV(pThis->bufIV);
    }

    //����һ�������ļ�
    CString strFileName = pThis->m_FileStatus.m_szFullName;
    int nIndex = strFileName.ReverseFind(_T('.'));
    LPCWSTR pFix = strFileName.GetString() + nIndex;
    CString strFix = pFix;
    strFileName.SetAt(nIndex, 0);
    LPCWSTR pOldName = strFileName.GetBuffer();
    CString strOldName = pOldName;
    CString strNewName = strOldName + _T("_enc") + strFix;

    CFile fileEncrped;
    if (!fileEncrped.Open(strNewName, CFile::modeReadWrite | CFile::typeBinary | CFile::modeCreate))
    {
        AfxMessageBox(_T("�ļ���ʧ��"));
        return 0;
    }



    pThis->m_FileToEnc.SeekToBegin();


    //��ȡ�ļ���ȡ����
    int nRead = 0;
    char zeroData[16] = { 0 };

    pThis->m_currentProgress = 0;
    pThis->m_prgProgress.SetPos(0);
    pThis->m_prgProgress.SetRange32(0, pThis->m_FileStatus.m_size);
    pThis->SetTimer(1, 10, NULL);

    while (TRUE)
    {
        nRead = pThis->m_FileToEnc.Read(bufRead, 16);
        memset(bufRead + nRead, 16 - nRead, 16 - nRead);
        aes.SetPT(bufRead);
        aes.AesEnc();

        //д���ļ�
        fileEncrped.SeekToEnd();
        fileEncrped.Write(aes.m_CipherText, 16);

        pThis->m_currentProgress += nRead;
        memset(bufRead, 0, 16);
        if (nRead >= 0 && nRead < 16)
        {
            break;
        }
    }

    fileEncrped.Close();
    pThis->m_FileToEnc.Close();
    pThis->SetDlgItemText(EDT_FILENAME, _T(""));
    CString msgInfo;
    msgInfo.Format(_T("���ܳɹ�!\r\n�ļ�·��: %s"), strNewName);
    AfxMessageBox(msgInfo);
    pThis->KillTimer(1);
    pThis->GetDlgItem(BTN_ENC)->EnableWindow(TRUE);
    pThis->GetDlgItem(BTN_DEC)->EnableWindow(TRUE);

    return 0;
}

void CAesEncDecDlg::OnBnClickedEnc()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);
    if (m_FileToEnc.m_hFile == CFile::hFileNull)
    {
        AfxMessageBox(_T("������һ���ļ�!"));
        return;
    }

    //������Կ
    GetDlgItemTextA(GetSafeHwnd(), EDT_KEY, bufKey, 17);
    CStringA strKey = bufKey;
    if (strKey.IsEmpty())
    {
        AfxMessageBox(_T("��������Կ!"));
        return;
    }
    

    //����IV
    if (m_nMode == 1)
    {
        GetDlgItemTextA(GetSafeHwnd(), EDT_IV, bufIV, 17);
        CStringA strIV = bufIV;
        if (strIV.IsEmpty())
        {
            AfxMessageBox(_T("������IV!"));
            return;
        }
    }

    GetDlgItem(BTN_ENC)->EnableWindow(FALSE);
    GetDlgItem(BTN_DEC)->EnableWindow(FALSE);

    //�����߳�
    HANDLE hEncThread = CreateThread(NULL, 0, AesEncProc, this, 0, NULL);
    if (hEncThread == NULL)
    {
        AfxMessageBox(_T("�̴߳���ʧ��"));
        return;
    }
    
}


//void CAesEncDecDlg::OnBnClickedQuit()
//{
//    // TODO:  �ڴ���ӿؼ�֪ͨ����������
//    EndDialog(0);
//}

DWORD WINAPI AesDecProc(
    _In_ LPVOID lpParameter
    )
{
    //����һ�������ļ�
    CAesEncDecDlg* pThis = (CAesEncDecDlg*)lpParameter;
    CMyAes aes_dec;
    char bufRead[16] = { 0 };

    //������Կ
    aes_dec.SetKey(pThis->bufKey);
    aes_dec.ExtendKey();

    //����IV
    if (pThis->m_nMode == 1)
    {
        aes_dec.SetIV(pThis->bufIV);
    }
    
    //�������ļ�д��
    CString strFileName = pThis->m_FileStatus.m_szFullName;
    int nIndex = strFileName.ReverseFind(_T('.'));
    LPCWSTR pFix = strFileName.GetString() + nIndex;
    CString strFix = pFix;
    strFileName.SetAt(nIndex, 0);
    LPCWSTR pOldName = strFileName.GetBuffer();
    CString strOldName = pOldName;
    CString strNewName = strOldName + _T("_dec") + strFix;

    CFile fileDec;
    if (!fileDec.Open(strNewName, CFile::modeReadWrite | CFile::typeBinary | CFile::modeCreate))
    {
        AfxMessageBox(_T("�ļ���ʧ��"));
        return 0;
    }

    pThis->m_FileToEnc.SeekToBegin();

    //��ȡ�ļ���ȡ����
    pThis->m_currentProgress = 0;
    pThis->m_prgProgress.SetPos(0);
    pThis->m_prgProgress.SetRange32(0, pThis->m_FileStatus.m_size);
    pThis->SetTimer(1, 10, NULL);

    while (TRUE)
    {
        int nRead = pThis->m_FileToEnc.Read(bufRead, 16);
        if (nRead == 0)
        {
            break;
        }
        aes_dec.SetCT(bufRead);
        aes_dec.AesDec();

        //д���ļ�
        fileDec.SeekToEnd();
        fileDec.Write(aes_dec.m_pPlainText, 16);

        pThis->m_currentProgress += nRead;

        memset(bufRead, 0, 16);
    }
    char chRead;
    fileDec.Seek(-1, CFile::end);
    fileDec.Read(&chRead, 1);
    fileDec.SetLength(fileDec.GetLength() - chRead);

    fileDec.Close();
    pThis->m_FileToEnc.Close();
    pThis->SetDlgItemText(EDT_FILENAME, _T(""));
    CString msgInfo;
    msgInfo.Format(_T("���ܳɹ�!\r\n�ļ�·��: %s"), strNewName);
    AfxMessageBox(msgInfo);
    pThis->KillTimer(1);
    pThis->GetDlgItem(BTN_ENC)->EnableWindow(TRUE);
    pThis->GetDlgItem(BTN_DEC)->EnableWindow(TRUE);

    return 0;
}


void CAesEncDecDlg::OnBnClickedDec()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);
    if (m_FileToEnc.m_hFile == CFile::hFileNull)
    {
        AfxMessageBox(_T("������һ���ļ�!"));
        return;
    }
    

    //������Կ
    GetDlgItemTextA(GetSafeHwnd(), EDT_KEY, bufKey, 17);
    CStringA strKey = bufKey;
    if (strKey.IsEmpty())
    {
        AfxMessageBox(_T("��������Կ!"));
        return;
    }
    

    //����IV
    if (m_nMode == 1)
    {
        GetDlgItemTextA(GetSafeHwnd(), EDT_IV, bufIV, 17);
        CStringA strIV = bufIV;
        if (strIV.IsEmpty())
        {
            AfxMessageBox(_T("������IV!"));
            return;
        }
    }
    
    GetDlgItem(BTN_ENC)->EnableWindow(FALSE);
    GetDlgItem(BTN_DEC)->EnableWindow(FALSE);

    //�����߳�
    HANDLE hDecThread = CreateThread(NULL, 0, AesDecProc, this, 0, NULL);
    if (hDecThread == NULL)
    {
        AfxMessageBox(_T("�̴߳���ʧ��"));
        return;
    }
}


void CAesEncDecDlg::OnBnClickedEbc()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);
    m_edtIV.EnableWindow(FALSE);
    UpdateData(FALSE);
}


void CAesEncDecDlg::OnBnClickedCbc()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);
    m_edtIV.EnableWindow(TRUE);
    UpdateData(FALSE);
}


void CAesEncDecDlg::OnTimer(UINT_PTR nIDEvent)
{
    // TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
    m_prgProgress.SetPos(m_currentProgress);

    CDialogEx::OnTimer(nIDEvent);
}
