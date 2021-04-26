// Флаги компиляции: -std=c99 -O3 -Wall -Wextra -Wno-unused-variable

#include <stdio.h>

// OpenVR
#include "openvr_capi.h"

// Windows
#define WIN32_LEAD_AND_MEAN
#include <windows.h>

// Все функции, которые можно получить сразу через GetProcAddress
// ==========================================================================================
int (*VR_IsHmdPresent)(void);
int (*VR_IsRuntimeInstalled)(void);
intptr_t (*VR_InitInternal)( EVRInitError *peError, EVRApplicationType eType );
void (*VR_ShutdownInternal)(void);
const char * (*VR_GetVRInitErrorAsSymbol)( EVRInitError error );
const char * (*VR_GetVRInitErrorAsEnglishDescription)( EVRInitError error );
intptr_t (*VR_GetGenericInterface)( const char *pchInterfaceVersion, EVRInitError *peError );
// ==========================================================================================

// Детальные сообщения о вызовах GetProcAddress
void* verbose_get_proc(HINSTANCE lib, char* identifier) {
	void* address = GetProcAddress(lib, identifier);
	
	if (!address) {
		printf("Не удалось отрезолвить: %s\n", identifier);
		exit(-1);
	} else {
		printf("%p %s\n", address, identifier);
		return address;
	}
}

// Подгрузка dll
void load_openvr_dll() {
	HINSTANCE lib = LoadLibrary("openvr_api.dll");
	
	if (!lib) {
		printf("Не удалось подгрузить библиотеку\n");
		exit(-1);
	}
	
	printf("Библиотека подгружена...\n");
	printf("%p\n", lib);
	
	VR_IsHmdPresent 							= verbose_get_proc(lib, "VR_IsHmdPresent");
	VR_IsRuntimeInstalled 						= verbose_get_proc(lib, "VR_IsRuntimeInstalled");
	VR_InitInternal 							= verbose_get_proc(lib, "VR_InitInternal");
	VR_ShutdownInternal 						= verbose_get_proc(lib, "VR_ShutdownInternal");
	VR_GetVRInitErrorAsSymbol 					= verbose_get_proc(lib, "VR_GetVRInitErrorAsSymbol");
	VR_GetVRInitErrorAsEnglishDescription 		= verbose_get_proc(lib, "VR_GetVRInitErrorAsEnglishDescription");
	VR_GetGenericInterface 						= verbose_get_proc(lib, "VR_GetGenericInterface");
}

// Инициализация VR сессии
void init_vr() {
	if( !VR_IsHmdPresent() )
	{
		printf("HMD отсутствуют\n");
		exit(-1);
	}

	if( !VR_IsRuntimeInstalled() )
	{
		printf("Среда VR не установлена\n");
		exit(-1);
	}
	
	printf("HMD обнаружен, среда готова к работе\n");
	printf("Инициализация VR...\n");
	
	EVRInitError rc;
	
	uint32_t vrToken = VR_InitInternal(&rc, EVRApplicationType_VRApplication_Scene);
	
	if (rc != EVRInitError_VRInitError_None) {
		printf("Не удалось инициализировать VR: %s\n", VR_GetVRInitErrorAsEnglishDescription(rc));
		exit(-1);
	}
	
	printf("Токен сессии: %d\n", vrToken);
}

int main() {
	SetConsoleOutputCP(CP_UTF8);
	
	load_openvr_dll();
	init_vr();
}

