#ifndef __USBLL_H__
#define __USBLL_H__
UINT8 EnableRootHubPort(UINT8 rootHubIndex);
void DisableRootHubPort(UINT8 RootHubIndex);
void SetHostUsbAddr(UINT8 addr);
void SetUsbSpeed(UINT8 FullSpeed);
void ResetRootHubPort(UINT8 RootHubIndex);
void SelectHubPort(UINT8 RootHubIndex, UINT8 HubPortIndex);
void InitUsbHost(void);
UINT8 USBHostTransact(UINT8 endp_pid, UINT8 tog, UINT16 timeout);
UINT8 HostCtrlTransfer(USB_SETUP_REQ *pSetupReq, UINT8 MaxPacketSize0, PUINT8 DataBuf, PUINT16 RetLen);
UINT8 TransferReceive(ENDPOINT *pEndPoint, UINT8 *pData, UINT16 *pRetLen, UINT16 timeout);
void InitHubPortData(USB_HUB_PORT *pUsbHubPort);
void InitRootHubPortData(UINT8 rootHubIndex);
__xdata USB_HUB_PORT* AllocateChildHubPorts(UINT8 numPorts);
void FreeChildHubPorts(__xdata USB_HUB_PORT* childPorts, UINT8 numPorts);
void SelectHubPortByDevice(USB_HUB_PORT *pUsbDevice);

//root hub port
extern USB_HUB_PORT __xdata RootHubPort[ROOT_HUB_PORT_NUM];

//sub hub port
extern USB_HUB_PORT __xdata SubHubPort[ROOT_HUB_PORT_NUM][MAX_EXHUB_PORT_NUM];

#define WAIT_USB_TOUT_200US 800
#endif