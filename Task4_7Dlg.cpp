
// Task4_7Dlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "Task4_7.h"
#include "Task4_7Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <ctime>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <regex>
#include <vector>


// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Диалоговое окно CTask47Dlg



CTask47Dlg::CTask47Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TASK4_7_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTask47Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTask47Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, &CTask47Dlg::OnBnClickedButtonBrowse)
END_MESSAGE_MAP()


// Обработчики сообщений CTask47Dlg

BOOL CTask47Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void CTask47Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CTask47Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CTask47Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


class Logger {
public:
	static void log(const std::string& level, const std::string& message) {
		std::ofstream logFile("log.txt", std::ios::app);
		if (!logFile) {
			std::cerr << "Ошибка: Не удалось открыть log.txt для записи." << std::endl;
			return;
		}

		std::time_t now = std::time(nullptr);
		std::tm localTime;
		localtime_s(&localTime, &now);

		logFile << (1900 + localTime.tm_year) << "-"
			<< (1 + localTime.tm_mon) << "-"
			<< localTime.tm_mday << " "
			<< localTime.tm_hour << ":"
			<< localTime.tm_min << ":"
			<< localTime.tm_sec << " "
			<< "[" << static_cast<std::string>(level) << "] "
			<< message << std::endl;
	}
};

struct MatchInfo
{
	int line;
	int col;
	std::string url;

	MatchInfo(int line, int col, std::string url)
	{
		this->line = line;
		this->col = col;
		this->url = url;
	}

	std::string to_string()
	{
		return "  Строка " + std::to_string(line) + ", столбец " + std::to_string(col) + ": " + url;
	}
};

static const std::regex url_pattern(R"((http(s)?|ftp|gopher|news|telnet|file)://www\.[a-zA-Z0-9.-]+\.[a-z]{2,})");

class FileProcessor {
public:
	static void parseFiles(const std::string& input, std::ostream& out) {
		Logger::log("INFO", "Начало парсинга файлов");

		std::istringstream stream(input);
		std::string filename;
		std::map<std::string, std::vector<MatchInfo>> results;

		while (std::getline(stream, filename, ',')) {
			processFile(filename, results);
		}

		for (const std::pair<std::string, std::vector<MatchInfo>> pair : results) {
			const auto& file = pair.first;
			const auto& matches = pair.second;

			out << "Файл: " << file << ": ";
			for (MatchInfo match_info : matches) {
				out << match_info.to_string() << "; ";
			}
		}

		Logger::log("INFO", "Парсинг файлов завершён");
	}
private:
	static void processFile(const std::string& filename, std::map<std::string, std::vector<MatchInfo>>& results) {
		std::ifstream file(filename);
		if (!file) {
			Logger::log("ERROR", "Не удалось открыть файл: " + filename);
			std::cerr << "Ошибка: Не удалось открыть файл " << filename << std::endl;
			return;
		}

		Logger::log("INFO", "Открыт файл: " + filename);

		std::string line;
		int line_number = 0;
		bool found = false;

		while (std::getline(file, line)) {
			++line_number;
			std::sregex_iterator begin(line.begin(), line.end(), url_pattern);
			std::sregex_iterator end;

			for (auto it = begin; it != end; ++it) {
				int col_number = it->position() + 1;
				std::string url = it->str();
				results[filename].emplace_back(MatchInfo(line_number, col_number, url));
				Logger::log("INFO", "Найден URL: " + url + " в файле " + filename +
					" (строка " + std::to_string(line_number) + ", столбец " + std::to_string(col_number) + ")");
				found = true;
			}
		}

		if (!found) {
			Logger::log("WARNING", "В файле " + filename + " не найдено URL-адресов");
		}
	}
};


void CTask47Dlg::OnBnClickedButtonBrowse()
{
	CFileDialog dlg(TRUE, _T("txt"), nullptr, OFN_FILEMUSTEXIST,
		_T("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||"));

	if (dlg.DoModal() == IDOK)
	{
		CString filePath = dlg.GetPathName();
		CT2A asciiPath(filePath);
		std::ifstream inputFile(asciiPath);

		if (!inputFile.is_open()) {
			AfxMessageBox(_T("Не удалось открыть файл."));
			return;
		}

		std::string fileListLine;
		std::getline(inputFile, fileListLine);

		Logger::log("INFO", "Прочитан список из файла: " + fileListLine);

		SetDlgItemText(IDC_EDIT_FILELIST, CString(fileListLine.c_str()));

		std::ostringstream resultOutput;
		FileProcessor::parseFiles(fileListLine, resultOutput);
		SetDlgItemText(IDC_EDIT_RESULTS, CString(resultOutput.str().c_str()));
	}
}

