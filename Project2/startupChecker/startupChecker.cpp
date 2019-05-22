#pragma(comment,"Advapi32.lib");
#include "startupChecker.h"
#include <Windows.h>
#include <utility/Logger.h>



startupChecker::startupChecker()
{
	logger = make_unique<Logger>("Startup Checker");
}

startupChecker::~startupChecker()
{
}

LONG GetStringRegKey(HKEY hKey, const std::wstring &strValueName, std::wstring &strValue, const std::wstring &strDefaultValue)
{
	strValue = strDefaultValue;
	WCHAR szBuffer[512];
	DWORD dwBufferSize = sizeof(szBuffer);
	ULONG nError;
	nError = RegQueryValueExW(hKey, strValueName.c_str(), 0, NULL, (LPBYTE)szBuffer, &dwBufferSize);
	if (ERROR_SUCCESS == nError)
	{
		strValue = szBuffer;
	}
	return nError;
}

bool startupChecker::checkJavaVersion()
{
	HKEY hKeyJava;
	LONG exists = RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\JavaSoft\\Java Runtime Environment",0,KEY_READ,&hKeyJava);
	if (exists != ERROR_SUCCESS) {
		logger->setLogLevel(Logger::log_error);
		logger->logString("Critical Error, Java installation not found!");
		return false;
	}
	else {
		wstring keyName = L"CurrentVersion";
		wstring currentVersion;
		LONG keyFound =	GetStringRegKey(hKeyJava, keyName, currentVersion, L"Not found");
		if (keyFound != ERROR_SUCCESS) {
			logger->setLogLevel(Logger::log_error);
			logger->logString("Critical Error, couldn't retrieve Java Version!");
			return false;
		}
		else {
			string versionFound = std::string(currentVersion.begin(), currentVersion.end());
			logger->setLogLevel(Logger::log_message);
			logger->logString("Found Java Runtime Version " + versionFound);
			return true;
		}

	}
	return false;
}

string startupChecker::retrieveJavaPath()
{
	return string();
}
