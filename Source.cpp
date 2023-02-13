#include <iostream>
#include "BigInt.h"
#include <assert.h>

int main() {

	BigInt a("9457326758647587085645678906474444444449444444444444444444448565684567457657975685698569569659987654364953762");
	BigInt c("-57372235775373573757237357737572377377572357737357723773257735873883298539839825983982359832598398534634599385302853");

	BigInt s("-12345678987654321");
	BigInt u(170501197);
	BigInt w("987654321123456789");

	std::cout << (a - c) << std::endl;
	std::cout << (a + c) << std::endl;
	std::cout << (a * c) << std::endl;
	std::cout << (c / a) << std::endl;
	std::cout << (a % c) << std::endl;
	std::cout << std::endl;
	std::cout << a++ << std::endl;
	std::cout << c-- << std::endl;
	std::cout << std::endl;
	std::cout << (a < c) << std::endl;
	std::cout << (a > c) << std::endl;
	std::cout << (a <= c) << std::endl;
	std::cout << (a >= c) << std::endl;
	std::cout << (a == c) << std::endl;
	std::cout << std::endl;

    long long int int_s = -12345678987654321, int_w = 987654321123456789, res = 0;
	assert((s + w) = (int_s + int_w));
	std::cout << (s + w) << std::endl;
	assert((s - w) = (int_s - int_w));
	std::cout << (s - w) << std::endl;
	assert((s * w) = (int_s * int_w));
	std::cout << (s * w) << std::endl;
	assert((s / w) = (int_s / int_w));
	std::cout << (s / w) << std::endl;
	assert((s % w) = (int_s % int_w));
	std::cout << (s % w) << std::endl;

	assert((++s) = (++int_s));
	std::cout << (++s) << std::endl;
	assert((s--) = (--int_s));
	std::cout << (--s) << std::endl;

	assert((s & w) = (int_s & int_w));
	std::cout << (s & w) << std::endl;
	assert((s | w) = (int_s | int_w));
	std::cout << (s | w) << std::endl;
	assert((s ^ w) = (int_s ^ int_w));
	std::cout << (s ^ w) << std::endl;

	return 0;
}