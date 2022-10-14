#include <QCoreApplication>
#include <qt_windows.h>
#include <CyAPI.h>
#include <SetupAPI.h>
#include <Windows.h>


#pragma comment(lib,"SetupAPI.lib")
#pragma comment(lib,"User32.lib")
#pragma comment(lib,"legacy_stdio_definitions.lib")


DEFINE_GUID( GUID_DEVCLASS_USB,  0x36FC9E60, 0xC465, 0x11CF, 0x80, 0x56, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00 );
void PrintAllUsbDevicesLocationInfo(QString curPath)
{
    DWORD i, nSize = 0;
    HDEVINFO hDevInfo = {0};
    TCHAR szLocationBuf[MAX_PATH] = {0};
    TCHAR DevInstanceId[MAX_PATH] = {0};
    SP_DEVINFO_DATA DeviceInfoData = {0};
    DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
    // 得到所有USB设备 HDEVINFO
    // 第一个参数不能设置为GUID_DEVCLASS_USB，因为hid设备类为HIDClass，以及一些其他设备的设备也不一定是USB
    hDevInfo = SetupDiGetClassDevs(&GUID_DEVCLASS_USB, TEXT("USB"), 0, DIGCF_PRESENT);
    if (hDevInfo == INVALID_HANDLE_VALUE)
        return;
    // 循环列举
    for (i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &DeviceInfoData); i++)
    {
        // 获取设备在Hub中的Port位置
        if (SetupDiGetDeviceRegistryProperty(hDevInfo, &DeviceInfoData, SPDRP_LOCATION_INFORMATION, NULL, (PBYTE)szLocationBuf, MAX_PATH-1, NULL)){
            if (SetupDiGetDeviceInstanceId(hDevInfo, &DeviceInfoData, DevInstanceId, sizeof(DevInstanceId), &nSize))
            {
                QString id = QString::fromWCharArray(DevInstanceId).replace("\\", "#");
//                qDebug() << id;
                if (curPath.contains(id))
                {
                    qDebug() << QString::fromWCharArray(szLocationBuf);
//                    printf("Location:%ls ", szLocationBuf);
                    break;
                }
            }
        }
    }
    //  释放
    SetupDiDestroyDeviceInfoList(hDevInfo);
}

/**
 * @brief sendData sendData 通过ControlEndPoint向USB发送数据
 * @param cept ControlEndPoint指针
 * @param reqCode 自定义请求码
 * @param value 这里的设定值将传给 setupdat 的[2:3]位
 * @param index 这里的设定值将传给setupdat的[4:5]位
 * @param ctrlBuf 传递的数据
 * @param bufLen 数据串长度
 * @return 是否成功
 */
bool sendData(CCyControlEndPoint *cept, UCHAR reqCode, LONG bufLen=0, PUCHAR ctrlBuf=new UCHAR, WORD value=0, WORD index=0)
{
    cept->Target    = TGT_DEVICE;    //不必关注，固定
    cept->ReqType   = REQ_VENDOR;    //请求类型：自定义请求(标准请求等)
    cept->Direction = DIR_TO_DEVICE; //传输方向：OUT
    cept->ReqCode   = reqCode;       //自定义请求码
    cept->Value     = value;         //这里的设定值将传给 setupdat 的[2:3]位,默认为0
    cept->Index     = index;         //这里的设定值将传给setupdat的[4:5]位,默认为0
    return cept->XferData(ctrlBuf, bufLen);
}

