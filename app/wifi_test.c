#include "mxchipWNet.h"
#include "mico_rtos.h"

int scan_end = 1;

void WifiStatusHandler(int event)
{
    switch (event)
    {
    case MXCHIP_WIFI_UP:
        printf("wifi up\r\n");
        break;

    case MXCHIP_WIFI_DOWN:
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
    strcpy(wNetConfig.wifi_ssid, "dd-wrt");
    strcpy(wNetConfig.wifi_key, "88888888");
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
            printf("wifi_softap_and_scan\r\n");
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

