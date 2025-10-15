#include <../../../../../include/Lab1/tbitfield.h>
#include <../../../../../include/Lab1/tset.h>

#include <string>

TSet::TSet(int mp) : BitField(mp), MaxPower(mp) {} //������ �������������. �� ��������� mp, ��� ��� �������� ���� � ������������ TBitField.

TSet::TSet(const TSet& s) : MaxPower(s.MaxPower), BitField(s.BitField) {} //������ �������������. BitField(s.BitField) - ��������� ����������� ����������� TBitField.

TSet::TSet(const TBitField& bf) : MaxPower(bf.GetLength()), BitField(bf) {} //������ �������������. BitField(bf) - ��������� ����������� ����������� TBitField.

TSet::operator TBitField() //TSet ������������� � TBitField.
{
	return BitField;
}

int TSet::GetMaxPower(void) const //������������ �������� ��������� (�������� ��������).
{
	return MaxPower;
}

void TSet::InsElem(const int Elem) //�������� ������� � ���������.
{
	BitField.SetBit(Elem); //�� ��������� Elem �� ������������, ��� ��� �������� ���� � SetBit(). (MaxPower = BitField.BitLen).
}

void TSet::DelElem(const int Elem) //������� ������� �� ���������.
{
	BitField.ClrBit(Elem); //�� ��������� Elem �� ������������, ��� ��� �������� ���� � ClrBit(). (MaxPower = BitField.BitLen).
}

int TSet::IsMember(const int Elem) const //��������� ������� �������� � ���������.
{
	return (BitField.GetBit(Elem)) ? 1 : 0; //��������� ��������. �� ��������� Elem �� ������������, ��� ��� �������� ���� � GetBit(). (MaxPower = BitField.BitLen). ���� ������� � ��������� => 1. ��� �������� � ��������� => 0.
}

int TSet::operator== (const TSet& s) const //C�������� ��������.
{
	return (BitField == s.BitField) ? 1 : 0; //��������� ��������. ���������� ������� ���� (������������������ �������) �������� (����� ���������� ������������� �������� == ��� ������� �����, ��� (MaxPower != s.MaxPower) ��� �����������, ��� ��� MaxPower = BitLen, s.MaxPower = s.BitLen). ����� => 1. �� ����� => 0.
}

int TSet::operator!= (const TSet& s) const //��������� ��������.
{
	return (*this == s) ? 0 : 1; //��������� ��������. ���������� ���������: (*this == s) - ���������� ������������� �������� == ��� ��������, ��� ��� ����������� (MaxPower != s.MaxPower). ����� => 0. �� ����� => 1.
}

TSet& TSet::operator=(const TSet& s) //������������ ��������� ������� ���������.
{
	MaxPower = s.MaxPower;
	BitField = s.BitField; //������������� �������� = ��� ������� �����.

	return *this; //���������� ������ �� ������.
}

TSet TSet::operator+ (const int Elem) //����������� ��������� � ���������. ������� ������ ���� �� ���� �� ��������, ��� � �������� ���������.
{
	TSet res(*this); //������ ����� ��������� ��������� (����������� �����������). ��� �������� ���������.
	res.InsElem(Elem); //�������� �� ���������� ��������� ����� ����� ���� (� InsElem()). ���� ������� ��� ��� � ��������� - ���� �����������.
	return res;
}

TSet TSet::operator- (const int Elem) //�������� ��������� � ���������. ������� ������ ���� �� ���� �� ��������, ��� � �������� ���������.
{
	TSet res(*this); //������ ����� ��������� ��������� (����������� �����������). ��� �������� ���������.
	res.DelElem(Elem); //�������� �� ���������� ��������� ����� ����� ���� (� DelElem()). ���� �������� �� ���� � ��������� - ���� �����������.
	return res;
}

TSet TSet::operator+ (const TSet& s) //����������� ��������.
{
	if (MaxPower != s.MaxPower) //�������� ����������� �������� �� ���������� ��� �������� ������ ���������.
	{
		throw std::invalid_argument("The universes of the sets do not coincide");
	}
	TSet res(MaxPower); //������ �������� ���������. ��������� ����������� ��������.
	res.BitField = BitField | s.BitField; //���������� ������������� �������� ���������� ��� ��� ������� �����.
	res.DelElem(0); //�� ����������� ������� ������� ���������.
	return res;
}

