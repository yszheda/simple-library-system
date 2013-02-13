// BorrowBook.cpp : implementation file
//

#include "stdafx.h"
#include "Library.h"
#include "BorrowBook.h"
#include "currentID.h"
extern CString currentID;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBorrowBook dialog


CBorrowBook::CBorrowBook(CWnd* pParent /*=NULL*/)
	: CDialog(CBorrowBook::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBorrowBook)
	m_strBookNum = _T("");
	m_strCardNum = _T("");
	//}}AFX_DATA_INIT
}


void CBorrowBook::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBorrowBook)
	DDX_Control(pDX, IDC_LIST1, m_ctrList);
	DDX_Text(pDX, IDC_BOOK_NUM, m_strBookNum);
	DDX_Text(pDX, IDC_CARD_NUM, m_strCardNum);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBorrowBook, CDialog)
	//{{AFX_MSG_MAP(CBorrowBook)
	ON_BN_CLICKED(IDC_QUERY, OnQuery)
	ON_BN_CLICKED(IDQUERY, OnQuery)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBorrowBook message handlers

void CBorrowBook::OnOK() 
{
	UpdateData();//���༭���е����ݸ��µ���Ա����m_strCardNum��m_strBookNum��
	
	//������Ų������򷵻أ��ܾ�����
	if (!IsValidCard(m_strCardNum)){
		CDialog::OnOK();
		return;
	}
	//������鳬���������޺ͽ����޶�򷵻ؾܾ�����
	if (AnyBookExpired(m_strCardNum)){
		CDialog::OnOK();
		return;
	}
	//�������ͼ�鲻���ڻ�û�п�棬�򷵻ؾܾ�����
	if(Nobook(m_strBookNum)){
		CDialog::OnOK();
		return;
	}
	//ֻ�����������û������ʱ�����������ͼ��
	if (BorrowBook(m_strCardNum,m_strBookNum))
	{
		AfxMessageBox("����ɹ�");
	}
	else
	{
		AfxMessageBox("����ʧ��");
	}
	CDialog::OnOK();
}

bool CBorrowBook::IsValidCard(const CString &cardNum)
{
	if (!m_cardSet.IsOpen()){
		m_cardSet.Open();
	}
	m_cardSet.m_strFilter.Format("cno='%s'",cardNum);
	m_cardSet.Requery();
	//�����Ų����ڣ�����������Ϣ������
	if (m_cardSet.IsEOF()){
		AfxMessageBox("�ÿ��Ų�����");
		return false;
	}
	return true;
}

bool CBorrowBook::AnyBookExpired(const CString &cardNum)
{
	int total=0;//��������
	if (!m_borrowSet.IsOpen()){
		m_borrowSet.Open();
	}
	m_borrowSet.m_strFilter.Format("cno='%s' and return_date is null",cardNum);
	m_borrowSet.Requery();
	CTime  today=CTime::GetCurrentTime();//��ǰʱ��
	CTimeSpan diffDays;
	while (!m_borrowSet.IsEOF()){		
		total++;		
		diffDays=today-m_borrowSet.m_borrow_date;//���㵱ǰʱ�������ʱ��ļ��
		//����30��������ޣ��򱨴���
		if (diffDays.GetDays()>30){
			AfxMessageBox("�����������");
			return true;
		}
		m_borrowSet.MoveNext();	
	}
	//�����������ܳ���12�������޶�
	if (total>12){
		AfxMessageBox("����������������");
		return true;
	}	
	return false;
}
bool CBorrowBook::Nobook(const CString &bookNum)
{
	CTimeSpan span(30,0,0,0);//30���ʱ����
	CTime nearest;//����黹��ʱ��
	if (!m_bookSet.IsOpen()){
		m_bookSet.Open();
	}
	//ָ����ѯ����bnoΪ���
	m_bookSet.m_strFilter.Format("bno='%s'",bookNum);
	m_bookSet.Requery();
	//�����鲻���ڣ��򱨴���
	if(m_bookSet.IsEOF())
	{
		AfxMessageBox("���鲻����");
		return true;
	}
	//�����Ϊ0���򵯳�û�п�����ʾ����ʾʹ���߸�������Ļ���ʱ���Է������ٴν���
	if(m_bookSet.m_stock==0)
	{
		AfxMessageBox("�����Ѿ�û�п��");
		if (!m_borrowSet.IsOpen())
		{
			m_borrowSet.Open();
		}
		m_borrowSet.m_strFilter.Format("bno='%s' and return_date is NULL",bookNum);
		m_borrowSet.m_strSort="borrow_date ASC";//������ʱ����������
		nearest=m_borrowSet.m_borrow_date+span;//30��������
		CString strTime="����Ļ���ʱ����:";  
		strTime+=nearest.Format("%Y-%m-%d   %H:%M:%S");
		MessageBox(strTime);
		return true;
	}	
	return false;
}


bool CBorrowBook::BorrowBook(const CString &cardNum, const CString &bookNum)
{
	CTimeSpan span(30,0,0,0);//30���ʱ����
	CTime nextMonth=CTime::GetCurrentTime()+span;//30��������
	//��book���еĿ���1
	if (!m_bookSet.IsOpen()){
		m_bookSet.Open();
	}	
	m_bookSet.m_strFilter.Format("bno='%s'",bookNum);
	m_bookSet.Requery();	
	m_bookSet.Edit();
	m_bookSet.m_stock=m_bookSet.m_stock-1;
	m_bookSet.Update();

	//��borrow��������һ�������¼
	if (!m_borrowSet.IsOpen()){
		m_borrowSet.Open();
	}	
	m_borrowSet.AddNew();
	m_borrowSet.m_bno=bookNum;
	m_borrowSet.m_cno=cardNum;
	m_borrowSet.m_borrow_date=CTime::GetCurrentTime();
	m_borrowSet.m_admini_ID=currentID;//��������Ϊ��ǰ��¼�Ĺ���ԱID
	m_borrowSet.Update();
	m_borrowSet.Requery();
	return true;
}

void CBorrowBook::OnQuery() 
{	
	m_ctrList.DeleteAllItems();
	m_ctrList.SetRedraw(FALSE);
	UpdateData();

	if(!m_cardSet.IsOpen())
	{
		m_cardSet.Open();
	}
	if(!m_borrowSet.IsOpen()){
		m_borrowSet.Open();
	}
	if(!m_bookSet.IsOpen()){
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

	m_borrowSet.m_strFilter.Format("cno=%s",m_strCardNum);
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
BOOL CBorrowBook::OnInitDialog() 
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
