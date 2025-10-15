#include <../../../../../include/Lab1/tbitfield.h>

TBitField::TBitField(int len)
{
	if (len <= 0)
	{
		throw std::out_of_range("The length of the bit field must be greater than 0"); //������� ����������.
	}

	BitLen = len; //����� �������� ����.
	unsigned int bit_telem = std::numeric_limits<TELEM>::digits; //������� ��� �������� TELEM.(���������� sizeof(TELEM)*8)
	MemLen = (BitLen + (bit_telem - 1)) / bit_telem; //���������� ��������� ���� TELEM � pMem ��� ������������� �������� ����.
	pMem = new TELEM[MemLen]; //������ ��� �������� ����.
	memset(pMem, 0, sizeof(TELEM)*MemLen); //�������������� ������� ���� (��� �����, � ���������� sizeof(TELEM)*MemLen) ������.
}

TBitField::TBitField(const TBitField& bf)
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; ++i) //�������� ������� ���� bf � ����� ������ ������.
	{
		pMem[i] = bf.pMem[i];
	}
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const
{
	if (n >= BitLen || n < 0)
	{
		throw std::out_of_range("Incorrect bit number"); //������� ����������.
	}

	return (MemLen - 1) -(n / std::numeric_limits<TELEM>::digits); //����� ������ � ������� pMem, ��� n-�� ���. std::numeric_limits<TELEM>::digits ����� ���������� ��� ���� TELEM.
	//(n / std::numeric_limits<TELEM>::digits) - ����� �������� pMem, ���� �� ���� ��� ����� �������. (MemLen - 1) -(n / std::numeric_limits<TELEM>::digits) - ��� ��� ����� �������� pMem, ����� ���� ���� ������ ������.
	//������: ����� ��� 64 (63..32, 31..0), n = 45, ������� 45 / 32 = 1, 2-1-1=0 (���� TELEm = unsigned int). �� ���� 45 ��� ��������� � pMem[0].
}

TELEM TBitField::GetMemMask(const int n) const //����� ������ ����� ��� ���������� sizeof(TELEM) (���� unsigned int, �� 4) ����, � ������� ��������� n-�� ���.
{
	if (n >= BitLen || n < 0)
	{
		throw std::out_of_range("Incorrect bit number"); //������� ����������.
	}

	unsigned int bit_telem = std::numeric_limits<TELEM>::digits; //������� ��� �������� TELEM.(����������� sizeof(TELEM)*8)
	TELEM mask = 1 << (n % bit_telem); //������� ������� ����� �� (n % bit_telem) ���, �������� �����.
	return mask;
}

int TBitField::GetLength(void) const
{
	return BitLen;
}

void TBitField::SetBit(const int n)
{
	if (n >= BitLen || n < 0)
	{
		throw std::out_of_range("Incorrect bit number"); //������� ����������.
	}

	pMem[GetMemIndex(n)] |= GetMemMask(n); //�������� �������� n-��� ���� �� �������. | - ��������� ���.
	//pMem[GetMemIndex(n)] = pMem[GetMemIndex(n)] | GetMemMask(n); - � ���������� ����.
}

void TBitField::ClrBit(const int n)
{
	if (n >= BitLen || n < 0)
	{
		throw std::out_of_range("Incorrect bit number"); //������� ����������.
	}

	pMem[GetMemIndex(n)] &= ~(GetMemMask(n)); //�������� �������� n-��� ���� �� ���� (1 & 0 = 0, 0 & 0 = 0). & - ��������� �. ~ - �������� �����(��������� ���������).
	//pMem[GetMemIndex(n)] = pMem[GetMemIndex(n)] & ~(GetMemMask(n)); - � ���������� ����.
}

int  TBitField::GetBit(const int n) const
{
	if (n >= BitLen || n < 0)
	{
		throw std::out_of_range("Incorrect bit number"); //������� ����������.
	}

	return (pMem[GetMemIndex(n)] & GetMemMask(n)) == 0 ? 0 : 1; //�������� �������� �������. & - ��������� �. 0 & 1 = 0. 1 & 1 = 1.
}

int TBitField::operator==(const TBitField& bf) const
{
	if (BitLen != bf.BitLen) //��������� �� ������������ ����� ������� �����.
	{
		return 0;
	}

	for (int i = 0; i < MemLen; ++i) //��������� ������� ���� �� �������� ��������� pMem.
	{
		if (pMem[i] != bf.pMem[i])
		{
			return 0;
		}
	}

	return 1;
}

int TBitField::operator!=(const TBitField& bf) const
{
	return (*this == bf) ? 0 : 1;//*this == bf (������������� �������� == ��� ������� �����, � �� ��� ���� �������� �� BitLen != bf.BitLen). ���� ����� 1, �� ������� 0. ���� ������� 0, �� ���� ������� �� ����������, ������� 1.
}

TBitField& TBitField::operator=(const TBitField& bf)
{
	if (this == &bf) //�������� �� ��, ��� ������� �� �� ����� ������� ����.
	{
		return *this;
	}

	delete[] pMem; //������� ������ ������ ��������� �������� ����.

	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; ++i) //�������� ������� ���� bf � ������ (��������) ������� ����.
	{
		pMem[i] = bf.pMem[i];
	}

	return *this; //���������� ������ �� ������.
}