TSet TSet::operator* (const TSet& s) //����������� ��������.
{
	if (MaxPower != s.MaxPower) //�������� ����������� �������� �� ���������� ��� �������� ������ ���������.
	{
		throw std::invalid_argument("The universes of the sets do not coincide");
	}
	TSet res(MaxPower); //������ �������� ���������. ��������� ����������� ��������.
	res.BitField = BitField & s.BitField; //���������� ������������� �������� ���������� � ��� ������� �����.
	res.DelElem(0); //�� ����������� ������� ������� ���������.
	return res;
}

TSet TSet::operator~ (void) //���������� ��������.
{
	TSet res(MaxPower); //������ �������� ���������. ��������� ���������� ���������.
	res.BitField = ~(BitField); //���������� ������������� �������� ���������� �� ��� ������� �����.
	res.DelElem(0); //�� ����������� ������� ������� ���������.
	return res;
}

std::istream& operator>>(std::istream& istr, TSet& bf) //������ �����:{ * * * * * * * }, ��� * - ����� ���������� ������ �������� ���������. {} => ������ ���������.
{
	for (int i = 0; i < bf.MaxPower; ++i) //���������� ��������� ��� ������ ������.
	{
		bf.DelElem(i);
	}

	std::string str; //������ ����� ��� ����� �� ������� (������).
	std::getline(istr, str); //���� �� �������.
	int str_len = str.length(), str_ind = -1; 
	std::string str_elem; //����� ��� ��������� �������� ��������� ���������.

	if (str.empty() || str.front() != '{' || str.back() != '}') //{ - ������ ���� ������ �������� ��������. } - ������ ���� ��������� �������� ��������. ����� �� ������� ���� ������.
	{
		throw std::invalid_argument("Incorrect input");
	}
	
	for (int i = 1; i < str_len-1; ++i) //�������� ��� ������� ������, ����� ������� � ����������.
	{
		switch (str[i])
		{
			case '{':
				throw std::invalid_argument("The opening curly brace should be the only one"); //�������� �� ��, ��� { ����������� ��� ���. �������� { 1 { 2 }. 
			case ' ':
				break;
			case '0':  case '1': case '2':  case '3': case '4':  case '5': case '6':  case '7': case '8':  case '9':
				str_elem.push_back(str[i]); //push_back() - ��������� ������ � ����� ������.
				for (i = i + 1; str[i] != ' ' && str[i] != '}'; ++i) //������� ��������� ������� �� ������� (��� �� }), ����� ��������� ������.
				{
					if (!(str[i] >= 48 && str[i] <= 57)) //��������� ����������� ������� �� �����.
					{
						throw std::invalid_argument("It is not a number that has been sent for input");
					}
					str_elem.push_back(str[i]);
				}
				str_ind = std::stoi(str_elem); //�������� ������ �������� ���������.
				bf.InsElem(str_ind); //���������� ������ �������� � ���������.
				str_elem.clear(); //������� ����� ��� �������� ��������� ���������.
				if (str[i] == '}')
				{
					i = i - 1;
				}
				break;
			case '}':
				throw std::invalid_argument("The closing curly brace should be the only one"); //�������� �� ��, ��� } ����������� ��� ���. �������� { 1 } 2 }.
			default:
				throw std::invalid_argument("Invalid character passed"); //�������� ������������ ������.
		}
	}
	return istr;
}

std::ostream& operator<<(std::ostream& ostr, const TSet& bf)
{
	ostr << "{";
	int extra = 0; //�������������� ���������� ��� ��������� ������.
	for (int i = 0; i < bf.MaxPower; ++i)
	{
		if (bf.BitField.GetBit(i))
		{
			if (extra == 0) //����� ������� ����� ������ {.
			{
				ostr << " ";
				extra = 1;
			}
			ostr << i << " ";
		}
	}
	ostr << "}";
	return ostr;
}