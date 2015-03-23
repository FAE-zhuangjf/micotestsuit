#include "mxchipWNet.h"
#include "mico_rtos.h"

static int scan_end = 1;
static int station_up = 0;

void WifiStatusHandler(int event)
{
    switch (event)
    {
    case MXCHIP_WIFI_UP:
        station_up = 1;
        printf("wifi up\r\n");
        break;

    case MXCHIP_WIFI_DOWN:
        station_up = 0;
        printf("wifi down\r\n");
        break;

    case MXCHIP_UAP_UP:
        printf("softap up\r\n");
        break;

    case MXCHIP_UAP_DOWN:
        printf("softap down\r\n");
        break;
    }
}

void ApListCallback(UwtPara_str *pApList)
{
    printf("%d APs found\r\n", pApList->ApNum);

    scan_end = 1;
}

void softap_start(void)
{
    network_InitTypeDef_st wNetConfig;

    memset(&wNetConfig, 0, sizeof(wNetConfig));

    wNetConfig.wifi_mode = Soft_AP;
    strcpy(wNetConfig.wifi_ssid, "MXCHIP_WIFITEST");
    strcpy(wNetConfig.wifi_key, "rqtewrewfwer");
    wNetConfig.dhcpMode = DHCP_Server;

    strcpy(wNetConfig.address_pool_start, "192.168.1.100");
    strcpy(wNetConfig.address_pool_end, "192.168.1.100");
    strcpy(wNetConfig.local_ip_addr, "192.168.1.1");
    strcpy(wNetConfig.net_mask, "255.255.255.0");
    strcpy(wNetConfig.gateway_ip_addr, "192.168.1.1");
    strcpy(wNetConfig.dnsServer_ip_addr, "192.168.1.1");

    StartNetwork(&wNetConfig);
}

void station_start(void)
{
    network_InitTypeDef_st wNetConfig;

    memset(&wNetConfig, 0, sizeof(wNetConfig));

    wNetConfig.wifi_mode = Station;
    strcpy(wNetConfig.wifi_ssid, "tao");
    strcpy(wNetConfig.wifi_key, "39376612");
    wNetConfig.dhcpMode = DHCP_Client;

    StartNetwork(&wNetConfig);
}

void wifi_softap_and_scan_test(void *arg)
{
    printf("Enter WiFi SoftAP and Scan Test\r\n");

    softap_start();

    while(1)
    {
        if(scan_end)
        {
            mxchipStartScan();
        }
    }
}

void tcp_speed_max_test(void *arg)
{
    int fd = -1;
    int ret;
    struct sockaddr_t addr;
    uint8_t *buffer = (uint8_t*)malloc(2048);
    
    printf("Enter tcp speed max Test\r\n");

    station_start();

    while(1)
    {
        if(!station_up)
        {
            msleep(100);
            continue;
        }

        if(-1 == fd)
        {
            fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if(-1 == fd)
            {
                sleep(1);
                continue;
            }

            memset(&addr, 0, sizeof(addr));
            addr.s_ip = inet_addr("192.168.1.104");
            addr.s_port = 8080;

            ret = connect(fd, &addr, sizeof(addr));
            if(-1 == ret)
            {
                close(fd);
                fd = -1;
                msleep(100);
                continue;
            }
        }
        else
        {
            ret = send(fd, buffer, 2048, 0);
            if(-1 == ret)
            {
                printf("send failed, to rebuild\n");
                close(fd);
                fd = -1;
                msleep(100);
                continue;
            }
            else
            {
                printf("send ok\r\n");
            }
        }
    }
}

void test_thread_init(mico_thread_t *handler, uint8_t cmd)
{
    switch(cmd)
    {
        case 11:
            mico_rtos_create_thread(handler,MICO_APPLICATION_PRIORITY,"ap_and_scan",
                wifi_softap_and_scan_test,0x400,NULL);
            break;
        case 12:
            printf("wifi_softap_and_scan\r\n");
            break;
        case 13:
            printf("wifi_softap_and_scan\r\n");
            break;
        case 21:
            printf("wifi_softap_and_scan\r\n");
            break;
        case 22:
            mico_rtos_create_thread(handler,MICO_APPLICATION_PRIORITY,"tcp_speed_max",
                tcp_speed_max_test,0x400,NULL);
            break;
        case 23:
            printf("wifi_softap_and_scan\r\n");
            break;
        case 24:
            printf("wifi_softap_and_scan\r\n");
            break;
        case 25:
            printf("wifi_softap_and_scan\r\n");
            break;
        default:
            printf("unsupported command\r\n");
            break;
    }
}