TBitField TBitField::operator|(const TBitField& bf)
{
	int MaxBitLen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
	TBitField res(MaxBitLen); //������ �������� ������� ���� ��������, ������ ������� ������ �������� �������� ���� �� ������������.

	const TBitField& LongBF = (BitLen >= bf.BitLen) ? *this : bf; //����� ����� ������� ������� ����.
	const TBitField& ShortBF = (BitLen < bf.BitLen) ? *this : bf; //����� ����� �������� ������� ����.

	int i;
	for (i = 0; i < (LongBF.MemLen - ShortBF.MemLen); ++i)
	{
		res.pMem[i] = LongBF.pMem[i]; //������������ �����, ������� ��� � ������� ������� ����, � �������� ������� ����.
	}
	int j = 0;
	for (i, j; i < LongBF.MemLen; ++i, ++j)
	{
		res.pMem[i] = LongBF.pMem[i] | ShortBF.pMem[j]; //���������� ��������� ��� ���� ���� ������� �����.
	}
	return res;
}

TBitField TBitField::operator&(const TBitField& bf)
{
	int MaxBitLen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
	TBitField res(MaxBitLen); //������ �������� ������� ���� ��������, ������ ������� ������ �������� �������� ���� �� ������������.

	const TBitField& LongBF = (BitLen >= bf.BitLen) ? *this : bf; //����� ����� ������� ������� ����.
	const TBitField& ShortBF = (BitLen < bf.BitLen) ? *this : bf; //����� ����� �������� ������� ����.

	int i;
	for (i = 0; i < (LongBF.MemLen - ShortBF.MemLen); ++i)
	{
		res.pMem[i] = 0; //�������������� � �������� ������� ���� �� ����� ������, ������� ��� � ������� ������� ����.
	}
	int j = 0;
	for (i, j; i < LongBF.MemLen; ++i, ++j)
	{
		res.pMem[i] = LongBF.pMem[i] & ShortBF.pMem[j]; //���������� ��������� � ���� ���� ������� �����.
	}
	return res;
}

TBitField TBitField::operator~(void)
{
	TBitField res(BitLen); //������ �������� ������� ����, ��������, ������ ������� ��������� �������� ����.
	for (int i = MemLen - 1; i > 0; --i)
	{
		res.pMem[i] = ~(pMem[i]);
	}
	const int bit_in_elem = std::numeric_limits<TELEM>::digits; //std::numeric_limits<TELEM>::digits (���������� sizeof(TELEM)*8).
	int bit_in_BF = -1, ind_bit_in_BF = -1;
	for (int k=0; k < bit_in_elem && ind_bit_in_BF < BitLen - 1; ++k) //����������� ����� ������� ����(����������� "��������" ������� � pMem). (ind_bit_in_BF < BitLen - 1) - ���������, �� �������� �� �� ���������� � ��������������� ����.
	{
		ind_bit_in_BF = (bit_in_elem * (MemLen - 1)) + k;
		bit_in_BF = GetBit(ind_bit_in_BF);
		if (bit_in_BF)
		{
			res.ClrBit(ind_bit_in_BF);
		}
		else
		{
			res.SetBit(ind_bit_in_BF);
		}
	}
	return res;
}

//������ �����:********, ��� * - 0 ��� 1.
std::istream& operator>>(std::istream& istr, TBitField& bf) //���� ������� ������� ���� ������, ��� ������ ���� bf, �� ��������� ������ ������ (�������) ����. ���� ������� ������� ���� ������, ��� ������ ���� bf, �� ��������� ��� ����, �� ������� ���� � bf ����� ����� 0. ��� ��������� - �� ������������.
{
	for (int i = 0; i < bf.GetLength(); ++i) //������� ������� ���� �� ������ ��������.
	{
		bf.ClrBit(i);
	}

	std::string str; //������ ����� ��� ����� �� ������� (������).
	istr >> str; //���� �� �������.
	int str_len = str.length(), str_bit = -1;
	for (int i = 0; i < str_len; ++i) //�������� �� ��, ��� ������� �� 0 ��� �� 1.
	{
		str_bit = str[i] - '0'; //���� str[i] == '0', �� str[i] ����� 48. ���� str[i] == '1', �� str[i] ����� 49. '0' == 48.
		if(!(str_bit == 0 || str_bit == 1))
		{
			throw std::invalid_argument("A non-bit field has been introduced");
		}
	}
	str_bit = -1;
	for (int i = 0, j = str_len - 1; i < bf.BitLen && j >= 0; ++i, --j) //���������� ���� � ������� ����, ������� � ����� ������ (������) � � ����� �������� ����.
	{
		str_bit = str[j] - '0'; //���� str[i] == '0', �� str[i] ����� 48. ���� str[i] == '1', �� str[i] ����� 49. '0' == 48.
		if (str_bit)
		{
			bf.SetBit(i);
		}
		else
		{
			bf.ClrBit(i);
		}
	}
	return istr;
}
std::ostream& operator<<(std::ostream& ostr, const TBitField& bf)
{
	for (int i = bf.BitLen - 1; i >= 0; --i)
	{
		ostr << bf.GetBit(i);
	}
	return ostr;
}