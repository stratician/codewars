/*char *GETDISKCONTROLLERID(void)
{
	HKEY	hKey[9];
	DWORD	dwNumKeys = 9;
	DWORD	dwKeyIndex;
	BOOL bkeyFound = FALSE;

	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,"HARDWARE",0,KEY_READ,&hKey[0]) != ERROR_SUCCESS)
		MessageBox("Cannot find registery key","Error",MB_ICONERROR);

	if(RegOpenKeyEx(hKey[0],"DESCRIPTION",0,KEY_READ,&hKey[1]) != ERROR_SUCCESS)
		MessageBox("Cannot find registery key","Error",MB_ICONERROR);

	if(RegOpenKeyEx(hKey[1],"System",0,KEY_READ,&hKey[2]) != ERROR_SUCCESS)
		MessageBox("Cannot find registery key","Error",MB_ICONERROR);

	if(RegOpenKeyEx(hKey[2],"MultifunctionAdapter",0,KEY_READ,&hKey[3]) != ERROR_SUCCESS)
		MessageBox("Cannot find registery key","Error",MB_ICONERROR);

	DWORD dwQueryNumber=0;
	char rgchSubQueryStringBuffer[MAX_STRING_SIZE];

	while(!bkeyFound)
	{
		if(RegOpenKeyEx(hKey[3],_itoa(dwQueryNumber++,rgchSubQueryStringBuffer,10),0,KEY_READ,&hKey[4]) == ERROR_SUCCESS)
		{
			if(RegOpenKeyEx(hKey[4],"DiskController",0,KEY_READ,&hKey[5]) == ERROR_SUCCESS)
				bkeyFound = TRUE;
			else
				RegCloseKey(hKey[4]);
		} else
			MessageBox("Cannot find registery key","Error",MB_ICONERROR);
	}
	
	if(RegOpenKeyEx(hKey[5],"0",0,KEY_READ,&hKey[6]) != ERROR_SUCCESS)
		MessageBox("Cannot find registery key","Error",MB_ICONERROR);

	if(RegOpenKeyEx(hKey[6],"DiskPeripheral",0,KEY_READ,&hKey[7]) != ERROR_SUCCESS)
		MessageBox("Cannot find registery key","Error",MB_ICONERROR);

	if(RegOpenKeyEx(hKey[7],"0",0,KEY_READ,&hKey[8]) != ERROR_SUCCESS)
		MessageBox("Cannot find registery key","Error",MB_ICONERROR);



	DWORD dwKeyType;

	if(RegQueryValueEx(hKey[8],"Identifier",NULL,&dwKeyType,NULL,pSize) != ERROR_SUCCESS)
		MessageBox("Cannot read registery value","Error",MB_ICONERROR);

	BYTE * pBuffer = new BYTE[*pSize];

	if(RegQueryValueEx(hKey[8],"Identifier",NULL,&dwKeyType,pBuffer,pSize) != ERROR_SUCCESS)
		MessageBox("Cannot read registery value","Error",MB_ICONERROR);

	for(dwKeyIndex=0;dwKeyIndex < dwNumKeys;dwKeyIndex++)	
		RegCloseKey(hKey[dwKeyIndex]);

	return (char *)pBuffer;
}*/