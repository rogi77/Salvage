
// SalvageDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "Salvage.h"
#include "SalvageDlg.h"
#include "afxdialogex.h"
#include <stdlib.h>
#include <stdio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BYTE g_buffer[110000000]; // 110Mbyte 確保
void TreatMessage( void );
//UINT SalvageCoreThread(LPVOID p);

// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ダイアログ データ
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// 実装
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSalvageDlg ダイアログ




CSalvageDlg::CSalvageDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSalvageDlg::IDD, pParent)
	, m_LogMessage(_T(""))
	, m_output_fname(_T(""))
	, m_freq(_T(""))
	, m_burstSize(_T(""))
	, m_disp_salvage_percent(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSalvageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_LogMessage);
	DDX_Text(pDX, IDC_FOUTNAME, m_output_fname);
	DDX_Text(pDX, IDC_SCLKFREQ, m_freq);
	DDX_Text(pDX, IDC_BURSTSIZE, m_burstSize);
	DDX_Control(pDX, IDC_SALVAGE_PROGRESS, m_salvageProgress);
	DDX_Text(pDX, IDC_PROGRESS_RATE, m_disp_salvage_percent);
}

BEGIN_MESSAGE_MAP(CSalvageDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_INITCHECK, &CSalvageDlg::OnBnClickedInitcheck)
	ON_BN_CLICKED(IDC_FILEOPEN, &CSalvageDlg::OnBnClickedFileopen)
	ON_BN_CLICKED(IDC_SALVAGE_START, &CSalvageDlg::OnBnClickedSalvageStart)
	ON_BN_CLICKED(IDC_SALVAGE_STOP, &CSalvageDlg::OnBnClickedSalvageStop)
END_MESSAGE_MAP()


// CSalvageDlg メッセージ ハンドラー

BOOL CSalvageDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// "バージョン情報..." メニューをシステム メニューに追加します。

	// IDM_ABOUTBOX は、システム コマンドの範囲内になければなりません。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。
	m_freq = "5000";
	m_burstSize = "2000";
	m_disp_salvage_percent = "0%";
	UpdateData(FALSE);
	m_salvageProgress.SetRange32(0, 1000); // progress barの最小値と最大値を設定

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

void CSalvageDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CSalvageDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CSalvageDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSalvageDlg::OnBnClickedInitcheck()
{
	UpdateData(TRUE);
	FT_STATUS status;
	FT_DEVICE_LIST_INFO_NODE devList;
	uint8 address=0;
	uint16 data;
	int i,j;
	uint32 channels;

	Init_libMPSSE();
	status = SPI_GetNumChannels(&channels);
	m_LogMessage.Format("SPI host number = %d\r\n", channels);
	UpdateData(FALSE);
	if(channels != 0){
		SPI_GetChannelInfo(0, &devList);
		m_LogMessage.AppendFormat("ID=0x%x\r\n",devList.ID);
		m_LogMessage.AppendFormat("LocId=0x%x\r\n",devList.LocId);
		m_LogMessage.AppendFormat("SerialNumber=%s\r\n",devList.SerialNumber);
		m_LogMessage.AppendFormat("Description=%s\r\n\r\n",devList.Description);
		UpdateData(FALSE);
	}
}


void CSalvageDlg::OnBnClickedFileopen()
{
	UpdateData(TRUE);
	CString filter("any file (*.*)|*.*||");
	CFileDialog selDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, filter);
	CString fname;
	if(selDlg.DoModal() == IDOK){
		fname = selDlg.GetPathName();
	}else return;
	m_fileName = fname;

	fpos_t fsize = 0;
	FILE *fp = fopen(fname,"rb");
	/* ファイルサイズを調査 */
	fseek(fp,0,SEEK_END);
	fgetpos(fp,&fsize);
	m_file_size = fsize;
	fclose(fp);


	CFile cfile;
	cfile.Open(fname, CFile::modeRead);
#if 0
	// 理由はわからないけど、なぜかdebugモードでコンパイル出来なくなる
	CFileStatus status;
	cfile.GetStatus(status);

	m_file_size = status.m_size;
