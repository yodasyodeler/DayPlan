#include "c_stl.h"

char* itoa( int num, char* buf,int base)
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
