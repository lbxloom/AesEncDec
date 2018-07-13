#pragma once
class CMyAes
{
public:
    CMyAes();
    virtual ~CMyAes();

    //��չ��Կ
    void ExtendKey();

    //����Կ��
    void RoundAddKey(int round);

    //�ֽڴ���
    void ByteSub();
    //�� �ֽڴ���
    void deByteSub();

    //����λ
    void RowShift();
    //�� ����λ
    void deShiftRows();

    //�л��
    void ColumnMix();
    //�� �л���
    void deColumnMix(int nArr[4][4]);
    //------------------------------------------------

    //ȡ�������ݵ�8λ ��4
    int GetLeft4Bit(int num);

    //ȡ�������ݵ�8λ ��4
    int GetRight4Bit(int num);

    //S��ȡԪ��
    int GetDataFromSBox(int index);
    //����S��ȡԪ��
    int GetDataFromdeSBox(int index);

    //������16�ַ�ת��4*4����
    void PlainText16ToArray();
    //������16�ַ�ת��4*4����
    void CipherText16ToArray();
    //����ʼ������ת��4*4����
    void IvText16ToArray();

    //���ַ�ת��Ϊ����
    int CharToInt(char ch);

    //��������4���ַ��ϲ���һ��4�ֽڵ�����
    int String4ToInt(char *str);

    //��һ�����ΰ��ֽڷ���4��Ԫ�ص���������
    void IntToArray4(int num, int arrInt[4]); 

    //�������е�Ԫ��ѭ������stepλ
    void LeftLoop4int(int array[4], int step);
    //�������е�Ԫ��ѭ������stepλ
    void RightLoop4int(int array[4], int step);

    //����������Ԫ�ذ��ֽ�ת��Ϊ����
    int Array4ToInt(int arrInt[4]);

    //��Կ��չT����
    int T(int num, int round);


    //�л�������
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

    //��4*4����ת��16�ַ�����
    void ArrayToPlainText16();
    //��4*4����ת��16�ַ�����
    void ArrayToCipherText16();

    //�����������
    void Arr1AddArr2(int Arr1[4][4], int Arr2[4][4]);

    //��������
    void Str1CopytoStr2(char Str1[16], char Str2[16]);

    //32λ��ԿW[i] => ��������
    void GetnArrFromWi(int i, int nArr[4][4]);
    
    //AES����
    BOOL AesEnc();

    //AES����
    BOOL AesDec();

    //������Կ
    void SetKey(char* pKey);

    //��������
    void SetPT(char* pPlainText);

    //��������
    void SetCT(char* pCipherText);

    //����IV
    void SetIV(char* pIV);
    
    
public:
    int W[44];           //��չ��Կ����
    static const int S[16][16]; //S��
    static const int deS[16][16]; //�� S��

    static const int Rcon[10];  //������ֵ��

    static const int colM[4][4];//�л�������
    static const int deColM[4][4]; //�� �л�������

    char m_pPlainText[16];      //�����ַ�����
    char m_CipherText[16];      //�����ַ�����
    int nArrStateMetrix[4][4];  //״̬����
    int nIvMetrix[4][4];        //��ʼ����������
    char szTmpIv[16];           //����ʱ������������, �����´ε�IV
    char m_Key[16];             //��Կ
    char m_IV[16];              //��ʼ������

    BOOL m_bKeySet;             //��ʾ��������Կ
    BOOL m_bPlainText;          //��ʾ����������
    BOOL m_bCipherText;         //��ʾ����������
    BOOL m_bIV;                 //��ʾ�����˳�ʼ������(CBCģʽ)
};

