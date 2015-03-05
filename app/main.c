#include <mxchipWNet.h>
#include <stdio.h>
#include <mico_rtos.h>
#include "types.h"

mico_thread_t test_thread_handler = NULL;

int fputc(int ch, FILE *f)
{
    while (RESET == USART_GetFlagStatus(USART1, USART_FLAG_TXE));

    USART_SendData(USART1, ch);
    return ch;
}

static const char *test_head_string =
"\
----------------------\r\n\
MXCHIP WiFi Test Suit \r\n\
----------------------\r\n\
\r\n\
";

static const char *help_string =
"\
-------------------------------------------------\r\n\
              802.11 Test Commands              |\r\n\
-------------------------------------------------\r\n\
11. wifi_softap_and_scan                        |\r\n\
12. wifi_ping_station                           |\r\n\
13. wifi_ping_softap                            |\r\n\
                                                |\r\n\
-------------------------------------------------\r\n\
           TCP/IP Stack Test Commands           |\r\n\
-------------------------------------------------\r\n\
21. tcp_single_reconnect                        |\r\n\
22. tcp_speed_max                               |\r\n\
23. tcp_connection_max                          |\r\n\
24. tcp_one_max_speed_and_other_reconnect       |\r\n\
25. udp_speed_max                               |\r\n\
-------------------------------------------------\r\n\
";

void tty_init(void)
{
    PlatformUartInitialize();
}

uint32_t tty_read(uint8_t *line, uint32_t linesize)
{
    uint32_t i = 0;
    char c;

    if(NULL == line)
    {
        return -1;
    }

    do
    {
        PlatformUartRecv(&c, 1, MICO_WAIT_FOREVER);

        if(0x03 != c)
        {
            printf("%c", c);
        }
        else
        {
            break;
        }

        line[i++] = c;
    } while(('\r' != c) && (i < linesize));

    if(i == linesize)
    {
        return -1;
    }

    if(0x03 == c)
    {
        printf("\r\n");
        strcpy(line, "ctrl+c");
        return strlen("ctrl+c");
    }

    printf("\n");

    line[i-1] = 0;

    return i-1;
}

bool is_valid_number(char *line)
{
    int i;

    if(NULL == line)
    {
        return false;
    }

    if(2 != strlen(line))
    {
        return false;
    }

    for(i=0; i<2; i++)
    {
        if((line[i] < '0') && (line[i] > '9'))
        {
            break;
        }
    }

    if(2 ==i)
    {
        return true;
    }

    return false;
}

void wait_for_user_break(void)
{
    int ret;
    uint8_t line[32];
    
    while(1)
    {
        ret = tty_read(line, sizeof(line));
        if(-1 == ret)
        {
            continue;
        }

        if(0 == strcmp(line, "ctrl+c"))
        {
            if(test_thread_handler)
            {
                mico_rtos_delete_thread(&test_thread_handler);
                break;
            }
        }
    }
}

int application_start(void)
{
    uint32_t linelen;
    uint8_t line[32];

    uint8_t cmd;
    
    mxchipInit();

    tty_init();

    printf(test_head_string);

    while(1)
    {
        printf(">> ");

        linelen = tty_read(line, sizeof(line));

        if(-1 == linelen)
        {
            continue;
        }

        if((0 == strcmp(line, "?")) || (0 == strcmp(line, "help")))
        {
            printf(help_string);
        }
        else if(! is_valid_number(line))
        {
            printf(help_string);
        }
        else
        {
            cmd = atol(line);

            test_thread_init(&test_thread_handler, cmd);

            wait_for_user_break();
        }
    }
}

