#ifndef __USBLL_H__
#define __USBLL_H__
UINT8 EnableRootHubPort(UINT8 rootHubIndex, UINT8 * DeviceSpeed);
void DisableRootHubPort(UINT8 RootHubIndex);
void SetHostUsbAddr(UINT8 addr);
void SetUsbSpeed(UINT8 FullSpeed);
void ResetRootHubPort(UINT8 RootHubIndex);
void SelectHubPort(USB_HUB_PORT *pUsbDevice);
void InitUsbHost(void);
UINT8 USBHostTransact(UINT8 endp_pid, UINT8 tog, UINT16 timeout);
UINT8 HostCtrlTransfer(USB_SETUP_REQ *pSetupReq, UINT8 MaxPacketSize0, PUINT8 DataBuf, PUINT16 RetLen);
UINT8 TransferReceive(ENDPOINT *pEndPoint, UINT8 *pData, UINT16 *pRetLen, UINT16 timeout);
void InitHubPortData(USB_HUB_PORT *pUsbHubPort);

#define WAIT_USB_TOUT_200US 800
extern __xdata LinkedList* PolledDevices;
#endif