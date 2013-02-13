// BasicQuery.cpp : implementation file
//

#include "stdafx.h"
#include "Library.h"
#include "BasicQuery.h"
#include "BookRS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBasicQuery dialog


CBasicQuery::CBasicQuery(CWnd* pParent /*=NULL*/)
	: CDialog(CBasicQuery::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBasicQuery)
	m_strKeyWord = _T("");
	//}}AFX_DATA_INIT
}


void CBasicQuery::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBasicQuery)
	DDX_Control(pDX, IDC_LISTCTRL, m_ctrList);
	DDX_Text(pDX, IDC_KEY_WORD, m_strKeyWord);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBasicQuery, CDialog)
	//{{AFX_MSG_MAP(CBasicQuery)
	ON_BN_CLICKED(IDC_QUERY, OnQuery)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LISTCTRL, OnColumnclickListctrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBasicQuery message handlers

void CBasicQuery::OnQuery() 
{
	m_ctrList.DeleteAllItems();//���в�ѯʱ�����ListControlԭ�е�����
	m_ctrList.SetRedraw(FALSE);// ListControl���ݽ��д�����������ʱ��������˸
	UpdateData();//���༭�����ݸ��µ���˰󶨵ĳ�Ա����m_strKeyWord��
	if (!m_bookSet.IsOpen()){
		m_bookSet.Open();
	}
	//���ò�ѯ���������������ؼ��ֽ���ģ����ѯ
	m_bookSet.m_strFilter="title like '%"+m_strKeyWord+"%'";
	m_bookSet.Requery();
	
	//���Ҳ������飬�򵯳���ʾ������
	if (m_bookSet.IsEOF()){
		AfxMessageBox("���޴���");
		return;
	}
	//��ȫ���Ľ����ʾ��ListControl��
	int i=0;
	CString str;//����CString���͵ĳ�Ա����ת��ΪCString�����Ա���ʾ
	while (!m_bookSet.IsEOF()){		
		m_ctrList.InsertItem(i,m_bookSet.m_bno);//����bno���Ե�ֵ��Ϊ����
		//�����������
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
		m_bookSet.MoveNext();
	}
	m_bookSet.Close();
	m_ctrList.SetRedraw(TRUE);
}
BOOL CBasicQuery::OnInitDialog() 
{
	CDialog::OnInitDialog();
	//��ӱ�ͷ	
	m_ctrList.InsertColumn(0,"���");
	m_ctrList.InsertColumn(1,"���");
	m_ctrList.InsertColumn(2,"����");
	m_ctrList.InsertColumn(3,"������");
	m_ctrList.InsertColumn(4,"���");
	m_ctrList.InsertColumn(5,"����");
	m_ctrList.InsertColumn(6,"�۸�");
	m_ctrList.InsertColumn(7,"�ܲ�����");
	m_ctrList.InsertColumn(8,"���");
	//�����п�
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
	//LVS_EX_FULLROWSELECTΪѡ��ĳ��ʹ���и������ܣ�Ϊ"����"��ʾ��ʽ������
	//LVS_EX_GRIDLINESΪ�����߹��ܣ�Ϊ"����"��ʾ��ʽ������
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBasicQuery::OnColumnclickListctrl(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	static int sortstyl=1;//����ʽ
	int selectcol = pNMListView->iSubItem;//��õ�ǰ��ѡ��
	int listcount;//������
	listcount = m_ctrList.GetItemCount();
	CString temp0,temp1,temp2;
	if(sortstyl==1)
	{
		//ð������
		 for(int j = 1;j<=listcount;j++)
		 {
			for(int i=0;i<listcount-j;i++)
			{
				temp1 = m_ctrList.GetItemText(i,selectcol);
				temp2 = m_ctrList.GetItemText(i+1,selectcol);
				if(temp1 > temp2)
				{
					for(int n=0;n<9;n++)
					{
						temp1 = m_ctrList.GetItemText(i,n);
						temp2 = m_ctrList.GetItemText(i+1,n);
						m_ctrList.SetItemText(i,n,temp2);
						m_ctrList.SetItemText(i+1,n,temp1);
					}
				}
			}  
		}
	 }
	//����ڶ���ʱ����һ�ַ�ʽ��
	else if(sortstyl==-1)
	{ 
		for(int j = 1;j<=listcount;j++)
		{
			for(int i=0;i<listcount-j;i++)
			{
				temp1 = m_ctrList.GetItemText(i,selectcol);
				temp2 = m_ctrList.GetItemText(i+1,selectcol);
				if(temp2 > temp1)
				{
					for(int n=0;n<9;n++)
					{
						temp1 = m_ctrList.GetItemText(i,n);
						temp2 = m_ctrList.GetItemText(i+1,n);
						m_ctrList.SetItemText(i,n,temp2);
						m_ctrList.SetItemText(i+1,n,temp1);
					}
				}
			}  
		}
	}
	sortstyl = sortstyl*(-1);//ÿ���һ��sortstyl���Ը�1�������´ε���ͻ�����һ�ַ�ʽ����	
	*pResult = 0;
}
