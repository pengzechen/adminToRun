#include <windows.h>
#include <stdio.h>

BOOL ManagerRun(LPCSTR exe, LPCSTR param, INT nShow)  {
	SHELLEXECUTEINFO ShExecInfo; 
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);  
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS ;  
	ShExecInfo.hwnd = NULL;  
	ShExecInfo.lpVerb = "runas";  
	ShExecInfo.lpFile = exe; 
	ShExecInfo.lpParameters = param;   
	ShExecInfo.lpDirectory = NULL;  
	ShExecInfo.nShow = nShow;  
	ShExecInfo.hInstApp = NULL;   
	BOOL ret = ShellExecuteEx(&ShExecInfo);  
	CloseHandle(ShExecInfo.hProcess);
	return ret;
}

BOOL is_admin() {
    BOOL isAdmin = FALSE;
    PSID administratorsGroup = NULL;

    SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
    if (AllocateAndInitializeSid(&NtAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &administratorsGroup)) {
        CheckTokenMembership(NULL, administratorsGroup, &isAdmin);
        FreeSid(administratorsGroup);
    }
    return isAdmin == TRUE;
}

int main(int argc, char *argv[])
{
    if(argc == 1) //初次运行，即双击EXE
    {
       ShowWindow(GetConsoleWindow(),SW_HIDE);
       ManagerRun(argv[0],"2", SW_SHOWNORMAL);  // 这里不关心返回值了。只给用户自动提示一次。
       return 1;
    }
	else if(argc == 2) //再次运行,即上面那个ManagerRun
    {
		if(!is_admin()) 
			return -1;
		printf(" run as admin\n");
		getchar();
    }
    return 0;
}

