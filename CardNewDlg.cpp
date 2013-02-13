// CardNewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Library.h"
//
#include "CardRS.h"
//
#include "CardNewDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCardNewDlg dialog


CCardNewDlg::CCardNewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCardNewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCardNewDlg)
	m_cardNum = _T("");
	m_department = _T("");
	m_name = _T("");
	m_type = _T("");
	//}}AFX_DATA_INIT
}


void CCardNewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCardNewDlg)
	DDX_Text(pDX, IDC_CARD_NUM, m_cardNum);
	DDX_Text(pDX, IDC_DEPARTMENT, m_department);
	DDX_Text(pDX, IDC_NAME, m_name);
	DDX_Text(pDX, IDC_TYPE, m_type);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCardNewDlg, CDialog)
	//{{AFX_MSG_MAP(CCardNewDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCardNewDlg message handlers

void CCardNewDlg::OnOK() 
{
	UpdateData();//���༭���е����ݸ��µ���Ա����m_CardNum��
	if (!m_cardSet.IsOpen()){
		m_cardSet.Open();
	}
	//���ҽ���֤������Ƿ��Ѿ�����
	m_cardSet.m_strFilter.Format("cno='%s'",m_cardNum);
	m_cardSet.Requery();
	//���ÿ������ڣ�����card��������һ���µļ�¼
	if(m_cardSet.IsEOF())
	{
		m_cardSet.AddNew();
		m_cardSet.m_cno=m_cardNum;
		m_cardSet.m_name=m_name;
		m_cardSet.m_department=m_department;
		m_cardSet.m_type=m_type;
		m_cardSet.Update();

		AfxMessageBox("����ɹ���");
	}
	//���ÿ��Ѵ��ڣ��򵯳���Ӧ��ʾ
	else
	{
		AfxMessageBox("�ÿ��Ѵ���");
	}
	CDialog::OnOK();
}
