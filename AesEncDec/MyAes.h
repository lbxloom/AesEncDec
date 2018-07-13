#pragma once
class CMyAes
{
public:
    CMyAes();
    virtual ~CMyAes();

    //扩展密钥
    void ExtendKey();

    //轮密钥加
    void RoundAddKey(int round);

    //字节代换
    void ByteSub();
    //逆 字节代换
    void deByteSub();

    //行移位
    void RowShift();
    //逆 行移位
    void deShiftRows();

    //列混合
    void ColumnMix();
    //逆 列混淆
    void deColumnMix(int nArr[4][4]);
    //------------------------------------------------

    //取整形数据低8位 左4
    int GetLeft4Bit(int num);

    //取整形数据低8位 右4
    int GetRight4Bit(int num);

    //S盒取元素
    int GetDataFromSBox(int index);
    //从逆S盒取元素
    int GetDataFromdeSBox(int index);

    //将明文16字符转成4*4数组
    void PlainText16ToArray();
    //将密文16字符转成4*4数组
    void CipherText16ToArray();
    //将初始化向量转成4*4数组
    void IvText16ToArray();

    //将字符转换为整型
    int CharToInt(char ch);

    //把连续的4个字符合并成一个4字节的整型
    int String4ToInt(char *str);

    //把一个整形按字节放入4个元素的整形数组
    void IntToArray4(int num, int arrInt[4]); 

    //将数组中的元素循环左移step位
    void LeftLoop4int(int array[4], int step);
    //将数组中的元素循环右移step位
    void RightLoop4int(int array[4], int step);

    //将整形数组元素按字节转换为整形
    int Array4ToInt(int arrInt[4]);

    //密钥扩展T函数
    int T(int num, int round);


    //列混淆函数
    int GFMul2(int s);
    int GFMul3(int s);
    int GFMul4(int s);
    int GFMul8(int s);
    int GFMul9(int s);
    int GFMul11(int s);
    int GFMul12(int s);
    int GFMul13(int s);
    int GFMul14(int s);
    int GFMul(int n, int s);

    //将4*4数组转成16字符明文
    void ArrayToPlainText16();
    //将4*4数组转成16字符密文
    void ArrayToCipherText16();

    //异或两个数组
    void Arr1AddArr2(int Arr1[4][4], int Arr2[4][4]);

    //复制数组
    void Str1CopytoStr2(char Str1[16], char Str2[16]);

    //32位密钥W[i] => 整形数组
    void GetnArrFromWi(int i, int nArr[4][4]);
    
    //AES加密
    BOOL AesEnc();

    //AES解密
    BOOL AesDec();

    //设置密钥
    void SetKey(char* pKey);

    //设置明文
    void SetPT(char* pPlainText);

    //设置密文
    void SetCT(char* pCipherText);

    //设置IV
    void SetIV(char* pIV);
    
    
public:
    int W[44];           //扩展密钥数组
    static const int S[16][16]; //S盒
    static const int deS[16][16]; //逆 S盒

    static const int Rcon[10];  //常量轮值表

    static const int colM[4][4];//列混淆矩阵
    static const int deColM[4][4]; //逆 列混淆矩阵

    char m_pPlainText[16];      //明文字符数组
    char m_CipherText[16];      //密文字符数组
    int nArrStateMetrix[4][4];  //状态矩阵
    int nIvMetrix[4][4];        //初始化向量数组
    char szTmpIv[16];           //解密时用来保存密文, 当作下次的IV
    char m_Key[16];             //密钥
    char m_IV[16];              //初始化向量

    BOOL m_bKeySet;             //表示设置了密钥
    BOOL m_bPlainText;          //表示设置了明文
    BOOL m_bCipherText;         //表示设置了密文
    BOOL m_bIV;                 //表示设置了初始化向量(CBC模式)
};

