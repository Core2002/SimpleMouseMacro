#include "NTDriverLoader.hpp"

namespace MyNTDriverLoader {

	BOOL __cdecl LoadNTDriver(char* lpszDriverName, char* lpszDriverPath)
	{
		//char szDriverImagePath[256];
		//�õ�����������·��
		//GetFullPathNameA(lpszDriverPath, 256, szDriverImagePath, NULL);

		BOOL bRet = FALSE;

		SC_HANDLE hServiceMgr = NULL;//SCM�������ľ��
		SC_HANDLE hServiceDDK = NULL;//NT��������ķ�����

		//�򿪷�����ƹ�����
		hServiceMgr = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

		if (hServiceMgr == NULL)
		{
			//OpenSCManagerʧ��
			printf("OpenSCManager() Faild %d ! \n", GetLastError());
			bRet = FALSE;
			goto BeforeLeave;
		}
		else
		{
			////OpenSCManager�ɹ�
			printf("OpenSCManager() ok ! \n");
		}

		//������������Ӧ�ķ���
		hServiceDDK = CreateServiceA(hServiceMgr,
			lpszDriverName, //�����������ע����е�����  
			lpszDriverName, // ע������������ DisplayName ֵ  
			SERVICE_ALL_ACCESS, // ������������ķ���Ȩ��  
			SERVICE_KERNEL_DRIVER,// ��ʾ���صķ�������������  
			SERVICE_DEMAND_START, // ע������������ Start ֵ  
			SERVICE_ERROR_IGNORE, // ע������������ ErrorControl ֵ  
			lpszDriverPath, // ע������������ ImagePath ֵ  
			NULL,
			NULL,
			NULL,
			NULL,
			NULL);

		DWORD dwRtn;
		//�жϷ����Ƿ�ʧ��
		if (hServiceDDK == NULL)
		{
			dwRtn = GetLastError();
			if (dwRtn != ERROR_IO_PENDING && dwRtn != ERROR_SERVICE_EXISTS)
			{
				//��������ԭ�򴴽�����ʧ��
				printf("CrateService() Faild %d ! \n", dwRtn);
				bRet = FALSE;
				goto BeforeLeave;
			}
			else
			{
				//���񴴽�ʧ�ܣ������ڷ����Ѿ�������
				printf("CrateService() Faild Service is ERROR_IO_PENDING or ERROR_SERVICE_EXISTS! \n");
			}

			// ���������Ѿ����أ�ֻ��Ҫ��  
			hServiceDDK = OpenServiceA(hServiceMgr, lpszDriverName, SERVICE_ALL_ACCESS);
			if (hServiceDDK == NULL)
			{
				//����򿪷���Ҳʧ�ܣ�����ζ����
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

		//�����������
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
					//�豸����ס
					printf("StartService() Faild ERROR_IO_PENDING ! \n");
					bRet = FALSE;
					goto BeforeLeave;
				}
				else
				{
					//�����Ѿ�����
					printf("StartService() Faild ERROR_SERVICE_ALREADY_RUNNING ! \n");
					bRet = TRUE;
					goto BeforeLeave;
				}
			}
		}
		bRet = TRUE;
		//�뿪ǰ�رվ��
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

	//ж����������  
	BOOL __cdecl UnloadNTDriver(char* szSvrName)
	{
		BOOL bRet = FALSE;
		SC_HANDLE hServiceMgr = NULL;//SCM�������ľ��
		SC_HANDLE hServiceDDK = NULL;//NT��������ķ�����
		SERVICE_STATUS SvrSta;
		//��SCM������
		hServiceMgr = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
		if (hServiceMgr == NULL)
		{
			//����SCM������ʧ��
			printf("OpenSCManager() Faild %d ! \n", GetLastError());
			bRet = FALSE;
			goto BeforeLeave;
		}
		else
		{
			//����SCM������ʧ�ܳɹ�
			printf("OpenSCManager() ok ! \n");
		}
		//����������Ӧ�ķ���
		hServiceDDK = OpenServiceA(hServiceMgr, szSvrName, SERVICE_ALL_ACCESS);

		if (hServiceDDK == NULL)
		{
			//����������Ӧ�ķ���ʧ��
			printf("OpenService() Faild %d ! \n", GetLastError());
			bRet = FALSE;
			goto BeforeLeave;
		}
		else
		{
			printf("OpenService() ok ! \n");
		}
		//ֹͣ�����������ֹͣʧ�ܣ�ֻ�������������ܣ��ٶ�̬���ء�  
		if (!ControlService(hServiceDDK, SERVICE_CONTROL_STOP, &SvrSta))
		{
			printf("ControlService() Faild %d !\n", GetLastError());
		}
		else
		{
			//����������Ӧ��ʧ��
			printf("ControlService() ok !\n");
		}
		//��̬ж����������  
		if (!DeleteService(hServiceDDK))
		{
			//ж��ʧ��
			printf("DeleteSrevice() Faild %d !\n", GetLastError());
		}
		else
		{
			//ж�سɹ�
			printf("DelServer:DeleteSrevice() ok !\n");
		}
		bRet = TRUE;
	BeforeLeave:
		//�뿪ǰ�رմ򿪵ľ��
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