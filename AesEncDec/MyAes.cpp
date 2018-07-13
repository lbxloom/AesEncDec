#include "stdafx.h"
#include "MyAes.h"


const int CMyAes::S[16][16] =
{ 0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };

const int CMyAes::deS[16][16] = 
{ 0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d };

const int CMyAes::Rcon[10] =
{ 0x01000000, 0x02000000,
0x04000000, 0x08000000,
0x10000000, 0x20000000,
0x40000000, 0x80000000,
0x1b000000, 0x36000000 };

const int CMyAes::colM[4][4] =
{ 2, 3, 1, 1,
1, 2, 3, 1,
1, 1, 2, 3,
3, 1, 1, 2 };

const int CMyAes::deColM[4][4] =
{ 0xe, 0xb, 0xd, 0x9,
0x9, 0xe, 0xb, 0xd,
0xd, 0x9, 0xe, 0xb,
0xb, 0xd, 0x9, 0xe };


CMyAes::CMyAes():
m_bKeySet(FALSE), m_bPlainText(FALSE), m_bCipherText(FALSE), m_bIV(FALSE)
{
}

//设置密钥
void CMyAes::SetKey(char* pKey)
{
    for (int i = 0; i < 16; i++)
    {
        m_Key[i] = pKey[i];
    }
    m_bKeySet = TRUE;
}

//设置明文
void CMyAes::SetPT(char* pPlainText)
{
    for (int i = 0; i < 16; i++)
    {
        m_pPlainText[i] = pPlainText[i];
    }
    m_bPlainText = TRUE;
}

void CMyAes::SetCT(char* pCipherText)
{
    for (int i = 0; i < 16; i++)
    {
        m_CipherText[i] = pCipherText[i];
    }
    m_bCipherText = TRUE;
}

//设置IV
void CMyAes::SetIV(char* pIV)
{
    for (int i = 0; i < 16; i++)
    {
        m_IV[i] = pIV[i];
    }
    m_bIV = TRUE;
}

CMyAes::~CMyAes()
{
}

//取整形数据低8位 左4
int CMyAes::GetLeft4Bit(int num)
{
    int nLeft = num & 0x000000f0;
    return nLeft >> 4;
}

//取整形数据低8位 右4
int CMyAes::GetRight4Bit(int num)
{
    return num & 0x0000000f;
}

//S盒取元素
int CMyAes::GetDataFromSBox(int index)
{
    int row = GetLeft4Bit(index);
    int col = GetRight4Bit(index);
    return S[row][col];
}

//从逆S盒取元素
int CMyAes::GetDataFromdeSBox(int index)
{
    int row = GetLeft4Bit(index);
    int col = GetRight4Bit(index);
    return deS[row][col];
}

//将16字符转成4*4数组
void CMyAes::PlainText16ToArray()
{
    int k = 0;
    for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++) 
    {
        nArrStateMetrix[j][i] = CharToInt(m_pPlainText[k]);
        k++;
    }
}

//将密文16字符转成4*4数组
void CMyAes::CipherText16ToArray()
{
    int k = 0;
    for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
    {
        nArrStateMetrix[j][i] = CharToInt(m_CipherText[k]);
        k++;
    }
}

//将初始化向量转成4*4数组
void CMyAes::IvText16ToArray()
{
    int k = 0;
    for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
    {
        nIvMetrix[j][i] = CharToInt(m_IV[k]);
        k++;
    }
}

//将字符转换为整型
int CMyAes::CharToInt(char ch)
{
    return (int)ch & 0x000000ff;
}

//把连续的4个字符合并成一个4字节的整型
int CMyAes::String4ToInt(char *str)
{
    int n1 = CharToInt(str[0]);
    n1 = n1 << 24;
    int n2 = CharToInt(str[1]);
    n2 = n2 << 16;
    int n3 = CharToInt(str[2]);
    n3 = n3 << 8;
    int n4 = CharToInt(str[3]);
    return n1 | n2 | n3 | n4;
}

//把一个整形按字节放入4个元素的整形数组
void CMyAes::IntToArray4(int num, int arrInt[4])
{
    int one = num >> 24;
    arrInt[0] = one & 0x000000ff;
    int two = num >> 16;
    arrInt[1] = two & 0x000000ff;
    int three = num >> 8;
    arrInt[2] = three & 0x000000ff;
    arrInt[3] = num & 0x000000ff;
}

//将数组中的元素循环左移step位
void CMyAes::LeftLoop4int(int array[4], int step)
{
    int ArrTmp[4] = { 0 };
    for (int i = 0; i < 4; i++)
    {
        ArrTmp[i] = array[i];
    }

    int index = step % 4 == 0 ? 0 : step % 4;

    for (int i = 0; i < 4; i++)
    {
        array[i] = ArrTmp[index];
        index++;
        index = index % 4;
    }
}

