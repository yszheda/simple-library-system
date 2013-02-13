// ReturnBook.cpp : implementation file
//

#include "stdafx.h"
#include "Library.h"
#include "ReturnBook.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReturnBook dialog


CReturnBook::CReturnBook(CWnd* pParent /*=NULL*/)
	: CDialog(CReturnBook::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReturnBook)
	m_strBookNum = _T("");
	m_strCardNum = _T("");
	//}}AFX_DATA_INIT
}


void CReturnBook::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReturnBook)
	DDX_Control(pDX, IDC_LIST1, m_ctrList);
	DDX_Text(pDX, IDC_BOOK_NUM, m_strBookNum);
	DDX_Text(pDX, IDC_CARD_NUM, m_strCardNum);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReturnBook, CDialog)
	//{{AFX_MSG_MAP(CReturnBook)
	ON_BN_CLICKED(IDC_QUERY, OnQuery)
	ON_BN_CLICKED(IDQUERY, OnQuery)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReturnBook message handlers

void CReturnBook::OnOK() 
{
	UpdateData(true);//���༭���е����ݸ��µ���Ա����m_strCardNum��m_strBookNum��

	if (!m_borrowSet.IsOpen()){
		m_borrowSet.Open();
	}
	//�ҳ������鼮�Ļ����¼
	m_borrowSet.m_strFilter.Format("cno='%s' and bno='%s' and return_date is null",m_strCardNum,m_strBookNum);
	m_borrowSet.Requery();
	//�������鼮�����ڻ������м�¼�Ļ���ʱ�䶼�ǿ�ʱ
	if (m_borrowSet.IsEOF()){
		AfxMessageBox("���鲻���ڻ��Ѿ�����");
		CDialog::OnOK();
		return ;
	}
	CTime today=CTime::GetCurrentTime();//��ǰ����
	CTimeSpan diffDays=today-m_borrowSet.m_borrow_date;	//���������鼮���������
	// ������ʱ�䳬��30���򵯳�������ڵ���ʾ�Ի��򲢼������л������
	if (diffDays.GetDays()>30){
 		AfxMessageBox("���������");
		CDialog::OnOK();
	}
	//���������ڸĳɵ�ǰ����
	m_borrowSet.Edit();
	m_borrowSet.m_return_date=today;
	m_borrowSet.Update();

	//book���иò���Ŀ���1
	if (!m_bookSet.IsOpen()){
		m_bookSet.Open();
	}
	m_bookSet.m_strFilter.Format("bno='%s'",m_strBookNum);
	m_bookSet.Requery();
	m_bookSet.Edit();
	m_bookSet.m_stock=m_bookSet.m_stock+1;
	m_bookSet.Update();	
	AfxMessageBox("����ɹ�");
	CDialog::OnOK();
}

void CReturnBook::OnQuery() 
{
	m_ctrList.DeleteAllItems();
	m_ctrList.SetRedraw(FALSE);
	UpdateData();
	if(!m_cardSet.IsOpen())
	{
		m_cardSet.Open();
	}
	if (!m_borrowSet.IsOpen()){
		m_borrowSet.Open();
	}
	if (!m_bookSet.IsOpen()){
		m_bookSet.Open();
	}
	m_cardSet.m_strFilter.Format("cno=%s",m_strCardNum);
	m_cardSet.Requery();
	//��������Ϣ����ȷ���򱨴���
	if(m_cardSet.IsEOF())
	{
		AfxMessageBox("������Ϣ����ȷ");
		return;
	}
	m_borrowSet.m_strFilter.Format("cno='%s'",m_strCardNum);
	m_borrowSet.Requery();
	//��û���ѽ��鼮���򱨴���
	if(m_borrowSet.IsEOF())
	{
		AfxMessageBox("û���ѽ��鼮");
		return;
	}
	int i=0;
	CString str;//����CString���͵ĳ�Ա����ת��ΪCString�����Ա���ʾ
	while(!m_borrowSet.IsEOF())
	{
		m_bookSet.m_strFilter.Format("bno='%s'",m_borrowSet.m_bno);
		m_bookSet.Requery();

		m_ctrList.InsertItem(i,m_bookSet.m_bno);
		m_ctrList.SetItemText(i,1,m_bookSet.m_category);
		m_ctrList.SetItemText(i,2,m_bookSet.m_title);
		m_ctrList.SetItemText(i,3,m_bookSet.m_press);
		str.Format("%d",m_bookSet.m_year);
		m_ctrList.SetItemText(i,4,str);
		m_ctrList.SetItemText(i,5,m_bookSet.m_author);
		str.Format("%f",m_bookSet.m_price);
		m_ctrList.SetItemText(i,6,str);
		str.Format("%d",m_bookSet.m_total);
		m_ctrList.SetItemText(i,7,str);
		str.Format("%d",m_bookSet.m_stock);
		m_ctrList.SetItemText(i,8,str);
		i++;
		m_borrowSet.MoveNext();
	}	
	m_borrowSet.Close();
	m_ctrList.SetRedraw(TRUE);	
}
BOOL CReturnBook::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_ctrList.InsertColumn(0,"���");
	m_ctrList.InsertColumn(1,"���");
	m_ctrList.InsertColumn(2,"����");
	m_ctrList.InsertColumn(3,"������");
	m_ctrList.InsertColumn(4,"���");
	m_ctrList.InsertColumn(5,"����");
	m_ctrList.InsertColumn(6,"�۸�");
	m_ctrList.InsertColumn(7,"�ܲ�����");
	m_ctrList.InsertColumn(8,"���");

	m_ctrList.SetColumnWidth(0,60);
	m_ctrList.SetColumnWidth(1,120);
	m_ctrList.SetColumnWidth(2,80);
	m_ctrList.SetColumnWidth(3,80);
	m_ctrList.SetColumnWidth(4,80);
	m_ctrList.SetColumnWidth(5,80);
	m_ctrList.SetColumnWidth(6,80);
	m_ctrList.SetColumnWidth(7,80);
	m_ctrList.SetColumnWidth(8,80);
	m_ctrList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
