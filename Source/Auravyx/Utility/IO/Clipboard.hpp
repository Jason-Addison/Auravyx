#pragma once

#include <string>
#include <exception>
#include <string.h>

#if defined(_WIN32) || defined(WIN32)
#define WINDOWS 1
#elif defined(__unix__)
#define LINUX 1
#else
#error "Not Supported OS"
#endif


class CExceptionXX : public std::exception
{
private:
	const char* m_pReason;

public:
	CExceptionXX(const char* pReason) : m_pReason(pReason) {};

	const char* what() {
		return m_pReason;
	}
};

class IClipboardOS 
{
protected:

	IClipboardOS() {};

public:

	virtual void copyText(const char* pText, size_t Length) {};
	virtual void PasteText(std::string& sString) {};
};
class Clipboard;



#ifdef WINDOWS


#include <Windows.h>

class CClipboardWindows : public IClipboardOS 
{

private:
	CClipboardWindows() 
	{
		if (!OpenClipboard(0))
		{
			throw CExceptionXX("Cannot open clipboard!");
		}
	}

public:
	~CClipboardWindows()
	{
		CloseClipboard();
	}

	void copyText(const char* pText, size_t length)
	{
		if (!EmptyClipboard())
		{
			throw CExceptionXX("Cannot empty clipboard!");
		}
		HGLOBAL clipBuffer;
		clipBuffer = GlobalAlloc(GMEM_DDESHARE, length + 1);
		char* buff = (char*)GlobalLock(clipBuffer);
		strcpy(buff, pText);
		GlobalUnlock(clipBuffer);
		SetClipboardData(CF_TEXT, clipBuffer);
		CloseClipboard();
	}

	void PasteText(std::string & sString) 
	{
		//char* pResult = (char*)GetClipboardData(CF_TEXT);

		//if (pResult == NULL)
		{
		//	throw CExceptionXX("Clipboard has no data to paste!");
		}

		//sString = std::string(pResult);
		//GlobalFree(pResult);
		OpenClipboard(nullptr);
		HANDLE hData = GetClipboardData(CF_TEXT);

		char* pszText = static_cast<char*>(GlobalLock(hData));
		std::string text(pszText);

		GlobalUnlock(hData);
		CloseClipboard();
		sString = text;
	}

	friend class Clipboard;
};

#elif LINUX


class CClipboardLinux : public IClipboardOS {
private:
	/*GtkClipboard* m_pClip;

	CClipboardLinux() {
		gtk_init(0, 0);
		m_pClip = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
	}

	static void copy_callback(GtkClipboard* pClip, const gchar* pText, gpointer pData) {
		if (pData) {
			gtk_clipboard_set_text(
				pClip,
				(const gchar*)pData,
				strlen((const char*)pData)
			);
		}

		gtk_main_quit();
	}

	static void paste_callback(GtkClipboard* pClip, const gchar* pText, gpointer pData) {
		std::string* pString = (std::string*)pData;
		(*pString) = pText;

		gtk_main_quit();
	}

public:
	void copyText(const char* pText, size_t Length) {
		gtk_clipboard_request_text(m_pClip, copy_callback, (gpointer)pText);
		gtk_main();
	}

	void PasteText(std::string& sString) {
		gtk_clipboard_request_text(m_pClip, paste_callback, (gpointer)& sString);
		gtk_main();
	}

	friend class Clipboard;
};*/

#endif

class Clipboard 
{
private:
	IClipboardOS* m_pClipboard = new

#ifdef WINDOWS
		CClipboardWindows();
#elif LINUX
		CClipboardLinux();
#endif


public:
	~Clipboard() {
		delete m_pClipboard;
	}
	void operator<<(const char* pText) 
	{
		m_pClipboard->copyText(pText, strlen(pText));
	}

	void operator<<(std::string& sText)
	{
		m_pClipboard->copyText(sText.c_str(), sText.size());
	}

	void operator>>(std::string& sResult) 
	{
		m_pClipboard->PasteText(sResult);
	}
};