//将数组中的元素循环右移step位
void CMyAes::RightLoop4int(int array[4], int step)
{
    int ArrTmp[4] = { 0 };
    for (int i = 0; i < 4; i++)
    {
        ArrTmp[i] = array[i];
    }

    int index = step % 4 == 0 ? 0 : step % 4;
    index = 3 - index;
    for (int i = 3; i >= 0; i--) 
    {
        array[i] = ArrTmp[index];
        index--;
        index = index == -1 ? 3 : index;
    }
}

//将整形数组元素按字节转换为整形
int CMyAes::Array4ToInt(int arrInt[4])
{
    int n1 = arrInt[0] << 24;
    int n2 = arrInt[1] << 16;
    int n3 = arrInt[2] << 8;
    int n4 = arrInt[3];
    return n1 | n2 | n3 | n4;
}

//密钥扩展T函数
int CMyAes::T(int num, int round)
{
    int numArray[4] = { 0 };
    IntToArray4(num, numArray);
    LeftLoop4int(numArray, 1);

    //字节代换
    for (int i = 0; i < 4; i++)
    {
        numArray[i] = GetDataFromSBox(numArray[i]);
    }

    int nRet = Array4ToInt(numArray);
    return nRet ^ Rcon[round];
}

//列混淆函数
int CMyAes::GFMul2(int s)
{
    int nRet = s << 1;
    int a7 = nRet & 0x00000100;

    if (a7 != 0) {
        nRet = nRet & 0x000000ff;
        nRet = nRet ^ 0x1b;
    }
    return nRet;
}

int CMyAes::GFMul3(int s)
{
    return GFMul2(s) ^ s;
}

int CMyAes::GFMul4(int s)
{
    return GFMul2(GFMul2(s));
}

int CMyAes::GFMul8(int s)
{
    return GFMul2(GFMul4(s));
}

int CMyAes::GFMul9(int s)
{
    return GFMul8(s) ^ s;
}

int CMyAes::GFMul11(int s)
{
    return GFMul9(s) ^ GFMul2(s);
}

int CMyAes::GFMul12(int s)
{
    return GFMul8(s) ^ GFMul4(s);
}

int CMyAes::GFMul13(int s)
{
    return GFMul12(s) ^ s;
}

int CMyAes::GFMul14(int s)
{
    return GFMul12(s) ^ GFMul2(s);
}

int CMyAes::GFMul(int n, int s)
{
    int nRet = 0;

    if (n == 1)
        nRet = s;
    else if (n == 2)
        nRet = GFMul2(s);
    else if (n == 3)
        nRet = GFMul3(s);
    else if (n == 0x9)
        nRet = GFMul9(s);
    else if (n == 0xb)
        nRet = GFMul11(s);
    else if (n == 0xd)
        nRet = GFMul13(s);
    else if (n == 0xe)
        nRet = GFMul14(s);
    return nRet;
}

void CMyAes::ArrayToPlainText16()
{
    int k = 0;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            m_CipherText[k++] = (char)nArrStateMetrix[j][i];
        }   
    }
}

void CMyAes::ArrayToCipherText16()
{
    int k = 0;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            m_pPlainText[k++] = (char)nArrStateMetrix[j][i];
        }
    }
}

//扩展密钥
void CMyAes::ExtendKey()
{
    for (int i = 0; i < 4; i++)
        W[i] = String4ToInt(m_Key + i * 4);

    for (int i = 4, j = 0; i < 44; i++)
    {
        if (i % 4 == 0) {
            W[i] = W[i - 4] ^ T(W[i - 1], j);
            j++;
        }
        else {
            W[i] = W[i - 4] ^ W[i - 1];
        }
    }
}

//轮密钥加
void CMyAes::RoundAddKey(int round)
{
    int wArr[4] = { 0 };
    for (int i = 0; i < 4; i++) 
    {
        IntToArray4(W[round * 4 + i], wArr);

        for (int j = 0; j < 4; j++) 
        {
            nArrStateMetrix[j][i] = nArrStateMetrix[j][i] ^ wArr[j];
        }
    }
}

//字节代换
void CMyAes::ByteSub()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            nArrStateMetrix[i][j] = GetDataFromSBox(nArrStateMetrix[i][j]);
        }    
    }
}

//逆 字节代换
void CMyAes::deByteSub()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            nArrStateMetrix[i][j] = GetDataFromdeSBox(nArrStateMetrix[i][j]);
        }
    }
}

//行移位
void CMyAes::RowShift()
{
    int rowTwo[4] = { 0 };
    int rowThree[4] = { 0 };
    int rowFour[4] = { 0 };
    for (int i = 0; i < 4; i++) 
    {
        rowTwo[i] = nArrStateMetrix[1][i];
        rowThree[i] = nArrStateMetrix[2][i];
        rowFour[i] = nArrStateMetrix[3][i];
    }

    LeftLoop4int(rowTwo, 1);
    LeftLoop4int(rowThree, 2);
    LeftLoop4int(rowFour, 3);

    for (int i = 0; i < 4; i++) 
    {
        nArrStateMetrix[1][i] = rowTwo[i];
        nArrStateMetrix[2][i] = rowThree[i];
        nArrStateMetrix[3][i] = rowFour[i];
    }
}

