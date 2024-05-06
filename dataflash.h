#ifndef __DATAFLASH_H__
#define __DATAFLASH_H__

UINT8 Flash_Op_Unlock( UINT8 flash_type );
void Flash_Op_Lock( UINT8 flash_type );
UINT8	EraseBlock( UINT16 Addr );
UINT8	ProgWord( UINT16 Addr, UINT16 Data );
UINT8 EraseDataFlash(UINT16 Addr);
UINT8 WriteDataFlash(UINT16 Addr,PUINT8 buf,UINT16 len);

#endif //__DATAFLASH_H__