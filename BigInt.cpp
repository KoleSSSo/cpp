#include "BigInt.h"


BigInt::BigInt(int x)
{
	this->sign = (x < 0) ? false : true;
	this->number.push_back(abs(x));
}

//Проверка корректности данныъ!
static int isValid(std::string arg)
{
	for (auto i : arg)
	{
		if (!isdigit(i)) throw std::invalid_argument("Argument is not a valid number");
	}
}

//BigInt Конструктор.
BigInt::BigInt(std::string other)
{
	bool sign = (other[0] == '-' ? false : true);

	std::string tempStr;
	for (int i = other.length(); i > 0 + !sign; i -= zerosNums)
	{
		if (i > zerosNums)
		{
			tempStr = other.substr(i - zerosNums, zerosNums);
			isValid(tempStr);
			this->number.push_back(std::stoi(tempStr, nullptr, 10));
		}
		else
		{
			tempStr = other.substr(!sign, i - !sign);
			isValid(tempStr);
			this->number.push_back(std::stoi(tempStr, nullptr, 10));
			break;
		}
	}

	this->sign = sign;
}

BigInt::BigInt(const BigInt& other)
{
	this->sign = other.sign;
	this->number = other.number;
}
//
//BigInt::BigInt(unsigned int other)
//{
//	this->sign = (other < 0) ? false : true;
//	this->number.push_back(other);
//}

BigInt::operator int() const
{
	return this->number[0];
}

BigInt::operator std::string() const
{
	std::string outputValue, tempStr;

	if (this->number == this->zero) return outputValue += "0";
	if (!this->sign) outputValue.push_back('-');

	int currentSize = this->number.size() - 1;
	for (long long int i = currentSize; i >= 0; i--)
	{
		tempStr.erase();
		if (this->number[i] == 0) tempStr = "000000000";
		else if (this->number[i] < (this->base / 10 - 1) && i != currentSize)
		{
			int temp = this->number[i], count = 0;
			while (temp < (this->base / 10 - 1) && i != currentSize)
			{
				temp *= 10;
				count++;
			}
			for (int j = 0; j < count; j++)
				tempStr += "0";
		}

		tempStr += std::to_string(this->number[i]);
		outputValue += tempStr;
	}
	return outputValue;
}

//Добавить нули в начало
void BigInt::addLeadingZeros(int num)
{
	for (int i = 0; i < num; i++)
	{
		this->number.push_back(0);
	}
}

//Убрать нули 
void BigInt::removeLeadingZeros()
{
	if (this->number.size() == 0) return;
	while (this->number.size() != 1 && this->number.back() == 0)
	{
		this->number.pop_back();
	}
}

//Подсчёт незначащих нулей
int BigInt::countLeadingNullsInString(std::string str)
{
	long long int count = 0;
	char currentSymbol = str[count];
	while (currentSymbol == '0')
	{
		currentSymbol = str[++count];
	}

	return count;
}

size_t BigInt::size() const
{
	return this->number.size();
}


std::ostream& operator<<(std::ostream& o, const BigInt& i)
{
	std::string outputValue = std::string(i);
	for (long long i = 0; i < outputValue.size(); i++)
		o << outputValue[i];
	return o;
}

std::istream& operator>>(std::istream& o, const BigInt& i)
{
	BigInt& i_ref = const_cast<BigInt&>(i);
	std::string inputValue;

	o >> inputValue;
	i_ref = BigInt(inputValue);

	return o;
}

const BigInt& min(const BigInt& a, const BigInt& b)
{
	if (a <= b) return a;
	return b;
}

const BigInt& max(const BigInt& a, const BigInt& b)
{
	if (a < b) return b;
	return a;
}


//Остаток от деления на 2
int _2_divide(std::vector<int>& a)
{
	int rest = 0, b = 2;
	for (int i = a.size() - 1; i >= 0; --i)
	{
		//1LL = 1 * sizeof(long long)
		long long cur = a[i] + rest * 1LL * 1000000000;
		a[i] = static_cast<int>(cur / b);
		rest = static_cast<int>(cur % b);
	}
	while (a.size() > 1 && a.back() == 0)
		a.pop_back();

	return rest;
}

