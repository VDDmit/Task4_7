
// Task4_7.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CTask47App:
// Сведения о реализации этого класса: Task4_7.cpp
//

class CTask47App : public CWinApp
{
public:
	CTask47App();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CTask47App theApp;