int main(int argc, char *argv[])
{
        QCoreApplication a(argc, argv);
    qInfo() << "[INFO]";
    CCyUSBDevice *USBDevice = new CCyUSBDevice(NULL);
    int deviceNumber = USBDevice->DeviceCount();
    qInfo() << "device number:" << deviceNumber;
    if (deviceNumber > 0)
    {
//        if (!USBDevice->Open(0))
//        {
//            qInfo() << "Open De vice 0 Error";
//        }
//        int endPointCount = USBDevice->EndPointCount();
//        qInfo() << "EndPointCount:" << endPointCount;
//        CCyUSBEndPoint *endpt;
//        for(int i=0; i<endPointCount; ++ i)
//        {
//            endpt = USBDevice->EndPoints[i];
//            printf("%x %d\n", endpt->Address, endpt->Attributes);
//        }
        //        int DataSize = 512;
        //        int ccnt = -1;
        //        UCHAR cmp = 0;
        //        UCHAR doubleCheck = 0;
        //        int pass = 0;
        //        while (true)
        //        {
        //            endpt = USBDevice->EndPointOf(0x86);
        //            endpt->TimeOut = 4000;
        //            endpt->SetXferSize(DataSize);
        //            OVERLAPPED inOvLap;
        //            inOvLap.hEvent = CreateEvent(NULL, false, false, L"CYUSB_IN");
        //            LONG inLen = DataSize;
        //            PUCHAR inBuf = new UCHAR[DataSize];
        //            memset(inBuf, 0, DataSize);
        //            PUCHAR inContext = endpt->BeginDataXfer(inBuf, inLen, &inOvLap);
        //            endpt->WaitForXfer(&inOvLap, endpt->TimeOut);
        //            endpt->FinishDataXfer(inBuf, inLen, &inOvLap, inContext);
        //            CloseHandle(inOvLap.hEvent);
        //            if (ccnt == -1)
        //            {
        //                ccnt = 0;
        //                cmp = inBuf[0];
        //            }
        //            for (int i = 0; i < inLen; ++ i)
        //            {
        //                if (cmp != inBuf[i] && pass == 0)
        //                {
        //                    if (doubleCheck == 0)
        //                    {
        //                        if (inBuf[i] != 0xEB && inBuf[i] != 0x90)
        //                        {
        //                            printf("\nERROR cmp=%02x buffer=%02x\n", cmp, inBuf[i]);
        //                            return 0;
        //                        }
        //                        doubleCheck = inBuf[i];
        //                    }
        //                    else
        //                    {
        //                        if (inBuf[i] == doubleCheck)
        //                        {
        //                            doubleCheck = 0;
        //                            if (inBuf[i] == 0xEB)
        //                            {
        //                                pass = 2;
        //                            }
        //                        }
        //                        else
        //                        {
        //                            printf("\nERROR cmp=%02x buffer=%02x\n", cmp, inBuf[i]);
        //                            return 0;
        //                        }
        //                    }
        //                    printf("%02x ", inBuf[i]);
        //                }
        //                else
        //                {
        //                    if (pass > 0)
        //                    {
        //                        pass --;
        //                    }
        //                    else
        //                    {
        //                        cmp ++;
        //                    }
        //                    printf("%02x ",inBuf[i]);
        //                }
        //            }
        //            for (int i = 0; i < inLen; ++ i)
        //            {
        //                printf("%02x ", inBuf[i]);
        //            }
        //            printf("\n");
        //            if (ccnt != -1)
        //            {
        //                for (int i = 0; i < inLen; ++ i)
        //                {
        //                    printf("%02x ", inBuf[i]);
        //                    ccnt ++;
        //                    if (ccnt == 236)
        //                    {
        //                        ccnt = 0;
        //                        printf("\n");
        //                    }
        //                }
        //            }
        //            else
        //            {
        //                ccnt = 0;
        //                int pos = 0;
        //                while(pos+235 < inLen)
        //                {
        //                    if (inBuf[pos]==0xEB && inBuf[pos+1]==0xEB && inBuf[pos+234]==0x90 && inBuf[pos+235]==0x90)
        //                    {
        //                        break;
        //                    }
        //                    pos ++;
        //                }
        //                while (pos < inLen)
        //                {
        //                    printf("%02x ", inBuf[pos ++]);
        //                    ccnt ++;
        //                    if (ccnt == 236)
        //                    {
        //                        ccnt = 0;
        //                        printf("\n");
        //                    }
        //                }
        //            }
        //        }
        //    }
        for(int i=0; i<deviceNumber; i++)
        {
            USBDevice->Open(i);
            QString path = QString(USBDevice->DevPath).toUpper();
//            printf("device%d: %s\n", i, path);
            qInfo() << "device" << i << ":" << path;
            PrintAllUsbDevicesLocationInfo(path);
//            qInfo() << USBDevice->EndPointCount();
        }

        //
    }
    return a.exec();
}
