// UsbDkHelper.cpp : Defines the exported functions for the DLL application.

#include "stdafx.h"
#include "UsbDkHelper.h"
#include "Installer.h"
#include "DriverAccess.h"

//-------------------------------------------------------------------------------------------

void printExceptionString(const char *errorStr)
{
    auto tString = string2tstring(string(errorStr));
    OutputDebugString(tString.c_str());
    tcout << tString;
}

//------------------------------------------------------------------------------------------
InstallResult InstallDriver()
{
    try
    {
        UsbDkInstaller installer;
        return installer.Install() ? InstallSuccess : InstallFailureNeedReboot;
    }
    catch (const exception &e)
    {
        printExceptionString(e.what());
        UninstallDriver();
        return InstallFailure;
    }
}
//-------------------------------------------------------------------------------------------

BOOL UninstallDriver()
{
    try
    {
        UsbDkInstaller installer;
        installer.Uninstall();
        return TRUE;
    }
    catch (const exception &e)
    {
        printExceptionString(e.what());
        return FALSE;
    }
}
//-------------------------------------------------------------------------------------------

BOOL GetDevicesList(PUSB_DK_DEVICE_ID *DevicesArray, ULONG *NumberDevices)
{
    try
    {
        UsbDkDriverAccess driver;
        driver.GetDevicesList(*DevicesArray, *NumberDevices);
        return TRUE;
    }
    catch (const exception &e)
    {
        printExceptionString(e.what());
        return FALSE;
    }
}
//-------------------------------------------------------------------------------------------

void ReleaseDeviceList(PUSB_DK_DEVICE_ID DevicesArray)
{
    try
    {
        UsbDkDriverAccess::ReleaseDeviceList(DevicesArray);
    }
    catch (const exception &e)
    {
        printExceptionString(e.what());
    }
}
//-------------------------------------------------------------------------------------------

template <typename TFunc>
static BOOL DoDriverBoolOp(TFunc Func)
{
    try
    {
        UsbDkDriverAccess driver;
        Func(driver);
        return TRUE;
    }
    catch (const exception &e)
    {
        printExceptionString(e.what());
        return FALSE;
    }
}
//-------------------------------------------------------------------------------------------

BOOL ResetDevice(PUSB_DK_DEVICE_ID DeviceID)
{
    return DoDriverBoolOp([&DeviceID](UsbDkDriverAccess &drv){ drv.ResetDevice(*DeviceID); });
}
//-------------------------------------------------------------------------------------------

BOOL AddRedirect(PUSB_DK_DEVICE_ID DeviceID)
{
    return DoDriverBoolOp([&DeviceID](UsbDkDriverAccess &drv){ drv.AddRedirect(*DeviceID); });
}
//-------------------------------------------------------------------------------------------

BOOL RemoveRedirect(PUSB_DK_DEVICE_ID DeviceID)
{
    return DoDriverBoolOp([&DeviceID](UsbDkDriverAccess &drv){ drv.RemoveRedirect(*DeviceID); });
}
//-------------------------------------------------------------------------------------------
