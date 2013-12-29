#include <stdlib.h>             
#include <stdio.h>             
#include <string.h>           
#include <sys/ioctl.h>
#include <fcntl.h>
#include <getopt.h>

#include "ac_ioctl.h"
#include "ac_api.h"

void show_usage(void)
{

    printf("Add Mac Upload Accounting Rule\n");
    printf("ac -a -m [Mac] \n");

    printf("Add Mac Download Accounting Rule\n");
    printf("ac -b -m [Mac] \n");
    
    printf("Del Mac Upload Accounting Rule\n");
    printf("ac -c -m [Mac]\n");
    
    printf("Del Mac download Accounting Rule\n");
    printf("ac -d -m [Mac]\n");
    
    printf("Add IP Upload Accounting Rule\n");
    printf("ac -e -i [IpS] -j [IpE]\n");
    
    printf("Add IP Download Accounting Rule\n");
    printf("ac -f -i [IpS] -j [IpE] \n");

    printf("Del IP Upload Accounting Rule\n");
    printf("ac -g -i [IpS] -j [IpE] \n");

    printf("Del IP Download Accounting Rule\n");
    printf("ac -h -i [IpS] -j [IpE]\n");

    printf("Show Upload Packet Count of the Mac\n");
    printf("ac -p -m [Mac] \n");
    
    printf("Show Download Packet Count of the Mac\n");
    printf("ac -q -m [Mac]\n");
    
    printf("Show Upload Byte Count of the Mac\n");
    printf("ac -r -m [Mac]\n");
    
    printf("Show Download Byte Count of the Mac\n");
    printf("ac -s -m [Mac]\n");

    printf("Show Upload Packet Count of the IP\n");
    printf("ac -t -i [IpS] -j [IpE]\n");
    
    printf("Show Download Packet Count of the IP\n");
    printf("ac -u -i [IpS] -j [IpE]\n");
    
    printf("Show Upload Byte Count of the IP\n");
    printf("ac -v -i [IpS] -j [IpE]\n");
    
    printf("Show Download Byte Count of the IP\n");
    printf("ac -w -i [IpS] -j [IpE]\n");

    printf("Clear Ac Table\n");
    printf("ac -z\n");

}

int main(int argc, char *argv[])
{
    int opt;
    char options[] = "abcdefghpqrstuvwz?m:i:j:";
    int fd;
    int method=-1;
    struct ac_args args;
    int result;


    fd = open("/dev/"AC_DEVNAME, O_RDONLY);
    if (fd < 0)
    {
	printf("Open %s pseudo device failed\n","/dev/"AC_DEVNAME);
	return 0;
    }

    if(argc < 2) {
	show_usage();
	return 0;
    }

    while ((opt = getopt (argc, argv, options)) != -1) {
	switch (opt) {
	case 'a':  
	    method=AC_ADD_MAC_UL_ENTRY;
	    break;
	case 'b':  
	    method=AC_ADD_MAC_DL_ENTRY;
	    break;
	case 'c': 
	    method=AC_DEL_MAC_UL_ENTRY;
	    break;
	case 'd': 
	    method=AC_DEL_MAC_DL_ENTRY;
	    break;
	case 'e': 
	    method=AC_ADD_IP_UL_ENTRY;
	    break;
	case 'f': 
	    method=AC_ADD_IP_DL_ENTRY;
	    break;
	case 'g': 
	    method=AC_DEL_IP_UL_ENTRY;
	    break;
	case 'h': 
	    method=AC_DEL_IP_DL_ENTRY;
	    break;
	case 'p': 
	    method=AC_GET_MAC_UL_PKT_CNT;
	    break;
	case 'q': 
	    method=AC_GET_MAC_DL_PKT_CNT;
	    break;
	case 'r': 
	    method=AC_GET_MAC_UL_BYTE_CNT;
	    break;
	case 's': 
	    method=AC_GET_MAC_DL_BYTE_CNT;
	    break;
	case 't': 
	    method=AC_GET_IP_UL_PKT_CNT;
	    break;
	case 'u': 
	    method=AC_GET_IP_DL_PKT_CNT;
	    break;
	case 'v': 
	    method=AC_GET_IP_UL_BYTE_CNT;
	    break;
	case 'w': 
	    method=AC_GET_IP_DL_BYTE_CNT;
	    break;
	case 'z': /* CleanTbl */
	    method=AC_CLEAN_TBL;
	    break;
	case 'm': /* Mac */
	    str_to_mac(args.mac, optarg);
	    break;
	case 'i': /* IP */
	    str_to_ip(&args.ip_s, optarg);
	    break;
	case 'j':
	    str_to_ip(&args.ip_e, optarg);
	    break;
	case '?': /* Help */
	    show_usage();
	    break;
	}
    } 


    switch(method) {
    case AC_ADD_MAC_UL_ENTRY:
    case AC_ADD_MAC_DL_ENTRY:
    case AC_ADD_IP_UL_ENTRY:
    case AC_ADD_IP_DL_ENTRY:
    case AC_CLEAN_TBL:
	    SetAcEntry(&args, method);
	    result = args.result;
	    break;
    case AC_DEL_MAC_UL_ENTRY:
    case AC_DEL_MAC_DL_ENTRY:
    case AC_DEL_IP_UL_ENTRY:
    case AC_DEL_IP_DL_ENTRY:
	    SetAcEntry(&args, method);
	    result = args.result;
	    break;
    case AC_GET_MAC_UL_PKT_CNT:
    case AC_GET_MAC_DL_PKT_CNT: 
    case AC_GET_IP_UL_PKT_CNT:
    case AC_GET_IP_DL_PKT_CNT:
    case AC_GET_MAC_UL_BYTE_CNT:
    case AC_GET_MAC_DL_BYTE_CNT:
    case AC_GET_IP_UL_BYTE_CNT: 
    case AC_GET_IP_DL_BYTE_CNT:  
	    GetAcEntry(&args, method);
	    result = args.result;
	    printf("Count=%d\n",args.cnt);
	    break;
    }

    if(result == AC_SUCCESS) {
	printf("done\n");
    }else if (result ==  AC_TBL_FULL) {
	printf("table full\n");
    } else {
	printf("fail\n");
    }

    return 0;
}