//Конвертирование BigInt в СС с основанием 2^30 
static std::vector<int> _to_2_30Base(BigInt& a)
{
	int count_bits = 0;
	std::vector<int> _currentNumCopy(a.number);
	std::vector<int> num_2_30Base;

	num_2_30Base.reserve(a.number.size() * 2);
	while (_currentNumCopy[0] >= 1)
	{
		int carry = _2_divide(_currentNumCopy);
		if (count_bits / 30 >= num_2_30Base.size())
			num_2_30Base.push_back((a.sign) ? carry : ~carry);
		else
			num_2_30Base[count_bits / 30] ^= (carry << (count_bits % 30));

		count_bits++;
	}

	if (!a.sign) num_2_30Base[0]++;

	return num_2_30Base;
}

BigInt powBI(const BigInt& a, const BigInt& b)
{
	BigInt outputValue(1);
	for (BigInt i(0); i < b; ++i)
	{
		outputValue *= a;
	}

	return outputValue;
} 

//Битовые операции
BigInt BigInt::operator~() const
{
	BigInt a = *this;
	a += 1;
	a.sign = !a.sign;
	if (a.number.size() == 1 && a.number[0] == 0) a.sign = true;
	return a;
}

BigInt operator&(const BigInt& a, const BigInt& b)
{
	BigInt outputValue(0), outputValue_2_30, _2_30Base("1073741824");
	BigInt& _a = const_cast<BigInt&>(a), & _b = const_cast<BigInt&>(b);

	std::vector<int> a_2_30 = _to_2_30Base(_a);
	std::vector<int> b_2_30 = _to_2_30Base(_b);

	while (a_2_30.size() < b_2_30.size()) a_2_30.push_back((a.sign) ? 0 : -1);
	while (a_2_30.size() > b_2_30.size()) b_2_30.push_back((b.sign) ? 0 : -1);

	for (int i = 0; i < a_2_30.size(); i++)
		outputValue_2_30.number.push_back((a_2_30[i] & b_2_30[i]));

	outputValue_2_30.removeLeadingZeros();

	//Обратно в 10^9
	for (int i = 0; i < outputValue_2_30.number.size(); i++)
		outputValue += (BigInt(outputValue_2_30.number[i]) * powBI(_2_30Base, BigInt(i)));

	outputValue.sign = a.sign || b.sign;

	return outputValue;
}

//Bitwise 'or'
BigInt operator|(const BigInt& a, const BigInt& b)
{
	BigInt outputValue(0), outputValue_2_30, _2_30Base("1073741824");
	BigInt& _a = const_cast<BigInt&>(a), & _b = const_cast<BigInt&>(b);

	std::vector<int> a_2_30 = _to_2_30Base(_a);
	std::vector<int> b_2_30 = _to_2_30Base(_b);

	while (a_2_30.size() < b_2_30.size()) a_2_30.push_back((a.sign) ? 0 : -1);
	while (a_2_30.size() > b_2_30.size()) b_2_30.push_back((b.sign) ? 0 : -1);

	for (int i = 0; i < a_2_30.size(); i++)
		outputValue_2_30.number.push_back((a_2_30[i] | b_2_30[i]));

	outputValue_2_30.removeLeadingZeros();
	
	//Обратно в 10^9
	for (int i = 0; i < outputValue_2_30.number.size(); i++)
		outputValue += (BigInt(outputValue_2_30.number[i]) * powBI(_2_30Base, BigInt(i)));

	outputValue.sign = a.sign && b.sign;

	return outputValue;
}

//Xor
BigInt operator^(const BigInt& a, const BigInt& b)
{
	BigInt outputValue(0), outputValue_2_30, _2_30Base("1073741824");

	BigInt& _a = const_cast<BigInt&>(a),
		& _b = const_cast<BigInt&>(b);

	std::vector<int> a_2_30 = _to_2_30Base(_a);
	std::vector<int> b_2_30 = _to_2_30Base(_b);

	while (a_2_30.size() < b_2_30.size()) a_2_30.push_back((a.sign) ? 0 : -1);
	while (a_2_30.size() > b_2_30.size()) b_2_30.push_back((b.sign) ? 0 : -1);

	for (int i = 0; i < a_2_30.size(); i++)
		outputValue_2_30.number.push_back((a_2_30[i] ^ b_2_30[i]));

	outputValue_2_30.removeLeadingZeros();

	//Обратно в 10^9
	for (int i = 0; i < outputValue_2_30.number.size(); i++)
		outputValue += (BigInt(outputValue_2_30.number[i]) * powBI(_2_30Base, BigInt(i)));

	outputValue.sign = !(a.sign ^ b.sign);

	return outputValue;
}

