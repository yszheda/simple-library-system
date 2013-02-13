// LoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Library.h"
#include "LoginDlg.h"
#include "BasicQuery.h"
#include "AdvancedQuery1.h"
#include"SingleAddDlg.h"
#include "CardNewDlg.h"
#include "CardLostDlg.h"
#include "MainMenu.h"
#include "LibraryDlg.h"
#include "currentID.h"
extern CString currentID;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLoginDlg dialog


CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoginDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLoginDlg)
	m_strAdminiID = _T("");
	m_strCode = _T("");
	//}}AFX_DATA_INIT
}


void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoginDlg)
	DDX_CBString(pDX, IDC_ADMINI_ID, m_strAdminiID);
	DDX_Text(pDX, IDC_CODE, m_strCode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialog)
	//{{AFX_MSG_MAP(CLoginDlg)
	ON_BN_CLICKED(IDC_READER, OnReader)
//	ON_COMMAND(ID_ADVANCED, OnAdvanced)
//	ON_COMMAND(ID_BASIC_QUERY, OnBasicQuery)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoginDlg message handlers

void CLoginDlg::OnOK() 
{
	CString str;
	CString strfilter;
    UpdateData(true);//���༭���е����ݸ��µ���Ա����m_strAdminiID��m_strCode��
	if(!m_adminiSet.IsOpen())
	{
		m_adminiSet.Open();
	}
	strfilter.Format("admini_ID='%s'",m_strAdminiID);
	m_adminiSet.m_strFilter=strfilter;
	m_adminiSet.Requery();
	//��ID�����ڣ�����������Ϣ������
	if(m_adminiSet.IsEOF())
	{
		AfxMessageBox("ID������");
		return;
	}
	else
	{
		str.Format(" and code='%s'",m_strCode);
		strfilter+=str;
		m_adminiSet.m_strFilter=strfilter;
		m_adminiSet.Requery();
		//�����벻ƥ�䣬����������Ϣ������
		if(m_adminiSet.IsEOF())
		{
			AfxMessageBox("���벻ƥ��");
			return;
		}
		//��������Ϣ��ȷ������µ�ǰ��¼��ID�������˵��Ի���
		else 
		{
			currentID=m_strAdminiID;
			CMainMenu a;
			a.DoModal();
		}
	}
}
//ѡ����ߵ�¼��򿪲�ѯ�Ի���
void CLoginDlg::OnReader() 
{
	CBasicQuery a;
	a.DoModal();
	CDialog::OnOK();
}
