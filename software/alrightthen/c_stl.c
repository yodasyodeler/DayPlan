#include "c_stl.h"

char* c_itoa( int num, char* buf,int base)
{
	int i=0;
	int j=0;
	char temp;

	if (base>=100){
		base -= 100;
		temp = 0x37;
	}
	else
		temp = 0x57;

	do{
		buf[i] = (num%base);
		if (buf[i] >= 10)
			buf[i] += temp;
		else
			buf[i] += 0x30;
		num = num/base;
		++i;
	}while(num > 0);

	buf[i] = '\0';
	--i;
	while(j<i){
		temp   = buf[j];
		buf[j] = buf[i];
		buf[i] = temp;
		++j;
		--i;
	}

	return buf;
}

int c_BCD(int num)
{
	int temp = 0;
	int mult = 1;

	while (num != 0)
	{
		temp += (num & 0x0F)* mult;
		mult *= 10;
		num = num >> 4;
	}

	return temp;
}

int	c_strcmp(const char* s1, const char* s2)
{

	while (*(s1) == *(s2)){
		if (!*s1)
			return 0;
		++s1;
		++s2;
	}

	return ((*s1 < *s2) ? -1 : 1);
}

void c_strcpy(char* dest, const char* src)
{
	while (*(dest++) = *(src++));
}
