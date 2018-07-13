
// AesEncDecDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AesEncDec.h"
#include "AesEncDecDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAesEncDecDlg 对话框



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


// CAesEncDecDlg 消息处理程序

BOOL CAesEncDecDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
    m_edtKey.SetLimitText(16);
    m_edtIV.SetLimitText(16);
    m_edtIV.EnableWindow(FALSE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CAesEncDecDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CAesEncDecDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CAesEncDecDlg::OnDropFiles(HDROP hDropInfo)
{
    // TODO:  在此添加消息处理程序代码和/或调用默认值
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
        AfxMessageBox(_T("文件打开失败"));
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

    //设置密钥
    aes.SetKey(pThis->bufKey);
    aes.ExtendKey();

    //设置IV
    if (pThis->m_nMode == 1)
    {
        aes.SetIV(pThis->bufIV);
    }

    //创建一个加密文件
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
        AfxMessageBox(_T("文件打开失败"));
        return 0;
    }



    pThis->m_FileToEnc.SeekToBegin();


    //读取文件读取明文
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

        //写入文件
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
    msgInfo.Format(_T("加密成功!\r\n文件路径: %s"), strNewName);
    AfxMessageBox(msgInfo);
    pThis->KillTimer(1);
    pThis->GetDlgItem(BTN_ENC)->EnableWindow(TRUE);
    pThis->GetDlgItem(BTN_DEC)->EnableWindow(TRUE);

    return 0;
}

void CAesEncDecDlg::OnBnClickedEnc()
{
    // TODO:  在此添加控件通知处理程序代码
    UpdateData(TRUE);
    if (m_FileToEnc.m_hFile == CFile::hFileNull)
    {
        AfxMessageBox(_T("请拖入一个文件!"));
        return;
    }

    //设置密钥
    GetDlgItemTextA(GetSafeHwnd(), EDT_KEY, bufKey, 17);
    CStringA strKey = bufKey;
    if (strKey.IsEmpty())
    {
        AfxMessageBox(_T("请输入密钥!"));
        return;
    }
    

    //设置IV
    if (m_nMode == 1)
    {
        GetDlgItemTextA(GetSafeHwnd(), EDT_IV, bufIV, 17);
        CStringA strIV = bufIV;
        if (strIV.IsEmpty())
        {
            AfxMessageBox(_T("请输入IV!"));
            return;
        }
    }

    GetDlgItem(BTN_ENC)->EnableWindow(FALSE);
    GetDlgItem(BTN_DEC)->EnableWindow(FALSE);

    //创建线程
    HANDLE hEncThread = CreateThread(NULL, 0, AesEncProc, this, 0, NULL);
    if (hEncThread == NULL)
    {
        AfxMessageBox(_T("线程创建失败"));
        return;
    }
    
}


//void CAesEncDecDlg::OnBnClickedQuit()
//{
//    // TODO:  在此添加控件通知处理程序代码
//    EndDialog(0);
//}

DWORD WINAPI AesDecProc(
    _In_ LPVOID lpParameter
    )
{
    //创建一个解密文件
    CAesEncDecDlg* pThis = (CAesEncDecDlg*)lpParameter;
    CMyAes aes_dec;
    char bufRead[16] = { 0 };

    //设置密钥
    aes_dec.SetKey(pThis->bufKey);
    aes_dec.ExtendKey();

    //设置IV
    if (pThis->m_nMode == 1)
    {
        aes_dec.SetIV(pThis->bufIV);
    }
    
    //创建新文件写入
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
        AfxMessageBox(_T("文件打开失败"));
        return 0;
    }

    pThis->m_FileToEnc.SeekToBegin();

    //读取文件读取明文
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

        //写入文件
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
    msgInfo.Format(_T("解密成功!\r\n文件路径: %s"), strNewName);
    AfxMessageBox(msgInfo);
    pThis->KillTimer(1);
    pThis->GetDlgItem(BTN_ENC)->EnableWindow(TRUE);
    pThis->GetDlgItem(BTN_DEC)->EnableWindow(TRUE);

    return 0;
}


void CAesEncDecDlg::OnBnClickedDec()
{
    // TODO:  在此添加控件通知处理程序代码
    UpdateData(TRUE);
    if (m_FileToEnc.m_hFile == CFile::hFileNull)
    {
        AfxMessageBox(_T("请拖入一个文件!"));
        return;
    }
    

    //设置密钥
    GetDlgItemTextA(GetSafeHwnd(), EDT_KEY, bufKey, 17);
    CStringA strKey = bufKey;
    if (strKey.IsEmpty())
    {
        AfxMessageBox(_T("请输入密钥!"));
        return;
    }
    

    //设置IV
    if (m_nMode == 1)
    {
        GetDlgItemTextA(GetSafeHwnd(), EDT_IV, bufIV, 17);
        CStringA strIV = bufIV;
        if (strIV.IsEmpty())
        {
            AfxMessageBox(_T("请输入IV!"));
            return;
        }
    }
    
    GetDlgItem(BTN_ENC)->EnableWindow(FALSE);
    GetDlgItem(BTN_DEC)->EnableWindow(FALSE);

    //创建线程
    HANDLE hDecThread = CreateThread(NULL, 0, AesDecProc, this, 0, NULL);
    if (hDecThread == NULL)
    {
        AfxMessageBox(_T("线程创建失败"));
        return;
    }
}


void CAesEncDecDlg::OnBnClickedEbc()
{
    // TODO:  在此添加控件通知处理程序代码
    UpdateData(TRUE);
    m_edtIV.EnableWindow(FALSE);
    UpdateData(FALSE);
}


void CAesEncDecDlg::OnBnClickedCbc()
{
    // TODO:  在此添加控件通知处理程序代码
    UpdateData(TRUE);
    m_edtIV.EnableWindow(TRUE);
    UpdateData(FALSE);
}


void CAesEncDecDlg::OnTimer(UINT_PTR nIDEvent)
{
    // TODO:  在此添加消息处理程序代码和/或调用默认值
    m_prgProgress.SetPos(m_currentProgress);

    CDialogEx::OnTimer(nIDEvent);
}