#endif
	if(m_file_size > 110000000){
		AfxMessageBox("Salvage size must be less than 100MB. Please try again");
		return;
	}

	// check sum 計算
	m_checksum = 0;
	cfile.Read(g_buffer, m_file_size);
	for(int i = 0; i < m_file_size; i++){
		m_checksum += g_buffer[i];
	}

	m_LogMessage = "";
	m_LogMessage.AppendFormat("fname = %s\r\n", fname);
	m_LogMessage.AppendFormat("file size = %d bytes\r\n", m_file_size);
	m_LogMessage.AppendFormat("checksum = %d(0x%x)\r\n", m_checksum, m_checksum);
	m_LogMessage.AppendFormat("--> Ready to salvage!!\r\n");
	UpdateData(FALSE);
}

#define PROGRESS_DISP_INTERVAL 100
void CSalvageDlg::OnBnClickedSalvageStart()
{
	UpdateData(TRUE);
	m_emergency_stop = FALSE;
	m_sclk_freq = atoi(m_freq);
	m_burst_size = atoi(m_burstSize);
	if(m_output_fname.GetLength() != 8){
		CString msg;
		msg.Format("文字数は８文字を指定してください(%d)\r\n", m_output_fname.GetLength());
		AfxMessageBox(msg);
		return;
	}
	m_LogMessage.AppendFormat("file name : <%s.dat>  sclk = %dkHz  bust size = %d\r\n", m_output_fname, m_sclk_freq, m_burst_size);
	UpdateData(FALSE);

	m_channelConf.ClockRate = m_sclk_freq * 1000; // unit is Hz
	m_channelConf.LatencyTimer= 255;
	m_channelConf.configOptions = SPI_CONFIG_OPTION_MODE0 | SPI_CONFIG_OPTION_CS_DBUS3 | SPI_CONFIG_OPTION_CS_ACTIVELOW;
	m_channelConf.Pin = 0x00000000;/*FinalVal-FinalDir-InitVal-InitDir (for dir 0=in, 1=out)*/	
	FT_STATUS status;
	FT_DEVICE_LIST_INFO_NODE devList;
	uint8 address=0;
	uint16 data;
	int i,j;
	uint32 channels;

	Init_libMPSSE();
	status = SPI_GetNumChannels(&channels);
	if(channels == 0){
		AfxMessageBox("can't find any SPI device. Please check the hardware");
		return;
	}

	/* Open the first available channel */
	status = SPI_OpenChannel(CHANNEL_TO_OPEN,&m_ftHandle);
	status = SPI_InitChannel(m_ftHandle, &m_channelConf);
	BYTE buffer[10000];
	uint32 sizeToTransfer, sizeTransfered;
	char out_fname_char[256];
	strncpy(out_fname_char, m_output_fname, 8);

	buffer[0] = 0x0; // dummy header
	buffer[1] = 0xa6; // start code
	for(int i = 0; i < 8; i++) buffer[i + 2] =  out_fname_char[i];
	buffer[10] = (m_file_size >> 24) & 0xff;
	buffer[11] = (m_file_size >> 16) & 0xff;
	buffer[12] = (m_file_size >> 8) & 0xff;
	buffer[13] = m_file_size & 0xff;
	buffer[14] = m_checksum;
	sizeToTransfer = 15;

	// Header送信開始
	status = SPI_Write(m_ftHandle, buffer, sizeToTransfer, &sizeTransfered,
		//SPI_TRANSFER_OPTIONS_SIZE_IN_BITS|
		SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES|
		SPI_TRANSFER_OPTIONS_CHIPSELECT_ENABLE|
		SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE);

	// Status 表示
		m_LogMessage.AppendFormat("\r\n<< Header >>\r\n");
	m_LogMessage.AppendFormat("%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\r\n", 
		buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5], buffer[6], buffer[7], 
		buffer[8], buffer[9], buffer[10], buffer[11], buffer[12], buffer[13], buffer[14]);
	UpdateData(FALSE);

	// コアデータSalvageを、別スレッドで開始
	int ttime = GetTickCount();
	int progress_disp_wait_timer = ttime + PROGRESS_DISP_INTERVAL; // progress display interval = PROGRESS_DISP_INTERVAL(msec)
	m_salvageCompleteByte = 0;
	Sleep(50); // DE0 wait
	for(;;){ // 転送ループ
		int progress_disp_wait_timer = GetTickCount();
		int restByte = m_file_size - m_salvageCompleteByte;
		int burst_size;
		if(restByte >= m_burst_size) burst_size = m_burst_size;
		else burst_size = restByte;

		if(burst_size <= 0) break;
		if(m_emergency_stop == TRUE){
			AfxMessageBox("転送を中止します");
			m_LogMessage.AppendFormat("### 転送を中止します ###\r\n");
			UpdateData(FALSE);
			break;
		}

		for(int m = 0; m < burst_size; m++){
			buffer[m] = g_buffer[m_salvageCompleteByte + m];
		}

		status = SPI_Write(m_ftHandle, buffer, burst_size, &sizeTransfered,
			//SPI_TRANSFER_OPTIONS_SIZE_IN_BITS|
			SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES|SPI_TRANSFER_OPTIONS_CHIPSELECT_ENABLE|SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE);

		m_salvageCompleteByte += burst_size;
//		TRACE("salvage complete byte = %d\n", m_salvageCompleteByte);

		int nowtime = GetTickCount();
		if(progress_disp_wait_timer < nowtime){
			progress_disp_wait_timer = nowtime + PROGRESS_DISP_INTERVAL;
			int pos = (int)((double)m_salvageCompleteByte / (double)m_file_size * 1000.0 + 0.5);
			m_salvageProgress.SetPos(pos);
			//CString msg;
			//msg.Format("%d", pos / 10);
			//GetDlgItem(IDC_PROGRESS_RATE)->SetWindowTextA(msg); //			m_disp_salvage_percent.Format("%d%", pos / 10);
			m_disp_salvage_percent.Format("%d%%", pos / 10);
			UpdateData(FALSE);
			TreatMessage();
		}
	}