BigInt& BigInt::operator&=(const BigInt& other)
{
	return *this = *this & other;
}

BigInt& BigInt::operator|=(const BigInt& other)
{
	return *this = *this | other;
}


BigInt& BigInt::operator^=(const BigInt& other)
{
	return *this = *this ^ other;
}

// Унарные знаки
BigInt BigInt::operator+() const
{
	BigInt outputValue(*this);
	if (!outputValue.sign) outputValue.sign = !outputValue.sign;
	return outputValue;
}

BigInt BigInt::operator-() const
{
	BigInt outputValue(*this);
	outputValue.sign = !outputValue.sign;
	return outputValue;
}


//Логические операции
bool BigInt::operator==(const BigInt& other) const
{

	if (this->sign != other.sign)
		return false;

	if (this->number.size() != other.number.size())
		return false;

	for (long long numbers_position = this->number.size() - 1; numbers_position >= 0; numbers_position--)
	{
		if (this->number[numbers_position] != other.number[numbers_position])
			return false;
	}

	return true;
}

bool BigInt::operator!=(const BigInt& other) const
{
	return !(*this == other);
}

bool BigInt::operator<(const BigInt& other) const
{
	if (*this == other) return false;

	if (this->sign > other.sign || this->number.size() > other.number.size()) return false;
	if (this->sign < other.sign || this->number.size() < other.number.size()) return true;

	switch (this->sign)
	{
	case(true):	//If both values are positive
		for (int i = this->number.size() - 1; i >= 0; i--)
		{
			if (this->number[i] < other.number[i]) return true;
			if (this->number[i] > other.number[i]) return false;
		}
		break;

	case(false): //If both values are negative
		for (int i = this->number.size() - 1; i >= 0; i--)
		{
			if (this->number[i] > other.number[i]) return true;
			if (this->number[i] < other.number[i]) return false;
		}
		break;
	}

	return false;
}

bool BigInt::operator>(const BigInt& other) const
{
	if (!(*this < other) && (*this != other))
		return true;
	return false;
}

bool BigInt::operator<=(const BigInt& other) const
{
	if (*this < other || *this == other)
		return true;
	return false;
}

bool BigInt::operator>=(const BigInt& other) const
{
	if (*this > other || *this == other)
		return true;
	return false;
}

//Арифметические операции

BigInt operator+(const BigInt& a, const BigInt& b)
{
	if (!a.sign && b.sign) return b - (-a);
	if (!a.sign && !b.sign) return -(a - b);

	unsigned long long int rest = 0;
	unsigned long long int num = 0;

	BigInt outputValue = BigInt();
	BigInt& minValue = const_cast<BigInt&>(min(a, b));
	BigInt& maxValue = const_cast<BigInt&>(max(a, b));

	unsigned long long int base = outputValue.getBase();

	minValue.addLeadingZeros(maxValue.number.size() - minValue.number.size());

	for (auto i = 0; i < maxValue.number.size(); i++)
	{
		num = maxValue.number[i] * 1ULL + minValue.number[i] + rest;
		outputValue.number.push_back(static_cast<unsigned int>(num % base));
		rest = num / base;
	}

	outputValue.removeLeadingZeros();
	minValue.removeLeadingZeros();

	return outputValue;
}

