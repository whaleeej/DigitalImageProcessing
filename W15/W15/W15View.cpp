
// W15View.cpp: CW15View 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "W15.h"
#endif

#include "W15Doc.h"
#include "W15View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CW15View

IMPLEMENT_DYNCREATE(CW15View, CView)

BEGIN_MESSAGE_MAP(CW15View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CW15View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_START_GRAY, &CW15View::OnStartGray)
	ON_COMMAND(ID_START_REVERSE, &CW15View::OnStartReverse)
	ON_COMMAND(ID_START_MOSAIC, &CW15View::OnStartMosaic)
	ON_COMMAND(ID_START_EMBOSSMENT, &CW15View::OnStartEmbossment)
	ON_COMMAND(ID_START_HIERARCHY, &CW15View::OnStartHierarchy)
	ON_COMMAND(ID_START_GRAYTRANS, &CW15View::OnStartGraytrans)
	ON_COMMAND(ID_OPEN, &CW15View::OnOpen)
END_MESSAGE_MAP()

// CW15View 构造/析构

CW15View::CW15View()
{
	// TODO: 在此处添加构造代码

}

CW15View::~CW15View()
{
}

BOOL CW15View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CW15View 绘图

void CW15View::OnDraw(CDC* pDC)
{
	CW15Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	mybmp.Draw(pDC, CPoint(0, 0), sizeimage);

	newbmp.Draw(pDC, CPoint(sizeimage.cx + 10, 0), sizeimage);

	// TODO: 在此处为本机数据添加绘制代码
}


// CW15View 打印


void CW15View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CW15View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CW15View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CW15View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CW15View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CW15View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CW15View 诊断

#ifdef _DEBUG
void CW15View::AssertValid() const
{
	CView::AssertValid();
}

void CW15View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CW15Doc* CW15View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CW15Doc)));
	return (CW15Doc*)m_pDocument;
}
#endif //_DEBUG


// CW15View 消息处理程序

void CW15View::OnOpen()
{
	// TODO: 在此添加命令处理程序代码
	CFileDialog FileDlg(TRUE, _T("*.bmp"), "", OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY, "image files (*.bmp) |*.bmp|AVI files (*.avi) |*.avi|All Files (*.*)|*.*||", NULL);
	char title[] = { "Open Image" };
	FileDlg.m_ofn.lpstrTitle = title;
	CFile file;
	if (FileDlg.DoModal() == IDOK)
	{
		if (!file.Open(FileDlg.GetPathName(), CFile::modeRead))
		{
			AfxMessageBox("cannot open the file");
			return;
		}
		if (!mybmp.Read(&file))
		{
			AfxMessageBox("cannot read the file");
			return;
		}
	}

	if (mybmp.m_lpBMIH->biCompression != BI_RGB)
	{
		AfxMessageBox("Can not read compressed file.");
		return;
	}
	sizeimage = mybmp.GetDimensions();
	Invalidate();
}




void CW15View::OnStartGray()
{
	// TODO: 在此添加命令处理程序代码
	newbmp.CreateCDib(sizeimage, 24);
	// 对图象的象素值进行变换
	// 每列
	for (int x = 0; x < sizeimage.cx; x++)
	{
		// 每行
		for (int y = 0; y < sizeimage.cy; y++)
		{
			RGBQUAD color;
			color = mybmp.GetPixel(x, y);
			//RGB 图像转灰度图像 Gray = R*0.299 + G*0.587 + B*0.114
			double gray = color.rgbRed*0.299 + color.rgbGreen*0.587 + color.rgbBlue*0.114; 
			color.rgbBlue = (unsigned char)gray;
			color.rgbGreen = (unsigned char)gray; 
			color.rgbRed = (unsigned char)gray; 
			newbmp.WritePixel(x, y, color);
		}
	}
	Invalidate();

}


