/*
 ***************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology 5th Rd.
 * Science-based Industrial Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2006, Ralink Technology, Inc.
 *
 * All rights reserved. Ralink's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attempt
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************

    Module Name:
    mtr_ioctl.c

    Abstract:

    Revision History:
    Who         When            What
    --------    ----------      ----------------------------------------------
    Name        Date            Modification logs
    Steven Liu  2007-02-15      Initial version
*/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>   
#include <linux/fs.h>       
#include <linux/errno.h>    
#include <linux/types.h>    
#include <linux/proc_fs.h>
#include <linux/fcntl.h>    
#include <asm/system.h>     
#include <asm/uaccess.h>

#include "frame_engine.h"
#include "mtr_ioctl.h"
#include "mtr_policy.h"
#include "ac_policy.h"
#include "util.h"

#ifdef  CONFIG_DEVFS_FS
#include <linux/devfs_fs_kernel.h>
static	devfs_handle_t devfs_handle;
#endif

int	mtr_major =  MTR_MAJOR;

struct _MtrParam {
        enum MtrType Type;
        enum MtrRuleType RuleType;

} MtrParam[]= {

        {PRE_MTR, MTR_MAC_GROUP}, /*MTR_ADD_MAC_UL_ENTRY*/
        {POST_MTR, MTR_MAC_GROUP}, /*MTR_ADD_MAC_DL_ENTRY*/
        {PRE_MTR, MTR_MAC_GROUP}, /*MTR_DEL_MAC_UL_ENTRY*/
        {POST_MTR, MTR_MAC_GROUP}, /*MTR_DEL_MAC_DL_ENTRY*/

        {PRE_MTR, MTR_IP_GROUP}, /*MTR_ADD_IP_UL_ENTRY*/
        {POST_MTR, MTR_IP_GROUP}, /*MTR_ADD_IP_DL_ENTRY*/
        {PRE_MTR, MTR_IP_GROUP}, /*MTR_DEL_IP_UL_ENTRY*/
        {POST_MTR, MTR_IP_GROUP}, /*MTR_DEL_IP_DL_ENTRY*/

	{PRE_MTR, MTR_SYN}, /*MTR_ADD_SYN_ENTRY*/
        {PRE_MTR, MTR_SYN}, /*MTR_DEL_SYN_ENTRY*/
        {PRE_MTR, MTR_FIN}, /*MTR_ADD_FIN_ENTRY*/
        {PRE_MTR, MTR_FIN}, /*MTR_DEL_FIN_ENTRY*/
        {PRE_MTR, MTR_PROTOCOL_UDP}, /*MTR_ADD_UDP_ENTRY*/
        {PRE_MTR, MTR_PROTOCOL_UDP}, /*MTR_DEL_UDP_ENTRY*/
        {PRE_MTR, MTR_PROTOCOL_ICMP}, /*MTR_ADD_ICMP_ENTRY*/
        {PRE_MTR, MTR_PROTOCOL_ICMP}, /*MTR_DEL_ICMP_ENTRY*/



        {PRE_MTR+POST_MTR, MTR_MAC_GROUP + MTR_IP_GROUP}, /* MTR_CLEAN_TBL */

};

/* Rule Boundary Check */
uint32_t  MtrBndryCheck(MtrPlcyNode *NewNode)
{
        uint32_t CurMtrEnd=0;
        uint32_t MaxMtrEnd=0;
	
	//exist node
	if(MtrExistNode(NewNode)!=NULL) {
		return MTR_SUCCESS;
	}

	//user want to create new node
        switch(NewNode->Type)
        {
        case PRE_MTR:
                CurMtrEnd=PpeGetPreMtrEnd();
                MaxMtrEnd=PpeGetPostAcStr();
                break;
        case POST_MTR:
                CurMtrEnd=PpeGetPostMtrEnd();
                MaxMtrEnd=PpeGetPostAcStr();
                break;
        }

	//MTR ip/mac rule needs 1 entry
        if(++CurMtrEnd >= MaxMtrEnd){
                return MTR_TBL_FULL;
        }else {
                return MTR_SUCCESS;
        }

}

