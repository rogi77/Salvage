
// SalvageDlg.cpp : �����t�@�C��
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

BYTE g_buffer[110000000]; // 110Mbyte �m��
void TreatMessage( void );
//UINT SalvageCoreThread(LPVOID p);

// �A�v���P�[�V�����̃o�[�W�������Ɏg���� CAboutDlg �_�C�A���O

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

// ����
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


// CSalvageDlg �_�C�A���O




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


// CSalvageDlg ���b�Z�[�W �n���h���[

BOOL CSalvageDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// "�o�[�W�������..." ���j���[���V�X�e�� ���j���[�ɒǉ����܂��B

	// IDM_ABOUTBOX �́A�V�X�e�� �R�}���h�͈͓̔��ɂȂ���΂Ȃ�܂���B
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

	// ���̃_�C�A���O�̃A�C�R����ݒ肵�܂��B�A�v���P�[�V�����̃��C�� �E�B���h�E���_�C�A���O�łȂ��ꍇ�A
	//  Framework �́A���̐ݒ�������I�ɍs���܂��B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R���̐ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R���̐ݒ�

	// TODO: �������������ɒǉ����܂��B
	m_freq = "5000";
	m_burstSize = "2000";
	m_disp_salvage_percent = "0%";
	UpdateData(FALSE);
	m_salvageProgress.SetRange32(0, 1000); // progress bar�̍ŏ��l�ƍő�l��ݒ�

	return TRUE;  // �t�H�[�J�X���R���g���[���ɐݒ肵���ꍇ�������ATRUE ��Ԃ��܂��B
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

// �_�C�A���O�ɍŏ����{�^����ǉ�����ꍇ�A�A�C�R����`�悷�邽�߂�
//  ���̃R�[�h���K�v�ł��B�h�L�������g/�r���[ ���f�����g�� MFC �A�v���P�[�V�����̏ꍇ�A
//  ����́AFramework �ɂ���Ď����I�ɐݒ肳��܂��B

void CSalvageDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �`��̃f�o�C�X �R���e�L�X�g

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// �N���C�A���g�̎l�p�`�̈���̒���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �A�C�R���̕`��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ���[�U�[���ŏ��������E�B���h�E���h���b�O���Ă���Ƃ��ɕ\������J�[�\�����擾���邽�߂ɁA
//  �V�X�e�������̊֐����Ăяo���܂��B
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
	/* �t�@�C���T�C�Y�𒲍� */
	fseek(fp,0,SEEK_END);
	fgetpos(fp,&fsize);
	m_file_size = fsize;
	fclose(fp);


	CFile cfile;
	cfile.Open(fname, CFile::modeRead);
#if 0
	// ���R�͂킩��Ȃ����ǁA�Ȃ���debug���[�h�ŃR���p�C���o���Ȃ��Ȃ�
	CFileStatus status;
	cfile.GetStatus(status);

	m_file_size = status.m_size;
#endif
	if(m_file_size > 110000000){
		AfxMessageBox("Salvage size must be less than 100MB. Please try again");
		return;
	}

	// check sum �v�Z
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
		msg.Format("�������͂W�������w�肵�Ă�������(%d)\r\n", m_output_fname.GetLength());
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

	// Header���M�J�n
	status = SPI_Write(m_ftHandle, buffer, sizeToTransfer, &sizeTransfered,
		//SPI_TRANSFER_OPTIONS_SIZE_IN_BITS|
		SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES|
		SPI_TRANSFER_OPTIONS_CHIPSELECT_ENABLE|
		SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE);

	// Status �\��
		m_LogMessage.AppendFormat("\r\n<< Header >>\r\n");
	m_LogMessage.AppendFormat("%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\r\n", 
		buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5], buffer[6], buffer[7], 
		buffer[8], buffer[9], buffer[10], buffer[11], buffer[12], buffer[13], buffer[14]);
	UpdateData(FALSE);

	// �R�A�f�[�^Salvage���A�ʃX���b�h�ŊJ�n
	int ttime = GetTickCount();
	int progress_disp_wait_timer = ttime + PROGRESS_DISP_INTERVAL; // progress display interval = PROGRESS_DISP_INTERVAL(msec)
	m_salvageCompleteByte = 0;
	Sleep(50); // DE0 wait
	for(;;){ // �]�����[�v
		int progress_disp_wait_timer = GetTickCount();
		int restByte = m_file_size - m_salvageCompleteByte;
		int burst_size;
		if(restByte >= m_burst_size) burst_size = m_burst_size;
		else burst_size = restByte;

		if(burst_size <= 0) break;
		if(m_emergency_stop == TRUE){
			AfxMessageBox("�]���𒆎~���܂�");
			m_LogMessage.AppendFormat("### �]���𒆎~���܂� ###\r\n");
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
			m_LogMessage.AppendFormat("Salvage�I��(%dmsec)\r\n", ttime);
			m_disp_salvage_percent = "0%";
			m_salvageProgress.SetPos(0);
			UpdateData(FALSE);
			break;
		}
		Sleep(90);
		int pos = (int)((double)m_salvageCompleteByte / (double)m_file_size * 1000.0);
		m_disp_salvage_percent.Format("%d\%", pos / 10);
		m_salvageProgress.SetPos(pos); // progress bar�̍X�V
		UpdateData(FALSE);
		TreatMessage();
	}
#endif

	ttime = GetTickCount() - ttime;
	m_LogMessage.AppendFormat("Salvage�I��(%dmsec)\r\n", ttime);
	m_LogMessage.AppendFormat("�]�����[�g%.1f(kByte/sec)\r\n", (double)m_file_size / (double)ttime);
	m_LogMessage.AppendFormat("Salvage finished!!\r\n");
	UpdateData(FALSE);
	AfxMessageBox("Salvage�I��!!");
	m_salvageProgress.SetPos(0);
	m_disp_salvage_percent = "0%";
	UpdateData(FALSE);
	// �I������
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
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	m_emergency_stop = TRUE;
}