void CW15View::OnStartReverse()
{
	// TODO: 在此添加命令处理程序代码
	newbmp.CreateCDib(sizeimage, 24);

	for (int x = 0; x < sizeimage.cx; x++)
	{
		for (int y = 0; y < sizeimage.cy; y++)
		{
			RGBQUAD color;
			color = mybmp.GetPixel(x, y); 
			color.rgbBlue = 255 - color.rgbBlue;
			color.rgbGreen = 255 - color.rgbGreen; 
			color.rgbRed = 255 - color.rgbRed; 
			newbmp.WritePixel(x, y, color);
		}
	}
	Invalidate();
}


void CW15View::OnStartMosaic()
{
	// TODO: 在此添加命令处理程序代码
	newbmp.CreateCDib(sizeimage, 24);
	int step = 3;
	for (int x = step; x < sizeimage.cx - step; x += (step*2+1))
	{
		for (int y = step; y < sizeimage.cy - step; y += (step * 2 + 1))
		{
			RGBQUAD color;
			color = mybmp.GetPixel(x, y);
			
			int r = 0, g = 0, b = 0, num = 0; 
			for (int m1 = -step; m1 <= step; m1++)
				for (int m2 = -step; m2 <= step; m2++)
				{
					if (x + m1 >= sizeimage.cx || x + m1 < 0 || y + m2 >= sizeimage.cy || y + m2< 0) 
						continue;
					num++; 
					RGBQUAD color1;
					color1 = mybmp.GetPixel(x + m1, y + m2); 
					r += color1.rgbRed;
					g += color1.rgbGreen; 
					b += color1.rgbBlue;
				}

			color.rgbRed = (unsigned char)(r*1.0 / num); 
			color.rgbGreen = (unsigned char)(g*1.0 / num); 
			color.rgbBlue = (unsigned char)(b*1.0 / num);

			for (int m1 = -step; m1 <= step; m1++)
				for (int m2 = -step; m2 <= step; m2++)
				{
					newbmp.WritePixel(x + m1, y + m2, color);
				}
		}
	}
	Invalidate();
}


void CW15View::OnStartEmbossment()
{
	// TODO: 在此添加命令处理程序代码
	newbmp.CreateCDib(sizeimage, 24);
	for (int x = 1; x < sizeimage.cx; x++)
	{
		for (int y = 1; y < sizeimage.cy; y++)
		{
			RGBQUAD color;
			color = mybmp.GetPixel(x, y);

			RGBQUAD color1;
			color1 = mybmp.GetPixel(x - 1, y);

			//G(i,j)= f(i,j)- f(i-1,j)+常量
			color.rgbBlue = color.rgbBlue - color1.rgbBlue + 128; 
			color.rgbGreen = color.rgbGreen - color1.rgbGreen + 128; 
			color.rgbRed = color.rgbRed - color1.rgbRed + 128;

			if (color.rgbBlue > 255) color.rgbBlue = 255; 
			if (color.rgbBlue < 0) color.rgbBlue = 0;
			if (color.rgbGreen > 255) color.rgbGreen = 255; 
			if (color.rgbGreen < 0) color.rgbGreen = 0;
			if (color.rgbRed > 255) color.rgbRed = 255; 
			if (color.rgbRed < 0) color.rgbRed = 0; 
			newbmp.WritePixel(x, y, color);
		}
	}
	Invalidate();
}


void CW15View::OnStartHierarchy()
{
}


void CW15View::OnStartGraytrans()
{
	// TODO: 在此添加命令处理程序代码
	newbmp.CreateCDib(sizeimage, 24);
	// 对图象的象素值进行变换
	for (int x = 0; x < sizeimage.cx; x++)
	{
		for (int y = 0; y < sizeimage.cy; y++)
		{
			RGBQUAD color;
			color = mybmp.GetPixel(x, y);

			color.rgbBlue = (int)(color.rgbBlue * 3) % 256; 
			color.rgbGreen = (int)(color.rgbGreen * 7) % 256;
			color.rgbRed = (int)(color.rgbRed * 4) % 256; 
			newbmp.WritePixel(x, y, color);
		}
	}
	Invalidate();
}