#if 0
	m_thread_end = FALSE;
//	CWinThread* pThread;
	AfxBeginThread(SalvageCoreThread, this);
	Sleep(10);
	for(;;){
		if(m_thread_end == TRUE){
			ttime = GetTickCount() - ttime;
			m_LogMessage.AppendFormat("Salvage終了(%dmsec)\r\n", ttime);
			m_disp_salvage_percent = "0%";
			m_salvageProgress.SetPos(0);
			UpdateData(FALSE);
			break;
		}
		Sleep(90);
		int pos = (int)((double)m_salvageCompleteByte / (double)m_file_size * 1000.0);
		m_disp_salvage_percent.Format("%d\%", pos / 10);
		m_salvageProgress.SetPos(pos); // progress barの更新
		UpdateData(FALSE);
		TreatMessage();
	}
#endif

	ttime = GetTickCount() - ttime;
	m_LogMessage.AppendFormat("Salvage終了(%dmsec)\r\n", ttime);
	m_LogMessage.AppendFormat("転送レート%.1f(kByte/sec)\r\n", (double)m_file_size / (double)ttime);
	m_LogMessage.AppendFormat("Salvage finished!!\r\n");
	UpdateData(FALSE);
	AfxMessageBox("Salvage終了!!");
	m_salvageProgress.SetPos(0);
	m_disp_salvage_percent = "0%";
	UpdateData(FALSE);
	// 終了処理
	status = SPI_CloseChannel(m_ftHandle);
	Cleanup_libMPSSE();
}

void TreatMessage( void )
{
	MSG message;
	for(;PeekMessage(&message,NULL,0,0,PM_REMOVE);){
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
}

UINT SalvageCoreThread(LPVOID p)
{
	CSalvageDlg *pt = (CSalvageDlg*)p;
	for(int i = 0; i < 100; i++){
		Sleep(10);
	}
	pt->m_thread_end = TRUE;

	return 1;
}



void CSalvageDlg::OnBnClickedSalvageStop()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	m_emergency_stop = TRUE;
}