//逆 行移位
void CMyAes::deShiftRows()
{
    int rowTwo[4] = { 0 };
    int rowThree[4] = { 0 };
    int rowFour[4] = { 0 };
    for (int i = 0; i < 4; i++)
    {
        rowTwo[i] = nArrStateMetrix[1][i];
        rowThree[i] = nArrStateMetrix[2][i];
        rowFour[i] = nArrStateMetrix[3][i];
    }

    RightLoop4int(rowTwo, 1);
    RightLoop4int(rowThree, 2);
    RightLoop4int(rowFour, 3);

    for (int i = 0; i < 4; i++)
    {
        nArrStateMetrix[1][i] = rowTwo[i];
        nArrStateMetrix[2][i] = rowThree[i];
        nArrStateMetrix[3][i] = rowFour[i];
    }
}

//列混合
void CMyAes::ColumnMix()
{
    int tmpArr[4][4] = { 0 };

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            tmpArr[i][j] = nArrStateMetrix[i][j];
        }

    }
   
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            nArrStateMetrix[i][j] = GFMul(colM[i][0], tmpArr[0][j]) ^ GFMul(colM[i][1], tmpArr[1][j])
                ^ GFMul(colM[i][2], tmpArr[2][j]) ^ GFMul(colM[i][3], tmpArr[3][j]);
        }
    } 
}

//逆 列混淆
void CMyAes::deColumnMix(int nArr[4][4])
{
    int tmpArr[4][4] = { 0 };

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            tmpArr[i][j] = nArr[i][j];
        }

    }

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            nArr[i][j] = GFMul(deColM[i][0], tmpArr[0][j]) ^ GFMul(deColM[i][1], tmpArr[1][j])
                ^ GFMul(deColM[i][2], tmpArr[2][j]) ^ GFMul(deColM[i][3], tmpArr[3][j]);
        }
    }
}

//异或两个数组
void CMyAes::Arr1AddArr2(int Arr1[4][4], int Arr2[4][4])
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            Arr1[i][j] = Arr1[i][j] ^ Arr2[i][j];
        }   
    }
}

void CMyAes::Str1CopytoStr2(char Str1[16], char Str2[16])
{
    for (int i = 0; i < 16; i++)
    {
        Str2[i] = Str1[i];
    }
}

//32位密钥W[i] => 整形数组
void CMyAes::GetnArrFromWi(int i, int nArr[4][4])
{
    int index = i * 4;
    int colOne[4] = { 0 };
    int colTwo[4] = { 0 };
    int colThree[4] = { 0 };
    int colFour[4] = { 0 };

    IntToArray4(W[index], colOne);
    IntToArray4(W[index + 1], colTwo);
    IntToArray4(W[index + 2], colThree);
    IntToArray4(W[index + 3], colFour);

    for (int i = 0; i < 4; i++) 
    {
        nArr[i][0] = colOne[i];
        nArr[i][1] = colTwo[i];
        nArr[i][2] = colThree[i];
        nArr[i][3] = colFour[i];
    }
}

BOOL CMyAes::AesEnc()
{
    if ((!m_bKeySet) || (!m_bPlainText))
    {
        return FALSE;
    }

    PlainText16ToArray();  //将字符明文转换成状态矩阵

    if (m_bIV)
    {
        IvText16ToArray();
        Arr1AddArr2(nArrStateMetrix, nIvMetrix);
    }

    RoundAddKey(0);     //首次的密钥轮加

    //1-9轮
    for (int i = 1; i < 10; i++)
    {
        ByteSub();      //字节代换

        RowShift();     //行移位

        ColumnMix();    //列混合

        RoundAddKey(i); //密钥轮加
    }

    //第10轮
    ByteSub();          //字节代换

    RowShift();         //行移位

    RoundAddKey(10);    //密钥轮加

    ArrayToPlainText16();  //将状态矩阵转换为密文字符数组

    if (m_bIV)
    {
        SetIV(m_CipherText);
    }

    //将状态改为未设置, 再次调用会失败, 除非重新调用setpt
    m_bPlainText = FALSE;

    return TRUE;

}

BOOL CMyAes::AesDec()
{
    if ((!m_bKeySet) || (!m_bCipherText))
    {
        return FALSE;
    }

    CipherText16ToArray();

    Str1CopytoStr2(m_CipherText, szTmpIv);

    RoundAddKey(10);

    int Arr2[4][4] = { 0 };
    for (int i = 9; i >= 1; i--) 
    {
        deByteSub();

        deShiftRows();

        deColumnMix(nArrStateMetrix);

        GetnArrFromWi(i, Arr2);

        deColumnMix(Arr2);

        Arr1AddArr2(nArrStateMetrix, Arr2);
    }
    deByteSub();

    deShiftRows();

    RoundAddKey(0);

    if (m_bIV)
    {
        IvText16ToArray();
        Arr1AddArr2(nArrStateMetrix, nIvMetrix);
    }

    ArrayToCipherText16();

    if (m_bIV)
    {
        SetIV(szTmpIv);
    }
    
    m_bCipherText = FALSE;

    return TRUE;
}
