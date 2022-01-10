#include <iostream>
#define MASK_WIDTH 28

using namespace std;

typedef union
{
   uint8_t bytes[4];
   uint32_t dw;
} 
ipv4;

//Перевод ip-адреса в двоичный вид
char* ip_to_bin(ipv4 ip)
{
    static char buffer[36], bb[9] = { 0 };
    buffer[0] = 0;

    for (int idx = 0; idx != 4; idx++)
    {
        for (int bit = 0; bit != 8; bit++)
            bb[7 - bit] = ((1 << bit) & ip.bytes[3 - idx]) ? '1' : '0';

        strcat_s(buffer, bb);
        if (idx != 3) strcat_s(buffer, ".");
    }
    return buffer;
}

//Ввод
ipv4 ip_input(void)
{
    ipv4 ip = { 0 };
    unsigned temp[4];
    do printf("Введи IP-Адрес:");
    while (scanf_s("%u.%u.%u.%u", &temp[3], &temp[2], &temp[1], &temp[0]) != 4);
    for (int idx = 0; idx != 4; idx++) ip.bytes[idx] = temp[idx] & 0xff;
    return ip;
}

//Вывод
void ip_print(const char* title, ipv4 ip)
{
    printf("%20s \t %u.%u.%u.%u \t %.2X.%.2X.%.2X.%.2X \t %s\n",
        title,
        ip.bytes[3], ip.bytes[2], ip.bytes[1], ip.bytes[0],
        ip.bytes[3], ip.bytes[2], ip.bytes[1], ip.bytes[0],
        ip_to_bin(ip));
}

//Маска подсети
ipv4 get_mask(uint32_t prefix)
{
    ipv4 temp;
    temp.dw = ~(uint32_t(-1) >> prefix);
    return temp;
}

//Обратная маска подсети
ipv4 get_inverted_mask(uint32_t prefix)
{
    ipv4 temp;
    temp.dw = uint32_t(-1) >> prefix;
    return temp;
}


int main()
{
    system("chcp 1251 & cls");
    system("color a");

    ipv4 addr = ip_input(); 
    ipv4 mask = get_mask(MASK_WIDTH);
    ipv4 inv_mask = get_inverted_mask(MASK_WIDTH);
    ipv4 net_addr = addr; net_addr.dw &= mask.dw;
    ipv4 bcast_addr = addr; bcast_addr.dw |= inv_mask.dw;

    ip_print("IP адрес:                  ", addr);
    printf("Префикс маски подсети: /%u\n ", MASK_WIDTH);
    ip_print("Маска подсети:             ", mask);
    ip_print("Обратная маска:            ", inv_mask);
    ip_print("IP адрес сети:             ", net_addr);
    ip_print("Широковещательный адрес:   ", bcast_addr);

    net_addr.dw++;
    ip_print("IP адрес первого хоста:    ", net_addr);

    bcast_addr.dw--;
    ip_print("IP адрес последнего хоста: ", bcast_addr);
}
