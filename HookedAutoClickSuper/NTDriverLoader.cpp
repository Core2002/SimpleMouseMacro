#include "NTDriverLoader.hpp"

namespace MyNTDriverLoader {

	BOOL __cdecl LoadNTDriver(char* lpszDriverName, char* lpszDriverPath)
	{
		//char szDriverImagePath[256];
		//得到完整的驱动路径
		//GetFullPathNameA(lpszDriverPath, 256, szDriverImagePath, NULL);

		BOOL bRet = FALSE;

		SC_HANDLE hServiceMgr = NULL;//SCM管理器的句柄
		SC_HANDLE hServiceDDK = NULL;//NT驱动程序的服务句柄

		//打开服务控制管理器
		hServiceMgr = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

		if (hServiceMgr == NULL)
		{
			//OpenSCManager失败
			printf("OpenSCManager() Faild %d ! \n", GetLastError());
			bRet = FALSE;
			goto BeforeLeave;
		}
		else
		{
			////OpenSCManager成功
			printf("OpenSCManager() ok ! \n");
		}

		//创建驱动所对应的服务
		hServiceDDK = CreateServiceA(hServiceMgr,
			lpszDriverName, //驱动程序的在注册表中的名字  
			lpszDriverName, // 注册表驱动程序的 DisplayName 值  
			SERVICE_ALL_ACCESS, // 加载驱动程序的访问权限  
			SERVICE_KERNEL_DRIVER,// 表示加载的服务是驱动程序  
			SERVICE_DEMAND_START, // 注册表驱动程序的 Start 值  
			SERVICE_ERROR_IGNORE, // 注册表驱动程序的 ErrorControl 值  
			lpszDriverPath, // 注册表驱动程序的 ImagePath 值  
			NULL,
			NULL,
			NULL,
			NULL,
			NULL);

		DWORD dwRtn;
		//判断服务是否失败
		if (hServiceDDK == NULL)
		{
			dwRtn = GetLastError();
			if (dwRtn != ERROR_IO_PENDING && dwRtn != ERROR_SERVICE_EXISTS)
			{
				//由于其他原因创建服务失败
				printf("CrateService() Faild %d ! \n", dwRtn);
				bRet = FALSE;
				goto BeforeLeave;
			}
			else
			{
				//服务创建失败，是由于服务已经创立过
				printf("CrateService() Faild Service is ERROR_IO_PENDING or ERROR_SERVICE_EXISTS! \n");
			}

			// 驱动程序已经加载，只需要打开  
			hServiceDDK = OpenServiceA(hServiceMgr, lpszDriverName, SERVICE_ALL_ACCESS);
			if (hServiceDDK == NULL)
			{
				//如果打开服务也失败，则意味错误
				dwRtn = GetLastError();
				printf("OpenService() Faild %d ! \n", dwRtn);
				bRet = FALSE;
				goto BeforeLeave;
			}
			else
			{
				printf("OpenService() ok ! \n");
			}
		}
		else
		{
			printf("CrateService() ok ! \n");
		}

		//开启此项服务
		bRet = StartService(hServiceDDK, NULL, NULL);
		if (!bRet)
		{
			DWORD dwRtn = GetLastError();
			if (dwRtn != ERROR_IO_PENDING && dwRtn != ERROR_SERVICE_ALREADY_RUNNING)
			{
				printf("StartService() Faild %d ! \n", dwRtn);
				bRet = FALSE;
				goto BeforeLeave;
			}
			else
			{
				if (dwRtn == ERROR_IO_PENDING)
				{
					//设备被挂住
					printf("StartService() Faild ERROR_IO_PENDING ! \n");
					bRet = FALSE;
					goto BeforeLeave;
				}
				else
				{
					//服务已经开启
					printf("StartService() Faild ERROR_SERVICE_ALREADY_RUNNING ! \n");
					bRet = TRUE;
					goto BeforeLeave;
				}
			}
		}
		bRet = TRUE;
		//离开前关闭句柄
	BeforeLeave:
		if (hServiceDDK)
		{
			CloseServiceHandle(hServiceDDK);
		}
		if (hServiceMgr)
		{
			CloseServiceHandle(hServiceMgr);
		}
		return bRet;
	}

	//卸载驱动程序  
	BOOL __cdecl UnloadNTDriver(char* szSvrName)
	{
		BOOL bRet = FALSE;
		SC_HANDLE hServiceMgr = NULL;//SCM管理器的句柄
		SC_HANDLE hServiceDDK = NULL;//NT驱动程序的服务句柄
		SERVICE_STATUS SvrSta;
		//打开SCM管理器
		hServiceMgr = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
		if (hServiceMgr == NULL)
		{
			//带开SCM管理器失败
			printf("OpenSCManager() Faild %d ! \n", GetLastError());
			bRet = FALSE;
			goto BeforeLeave;
		}
		else
		{
			//带开SCM管理器失败成功
			printf("OpenSCManager() ok ! \n");
		}
		//打开驱动所对应的服务
		hServiceDDK = OpenServiceA(hServiceMgr, szSvrName, SERVICE_ALL_ACCESS);

		if (hServiceDDK == NULL)
		{
			//打开驱动所对应的服务失败
			printf("OpenService() Faild %d ! \n", GetLastError());
			bRet = FALSE;
			goto BeforeLeave;
		}
		else
		{
			printf("OpenService() ok ! \n");
		}
		//停止驱动程序，如果停止失败，只有重新启动才能，再动态加载。  
		if (!ControlService(hServiceDDK, SERVICE_CONTROL_STOP, &SvrSta))
		{
			printf("ControlService() Faild %d !\n", GetLastError());
		}
		else
		{
			//打开驱动所对应的失败
			printf("ControlService() ok !\n");
		}
		//动态卸载驱动程序。  
		if (!DeleteService(hServiceDDK))
		{
			//卸载失败
			printf("DeleteSrevice() Faild %d !\n", GetLastError());
		}
		else
		{
			//卸载成功
			printf("DelServer:DeleteSrevice() ok !\n");
		}
		bRet = TRUE;
	BeforeLeave:
		//离开前关闭打开的句柄
		if (hServiceDDK)
		{
			CloseServiceHandle(hServiceDDK);
		}
		if (hServiceMgr)
		{
			CloseServiceHandle(hServiceMgr);
		}
		return bRet;
	}
}