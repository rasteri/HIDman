#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "ch559.h"
#include "util.h"
#include "usbhost.h"
#include "uart.h"
#include "ps2protocol.h"
#include "ps2.h"
#include "parsedescriptor.h"
#include "menu.h"
#include "mouse.h"
#include "xt.h"
#include "pwm.h"
#include "keyboardled.h"

#define bit bool

/*******************************************************************************
* Function Name  : Flash_Op_Unlock
* Description    : Flash��������
* Input          : flash_type: bCODE_WE(Code Flash); bDATA_WE(Data Flash) 
* Output         : None
* Return         : 0xFF(Flash Operation Flags Error)/0x00(Flash Operation Flags Correct)
*******************************************************************************/
UINT8 Flash_Op_Unlock( UINT8 flash_type )
{
    bit ea_sts;
    
    /* Disable all INTs to prevent writing GLOBAL_CFG from failing in safe mode. */
    ea_sts = EA;                                
    EA = 0;
    
    /* Enable Flash writing operations. */
    SAFE_MOD = 0x55;
	SAFE_MOD = 0xAA;
	GLOBAL_CFG |= flash_type;
    SAFE_MOD = 0x00;

    /* Restore all INTs. */
    EA = ea_sts;
    
    return 0x00;
}

/*******************************************************************************
* Function Name  : Flash_Op_Lock
* Description    : Flash��������
* Input          : flash_type: bCODE_WE(Code Flash)/bDATA_WE(Data Flash) 
* Output         : None
* Return         : None
*******************************************************************************/
void Flash_Op_Lock( UINT8 flash_type )
{
    bit ea_sts;
    
    /* Disable all INTs to prevent writing GLOBAL_CFG from failing in safe mode. */
    ea_sts = EA;                                
    EA = 0;
    
    /* Disable Flash writing operations. */
    SAFE_MOD = 0x55;
	SAFE_MOD = 0xAA;
	GLOBAL_CFG &= ~flash_type;
    SAFE_MOD = 0x00;
    
    /* Restore all INTs. */
    EA = ea_sts;
}

/*******************************************************************************
* Function Name  : EraseBlock(UINT16 Addr)
* Description    : CodeFlash块擦除(1KB)，全部数据位写1
* Input          : UINT16 Addr
* Output         : None
* Return         : None
*******************************************************************************/
UINT8	EraseBlock( UINT16 Addr )
{
	ROM_ADDR = Addr;

    while (!(ROM_STATUS & bROM_ADDR_OK));

	if ( ROM_STATUS & bROM_ADDR_OK ) {                                          // 操作地址有效
		ROM_CTRL = ROM_CMD_ERASE;
		return( ( ROM_STATUS ^ bROM_ADDR_OK ) & 0x7F );                           // 返回状态,0x00=success, 0x01=time out(bROM_CMD_TOUT), 0x02=unknown command(bROM_CMD_ERR)
	}
	else{ 
        return( 0x40 );}
}

/*******************************************************************************
* Function Name  : ProgWord( UINT16 Addr, UINT16 Data )
* Description    : 写EEPROM，双字节写
* Input          : UNIT16 Addr,写地址
                   UINT16 Data,数据
* Output         : None
* Return         : SUCESS 
*******************************************************************************/
UINT8	ProgWord( UINT16 Addr, UINT16 Data )
{
	ROM_ADDR = Addr;
	ROM_DATA = Data;

    while (!(ROM_STATUS & bROM_ADDR_OK));

	if ( ROM_STATUS & bROM_ADDR_OK ) {                                           // 操作地址有效
		ROM_CTRL = ROM_CMD_PROG;
		return( ( ROM_STATUS ^ bROM_ADDR_OK ) & 0x7F );                            // 返回状态,0x00=success, 0x01=time out(bROM_CMD_TOUT), 0x02=unknown command(bROM_CMD_ERR)
	}
	else return( 0x40 );
}

/*******************************************************************************
* Function Name  : EraseDataFlash(UINT16 Addr)
* Description    : DataFlash块擦除(1KB)，全部数据位写1
* Input          : UINT16 Addr
* Output         : None
* Return         : UINT8 status
*******************************************************************************/
UINT8 EraseDataFlash(UINT16 Addr)
{
    UINT8 status;

    if( Flash_Op_Unlock( bDATA_WE ) )                                         
    {
        return( 0xFF );
    }
    status = EraseBlock(Addr);
    Flash_Op_Lock( bDATA_WE );

    return status;
}

/*******************************************************************************
* Function Name  : WriteDataFlash(UINT16 Addr,PUINT8 buf,UINT16 len)
* Description    : DataFlash写
* Input          : UINT16 Addr，PUINT16 buf,UINT16 len
* Output         : None
* Return         : 
*******************************************************************************/
UINT8 WriteDataFlash(UINT16 Addr,PUINT8 buf,UINT16 len)
{
    UINT16 j,tmp;   
    UINT8 status;

    if( Flash_Op_Unlock( bDATA_WE ) )                                         
    {
        return( 0xFF );
    }
    
    for(j=0;j<len;j=j+2)
    {
        tmp = buf[j+1];
        tmp <<= 8;
        tmp += buf[j];			
        status = ProgWord(Addr,tmp);
        if( status != 0x00 )
        {
            return status;
        }
        Addr = Addr + 2;
    }
    Flash_Op_Lock( bDATA_WE );
    
    return status;
}