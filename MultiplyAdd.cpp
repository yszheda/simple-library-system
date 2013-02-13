// MultiplyAdd.cpp : implementation file
//

#include "stdafx.h"
#include "Library.h"
#include "MultiplyAdd.h"
#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#include<stdio.h>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMultiplyAdd dialog


CMultiplyAdd::CMultiplyAdd(CWnd* pParent /*=NULL*/)
	: CDialog(CMultiplyAdd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMultiplyAdd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMultiplyAdd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMultiplyAdd)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMultiplyAdd, CDialog)
	//{{AFX_MSG_MAP(CMultiplyAdd)
//	ON_COMMAND(ID_MULTIPLY_ADD, OnMultiplyAdd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMultiplyAdd message handlers
//txt��һ����Ϣ��(���,���,����,������,���,����,�۸�,����) 
void CMultiplyAdd::OnOK() 
{
	CString filePath;//�û���ѡ����ļ���·��
	CFileDialog filedlg(TRUE);//����ΪTRUE���ļ�ģʽ
	if(IDOK==filedlg.DoModal())//��ʾ���ļ��Ի���
		filePath=filedlg.GetFileName();//�õ�CFileDialog���ļ���
	UpdateData();//��������
	int length;
	char * FileName;//�ļ���
	//��CString���͵�filePathת��Ϊchar *���͵�FileName
	length=filePath.GetLength();//�õ�filePath����
	FileName=new char[length];//�����ڴ�ռ�
	//����filePath���ݵ�FileName
	for(int j=0;j<length;j++)
		*(FileName+j)=filePath.GetAt(j);
	*(FileName+length)='\0';

	FILE *file;
	file=fopen(FileName,"r");
	if(!file)  //�ж��Ƿ�򿪳ɹ�
	{
		cout<<"�ļ���ʧ��"<<endl;
		exit(0);
	}
    char *attribute[10];//��Ӧ���Ե��ַ���ֵ
	char *line;//������һ���ַ���
	//�����ڴ�ռ�
	line=(char *)malloc(1000);
	for(int i=0;i<10;i++){
		attribute[i]=(char *)malloc(1000);
	}
	//���book���Ƿ��Ѿ���
	if (!m_bookSet.IsOpen()){
		m_bookSet.Open();
	}
	int num=0;//������������
	while(fgets(line,1000,file)!=NULL)//���ļ�����һ���ַ���
	{
		line[strlen(line)-1]='\0';
		for(int i=0;i<strlen(line);i++)
		{
			//����")"�ַ�
			if(line[i]==')')
			{
				line[i]='\0';
			}
		}
		//��һ����"("�ַ������ԣ�����","Ϊ�ָ������������ַ���д��attribute����Ԫ����
		attribute[0]=strtok(line+1,",");
		for(int k=1;k<=7;k++){
			attribute[k]=strtok(NULL,",");
		}
		//����attribute���ַ���ֵ�������ݣ�����year��numberҪת����int��
		CString m_bno,m_category,m_title,m_press,m_author,m_price;
		int m_year,m_number;
		m_bno.Format("%s",attribute[0]);
		m_category.Format("%s",attribute[1]);
		m_title.Format("%s",attribute[2]);
		m_press.Format("%s",attribute[3]);
		m_year=atoi(attribute[4]);
		m_author.Format("%s",attribute[5]);
		m_price.Format("%s",attribute[6]);
		m_number=atoi(attribute[7]);
		//��ѯ�����Ƿ����
		m_bookSet.m_strFilter.Format("bno='%s'",m_bno);
		m_bookSet.Requery();
		//�����鲻���ڣ��򴴽�����Ŀ
		if(m_bookSet.IsEOF())
		{
			m_bookSet.AddNew();
			m_bookSet.m_bno=m_bno;
			m_bookSet.m_category=m_category;
			m_bookSet.m_title=m_title;
			m_bookSet.m_press=m_press;
			m_bookSet.m_year=m_year;
			m_bookSet.m_author=m_author;
		    m_bookSet.m_price=m_price;
			m_bookSet.m_total=m_number;
			m_bookSet.m_stock=m_number;
			m_bookSet.Update();
		}
		//�������Ѵ��ڣ���������ܲ������Ϳ��
		else
		{
			m_bookSet.Edit();
			m_bookSet.m_total+=m_number;
			m_bookSet.m_stock+=m_number;
			m_bookSet.Update();
		}
		num++;//���²�����������
	}
	CString str;
	str.Format("�ɹ�������%d����¼",num);
	MessageBox(str);
	fclose(file); 
	CDialog::OnOK();
}