BigInt operator-(const BigInt& a, const BigInt& b)
{
	if (a.sign == false && b.sign == false) return -(-a - -b);
	if (b.sign == false) return a + (-b);
	if (a.sign == false) return -(-a + b);
	if (a < b) return -(b - a);

	long long int rest = 0;
	long long int num = 0;

	BigInt outputValue;
	outputValue.number.reserve(a.number.size() * 3);

	BigInt& minValue = const_cast<BigInt&>(min(a, b));
	BigInt& maxValue = const_cast<BigInt&>(max(a, b));

	unsigned int base = outputValue.getBase();
	for (auto i = 0; i < maxValue.number.size(); i++)
	{
		num = (i < b.number.size() ? static_cast<int>(a.number[i] - b.number[i] - rest) : static_cast<int>(a.number[i] - rest));
		rest = num < 0;
		if (rest) num += base;
		outputValue.number.push_back(static_cast<unsigned int>(num));
	}

	outputValue.removeLeadingZeros();

	return outputValue;
}

BigInt operator*(const BigInt& a, const BigInt& b)
{
	BigInt outputValue = BigInt();
	unsigned int base = outputValue.getBase();
	outputValue.number.resize(a.number.size() + b.number.size());

	if (b.number == b.zero) return BigInt(0);
	if (b.number == b.one) return a;

	unsigned long long int rest, current;
	for (auto i = 0; i < a.number.size(); i++)
	{
		rest = 0;
		for (auto j = 0; j < b.number.size() || rest; j++)
		{
			current = outputValue.number[i + j] + a.number[i] * 1ULL * (j < b.number.size() ? b.number[j] : 0) + rest;
			outputValue.number[i + j] = static_cast<unsigned int>(current % base);
			rest = static_cast<unsigned int>(current / base);
		}
	}

	if (a.sign == b.sign) outputValue.sign = true;
	else outputValue.sign = false;

	outputValue.removeLeadingZeros();

	return outputValue;
}

BigInt operator/(const BigInt& a, const BigInt& b)
{
	if (b.number == b.zero) throw "Division by zero!";
	if (a < b) return 0;

	unsigned int quotient, left, middle, right;
	BigInt tmp, result, part;

	result.sign = (a.sign == b.sign);
	result.number.resize(0);
	part.number.resize(0);

	for (long long pos = a.number.size() - 1; pos >= 0; pos--)
	{
		std::reverse(part.number.begin(), part.number.end());
		part.number.push_back(a.number[pos]);
		std::reverse(part.number.begin(), part.number.end());

		quotient = 0;
		left = 0;
		right = 1000000000;

		while (left <= right)
		{
			middle = (left + right) / 2;
			tmp = b * BigInt(middle);

			if (tmp <= part)
			{
				quotient = middle;
				left = middle + 1;
			}
			else
			{
				right = middle - 1;
			}
		}

		part = part - (b * BigInt(quotient));

		if (!result.number.empty() || quotient != 0)
			result.number.push_back(quotient);

		if (part.number == a.zero)
			part.number.resize(0);
	}

	std::reverse(result.number.begin(), result.number.end());
	return result;
}

BigInt operator%(const BigInt& a, const BigInt& b)
{
	if (b == a) return 0;
	if (b > a) return a;

	BigInt outputValue;
	outputValue = (a - ((a / b) * b));

	return outputValue;
}


BigInt& BigInt::operator-=(const BigInt& other)
{
	return *this = *this - other;
}

BigInt& BigInt::operator/=(const BigInt& other)
{
	return *this = *this / other;
}

BigInt& BigInt::operator%=(const BigInt& other)
{
	return *this = *this % other;
}

BigInt& BigInt::operator*=(const BigInt& other)
{
	return *this = *this * other;
}

BigInt& BigInt::operator--()
{
	return *this = (*this - BigInt(1));
}

const BigInt BigInt::operator--(int)
{
	*this -= 1;
	return *this;
}


BigInt& BigInt::operator+=(const BigInt& other)
{
	return *this = *this + other;
}

BigInt& BigInt::operator++()
{
	return *this += 1;
}

const BigInt BigInt::operator++(int)
{
	*this += 1;
	return *this;
}

BigInt& BigInt::operator=(const BigInt& other)
{
	if (*this == other) return *this;

	if (other.number.size() == 0) return *this = BigInt();

	this->number.resize(other.number.size());
	std::copy(other.number.begin(), other.number.end(), this->number.begin());
	this->sign = other.sign;

	return *this;
}

