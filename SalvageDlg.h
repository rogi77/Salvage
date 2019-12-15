
// SalvageDlg.h : ヘッダー ファイル
//
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma once
#include "ftd2xx.h"
#include "libMPSSE_spi.h"
#include "afxcmn.h"
#define SPI_WRITE_COMPLETION_RETRY 10
#define START_ADDRESS_EEPROM 0x00 /*read/write start address inside the EEPROM*/
#define END_ADDRESS_EEPROM 0x10
#define RETRY_COUNT_EEPROM 10 /* number of retries if read/write fails */
#define CHANNEL_TO_OPEN 0 /*0 for first available channel, 1 for next... */
#define SPI_SLAVE_0 0
#define SPI_SLAVE_1 1
#define SPI_SLAVE_2 2
#define DATA_OFFSET 3


// CSalvageDlg ダイアログ
class CSalvageDlg : public CDialogEx
{
// コンストラクション
public:
	CSalvageDlg(CWnd* pParent = NULL);	// 標準コンストラクター

// ダイアログ データ
	enum { IDD = IDD_SALVAGE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_LogMessage;
	afx_msg void OnBnClickedInitcheck();
	CString m_output_fname;
	CString m_freq;
	CString m_burstSize;
	afx_msg void OnBnClickedFileopen();

private:
	int m_file_size;
	CString m_fileName;
	int m_sclk_freq;
	int m_burst_size;
	FT_HANDLE m_ftHandle;
	ChannelConfig m_channelConf;
	BYTE m_checksum;
	int m_salvageCompleteByte;
	BOOL m_thread_end;
	BOOL m_emergency_stop;

public:
	afx_msg void OnBnClickedSalvageStart();
	friend UINT SalvageCoreThread(LPVOID p);
	CProgressCtrl m_salvageProgress;
	CString m_disp_salvage_percent;
	afx_msg void OnBnClickedSalvageStop();
};