#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,35)
long MtrIoctl(struct file *file, unsigned int cmd,
	                unsigned long arg)
#else
int MtrIoctl(struct inode *inode, struct file *filp,
                  unsigned int cmd, unsigned long arg)
#endif
{
	struct mtr_args *opt=(struct mtr_args *)arg;
	struct mtr_list_args *opt2=(struct mtr_list_args *)arg;

	MtrPlcyNode node;

	MacReverse(opt->mac);
	memcpy(node.Mac,opt->mac,ETH_ALEN);
	node.IpS=opt->ip_s; 
	node.IpE=opt->ip_e; 

	if(opt->mtr_mode==0) {//Byte Mode
	    node.ByteBase.MtrMode = 0;
	    node.ByteBase.MaxBkSize = opt->bk_size;
	    node.ByteBase.TokenRate = opt->token_rate;
	}else { //Pkt Mode
	    node.PktBase.MtrMode = 1;
	    node.PktBase.MaxBkSize = opt->bk_size;
	    node.PktBase.MtrIntval = opt->mtr_intval;
	}

	node.Type=MtrParam[cmd].Type;
	node.RuleType=MtrParam[cmd].RuleType;

	switch(cmd) 
	{
	case MTR_ADD_MAC_UL_ENTRY:        
	case MTR_ADD_MAC_DL_ENTRY:        
	case MTR_ADD_IP_UL_ENTRY:          
	case MTR_ADD_IP_DL_ENTRY:
	case MTR_ADD_SYN_ENTRY:
	case MTR_ADD_FIN_ENTRY:
	case MTR_ADD_UDP_ENTRY:
	case MTR_ADD_ICMP_ENTRY:
		opt->result=MtrBndryCheck(&node);
		if(opt->result!=MTR_TBL_FULL) {
			MtrAddNode(&node);
		}
		break;
	case MTR_DEL_MAC_UL_ENTRY:          
	case MTR_DEL_MAC_DL_ENTRY:          
	case MTR_DEL_IP_UL_ENTRY:         
	case MTR_DEL_IP_DL_ENTRY:  
	case MTR_DEL_SYN_ENTRY:
	case MTR_DEL_FIN_ENTRY:
	case MTR_DEL_UDP_ENTRY:
	case MTR_DEL_ICMP_ENTRY:
		opt->result=MtrDelNode(&node);
		break;
	case MTR_CLEAN_TBL:
		MtrCleanTbl();
		break;
	case MTR_GET_ALL_ENTRIES:
		opt2->result = MtrGetAllEntries(opt2);
		break;
	default:
		break;
	}

	return 0;
}

struct file_operations mtr_fops = {
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,35)
unlocked_ioctl:      MtrIoctl,
#else
ioctl:		     MtrIoctl,
#endif
};


int MtrRegIoctlHandler(void)
{

#ifdef  CONFIG_DEVFS_FS
	if(devfs_register_chrdev(mtr_major, MTR_DEVNAME , &mtr_fops)) {
		NAT_PRINT(KERN_WARNING " mtr: can't create device node - %s\n",MTR_DEVNAME);
		return -EIO;
	}

	devfs_handle = devfs_register(NULL, MTR_DEVNAME, DEVFS_FL_DEFAULT, mtr_major, 0, 
			S_IFCHR | S_IRUGO | S_IWUGO, &mtr_fops, NULL);
#else
	int result=0;
	result = register_chrdev(mtr_major, MTR_DEVNAME, &mtr_fops);
	if (result < 0) {
		NAT_PRINT(KERN_WARNING "mtr: can't get major %d\n",mtr_major);
		return result;
	}

	if (mtr_major == 0) {
		mtr_major = result; /* dynamic */
	}
#endif

	return 0;
}

void MtrUnRegIoctlHandler(void)
{
#ifdef  CONFIG_DEVFS_FS
	devfs_unregister_chrdev(mtr_major, MTR_DEVNAME);
	devfs_unregister(devfs_handle);
#else
	unregister_chrdev(mtr_major, MTR_DEVNAME);
#endif
}

